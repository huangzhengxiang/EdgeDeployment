## Git

### 0. Congif & clone

#### 0.1 Buffer Size
To make sure you can download smoothly, you can enlarge the postBuffer size.
```bash
# enlarge buffer to 500MB
git config --global http.postBuffer 524288000
# enlarge buffer to 1GB
git config --global http.postBuffer 1073741824
```

#### 0.2 clone
`git clone` is where everything starts.
```bash
git clone <url>
git clone <url> <local_path>
git clone --recursive <url>
```

Basically, a `git clone` clones a repo from a url (to a local path). Adding `--recursive` can resolve submodule download.

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
# delete a local branch
git branch -d ${branch-name}
# delete a remote branch
git branch -d -r ${branch-name}
```

#### 4.4 rename a branch
For example, a branch called `HEAD` is accidentally created, the only way to resolve it to disambiguous it from the detached HEAD is to rename the `HEAD` branch to other name.

```bash
git branch -m HEAD tmp
```

### 5. reset
Sometimes when we want to   
- **remove previous commit**, or   
- **(dangerously) reset our local repo to some previous version**,  

using `reset` would come in quite handy.
#### 5.1 soft reset
To clear our commit to some previous version, type in the command
```bash
git reset --soft <SHA of the previous commit>
```
You can look up the SHA of the previous commit by clicking the `n commits ahead of ...` link in the following figure
![](./Screenshot%20from%202025-04-13%2012-54-04.png)
You can then copy the desired commit SHA in the following page
![](./Screenshot%20from%202025-04-13%2012-52-10.png)

**NOTE**: `git reset --soft` will only clear your commit history without throwing away all the uncommited changes in your local repo, if somehow, the latter is your actual need, use hard reset described in the following section.
#### 5.2 hard reset
To hard reset your local repo, just modify `--soft` to `--hard` in the previous section
```bash
git reset --hard <SHA of the previous commit>
```
### commit
One thing to note about the simple command `git commit` is that, if previous commit(s) failed(e.g. forget to use `git lfs` to commit large files), the failure **will not be removed in future commits** unless you have removed your previous failed commits. This is because `git commit` will try to commit all the uncommited files in the local commit history.
