### Nvidia driver issue

#### 1. driver/library version mismatch
driver/library version mismatch error.
```
Failed to initialize NVML: driver/library version mismatch
```

See driver version.
```bash
cat /proc/driver/nvidia/version
```

You will see sth. like this.
```
NVRM version: NVIDIA UNIX x86_64 Kernel Module 440.100 Fri May 29 08:45:51 UTC 2020
``` 

Download driver from [https://www.geforce.cn/drivers/beta-legacy](https://www.geforce.cn/drivers/beta-legacy). You enter your GPU version and Host version and search!
Install, for example:
```bash
sudo sh ~/shared_directory/NVIDIA-Linux-x86_64-440.100.run --no-kernel-module
```