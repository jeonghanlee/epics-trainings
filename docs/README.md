# ALS-U EPICS environment training

Learn to run an Input/Output Controller (IOC), connect simulated devices,
and create reusable configurations with EPICS-env 1.5.0 and EPICS Base
7.0.10. The exercises use local simulators rather than physical hardware.

## Scope and intended readers

This book is for engineers learning the ALS-U EPICS environment. You need
basic Bash commands and a text editor. Install the
[prerequisites](prerequisites.md), then work through the chapters in order.

The binary distribution supplies Base and module libraries. Chapters 2
through 5 compile example IOC applications against those libraries;
they do not rebuild the environment. Chapter 6 uses `iocsh.bash` to load
installed support without an application executable.

Production deployment, access security policy, and facility services are
outside these lessons. Use a test host without competing IOC instances
or simulators on the exercise ports. Use one active IOC per PV prefix.

## Learning path through the book

| Chapter | What you can do afterward |
| --- | --- |
| [1. Environment](CH1/) | Select the matching OS tree and read and write one PV |
| [2. IOC applications](CH2/) | Generate, build, and run several boot instances |
| [3. Device communication](CH3/) | Send commands to a TCP simulator through StreamDevice |
| [4. Reusable configuration](CH4/) | Configure multiple devices and load 32-channel databases |
| [5. Build and shell configuration](CH5/) | Explain dependency paths, installation paths, and command syntax |
| [6. Module loader](CH6/) | Run the same devices with installed support through `iocsh.bash` |

Each lesson names the files to save, the directory to work in, and the
result to check. Commands for Bash and the IOC prompt are labeled
separately. Stop the preceding IOC before starting another with the same
PV names. Save your source files for the later lessons.

Use the [glossary](glossary.md) to check terminology and the
[environment variable reference](misc/EPICSEnv.md) for local discovery.
The [online book](https://jeonghanlee.github.io/epics-trainings/)
contains the published training, and `SUMMARY.md` defines the book's
complete navigation.
