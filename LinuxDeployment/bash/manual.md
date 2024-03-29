## Linux Bash

### 1. apt apt-get
1. add apt-repository
e.g., add
~~~bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
~~~

### 2. gcc g++ toolchain
#### 2.1 Install higher version of gcc, g++.
First, add new repo.
~~~bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
~~~
Then, install gcc-11, g++-11
~~~bash
sudo apt-get install gcc-11 g++-11
~~~
Finally, set the new GCC version as the default:
~~~bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 --slave /usr/bin/g++ g++ /usr/bin/g++-11
~~~
You can switch between different versions via `update-alternatives` command.

###  resize swap
1. Turn off all running swap processes: 
```bash
sudo swapoff -a
```
2. Resize swap: 
```bash 
sudo fallocate -l 8G /swapfile
``` 
(change 1G to the gigabyte size you want it to be)
3. CHMOD swap: 
```bash
sudo chmod 600 /swapfile
```
4. Make file usable as swap: 
```bash
sudo mkswap /swapfile
```
5. Active the swap file: 
```bash
sudo swapon /swapfile
```