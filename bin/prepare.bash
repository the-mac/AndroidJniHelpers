#!/usr/bin/env bash

set -e
# TO CHANGE DIRECTORY QUICKLY: cd the-mac/AndroidJni/AndroidJniHelpers/bin/prepare.bash
# TO MAKE DEFAULT PREPARATION: clear && printf '\e[3J' && bin/prepare.bash no

# Define a timestamp function
createTimestamp() {
  date +"%T"
}

# function to easily determine if a set of programs and files are available.
function display_changed_files {
	echo ""
	statusContents=$(git status)
	if [[ $statusContents == *"nothing to commit"* ]]; then
		createTimestamp > timestamp
		statusContents=$(git status)
		echo "$statusContents"
	else
		echo "$statusContents"
	fi
	
}
function check_diff {

while true; do
	echo ""
	
	if [[ -z "$@" ]]; then		read -p "Is there a file you would like to diff (No/no) > " diff
	else
	     diff="$@"
		 echo ""
	     echo "You selected option(s): $diff"
		 echo ""
	fi
	if [[ $diff == *"no"* || $diff == *"No"*  || $diff == *"NO"* ]]; then
	
	
		break;
	else
		git diff -m "$diff"
		git status

	fi
done
}
function accept_commit_message {
	echo ""
	
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
		git add -A && git commit -m "$commit" && git push origin master
	elif [[ $size == 0 ]]; then
		git add -A && git commit -m "Default commit message using bin/prepare.bash script." && git push origin master --force
	fi
}

function prepare_push {
	
	echo "Showing Changed files: "
	display_changed_files

	check_diff $@
	
	accept_commit_message $@
}

prepare_push $@
