# Introduction to the ALS-U EPICS Environment Documentation

Welcome to the documentation for the ALS-U EPICS Environment setup. This guide is designed to assist in developing and testing Input/Output Controllers (IOCs) for the Advanced Light Source Upgrade (ALS-U).

This environment is designed to **simplify** the development process, ensure code quality, and promote consistency across all ALS-U IOC projects. Furthermore, it aims to remove tedious and error-prone procedures often associated with traditional EPICS IOC development and deployment.

This documentation is built and published through our Continuous Integration (CI) pipeline, ensuring that you always have access to the latest information.

You can view the complete documentation online via the CI published GitLab Pages:

https://alsu.pages.als.lbl.gov/epics/alsu-epics-environment

---

## CONTENTS

* **1: Getting Started with the ALS-U EPICS Environment**
    * [About This Documentation](README.md)
    * [Install the EPICS Environment](installation.md)
    * [Test the Environment Setup](testenv.md)

* **2: Your First IOC and Continuous Integration**
    * [Create Your First IOC](yourfirstioc.md)
    * [Expand Your First IOC](addioctofirst.md)
    * [Use CI with GitLab](ciwithgitlab.md)

* **3: Example - IOC with TCP Device (Asyn/StreamDevice)**
    * [Part 1: Configure the Second IOC](yoursecondioc.md)
    * [Part 2: Create the TCP Simulator](iocsimulator.md)
    * [Part 3: Test IOC-Simulator Communication](secondiocwithsim.md)

* **4: Environment Maintenance**
    * [Maintenance Overview](maintenance.md)
    * [Initial Preparation](preparation.md)
    * [Prepare a New Release](newrelease.md)