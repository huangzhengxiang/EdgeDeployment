## Git

### Merge

#### Fast-Forward
If no conflicts take place, use `git pull` and `git push` to synchronize with the remote upstreams is possible.

```bash
git pull origin ${remote}/${local}
git push origin ${local}/${remote}
```

#### Merge
However, if ff is not possible due to conflicts, please use `git merge` to merge other stream to current stream and resolve conflicts in vscode manually.

```bash
git merge ${other}
```