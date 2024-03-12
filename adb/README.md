## adb shell


[guide](https://developer.android.com/tools/releases/platform-tools?hl=zh-cn)

### 1. Enable `Developer options`(开发人员/者选项)
[guide](https://developer.android.com/studio/debug/dev-options?hl=zh-cn#enable)

`Settings`(设置) > `About phone`(关于手机) > `Build number`(版本号)
1. Tap the `Build Number` option <b>seven times</b> until you see the message `You are now a developer!` This enables `developer options` on your device.

2. Enable `USB Debugging`

### 2. Configurate your PC
1. MacOS: Okay.
2. Windows: [Install OEM USB drivers](https://developer.android.com/studio/run/oem-usb)
3. Linux: TO BE DONE

Try adb devices, your device shall be on the list if it's detected.
```bash
adb devices
```

### 3. ADB Shell
[Manual](https://adbshell.com/)