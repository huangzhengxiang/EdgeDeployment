## zsh

### 0. zsh introduction
zsh is a bash alternative/counterpart on macOS.

#### 0.1 ~/.zshrc
~/.bashrc counterpart, can be configured the activate environment variables.

### 1. brew

#### 1.1 brew installation
Refer to https://brew.sh, to download homebrew from the official website.
It's the counterpart of linux `pm` or `apt`, because its most common usage is : `brew install <pkg-name>`.

After installation, append brew binary path (e.g., /opt/homebrew/bin) to `PATH` in `~/.zshrc`.

```zsh
brew install git
```