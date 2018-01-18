#!/usr/bin/env bash

set -e
# TO CHANGE DIRECTORY QUICKLY: cd the-mac/AndroidJni/AndroidJniHelpers/bin/prepare.bash
# TO MAKE DEFAULT PREPARATION: clear && printf '\e[3J' && bin/prepare.bash no
# TO MAKE DEFAULT PREPARATION: filePath="" && clear && printf '\e[3J' && bin/prepare.bash --diff $filePath

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

function prepare_push {
	
	echo "Showing Changed files: "
	display_changed_files

	check_diff $@
	
	accept_commit_message $@
}

prepare_push $@
