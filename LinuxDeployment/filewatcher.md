## Linux file watcher issue

https://github.com/guard/listen/blob/fd85e1cb2375767e3cbc4b5743ff50061e8a6c75/README.md#the-technical-details


The number of files that can be tracked in a directory is by default `fs.inotify.max_user_watches`.
They shall be reset by executing:
```bash
sudo sh -c "echo fs.inotify.max_user_watches=1048576 >> /etc/sysctl.conf"
sudo sysctl -p
```

And reset your compile cache and compiler daeon (such as `buck2 kill`). Everything will be alright then.