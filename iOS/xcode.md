## Xcode

### 1. native build

#### 1.1 add the native build script
1. Add `Run Script` phase in Xcode
    The native build script shall be add to Xcode `Build Phases`, by pressing `+`，and then selecting `New Run Script Phase`. Press the column and drag the `Run Script` column in front of `Compile Sources`.

    * ***Caution***: For directories/files modification/creation, User Sandbox may block it after Xcode 15, disable it by setting `Build Settings` -> `Build Options` -> `User Script Sandboxing` from `Yes` to `No`.

2. modify the `Run Script` scipt
    You shall switch to the project source root by `cd ${SRCROOT}/` command, and then you just code as if you're writing in an ordinary `.sh` file.

    * ***Caution***: All the environment variables are not set by default, and shall be export manually. Therefore, `cmake` is encouraged to be installed at `/opt/homebrew/bin` and exported by default.

#### 1.2 bundle native framework
1. Add framework
    Drag pretrained C framework into Xcode Project.In Xcode, enter `General` -> `Frameworks, Libraries, and Embedded Content`, press `+` to add the framework.
    * `Embed & Sign` for shared library, and `Do not Embed` for static build.

    Add the framework parent path to `Build Settings` -> `Framework Search Paths`.

2. Copy Bundle framework & Embed Framework
    Add the framework to `Build Phase` -> `Copy Bundle Resources`, so that the native framework can be bundled into the app. (Other assets are also needed to be bundled in this way.) (Select `Copy items if needed`)

3. Include Headers
    If Objective-C++ files include the framework headers, the framework root shall be added to `Build Settings` -> `Header Search Paths`.

4. Link Binary
    Add framework to `Build Phase` -> `Link Binary with Libraries`.
    (Select the added framework, no need to add again.)

### 2. Energy Monitor
(diagnose靠谱点)
iOS 18 may encounter Zero Energy: https://github.com/doronz88/pymobiledevice3/issues/1146#issuecomment-2753655815  (reset DeveloperModeand restart the phone resolves this)

1. 使用Xcode中Energy Impact直接读取，问题是仅有可视界面
2. 使用pymobiledevice3，利用与Xcode类似的方法读取瞬时功耗，估算总功耗（2s更新一次），此方法与Xcode读数相同；另外可以用diagnostics读取电量的状态量，则与sysdiagnose相同（20s更新一次）。 https://github.com/doronz88/pymobiledevice3/tree/master
 ```zsh
 # get instaneous energy amount for pid1 and pid2
 pymobiledevice3 developer dvt energy <pid1> <pid2>
 # monitor/get once battery
 pymobiledevice3 diagnostics battery monitor
 pymobiledevice3 diagnostics battery single
 ```
    pymobiledevice3 forum: https://discord.com/channels/1133265168051208214/1133265168730701897
1. 使用Apple Sysdiagnose，可以每20秒监控一次手机的电量变化（单位mAh），如此，只需要每分钟内至多进行一次原子测试即可成功。https://it-training.apple.com/tutorials/support/sup075/
    (把文件AirDrop到mac——目标端需要将所有人可见性打开；然后通过sqlite  https://sqlitebrowser.org/dl/ 然后Open Database打开解压后压缩包中logs/powerlogs/*.PLSQL的数据库；查看PLBatteryAgent_EventBackward_Battery表中AppleRawCurrentCapacity与AppleRawMaxCapacity两项即可。注意最终测试结果的timestamp要对齐。)