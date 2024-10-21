## installation

### Download cuda toolkit

1. First, check the cuda version and drivers' version with `nvidia-smi`.

For a legacy version of ubuntu and driver, visit 
https://developer.nvidia.com/cuda-toolkit-archive

2. Install `runfile` instead of `deb` can avoid ubuntu version dependencies.

```bash
# e.g. cuda 12.2.2 535.104.05
wget https://developer.download.nvidia.com/compute/cuda/12.2.2/local_installers/cuda_12.2.2_535.104.05_linux.run
sudo sh cuda_12.2.2_535.104.05_linux.run
```

Caution: 
1. Make sure your terminal window is big enough to see the whole terms.
2. If driver is installed before, do not install again.

3. export PATH and LD_LIBRARY_PATH
Add in ~/.bashrc
```bash
# e.g. (You shall read the installation prompt carefully)
export PATH=$PATH:"/usr/local/cuda-12.2/bin"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/usr/local/cuda-12.2/lib64"
```