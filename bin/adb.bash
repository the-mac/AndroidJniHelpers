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

# Define a devices function
function checkDevices() {
  devices=$(adb devices)
  echo $devices > bin/adb.files/devices
  echo "Devices:"
  cat bin/adb.files/devices

}
function disconnectDevice() {
  killServer=$(adb kill-server)
  echo "Result: $killServer"

}
function resetApp {

	adb shell pm clear $1

	adb uninstall $1

if [[ $2 == *--reboot* ]] ; then
    adb reboot
fi
}
function connectToWifi() {
    specificIp=$(cat bin/adb.files/deviceIP)
    connectionStatus=$(adb connect $specificIp:5555)
}
function setUpWifiDevice {
# adb shell ip -f inet addr show wlan0 | grep "inet " | awk -F'[inet ]+' '{ print $2 }'

    specificDevice=$2

    if [[ -z $specificDevice ]] ; then
    echo "Result: No devices available"
    echo "" && echo "Usage: bin/adb.bash --setup DEVICE_SERIAL_NAME"
    echo "Try: Plugging your device via USB and then execute: bin/adb.bash --devices"
    echo "Then Try: bin/adb.bash --setup COPIED_DEVICE_SERIAL"
    echo ""
    exit;
    fi
	if [[ $hasDeviceConnected == *"LGLS99021ecc9f2"* ]]; then
		specificDevice="LGLS99021ecc9f2"
	elif [[ $hasDeviceConnected == *"LGD85124d9f831"* ]]; then
		specificDevice="LGD85124d9f831"
	elif [[ $hasDeviceConnected == *"771e3fdc"* ]]; then
		specificDevice="771e3fdc"
	else
    echo "Result: No devices available"
    echo "Try plugging in the device by usb"
    echo ""
	exit;
	fi

    port=5555
#    specificDevice=$(adb devices | awk -F"[inet ]+" "{ print $2 }")
    deviceIp=$(adb -s $2 shell 'ip -f inet addr show wlan0 | grep "inet " | awk -F"[inet ]+" "{ print $2 }"')

    deviceIp=$(adb shell ip -f inet addr show wlan0)
    echo $deviceIP > bin/adb.files/deviceIP
    echo "Device IP:"
    cat bin/adb.files/deviceIP
}

function execute_commands {

    if [[ $1 == *--devices* ]] ; then
		checkDevices
	elif [[ $1 == *--disconnect* ]] ; then
		disconnectDevice
	elif [[ $1 == *--setup* ]] ; then
		setUpWifiDevice
	elif [[ $1 == *--wifi* ]] ; then
		connectToWifi
	elif [[ $1 == *--reset* ]] ; then
		resetApp $2 $3
    else
        echo "Usage: bin/adb.bash --devices OR"
        echo "Usage: bin/adb.bash --setup DEVICE_SERIAL_NAME OR"
        echo "Usage: bin/adb.bash --disconnect OR"
        echo "Usage: bin/adb.bash --wifi" && echo ""
        exit;
    fi
}

execute_commands $@
