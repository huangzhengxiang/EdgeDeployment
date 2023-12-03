## 1. CUBE IDE Website （New User need registration）
https://www.st.com/en/development-tools/stm32cubeide.html#get-software
download and setup the IDE

## 2. TI CCS (Code Composer Studio)

### 2.1 SDK download
MCU_PLUS_SDK_AM243X User Guide
	https://dev.ti.com/tirex/explore/node?node=A__AD2nw6Uu4txAz2eqZdShBg__com.ti.MCU_PLUS_SDK_AM243X__AROnekL__LATEST
0. MCU-PLUS-SDK-AM243X
	https://www.ti.com.cn/tool/cn/download/MCU-PLUS-SDK-AM243X
	https://www.ti.com.cn/tool/cn/download/MCU-PLUS-SDK-AM243X/08.04.00.17
1. SysConfig 1.17 C://ti
2. python3
3. openssl-1.1.1 
	https://slproweb.com/products/Win32OpenSSL.html
4. dfu-util-0.8
	http://dfu-util.sourceforge.net/releases/dfu-util-0.8-binaries/win32-mingw32/dfu-util-static.exe
5. windows generic USB driver （zadig-2.7）
	https://github.com/pbatard/libwdi/releases/download/v1.4.1/zadig-2.7.exe


### 2.2 CCS download

1. CCS Download
	https://www.ti.com/tool/CCSTUDIO
2. CCS Installation Guide
	https://dev.ti.com/tirex/explore/content/mcu_plus_sdk_am243x_09_00_00_35/docs/api_guide_am243x/CCS_SETUP_PAGE.html
3. Set package paths
	CCS > Window > Preferences > Code Composer Studio > Products
	![products discovery path](TI/package-path.png)

### 2.3 Board Setup
1. set the board to be uart boot mode.
![UART Board Configuration](TI/uart-boot.png)

2. run the following script:
C:\ti\mcu_plus_sdk_am243x_08_04_00_17 is the location of installed sdk. COM4 is the corresponding serial port.
```
cd C:\ti\mcu_plus_sdk_am243x_08_04_00_17\tools\boot
python uart_uniflash.py -p COM4 --cfg=sbl_prebuilt/am243x-lp/default_sbl_null.cfg
```