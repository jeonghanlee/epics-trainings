# Introduction to the ALS-U EPICS Environment Documentation

Welcome to the documentation for the ALS-U EPICS Environment setup. This guide is designed to assist in developing and testing Input/Output Controllers (IOCs) for the Advanced Light Source Upgrade (ALS-U).

This environment is designed to **simplify** the development process, ensure code quality, and promote consistency across all ALS-U IOC projects. Furthermore, it aims to remove tedious and error-prone procedures often associated with traditional EPICS IOC development and deployment.

This documentation is built and published through our Continuous Integration (CI) pipeline, ensuring that you always have access to the latest information.

You can view the complete documentation online via the CI published GitLab Pages:

https://alsu.pages.als.lbl.gov/epics/alsu-epics-environment

---

## 1. Getting Started
- [Installation](CH1/installation.md)
- [Test Environment](CH1/testenv.md)

## 2. First EPICS IOC and Gitlab CI
- [First EPICS IOC](CH2/yourfirstioc.md)
- [Expand the First IOC](CH2/addioctofirst.md)
- [Continous Integration](CH2/ciwithgitlab.md)

## 3. Second EPICS IOC and Serial Simulator
- [Configure the Second IOC](CH3/yoursecondioc.md)
- [Create the TCP Simulator](CH3/iocsimulator.md)
- [Test IOC-Simulator Communication](CH3/secondiocwithsim.md)

## Maintenance
- [Maintenance Overview](maintenance.md)
- [Initial Preparation](preparation.md)
- [Prepare a New Release](newrelease.md)
