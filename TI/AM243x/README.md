# AM243x
datasheet and guide.
https://www.ti.com/lit/an/spracv1a/spracv1a.pdf
https://www.ti.com/lit/ds/symlink/am2434.pdf

## Memory Layout
Only MSRAM memory space can be allocated to code and data, among which first 512K and last 256K are also reserved, only 1280K memory available.
FLASH is reserved for bootloader stuff. (<b>all 512KB flash is reserved under default bootloader.</b>)
https://dev.ti.com/tirex/explore/content/mcu_plus_sdk_am243x_09_00_00_35/docs/api_guide_am243x/MEMORY_MAP.html