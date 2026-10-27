# AndroTouch
AndroTouch is a tool to replicate the screen of an Android device on your PC. AndroTouch tries also to forward clicks from the PC to the Device.
## How it works
AndroTouch uses [USB debug](https://developer.android.com/studio/debug/dev-options) to execute some commands on the device, so it must be enabled first.  
[ADB shell](https://developer.android.com/studio/command-line/adb) is used to run `screencap` to capture the screen and `input` to forward clicks and keypresses.
On some device `input` must be run as root, so the phone must be rooted.

## Build

Requirements: CMake 3.19 or newer, Qt 6 (Widgets), and `adb` in the `PATH` to run it.

```
cmake -B build
cmake --build build
```

## License

Copyright (C) 2015 Matteo Croce

AndroTouch is released under the GNU General Public License version 3 or later, see [COPYING](COPYING).
