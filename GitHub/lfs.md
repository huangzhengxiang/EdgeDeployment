## git-lfs

git-lfs (Git Larg File Storage) is a tool extensively used on huggingface.co to store large datasets and LLMs.

### installation
https://github.com/git-lfs/git-lfs?utm_source=gitlfs_site&utm_medium=installation_link&utm_campaign=gitlfs#installing

```bash
# Linux
sudo apt install git-lfs
# MacOS
brew install git-lfs
```

### cloning large files
```bash
git lfs install
git clone [yout-repo]
```

### pushing large files
https://medium.com/junior-dev/how-to-use-git-lfs-large-file-storage-to-push-large-files-to-github-41c8db1e2d65

```bash
git lfs install
git lfs track [your-large-files]
# remove your ignore line in .gitignore
git add .
```