# AndroTouch
AndroTouch is a tool to replicate the screen of an Android device on your PC. AndroTouch tries also to forward clicks from the PC to the Device.

Its most useful use case is recovering data from a phone with a broken screen or touchscreen:
the phone can be operated from the PC with the mouse and the keyboard.
This only works if USB debugging was enabled on the phone before it broke.

<p align="center">
<img src="https://github.com/user-attachments/assets/b236f035-5ce8-4f5a-a15b-02f5c06e6367" alt="AndroTouch showing the app drawer" width="270">
<img src="https://github.com/user-attachments/assets/ea68b35d-9de9-4f3b-9ae5-1064f661b542" alt="AndroTouch showing Google Maps" width="270">
<img src="https://github.com/user-attachments/assets/af87efdb-db9f-4bee-adf6-49095d022fee" alt="AndroTouch showing Google Photos" width="270">
</p>

## How it works
AndroTouch uses [USB debug](https://developer.android.com/studio/debug/dev-options) to execute some commands on the device, so it must be enabled first.  
[ADB shell](https://developer.android.com/studio/command-line/adb) is used to run `screencap` to capture the screen and `input` to forward clicks and keypresses,
and the mouse wheel as a swipe.
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
