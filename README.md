# AndroTouch
AndroTouch is a tool to replicate the screen of an Android device on your PC. AndroTouch tries also to forward clicks from the PC to the Device.
## How it works
AndroTouch uses [USB debug](https://developer.android.com/studio/debug/dev-options) to execute some commands on the device, so it must be enabled first.  
[ADB shell](https://developer.android.com/studio/command-line/adb) is used to run `screencap` to capture the screen and `input` to forward clicks and keypresses.
On some device `input` must be run as root, so the phone must be rooted.
