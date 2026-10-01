# Prerequisites: Required System Packages

This page lists the operating system packages required to complete the hands-on lessons, in addition to the EPICS environment itself ([Installation](CH1/01.01.installation.md)). Install them **before** starting Chapter 1 to avoid interruptions.

## Lesson Overview

In this lesson, you will learn:
* Which system packages each chapter's hands-on exercises require
* How to install them on the supported Linux distributions
* How to verify the installation

## Required Packages

| Package | Used in | Purpose |
|---|---|---|
| `libevent-pthreads-2.1-7t64`, `libevent-2.1-7t64`, `libevent-extra-2.1-7t64`, `libevent-openssl-2.1-7t64` | CH1 | Runtime libraries required by `softIocPVX` (missing on minimal installs) |
| `libevent-dev` | CH2 | Development headers/symlinks to link `libevent` when building an IOC (`cannot find -levent_core` without it) |
| `socat` | CH3, CH4 | TCP servers and TCP-PTY bridge used by the simulator scripts |
| `bc` | CH4 (04.04) | Floating-point temperature simulation in `tc32_emulator.bash` |
| `parallel` (optional) | CH4 (04.02, 04.04) | Running multiple simulator instances with one command |
| `mktemp` (coreutils) | CH4 (04.04) | Temporary file handling in `tc32_emulator.bash` (usually pre-installed) |

The base tools `bash`, `perl`, `git`, `make`, and a C compiler are already covered by the EPICS environment setup.

## Installation

### Ubuntu 24.04 / Debian 12/13

```shell
$ sudo apt update
$ sudo apt install -y \
    libevent-pthreads-2.1-7t64 \
    libevent-2.1-7t64 \
    libevent-extra-2.1-7t64 \
    libevent-openssl-2.1-7t64 \
    libevent-dev \
    socat bc parallel
```

> **Note:** On Ubuntu 24.04 (noble), the `libevent` runtime packages carry the `t64` suffix due to the 64-bit `time_t` transition. If `apt` reports the package as not found, check the exact name with `apt search libevent-pthreads`.

### Rocky 8.10 / 10.2

```shell
$ sudo dnf install -y libevent libevent-devel socat bc parallel
```

## Verification

```shell
$ command -v socat bc parallel mktemp
$ ldconfig -p | grep libevent_pthreads
```

All four commands should print paths, and `ldconfig` should list `libevent_pthreads-2.1.so.7`. If `softIocPVX` still reports a missing `libevent` library after installation, run `sudo ldconfig` once to refresh the loader cache.

## If the Package Manager Is Unavailable

If `apt`/`dnf` cannot reach a repository, download the `.deb` files directly from `http://archive.ubuntu.com/ubuntu/pool/` and install them with `sudo dpkg -i <file>.deb`. For example, the `bc` package for Ubuntu 24.04 lives under `pool/main/b/bc/`.
