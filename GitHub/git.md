## Git

### 0. Congif

#### 0.1 Buffer Size
To make sure you can download smoothly, you can enlarge the postBuffer size.
```bash
# enlarge buffer to 500MB
git config --global http.postBuffer 524288000
# enlarge buffer to 1GB
git config --global http.postBuffer 1073741824
```

### 1. Merge

#### 1.1 Fast-Forward
If no conflicts take place, use `git pull` and `git push` to synchronize with the remote upstreams is possible.

```bash
git pull origin ${remote}:${local}
git push origin ${local}:${remote}
```

You can also push a detached head to a remote stream. Notice that head is referred to as `HEAD`.
```bash
git push <stream> HEAD:<branch>
```

#### 1.2 Merge
However, if ff is not possible due to conflicts, please use `git merge` to merge other stream to current stream and resolve conflicts in vscode manually.

```bash
git merge ${other}
```

Note: The remote branch is in the form of `<stream>/<branch>`, such as `origin/master`.

#### 1.3 upstream
Add a new upstream to a directory.
```bash
git remote add <stream> <url>
```

### 2. git diff

To ensure the correctness of merge, git diff can be used to view the differences between branches.

#### 2.1 Branch comparison
You can compare 2 branches with the following command.
```bash
git diff ${branch1} ${branch2}
```

#### 2.2 File comparison
You can compare 2 files from different branches with the following command.
```bash
git diff ${branch1}:${file1} ${branch2}:${file2}
```

### 3. Submodule
https://github.blog/open-source/git/working-with-submodules/

#### 3.1 submodule add
If your GitHub repository is referencing another GitHub repo, git submodule can be used to sync it to your repo.

```bash
# use git submodule add to automatically generate a .gitmodules file. 
git submodule add ${submodule-url} ${your-folder}
```

A .gitmodules file will be generated afterwards.

#### 3.2 submodule update and syc
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

#### 3.3 update a submodule after clone
Change directory to the folder and update the submodule. This command combine init and update together.
```bash
# update to the previously indexed commit. Combine init and update.
git submodule update --init --recursive
```

### 4. git branch

#### 4.1 open up a local branch
```bash
git branch ${branch-name}
```

#### 4.2 checkout
Switch the current workplace to the new local branch.

```bash
git checkout ${branch-name}
```

#### 4.3 delete a branch

```bash
git branch -d -r ${branch-name}
```
