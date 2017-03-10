#!/usr/bin/env bash
# TO CHANGE DIRECTORY QUICKLY: cd the-mac/AndroidJni/AndroidJniHelpers
# TO MAKE DEFAULT PREPARATION: clear && printf '\e[3J' && bin/adb.bash --wifi

debug=0

if [[ $debug == 1 ]] ; then
set -xe
fi

if [[ $@ == *--debug* ]] ; then
debugFlag="--debug"
fi

function checkDevices() {
  devices=$(adb devices)
  echo "$devices" > bin/adb.files/devices
  echo "Devices:"
  cat bin/adb.files/devices

}
function checkProperties() {

    specificDevice=$1
    deviceProp=$(adb -s $specificDevice shell "getprop")

    echo $deviceProp > bin/adb.files/deviceProp
    echo "Device Property (all):"
    cat bin/adb.files/deviceProp

}
function checkProperty() {

    specificDevice=$1
    deviceProp=$(adb -s $specificDevice shell "getprop ${2}")

    echo $deviceProp > bin/adb.files/deviceProp
    echo "Device Property ($2):"
    cat bin/adb.files/deviceProp

}
function disconnectDevice() {
    killServer=$(adb kill-server)
    echo "Result: $killServer"

}
function resetApp {

	echo -n "Clearing app data: " && adb shell pm clear $1 && sleep 2

	echo -n "Uninstalling app: " && adb uninstall $1 && sleep 2

if [[ $2 == *--reboot* ]] ; then
    echo -n "Rebooting device: " && adb reboot && sleep 2
fi
}
function connectToWifi() {
    specificIp=$(cat bin/adb.files/deviceIP)
    specificDevice=$specificIp:5555

    adb connect $specificDevice && sleep 2
    adb -s $specificDevice shell 'setprop dalvik.vm.checkjni true' && sleep 3
}
function setUpWifiDevice {
    # adb shell ip -f inet addr show wlan0 | grep "inet " | awk -F'[inet ]+' '{ print $2 }'

    specificDevice=$1
    port=5555
    adb tcpip $port && sleep 15
    deviceIp=$(adb -s $specificDevice shell 'ip -f inet addr show wlan0' | grep "inet " | sed -e 's/^[^0-9]*//; s/\(\([0-9]\{1,3\}\.\)\{3\}[0-9]\{1,3\}\).*/\1/')

    echo $deviceIp > bin/adb.files/deviceIP
    echo "Device IP:"
    cat bin/adb.files/deviceIP
}

function execute_commands {

    if [[ $1 == *--devices* ]] ; then
		checkDevices
	elif [[ $1 == *--properties* ]] ; then
		checkProperties $2
	elif [[ $1 == *--property* ]] ; then
		checkProperty $2 $3
	elif [[ $1 == *--disconnect* ]] ; then
		disconnectDevice
	elif [[ $1 == *--setup* ]] ; then
		setUpWifiDevice $2
	elif [[ $1 == *--wifi* ]] ; then
		connectToWifi
	elif [[ $1 == *--reset* ]] ; then
		resetApp $2 $3
    else
        echo "Usage: bin/adb.bash --devices OR"
        echo "Usage: bin/adb.bash --setup DEVICE_SERIAL_NAME OR"
        echo "Usage: bin/adb.bash --propties DEVICE_SERIAL_NAME OR"
        echo "Usage: bin/adb.bash --property DEVICE_SERIAL_NAME DEVICE_PROPERTY_NAME OR"
        echo "Usage: bin/adb.bash --disconnect OR"
        echo "Usage: bin/adb.bash --reset [--reboot] OR"
        echo "Usage: bin/adb.bash --wifi" && echo ""
        exit;
    fi
}

execute_commands $@
