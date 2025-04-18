# ALS-U EPICS Environment Configuration and Installation

This section covers the essential first steps for setting up the ALS-U EPICS environment locally. You will clone the central environment repository using git from the ALS GitLab server and then learn how to activate a specific version (e.g., for Debian 12 or Rocky 8) by sourcing the appropriate `setEpicsEnv.bash` script in your terminal session. Basic verification steps are also included.

## Lesson Overview

In this lesson, you will learn how to do the following:
* Clone the cloneable ALS-U EPICS environment by using git
* Configure the ALS-U EPICS environment locally
* Test the cloned EPICS enivornment with a few EPICS command line tools
* Switch the different version of the EPICS environment within a terminal

## Get the ALS-U EPICS environment 

Clone the ALS-U EPICS repository using Git.

### Clone the ALS-U EPICS environment by using `git clone`

Users may need to have SSH access to the ALS GitLab repository to clone the following repositories.

```shell
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/epics/alsu-epics-environment.git ~/epics
```

By cloning the repository, you have the environment at the `${HOME}/epics` folder. In most cases, you are ready to use it.


## Configure the ALS-U EPICS enviornment
The ALS-U EPICS environment supports multiple operating system versions and EPICS versions. **Please note that the pre-built binaries included in this environment currently target the Linux x86_64 architecture exclusively.**

To select and activate a specific environment version in your current terminal session, you need to source the appropriate `setEpicsEnv.bash` script corresponding to your operating system and desired EPICS version:

```shell
# Example for EPICS 7.0.7 on Debian 12 (x86_64)
source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash
# or
# Example for EPICS 7.0.7 on Rocky 8.10 (x86_64)
source ~/epics/1.1.1/rocky-8.10/7.0.7/setEpicsEnv.bash
```
Sourcing the script sets up necessary environment variables like `EPICS_BASE`, `PATH`, and `LD_LIBRARY_PATH`. The output should resemble this (user and specific paths will vary):
```shell
Set the EPICS Environment as follows:
THIS Source NAME    : setEpicsEnv.bash
THIS Source PATH    : /home/jeonglee/epics/1.1.1/debian-12/7.0.7
EPICS_BASE          : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/base
EPICS_HOST_ARCH     : linux-x86_64  # <-- Note the architecture
EPICS_MODULES       : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules
PATH                : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pmac/bin/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pvxs/bin/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/base/bin/linux-x86_64:/home/jeonglee/programs/root_v6-28-04/bin:/home/jeonglee/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games
LD_LIBRARY_PATH     : /home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pmac/lib/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pvxs/bundle/usr/linux-x86_64/lib:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/modules/pvxs/lib/linux-x86_64:/home/jeonglee/epics/1.1.1/debian-12/7.0.7/base/lib/linux-x86_64:/home/jeonglee/programs/root_v6-28-04/lib/root

Enjoy Everlasting EPICS!
```
Note how the `EPICS_HOST_ARCH` variable and the paths explicitly mention `linux-x86_64`.


## Testing the Environment

Once the environment is sourced, verify that the EPICS command-line tools are accessible in your PATH:

```shell
# Check help output for an EPICS command-line tool (e.g., caput)
$ caput -h

# Verify the location of an EPICS command-line tool (e.g., caget)
$ which caget
```

If these commands run successfully and show help/path information, you have successfully configured the ALS-U EPICS environment in your current terminal session.

## Alternative Environment for Other Linux Distributions (Training Contingency)
This training and ALS-U Controls officially supports **Debian 12** and **Rocky 8.10** on the **Linux x86_64** architecture, using the primary ALS-U EPICS environment repository cloned into `~/epics` as described in the main [Get the ALS-U environment](installation.md##Get the ALS-U EPICS environment) section.

**If, and only if,** you are attending this training using **Rocky 9.5**, **Ubuntu 22.04**, or **Ubuntu 24.04** (on Linux `x86_64`) and cannot use the officially supported setup, the following alternative environment repository can be used in the training exercises.

**Important Considerations:**
* This alternative repository provides builds that are functional for the training but may **not** be the latest official ALS-U EPICS environment production versions.
* It is **not** the recommended environment for actual development or deployment work at ALS-U Controls System.
* Using this path requires adjusting subsequent commands (like `source`) and potentially other paths mentioned in the training materials.

**Alternative Repository Setup:**

1.  **Clone the Repository:** Use the following command to clone into a separate directory (e.g., `~/epics-training-alt`). (This uses HTTPS and typically does not require SSH keys, though ALS GitLab login might be needed).
```shell
$ git clone https://git.als.lbl.gov/jeonglee/alsu-epics-environment.git ~/epics-training-alt
```

2.  **Verify Structure:** The relevant structure inside this directory should look like this (confirming builds for various OS exist):
```
$ tree --charset=ascii -L 2 ~/epics-training-alt/1.1.1/
/home/user/epics-training-alt/1.1.1/  # Path will vary based on user/clone location
|-- debian-12
|   |-- 7.0.7
|   `-- vendor
|-- rocky-8.10
|   |-- 7.0.7
|   `-- vendor
|-- rocky-9.5
|   |-- 7.0.7
|   `-- vendor
|-- ubuntu-22.04
|   |-- 7.0.7
|   `-- vendor
`-- ubuntu-24.04
    |-- 7.0.7
    `-- vendor
```

3.  **Configure:** Proceed to the main "Configure the ALS-U EPICS environment" steps, but ensure you `source` the `setEpicsEnv.bash` script from the correct subdirectory within *your chosen alternative path* (e.g., `~/epics-training-alt/1.1.1/ubuntu-22.04/7.0.7/setEpicsEnv.bash`).


## Summary
In this section, you learned the fundamental steps to get started with the ALS-U EPICS environment on your local Linux `x86_64` machine. You successfully cloned the environment repository using git and learned how to activate a specific version by sourcing the corresponding `setEpicsEnv.bash` script. Finally, you verified the setup by confirming that essential EPICS command-line tools are available in your path. You are now ready to use this configured environment for running IOCs and developing EPICS applications.

