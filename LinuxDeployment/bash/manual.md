## Linux Bash

### 0. variables
For local variables in this bash.
1. Export it by:
```bash
export var=123
```
2. Use it by:
```bash
echo ${var}
```

For user variables defined for all bashes for this user.
Append `export var=123` to the end of `~/.bashrc`


### 1. if clause & existence testing

#### 1.1 if-then-else-fi pattern
The condition clause (if clause) takes the format as follows:
~~~bash
export condition=false
if ${condition}
then
    echo "do when condition is true"
else
    echo "do when condition is false"
fi 
~~~

#### 1.2 Test the existence of a file/dir
For directory existence, `test -d ${dir_name}`.
For file existence, `test -f ${file_name}`.

### 2. for clause & iteration




### 3. apt apt-get
1. add apt-repository
e.g., add
~~~bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
~~~

### 4. gcc g++ toolchain
#### 4.1 Install higher version of gcc, g++.
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

#### 4.2 update cmake
Download a newest cmake version from https://cmake.org/download/.
Append the directory to PATH.
~~~bash
export PATH=${PATH}:/path/to/cmake/bin
# you can also add this line to ~/.bashrc
~~~

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