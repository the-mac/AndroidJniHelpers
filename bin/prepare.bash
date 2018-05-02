#!/usr/bin/env bash

set -e
# TO CHANGE DIRECTORY QUICKLY: cd the-mac/AndroidJni/AndroidJniHelpers/bin/prepare.bash
# TO MAKE DEFAULT PREPARATION: clear && printf '\e[3J' && bin/prepare.bash no
# TO MAKE DEFAULT PREPARATION: filePath="" && clear && printf '\e[3J' && bin/prepare.bash --diff $filePath

export version=$(cat bin/prepare.files/version)
export uploadedVersion=$(cat bin/prepare.files/uploaded)

# Define a timestamp function
createTimestamp() {
  date +"%T"
}

# function to easily determine if a set of programs and files are available.
function display_changed_files {
	echo ""
	statusContents=$(git status)
	if [[ $statusContents == *"nothing to commit"* ]]; then
		createTimestamp > bin/prepare.files/timestamp
		statusContents=$(git status)
		echo "$statusContents"
	else
		echo "$statusContents"
	fi
	
}

function check_diff {

while true; do
	echo ""

        if [[ -z "$@" ]]; then          read -p "Is there a file you would like to reset (No/no) > " fileToReset
        else
             fileToReset="$@"
	     ignore="$@"
             diff="$@"
        fi
        if [[ $fileToReset != *"no"* && $fileToReset != *"No"*  && $fileToReset != *"NO"* ]]; then

                git checkout -- $fileToReset
                git status
        continue;

        fi

	if [[ -z "$@" ]]; then		read -p "Is there a file you would like to ignore (No/no) > " ignore
	else
             fileToReset="$@"
	     ignore="$@"
	     diff="$@"
	fi
	if [[ $ignore != *"no"* && $ignore != *"No"*  && $ignore != *"NO"* ]]; then

		echo "$ignore" >> .gitignore
		git add $ignore && git commit -m "Adding $ignore to .gitignore"
		git status
        continue;

	fi
	if [[ -z "$@" || -z $diff ]]; then		read -p "Is there a file you would like to diff (No/no) > " diff
	else
	     diff="$@"
		 echo ""
	     echo "You selected option(s): $diff"
		 echo ""
	fi
	if [[ $diff == *"no"* || $diff == *"No"*  || $diff == *"NO"* ]]; then
	    break;
	else
		diff_individual_file $diff
	fi
done
}

function diff_individual_file {

    if [[ -z "$@" ]]; then
        display_changed_files
        echo "" && read -p "What is the file you would like to diff > " fileToDiff
    else
        fileToDiff=$1
    fi

	if [[ -z "$fileToDiff" ]] || [ ! -f "$fileToDiff" ]; then
        echo "Usage: bin/prepare.bash --diff FILE_PATH"
	    exit
	else
        echo "Diffing file: $fileToDiff" && gitk --max-count=3 --all -- $fileToDiff
	fi

}

