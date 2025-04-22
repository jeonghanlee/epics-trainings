# Introduction to the ALS-U EPICS Environment and Documentation

## Welcome

Welcome to the official documentation for the Advanced Light Source Upgrade (ALS-U) EPICS Environment. This guide serves as the central resource for understanding, installing, using, and maintaining the standardized EPICS software environment specifically tailored for developing and deploying Input/Output Controllers (IOCs) at ALS-U.

## Purpose of the ALS-U EPICS Environment

The ALS-U Controls group maintains this EPICS environment to support the development of robust, consistent, and maintainable IOC applications across the facility. It consists of:

* A specific version of **EPICS Base**.
* A defined set of **EPICS Modules** (like Asyn, StreamDevice, Calc, PVXS, etc.) built against that base version.
* Standardized **IOC templates** and **build tools** (like the `generate_ioc_structure.bash` script) to ensure common structure and practices.
* Defined **procedures** for development, testing, deployment, and maintenance.

The primary goals of establishing this environment are to:

* **Simplify** the development process for IOC engineers.
* Ensure high **code quality** and reliability.
* Promote **consistency** across all ALS-U IOC projects, making them easier to understand and manage.
* Reduce tedious and **error-prone procedures** often associated with manual dependency management and deployment in traditional EPICS development.

## Purpose and Scope of This Documentation

This documentation aims to be the comprehensive guide for interacting with the ALS-U EPICS Environment. It covers topics including:

* **Setting up** the development environment on supported systems.
* **Testing** the environment to ensure proper configuration.
* **Developing IOCs** using the provided templates and tools, from basic examples to more complex device communication scenarios (like the Asyn/StreamDevice over TCP example).
* Understanding and utilizing **Continuous Integration (CI)** practices within the ALS-U GitLab framework.
* Procedures for **maintaining** the EPICS environment itself (primarily for environment maintainers).

## Target Audience

This documentation is primarily intended for **engineers, software developers, and scientists** involved in creating, deploying, or maintaining EPICS IOCs for the ALS-U project at LBNL.

While some sections assume familiarity with EPICS core concepts and Linux environments, the initial chapters are designed to guide users new to *this specific ALS-U environment* through the setup and basic usage. Later chapters delve into more specific examples and maintenance tasks requiring deeper EPICS knowledge.

## Documentation Structure (Chapters)

This guide is organized into several chapters, reflecting a typical workflow:

* **CHAPTER 1: Getting Started:** Covers this introduction, how to install the environment (`installation.md`), and how to run initial tests (`testenv.md`). **New users should start here.**
* **CHAPTER 2: Basic IOC Development & CI:** Guides you through creating your first simple IOC (`yourfirstioc.md`), potentially expanding its structure (`addioctofirst.md`), and integrating with GitLab CI (`ciwithgitlab.md`). It also includes background on the build system (`build_overview.md` - *Note: This file was suggested, adjust link if different*).
* **CHAPTER 3: Second EPICS IOC and Serial Simulator:** Provides a detailed walkthrough of creating an IOC for device communication using Asyn and StreamDevice over TCP/IP, including configuring the IOC (`yoursecondioc.md`), creating a simulator (`iocsimulator.md`), and testing the communication (`secondiocwithsim.md`).

* **CHAPTER 4: Advanced IOC Configuration and Startup:**

## How to Use This Documentation

* **New Users:** Start with Chapter 1 (Installation, Testing) and proceed through Chapter 2 (First IOC).
* **Experienced Users:** You may jump directly to relevant sections, such as the device communication example (Chapter 3) or CI integration (Chapter 2).


* **Online Version:** Remember that this documentation is built automatically via CI. The latest official version is always available online at: https://alsu.pages.als.lbl.gov/epics/alsu-epics-environment

## General Prerequisites

Most sections assume you are working in a Linux environment (like Debian 12 or Rocky 8) and have basic familiarity with shell commands, text editors (like `nano`, `vi`, `emacs`), and version control with `git`. Specific EPICS version requirements are detailed in the installation section.

We hope this documentation serves as a valuable resource for your EPICS development work at ALS-U!