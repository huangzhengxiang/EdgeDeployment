## Git

### Merge

#### Fast-Forward
If no conflicts take place, use `git pull` and `git push` to synchronize with the remote upstreams is possible.

```bash
git pull origin ${remote}:${local}
git push origin ${local}:${remote}
```

#### Merge
However, if ff is not possible due to conflicts, please use `git merge` to merge other stream to current stream and resolve conflicts in vscode manually.

```bash
git merge ${other}
```

Note: The remote branch is in the form of `<stream>/branch`, such as `origin/master`.


### git diff

To ensure the correctness of merge, git diff can be used to view the differences between branches.

#### Branch comparison
You can compare 2 branches with the following command.
```bash
git diff ${branch1} ${branch2}
```

#### File comparison
You can compare 2 files from different branches with the following command.
```bash
git diff ${branch1}:${file1} ${branch2}:${file2}
```