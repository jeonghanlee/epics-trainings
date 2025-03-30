# Installation

## Lesson Overview

In this lesson, you will learn how to do the following:
* Clone the cloneable ALS-U EPICS Environment by using git
* Configure the ALS-U EPICS Environment locally
* Test the cloned EPICS enivornment with a few EPICS command line tools
* Switch the different version of the EPICS environment within a terminal

## Get the ALS-U EPICS environment 

Clone the ALS-U EPICS repository using Git.

### Prerequisites

Users may need to have SSH access to the ALS GitLab repository to clone the following repositories.

```bash
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/epics/alsu-epics-environment.git ~/epics
```

By the cloning the repository, you have the environment at the `${HOME}/epics` folder. In most case, you are ready to use it.


## Configure the ALS-U EPICS enviornment

The ALS-U EPICS environment supports multiple versions. To select a version, source the appropriate `setEpicsEnv.bash` script:

```bash
source ~/alsu-epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash  # For Debian 12
# or
source ~/alsu-epics/1.1.1/rocky-8.10/7.0.7/setEpicsEnv.bash # For Rocky 8.10
```
The output should resemble this (user and paths will vary):
```bash
Set the EPICS Environment as follows:
THIS Source NAME    : setEpicsEnv.bash
THIS Source PATH    : /home/jeonglee/epics/1.1.1/debian-12/7.0.7
EPICS_BASE          : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/base
EPICS_HOST_ARCH     : linux-x86_64
EPICS_MODULES       : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules
PATH                : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pmac/bin/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pvxs/bin/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/base/bin/linux-x86_64:/home/jeonglee/programs/root_v6-28-04/bin:/home/jeonglee/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games
LD_LIBRARY_PATH     : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pmac/lib/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pvxs/bundle/usr/linux-x86_64/lib:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pvxs/lib/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/base/lib/linux-x86_64:/home/jeonglee/programs/root_v6-28-04/lib/root

Enjoy Everlasting EPICS!

```

## Testing the Environment

Verify the installation by running the following commands:

```bash
$ caput -h
$ which caget
```

Technically, you have now the ALS-U EPICS environment.

