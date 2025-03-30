# How to add the new version of ALS-U EPICS Environment

The following procedure has the pre-requirements which we must define all others versions since
this repository only cover the cloneable folder.

For example, we have the ALS-U EPICS environment `1.1.1`.

```shell
git clone ssh://git@git-local.als.lbl.gov:8022/alsu/epics/alsu-epics-environment.git ~/epics
epics (master)$ git branch 1.1.1
epics (1.1.1)$ ls
1.1.1-rc1  README.md  README_PRE.md install.bash
epics (1.1.1)$ bash install.bash
```

