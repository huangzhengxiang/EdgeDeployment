## adb shell


[guide](https://developer.android.com/tools/releases/platform-tools?hl=zh-cn)

### 1. Enable `Developer options`(开发人员/者选项)
Please check it each time you connect to a phone.

[guide](https://developer.android.com/studio/debug/dev-options?hl=zh-cn#enable)

`Settings`(设置) > `About phone`(关于手机) > `Build number`(版本号)
1. Tap the `Build Number` option <b>seven times</b> until you see the message `You are now a developer!` This enables `developer options` on your device.

2. Enable `USB Debugging`

### 2. Configurate your PC
1. MacOS: Okay.
2. Windows: [Install OEM USB drivers](https://developer.android.com/studio/run/oem-usb)
   Such platform tools are also called SDK, download such platform tools as adb in this website: [SDK](https://developer.android.com/tools/releases/platform-tools)
3. Linux: TO BE DONE

Try adb devices, your device shall be on the list if it's detected.
```bash
adb devices
```

### 3. ADB Shell
[Manual](https://adbshell.com/)

### 4. execution access
executable usually can only executed in `/data/local/tmp`.

```bash
cd /data/local/tmp
chmod 777 demo
```

### 5. top command
~~~bash
# sort by memory usage.
top -s 6
# sort by CPU usage.
top -s 9
# update every 1 sec
top -d 1 -s 6
~~~

### 6. Android NDK for Compilation
https://developer.android.com/ndk/downloads

### 7. apk installation

```bash
adb install /path/to/apk
```

### adb over WLAN
When PC and phone are connected to the same WLAN, adb can be connected over the WLAN. Enable `Wireless Debuggging`(无线调试) or anything similar in `Settings`(设置) on phone.

1. adb pair
Find the pairing number(配对码) on phone and the pairing IP and port. 

Execute `adb pair` on PC.

```bash
adb pair <ip>:<port>
```

and then enter the pairing code.

2. adb connect
After pairing, the PC is allowed to be connected to the phone. Find the connection IP and port on phone. This port is usually different from the previous pairing port, though the IP is usually the same.

Execute `adb connect` on PC.

```bash
adb connect <ip>:<port>
```

and then adb is ready to be used.