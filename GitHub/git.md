## Git

### 1. Merge

#### 1.1 Fast-Forward
If no conflicts take place, use `git pull` and `git push` to synchronize with the remote upstreams is possible.

```bash
git pull origin ${remote}/${local}
git push origin ${local}/${remote}
```

#### 1.2 Merge
However, if ff is not possible due to conflicts, please use `git merge` to merge other stream to current stream and resolve conflicts in vscode manually.

```bash
git merge ${other}
```


### 2. Submodule
https://github.blog/open-source/git/working-with-submodules/

#### 2.1 submodule add
If your GitHub repository is referencing another GitHub repo, git submodule can be used to sync it to your repo.

```bash
# use git submodule add to automatically generate a .gitmodules file. 
git submodule add ${submodule-url} ${your-folder}
```

A .gitmodules file will be generated afterwards.

#### 2.2 submodule update and syc
https://stackoverflow.com/questions/5828324/update-git-submodule-to-latest-commit-on-origin

To sync the latest change of the submodule repo, 2 options are possible:
1. update to the previously indexed commit.
2. update to the latest commit available.

```bash
# update to the previously indexed commit.
git submodule update --recursive
# update to the latest commit available. (all)
git submodule foreach git pull origin master
# update to the latest commit available. (only for 1 submodule)
cd ${submodule-dir} && git pull
```