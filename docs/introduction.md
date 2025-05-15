# Introduction to the ALS-U EPICS Environment Training Guide

## Welcome

Welcome to the official training documentation for the Advanced Light Source Upgrade (ALS-U) EPICS Environment. This guide serves as the central resource for understanding, installing, using, and maintaining the standardized EPICS software environment specifically tailored for developing and deploying Input/Output Controllers (IOCs) at ALS-U.

This training is hosted on both GitHub Pages and GitLab Pages as part of a set of resources aimed at providing comprehensive guidance for the ALS-U EPICS development workflow.

## Purpose of the ALS-U EPICS Environment

The ALS-U EPICS environment is maintained to support the development of robust, consistent, and maintainable IOC applications across the facility. It is typically distributed as a pre-built package (originating from repositories like [`https://github.com/jeonghanlee/EPICS-env`](https://github.com/jeonghanlee/EPICS-env) and [`https://github.com/jeonghanlee/EPICS-env-support`](https://github.com/jeonghanlee/EPICS-env-support)) and consists of:

* A specific version of **EPICS Base**.
* A defined set of **EPICS Modules** (like Asyn, StreamDevice, Calc, PVXS, etc.) built against that base version.
* Standardized **IOC templates** and **build tools** (including scripts like `generate_ioc_structure.bash`) to ensure common structure and practices.
* Defined **procedures** for development, testing, deployment, and maintenance.

The primary goals of establishing this environment are to:

* **Simplify** the development process for IOC engineers.
* Ensure high **code quality** and reliability.
* Promote **consistency** across all ALS-U IOC projects, making them easier to understand and manage.
* Reduce tedious and **error-prone procedures** often associated with manual dependency management and deployment in traditional EPICS development.

## Purpose and Scope of This Training Guide

This training guide provides a step-by-step walkthrough of the ALS-U EPICS Environment, covering everything from initial setup to advanced configuration and development techniques. It aims to provide users with the knowledge and skills needed to effectively develop, test, and deploy EPICS IOCs for ALS-U.

Key topics covered include:

* **Setting up and verifying** the development environment.
* Developing **basic and advanced IOC applications** using standardized templates and tools.
* Simulating **device communication** for testing and development.
* Utilizing **`iocsh` scripts** and **database templates** for efficient and scalable configuration.
* Understanding the structure and function of key **IOC configuration files** (`st.cmd`, `RELEASE`, `CONFIG_SITE`, `system.dbd`).
* Integrating **Continuous Integration (CI)** practices into the development workflow (details specific to the ALS-U internal GitLab repository).

## Target Audience

This training guide is primarily intended for **engineers, software developers, and scientists** involved in creating, deploying, or maintaining EPICS IOCs for the ALS-U project at LBNL. While its focus is on the ALS-U implementation, much of the content is also applicable and valuable to the broader global EPICS community.

While some sections assume familiarity with EPICS core concepts and Linux environments, the initial chapters are designed to guide users new to *this specific ALS-U environment* through the setup and basic usage. Later chapters delve into more specific examples and advanced topics.

## Training Structure (Chapters)

This guide is organized into chapters designed to lead you through the ALS-U EPICS development process:

* **Chapter 1: Environment Setup and Verification:** Focuses on getting the environment operational, covering installation, initial testing, and understanding host architecture concepts like `EPICS_HOST_ARCH` and OS-specific directories.

* **Chapter 2: First EPICS IOC and GitLab CI:** Guides you through creating and expanding your first basic EPICS IOC within the environment and integrating it with GitLab Continuous Integration (CI) pipelines.

* **Chapter 3: Second EPICS IOC and Device Simulation:** Builds on basic development by demonstrating how to configure an IOC for device communication and simulating that communication using a TCP-based simulator.

* **Chapter 4: Advanced IOC Configuration and Startup:** Delves into more complex configuration techniques, including working with `iocsh` scripts, managing multiple devices in `st.cmd`, using database templates and substitution, and understanding the phases of the IOC startup sequence. Includes development of more advanced simulators.

* **Chapter 5: Understanding IOC Application Configuration:** Provides a deep dive into the critical configuration files that define an IOC's behavior, such as the style of `st.cmd`, `configure/RELEASE`, `configure/CONFIG_SITE`, and `system.dbd`.

*Note: This structure reflects the current organization of the training guide and may evolve over time.*

## How to Use This Training Guide

* **New Users:** Start with Chapter 1 (Installation, Testing) and proceed through Chapter 2 (First IOC) to build a foundational understanding.

* **Experienced Users:** You may jump directly to relevant sections based on your needs, such as advanced configuration (Chapter 4) or detailed file explanations (Chapter 5).

## Online Version

The latest official version of this training guide is always available online at the ALS-U internal Gitlab Pages site:
[`https://jeonglee.pages.als.lbl.gov/epics-trainings/`](https://jeonglee.pages.als.lbl.gov/epics-trainings/)

The latest mirror version of this training guide is always available online at the GitHub Pages site:
[`https://jeonghanlee.github.io/epics-trainings/`](https://jeonghanlee.github.io/epics-trainings/)

## General Prerequisites

Most sections assume you are working in a Linux environment (like Debian 12 or Rocky 8) and have basic familiarity with shell commands, text editors (like `nano`, `vi`, `emacs`), and version control with `git`. Specific EPICS version requirements are detailed in the installation section.

We hope this training guide serves as a valuable resource for your EPICS development work at ALS-U and at any EPICS facilities as well!