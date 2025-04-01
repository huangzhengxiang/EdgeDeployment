## Linux Bash

### 0. variables
For local variables in this bash.
1. Export it by:
```bash
export var=123
# set Link Libraty Path
export LD_LIBRARY_PATH=.
```

2. Use it by `${var}`:
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

### 5. top/htop monitoring
#### 5.1 top command
top command don't have horizontal usage bars.
~~~bash
# sort by memory usage.
top -s 6
# sort by CPU usage.
top -s 9
# update every 1 sec
top -d 1 -s 6
~~~

#### 5.2 htop command

### 6. resize swap
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

### 7. Networking

#### 7.1 Connection
1. Connect with gui.
Connect to WIFI with Network Manager GUI (Graphic User Interface).
~~~bash
# install Network Manager
sudo apt install network-manager network-manager-gnome
sudo apt install gnome-control-center
# initiate gui
gnome-control-center
~~~

2. Connect from terminal
Connect to WIFI with Network Manager CMI (Command Line Interface), which functions just the same as the GUI interface on the up-right corner of Ubuntu.
```bash
# see available wifi
nmcli dev wifi list
# connect to a wifi with WPA/WPA2 authentication
sudo nmcli dev wifi con "WIFI-NAME" password "WIFI-PASSWORD"
```


### 8. Lookup for a file

#### 8.1 find
search for a file in a designated directory.

```bash
find ./ -name libc++_shared.so
# in the format of
find ${search-dir} -name ${search-file}
```

### 9. file system operations

#### 9.1 mkdir
you can create a directory only if it doesn't already exist using the `mkdir` command with the `-p` option. The `-p` flag ensures that mkdir will not throw an error if the directory already exists.
