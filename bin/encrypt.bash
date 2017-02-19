#!/usr/bin/env bash
# Generates .encrypt file for class that was passed in
# keyPath="" && clear && printf '\e[3J' && bin/encrypt.bash --key $keyPath && cat $keyPath
# inputString="Input String" && keyPath="" && clear && printf '\e[3J' && bin/encrypt.bash --encrypt "$inputString" $keyPath --silent
# keyPath="" && className="" && filePath="" && clear && printf '\e[3J' && bin/encrypt.bash $className $filePath $keyPath && cat bin/encrypt.files/generated/$className.encrypt
# keyPath="" && className="" && filePath="" && clear && reset && bin/encrypt.bash $className $filePath $keyPath && cat bin/encrypt.files/generated/$className.encrypt

debug=0

if [[ $debug == 1 ]] ; then
set -xe
fi

if [[ $@ == *--debug* ]] ; then
debugFlag="--debug"
fi

androidVersion=android-24

currentPath=$PWD
cp ../local.properties bin/encrypt.files
sdkPath=$(grep "^sdk.dir=" "bin/encrypt.files/local.properties" | cut -d'=' -f2)

	if [[ -z $sdkPath ]] ; then
echo "Result: $sdkPath was not found in the project"
echo "Check you project's local.properties file"
echo ""
	exit;
	fi

androidJarFile=$sdkPath/platforms/$androidVersion/android.jar
apacheHttpJarFile=$sdkPath/platforms/$androidVersion/optional/org.apache.http.legacy.jar
apacheCryptoJarFile=$currentPath/bin/encrypt.files/commons-codec-1.10.jar

	if [ ! -f $androidJarFile ] ; then
echo "Result: Android version $androidVerison was not found in the android sdk: $androidJarFile"
echo "Try: " $(grep $sdkPath/platforms)
echo ""
	exit;
	fi

className=${1##*.} #String


if [[ $1 == *--encrypt* ]] ; then
	rm -rf bin/encrypt.files/build.encrypt
	mkdir bin/encrypt.files/build.encrypt
	cd bin/encrypt.files/build.encrypt
	packageName=${1//".$className"}. #java.lang
	filePath=${packageName//.//} #java/lang
	keyPath=$3 #../../../library/src/main/assets/native_key

	if [[ -z $keyPath ]] ; then
echo "Result: key file was not found in the execution path"
echo "" && echo "Usage: bin/encrypt.bash --key FULLY_QUALIFIED_KEY_PATH"
echo "Try: bin/encrypt.bash --key file/path/to/your/key/file/native_key"
echo ""
	exit;
	fi

    cp  ../../../library/src/helpers/java/GenerateRSAHelpers.java .

    javac GenerateRSAHelpers.java -classpath $apacheCryptoJarFile

    java -classpath .:$apacheCryptoJarFile GenerateRSAHelpers "$1" "$2" "$keyPath" "$debugFlag" > string_encryption
    encryption=$(cat string_encryption)


	if [[ $@ == *--silent* ]] ; then
        echo "$encryption"
	else
        echo "Your input is encrypted and is ready to use."
        echo "" && echo "Input:
        $2"
        echo "" && echo "Output:
        $encryption" && echo ""
	fi

elif [[ $1 == *--key* ]] ; then
	rm -rf bin/encrypt.files/build.encrypt
	mkdir bin/encrypt.files/build.encrypt
	cd bin/encrypt.files/build.encrypt
	packageName=${1//".$className"}. #java.lang
	filePath=${packageName//.//} #java/lang
	keyPath=$2 #../../../library/src/main/assets/native_key

	if [[ -z $keyPath ]] ; then
echo "Result: key file was not found in the execution path"
echo "" && echo "Usage: bin/encrypt.bash --key FULLY_QUALIFIED_KEY_PATH"
echo "Try: bin/encrypt.bash --key file/path/to/your/key/file/native_key"
echo ""
	exit;
	fi

    cp  ../../../library/src/helpers/java/GenerateRSAHelpers.java .

    javac GenerateRSAHelpers.java -classpath $apacheCryptoJarFile

    java -classpath .:$apacheCryptoJarFile GenerateRSAHelpers "$1" "$debugFlag"  > native_key
    cp native_key $keyPath

    echo "Result: native_key has been generated and is ready to use."
    echo "Path: $keyPath"

elif [[ $2 == *.java* ]] ; then
	rm -rf bin/encrypt.files/build.encrypt
	mkdir bin/encrypt.files/build.encrypt
	cd bin/encrypt.files/build.encrypt
	packageName=${1//".$className"}. #java.lang
	filePath=${packageName//.//} #java/lang
	keyPath=$3 #../../../library/src/main/assets/native_key

	if [[ -z $keyPath ]] ; then
echo "Result: key file was not found in the execution path"
echo "" && echo "Usage: bin/encrypt.bash FULLY_QUALIFIED_CLASS_NAME FULLY_QUALIFIED_JAVA_FILE_PATH FULLY_QUALIFIED_KEY_PATH OR"
echo "Try: bin/encrypt.bash your.class.package.ClassName file/path/to/your/java/class/YourFile.java file/path/to/your_key"
echo ""
	exit;
	fi

	mkdir -p $filePath && cp $2 $filePath$className.java
	grep -R '"*"' $filePath$className.java > "$1.encryptBlueprint"

    cp  ../../../library/src/helpers/java/GenerateRSAHelpers.java .

    javac GenerateRSAHelpers.java -classpath $apacheCryptoJarFile

    java -classpath .:$apacheCryptoJarFile GenerateRSAHelpers "$1.encryptBlueprint" "$3" "$debugFlag" > $1.encrypt
    cp $1.encrypt ../generated/$1.encrypt

    echo "Result: $className.encrypt has been generated and is ready to use."
    echo "Path: ${PWD}/$1.encrypt"

else
    echo "Usage: bin/encrypt.bash FULLY_QUALIFIED_CLASS_NAME FULLY_QUALIFIED_JAVA_FILE_PATH FULLY_QUALIFIED_KEY_PATH OR"
    echo "Usage: bin/encrypt.bash --encrypt INPUT_STRING_TO_ENCRYPT FULLY_QUALIFIED_KEY_PATH OR"
    echo "Usage: bin/encrypt.bash --key FULLY_QUALIFIED_KEY_PATH" && echo ""
	exit;
fi

cd ../..
# rm -rf bin/encrypt.files/build.encrypt