function accept_commit_message {
	echo ""

	status=$(git status)
	if [[ $status == *"On branch master"* ]]; then
		# read -p "You are on the master branch, commit here? > " useMasterBranch
		branchName="master"
	else
		branchName="testing"
	fi

#	if [[ $useMasterBranch == *"yes"* ]]; then
#		branchName="master"
#	elif [[ $useMasterBranch == *"no"* ]]; then
#	    # git checkout testing
#		branchName="testing"
#	else
#		branchName="testing"
#	fi

	if [[ -z "$@" ]]; then
		read -p "Please type commit message > " commit
		size=${#commit}
	else
	     size=0
		 echo ""
	     echo "You selected the default message"
		 echo ""
	fi
	
	if [[ $commit ]]; then
		git add -A && git commit -m "$commit" && git pull origin $branchName && git push origin $branchName
	elif [[ $size == 0 ]]; then
		git add -A && git commit -m "Default commit message using bin/prepare.bash script." && git pull origin $branchName && git push origin $branchName --force
	fi
}

function publish_version {


    if [[ $uploadedVersion == *"$version"* ]]; then
        echo "Please update current version, because the versions are the same"
        echo ""
        exit 1
    fi


    currentPath=$PWD
    fromPath="$currentPath/library/build/outputs"
    apkPath="$fromPath/apk/androidTest/debug"
    statusPath=$currentPath/bin/prepare.files/build.test


#    bin/adb.bash --findDevice
    specificDevice=$(cat bin/adb.files/specificDevice)
    if [[ -z "$specificDevice" ]]; then
        sleep 2 && echo "No device is connected, please connect device or try (bin/adb.bash --startEmulator) in new terminal window" && exit 1
    fi

    rm -rf $statusPath
    mkdir -p $statusPath

    # CLEAR STATUS FOR TEST
    echo "" > $statusPath/status_android

    ./gradlew assembleDebug library:packageDebugAndroidTest --info #app:connectedAndroidTest
    cp $apkPath/library-debug-androidTest.apk $statusPath


	sleep 2 && deviceReady=$(adb -s $specificDevice shell 'pwd')
	if [[ $deviceReady == *"/"* ]]; then
#		set -xe

		appPackage="us.the.mac.android.jni.helpers"
		basePackage="$appPackage"
		testPackage="$appPackage.test"
		testClass="$appPackage.NetworkTest"
        bintrayUser=$(cat bin/prepare.files/.bintrayUser)
        bintrayKey=$(cat bin/prepare.files/.bintrayKey)

#    adb push /Users/christopher/git/the-mac/AndroidJniHelpers/library/build/outputs/apk/androidTest/debug/library-debug-androidTest.apk /data/local/tmp/us.the.mac.android.jni.helpers.test
#    adb shell pm install -t -r "/data/local/tmp/us.the.mac.android.jni.helpers.test"#
#    adb shell am instrument -w -r   -e debug false -e class us.the.mac.android.jni.helpers.NetworkTest us.the.mac.android.jni.helpers.test/android.support.test.runner.AndroidJUnitRunner

		# SAVE STATUS FOR ARCHIVE
#        bin/adb.bash --test $statusPath/library-debug-androidTest.apk $statusPath/app-debug-androidTest.apk $basePackage $testPackage $testClass $debugFlag | tee -a $statusPath/status_android


        adb push $statusPath/library-debug-androidTest.apk /data/local/tmp/us.the.mac.android.jni.helpers.test
        adb shell pm install -t -r "/data/local/tmp/us.the.mac.android.jni.helpers.test"


        adb shell am instrument -w -r   -e debug false -e class us.the.mac.android.jni.helpers.NetworkTest us.the.mac.android.jni.helpers.test/android.support.test.runner.AndroidJUnitRunner | tee -a $statusPath/status_android
		testResult=$(cat $statusPath/status_android)


        if [[ $testResult != *"OK (13 tests)"* ]]; then
            echo ""
            echo "'Test Execution Failed'"
            echo ""
            exit 1
        fi

        statusCheck=$(git status)
        if [[ $statusCheck != *"nothing to commit"* ]]; then

		    DATE=`date +%Y-%m-%d`
            sleep 2 && git add -A && git commit -m "Passed Network Test - $DATE" || git tag -d $version
            sleep 2 && git tag -a $version -m "Release $version" && git pull origin $branchName

            sleep 2 && git push origin $branchName && ./gradlew clean build bintrayUpload -PbintrayUser=$bintrayUser -PbintrayKey=$bintrayKey -PdryRun=false

            echo "$version" > bin/prepare.files/uploaded
        fi


		set -e
    else
        echo ""
        echo "'Your specific device is not ready: $specificDevice'"
        echo ""
        exit 1
    fi

}
function prepare_push {
	
	echo "Showing Changed files: "
	display_changed_files

	check_diff $@
	
	accept_commit_message $@
}


function execute_commands {

    echo "=================================="
    echo "Current Version: $version"
    echo "Uploaded Version: $uploadedVersion"

    echo "=================================="
    echo ""

    if [[ $1 == *--push* ]] ; then
		prepare_push $@
	elif [[ $1 == *--publish* ]] ; then
		publish_version $@
    else
        echo "Usage: bin/prepare.bash --push [no] OR"
        echo "Usage: bin/prepare.bash --publish" && echo ""# []
        exit;
    fi
}

execute_commands $@