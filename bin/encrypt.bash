#!/usr/bin/env bash
# Generates .encrypt file for class that was passed in
# className="" && filePath="" && clear && printf '\e[3J' && bin/encrypt.bash $className $filePath && cat bin/encrypt.files/generated/$className.encrypt
# className="" && filePath="" && clear && reset && bin/encrypt.bash $className $filePath && cat bin/encrypt.files/generated/$className.encrypt

debug=0

if [[ $debug == 1 ]] ; then
set -xe
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

if [[ $2 == *.java* ]] ; then
	rm -rf bin/encrypt.files/build.encrypt
	mkdir bin/encrypt.files/build.encrypt
	cd bin/encrypt.files/build.encrypt
	packageName=${1//".$className"}. #java.lang
	filePath=${packageName//.//} #java/lang

	mkdir -p $filePath && cp $2 $filePath$className.java
	grep -R '"*"' $filePath$className.java > "$1.encryptBlueprint"

else
echo "Usage: bin/encrypt.bash FULLY_QUALIFIED_CLASS_NAME [OPTIONAL JAVA FILE PATH]"
	exit;
fi

cp  ../../../library/src/test/java/GenerateRSAHelpers.java .


javac GenerateRSAHelpers.java -classpath $apacheCryptoJarFile

java -classpath .:$apacheCryptoJarFile GenerateRSAHelpers "$1.encryptBlueprint" > $1.encrypt
cp $1.encrypt ../generated/$1.encrypt

echo "Result: $className.encrypt has been generated and is ready to use."
echo "Path: ${PWD}/$1.encrypt"

cd ../..
# rm -rf bin/encrypt.files/build.encrypt
