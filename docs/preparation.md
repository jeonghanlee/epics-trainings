# Preparation



## Required Vendor Library

```bash
$ git clone https://github.com/jeonghanlee/uldaq-env
$ cd uldaq-env
uldaq-env (master)$ echo "INSTALL_LOCATION=${HOME}/epics/1.1.1-rc1" > configure/CONFIG_SITE.local
uldaq-env (master)$ make init
uldaq-env (master)$ make conf
uldaq-env (master)$ make build
uldaq-env (master)$ make install
```

## Basis EPICS Enviornment

```bash
$ git clone https://github.com/jeonghanlee/EPICS-env
$ cd EPICS-env
EPICS-env (master)$ echo "EPICS_TS_NTP_NET=2.debian.pool.ntp.org" > configure/RELEASE.local
EPICS-env (master)$ echo "VENDOR_ULDAQ_PATH=${HOME}/epics/1.1.1-rc1/debian-12/vendor" >> configure/RELEASE.local
EPICS-env (master)$ make init
EPICS-env (master)$ make patch
EPICS-env (master)$ make conf
EPICS-env (master)$ make build
EPICS-env (master)$ make install
EPICS-env (master)$ make symlinks
```

## Area Detector Support

```bash
$ git clone https://github.com/jeonghanlee/EPICS-env-support.git
EPICS-env-support (master)$ echo "INSTALL_LOCATION=${HOME}/epics/1.1.1-rc1/debian-12/7.0.7/base" > configure/CONFIG_SITE.local
EPICS-env-support (master)$ make init
EPICS-env-support (master)$ make conf
EPICS-env-support (master)$ make build
EPICS-env-support (master)$ make symlinks
```

## ALS-U Site Specific Modules

```bash
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/epics/alsu-site-modules.git
$ cd alsu-site-modules/
alsu-site-modules (master)$ echo "INSTALL_LOCATION=${HOME}/epics/1.1.1-rc1/debian-12/7.0.7/base" > configure/CONFIG_SITE.local
alsu-site-modules (master)$ make init
alsu-site-modules (master)$ make build

```

## Useful git commands

```bash
apt install git-lfs
dnf install git-lfs
git lfs install
```
```bash
git add ...
git commit -m "update ..."
git lfs push origin master
git push
```
