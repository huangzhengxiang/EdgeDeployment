## I. Linux Server Connection

### 1. OpenSSH KeyGen
Install OpenSSH client on Linux.
```bash
sudo apt install openssh-client
```

Make sure you have OpenSSH installed on your computer (local PC).
If so, you shall be able to run the following line successfully.

```bash
ssh-keygen -t rsa -C "some comments"
```

Then, OpenSSH will instruct you following up. 

The private key will be generated in `~/.ssh/id_rsa`, while the public key will be generated in `~/.ssh/id_rsa.pub`.
You can then send your `id_rsa.pub` to administrator.

### 2. ssh login

ssh login with the following cl instruction.
```bash
ssh -p [Port] [UserName]@[IP]
```

### 3. Add new machine to the server
Append content of `id_rsa.pub` of new client machine to the end of `/root/.ssh/authorized_keys` on the server.
Then, you can connect to the server on another client machine.

### 4. vscode remote ssh
Download VScode extension `Remote SSH`. Add these lines to your remote ssh.
Host Name
    HostName xxx.xxx.xxx.xxx
    User UserName
    Port PortNumber
    IdentityFile "~/.ssh/id_rsa"

### 5. remote login with username & password
You may also login with username & password, without ssh-keygen.
Host Name
    HostName xxx.xxx.xxx.xxx
    User UserName
    Port PortNumber

## II. Ubuntu Server Enable & Disable
Install ssh server.
```bash
sudo apt install openssh-server
```
Stop ssh server
```bash
sudo systemctl stop ssh
sudo systemctl disable ssh
```
Restart ssh server
```bash
sudo systemctl enable ssh
sudo systemctl restart ssh
```