## Go compile

install go.
```bash
sudo apt install golang-go
```

### 1. init a module
```bash
go mod init <module-name>
```

### 2. get from a repo

#### 2.1 set GOPATH
GOPATH shall be set to the subdirectory to where you execute `go mod init`. (else you may get `go: go.mod file not found in current directory or any parent directory；` error.)

Note that GOPATH shall not collide with your module/project path. (else you may get `$GOPATH/go.mod exists but should not` error.)

```bash
mkdir src
export GOPATH=~/battery-historian/src
```

#### 2.2 execute `go get`
For example, getting github.com/google/battery-historian/
```bash
go get github.com/google/battery-historian/...
```

### 3. build and compile

