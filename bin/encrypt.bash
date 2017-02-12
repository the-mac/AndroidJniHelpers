#!/usr/bin/env bash
# Generates .encrypt file for class that was passed in
# className="" && filePath="" && clear && printf '\e[3J' && bin/encrypt.bash $className $filePath && cat bin/encrypt.files/generated/$className.encrypt
# className="" && filePath="" && clear && reset && bin/encrypt.bash $className $filePath && cat bin/encrypt.files/generated/$className.encrypt

debug=1

if [[ -z !$debug ]] ; then
set -xe
fi

androidVersion=android-24

cp ../local.properties bin/encrypt.files
sdkPath=$(grep "^sdk.dir=" "bin/encrypt.files/local.properties" | cut -d'=' -f2)

	if [[ -z $sdkPath ]] ; then
echo "Result: $sdkPath was not found in the project"
echo "Check you project's local.properties file"
echo ""
	exit;
	fi

androidJarFile=$sdkPath/platforms/$androidVersion/android.jar
apacheJarFile=$sdkPath/platforms/$androidVersion/optional/org.apache.http.legacy.jar

	if [ ! -f $androidJarFile ] ; then
echo "Result: Android version $androidVerison was not found in the android sdk: $androidJarFile"
echo "Try: " $(grep $sdkPath/platforms)
echo ""
	exit;
	fi

className=${1##*.} #String

if [[ $2 == *.java* ]] ; then
	rm -rf bin/encrypt.files/build.encrypt
	mkdir bin/encrypt.files/build.encrypt
	cd bin/encrypt.files/build.encrypt
	packageName=${1//".$className"}. #java.lang
	filePath=${packageName//.//} #java/lang

	
elif [[ $1 == *.* ]] ; then
	rm -rf bin/encrypt.files/build.encrypt
	mkdir bin/encrypt.files/build.encrypt
	cd bin/encrypt.files/build.encrypt
	packageName=${1//".$className"}. #java.lang
	filePath=${packageName//.//} #java/lang

	if [[ -z $classFilePath ]] ; then
echo "Result: $1 was not found in the android library or apache legacy"
echo "" && echo "Usage: bin/encrypt.bash FULLY_QUALIFIED_CLASS_NAME [OPTIONAL JAVA FILE PATH]"
echo "Try: bin/encrypt.bash $1 file/path/to/java/file/$className.java"
echo ""
	exit;
	fi

	if [ ! -f $filePath$className.class ] ; then
echo "Result: $classFilePath was not found in the legacy jar: $apacheJarFile"
echo ""
	exit;
	fi

else
echo "Usage: bin/encrypt.bash FULLY_QUALIFIED_CLASS_NAME [OPTIONAL JAVA FILE PATH]"
	exit;
fi

cp  ../../../library/src/test/java/CryptoHelper.java .
cp ../encryptDataTypes.properties .
cp ../encryptReturnValues.properties .
cp -rf ../encryptMethods .


javac CryptoHelper.java

java CryptoHelper "$1.encryptBlueprint" > $1.encrypt
cp $1.encrypt ../generated/$1.encrypt

echo "Result: $className.encrypt has been generated and is ready to use."
echo "Path: ${PWD}/$1.encrypt"

cd ../..
# rm -rf bin/encrypt.files/build.encrypt
