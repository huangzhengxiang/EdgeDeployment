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

2. Bundle framework
    Add the framework to `Build Phase` -> `Copy Bundle Resources`, so that the native framework can be bundled into the app. (Other assets are also needed to be bundled in this way.)

3. Include Headers
    If Objective-C++ files include the framework headers, the framework root shall be added to `Build Settings` -> `Header Search Paths`.
