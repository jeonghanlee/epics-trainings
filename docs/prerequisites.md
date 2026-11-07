# Install the training prerequisites

Install Linux tools needed to run the binary distribution, build the
example applications, and run local device simulators. Use the package
manager for the operating system you selected in
[installation](CH1/01.01.installation.md).

1. On Debian 12/13 or Ubuntu 24.04/26.04, install these packages:

   ```bash
   sudo apt update
   sudo apt install -y build-essential perl git libevent-dev libreadline-dev libtirpc-dev binutils
   sudo apt install -y socat bc procps iproute2 netcat-openbsd
   ```

   On Rocky Linux 8.10/10.2, use these commands instead:

   ```bash
   sudo dnf install -y gcc gcc-c++ make perl git libevent-devel readline-devel libtirpc binutils
   sudo dnf install -y socat bc procps-ng iproute findutils which
   ```

   The development packages also supply their runtime libraries. Naming
   them avoids assuming that Debian 12 uses Ubuntu's `t64` package names.
   Commands need administrator privileges and working package repositories.

2. In an ordinary Bash terminal, verify the tools:

   ```bash
   command -v bash perl git make gcc g++ readelf
   command -v socat bc mktemp pgrep ss
   ```

   Each name must print a path. `readelf` comes from `binutils` and is
   required by the module loader. `pgrep` supports simulator cleanup.

3. After activating EPICS in Chapter 1, check the executable's libraries:

   ```bash
   ldd "$(command -v softIocPVX)"
   ```

   Every dependency must resolve to a file. `not found` identifies a
   missing runtime library.

## Optional parallel launcher

The main lessons launch simulators in separate terminals. GNU Parallel
is optional. On Debian or Ubuntu, install it only if you use the optional
launcher commands:

```bash
sudo apt install -y parallel
parallel --version
```

On Rocky Linux, continue with separate terminals if GNU Parallel is not
available from your configured repositories. No additional repository is
required for the main lessons.
