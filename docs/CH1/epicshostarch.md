# ALS-U EPICS Environment Design Principles (`linux-x86_64` + OS-Specific Folders)

**Note:** This section discusses the underlying design choices for the EPICS environment's architecture and cross-distribution support. While important for a deep understanding, some concepts related to build systems, `EPICS_HOST_ARCH`, operating system specifics, library compatibility (like `glibc`), and Application Binary Interfaces (ABIs) may be considered advanced. A full grasp of these details is not required for basic environment usage if following standard procedures.

## Introduction

This section covers the in-depth design principles and fundamental reasons why the ALS-U EPICS environment standardizes on `EPICS_HOST_ARCH=linux-x86_64` for building core EPICS and its modules, while simultaneously utilizing OS-specific folders (e.g., `debian-12`, `rocky-8.10`) to manage distribution-level differences. This combined approach addresses the significant challenge of ensuring consistent EPICS operation across different Linux distributions and their various versions, which inherently vary in system libraries, package managers, and configurations. It leverages industry standards while ensuring adaptability across supported operating systems.

## Principle One: Adherence to Linux Architecture Standards (`linux-x86_64`)

A core principle is to align with broader Linux ecosystem standards. The use of architecture names like `linux-x86_64` (instead of distribution-specific names like `rocky8-x86_64`) is rooted in historical conventions, standardization, and practical considerations within the Linux world. The `EPICS_HOST_ARCH` variable is central to the EPICS build system, guiding the selection of compiler flags, linker options, and determining the output directories for compiled binaries and libraries (e.g., `bin/linux-x86_64`, `lib/linux-x86_64`).

### Historical Context
In the early days of Linux, standardized names (i386, alpha, sparc) were crucial for distinguishing builds for different CPU architectures. With the advent of 64-bit x86 architecture around 2003 (`AMD64/Intel64`), `x86_64` quickly became the vendor-neutral standard adopted by major distributions (Debian, Red Hat, etc.). EPICS adopted these conventions for its `EPICS_HOST_ARCH` variable to identify build targets.

### Cross-Distribution Compatibility
A common architecture name (`x86_64`, `arm64`) allows upstream projects (including EPICS itself and many modules) and developers to create builds that are fundamentally compatible at the CPU instruction set level across various Linux distributions running on that hardware.

### Separation of Concerns
`x86_64` describes the hardware architecture, while the distribution version (Debian 12, Rocky 8) describes the software stack (specific library versions like glibc, kernel, tools). Keeping these separate avoids confusion and simplifies compatibility reasoning, particularly concerning critical system library differences and potential ABI incompatibilities between distributions.

### Stability and Longevity
Architecture names are stable, while distribution versions change frequently. Tying architecture names to distribution versions would create an unstable and cumbersome naming scheme.

### Upstream and Tooling Standards
Build tools (GCC, Autotools, Make) and package managers (APT, RPM/DNF) rely on these standardized architecture names, simplifying development, builds, and packaging.

### Historical Convention and Community Adoption
The standard was adopted early and changing it would cause disruption without significant benefit.

## Principle Two: Avoiding Unnecessary Build Complexity

A related principle is to minimize redundant effort and complexity. Using distribution-specific architecture names like ``rocky8-x86_64` for `EPICS_HOST_ARCH` was rejected because it would lead to significant inefficiencies and risks:

* **Massive Duplication of Effort:** Requiring separate, nearly identical EPICS Base/module builds for every supported OS version, wasting significant build time.
* **Increased Complexity and Maintenance Burden:** Fragmenting build artifacts across numerous directories (e.g., `bin/debian12-x86_64`, `lib/rocky8-x86_64`) and multiplying the effort required for updates, patches, and testing across all targets.
* **Potential for Inconsistencies:** Increasing the risk of subtle, unintended differences creeping into builds for different OS versions over time.

Furthermore, this structure simplifies the process of adding support for new OS versions; it typically involves creating a new OS-specific folder to manage its unique dependencies and configurations, while leveraging the existing `linux-x86_64` core components.

## Principle Three: Managing Distribution-Specific Variations (`debian-12`, `rocky-8.10` Folders)

While standardizing on `EPICS_HOST_ARCH=linux-x86_64` ensures architectural compatibility for the core EPICS build, it's recognized that critical differences exist *between the distributions themselves*. Therefore, a key principle is to manage these variations explicitly using OS-specific folders.

Even when running on the same `x86_64` hardware, distributions like Debian 12 and Rocky Linux 8 differ in:

* **System Library Versions:** Versions of core libraries like `glibc`, `libstdc++`, `OpenSSL`, `readline`, etc., can vary, potentially leading to runtime errors if a binary built against a newer library is run on a system with an older one.
* **Package Management:** Different tools (`apt` vs. `dnf`) and package naming conventions require distinct procedures for installing prerequisites.
* **Filesystem Layout:** Standard locations for libraries, headers, or configuration files might differ slightly, requiring path adjustments.
* **Available Dependencies:** Specific versions or availability of required third-party tools and libraries (e.g., compilers, Python versions, specific development packages like `libreadline-dev` vs `readline-devel`) can vary.

The OS-specific folders (`debian-12`, `rocky-8.10`, etc.) within the ALS-U EPICS environment are designed according to the principle of isolating and managing these distribution-level variations. They act as adapters, providing the necessary "glue" for the standardized `linux-x86_64` binaries to function correctly on each specific OS. Their purpose typically includes:

* **Managing OS Dependencies:** Providing manifests or scripts listing required OS packages (e.g., specific library versions, tools) to be installed via the native package manager (`apt` or `dnf`).
* **Providing Wrapper Scripts:** Using scripts to handle differences in paths (e.g., adjusting `PATH`) or setting necessary environment variables (like `LD_LIBRARY_PATH`, used carefully) needed for tools on a specific OS.
* **Hosting Pre-compiled Dependencies (If Necessary):** In some cases, they might contain specific external libraries or tools pre-compiled for that particular OS version if they cannot be easily managed otherwise.
* **Symbolic Links:** Pointing to the correct version or location of system libraries if needed to resolve path or version conflicts.
* **Configuration Overrides:** Potentially working with EPICS build system overrides (like `CONFIG_SITE.*` files) to define distribution-specific library paths or compiler flags needed when building applications *against* the core EPICS installation on that OS.

This allows the core EPICS components (Base, support modules, IOC applications), built once for the common `linux-x86_64` architecture, to function correctly by resolving distribution-specific needs through these dedicated folders.

## A Combined Approach for Robustness, Adaptability, and Long-Term Simplicity

The ALS-U EPICS environment design principles culminate in a strategic combination:

* **Follow Standards**: Adhere to the stable, standardized `linux-x86_64` architecture identifier (via `EPICS_HOST_ARCH`) for building core EPICS components, maximizing compatibility and following widespread Linux/EPICS conventions.
* **Isolate and Manage Variation**: Utilize OS-specific folders (e.g., debian-12, rocky-8.10) to effectively manage the unavoidable differences in dependencies, paths, and configurations between specific Linux distributions.

This hybrid approach results in an environment that is not only robust and adaptable but also significantly simpler for long-term maintenance. It benefits from architectural standardization while precisely accommodating the nuances of supported operating systems. Crucially, it avoids the significant inefficiencies and maintenance overhead associated with building and managing separate, nearly identical EPICS cores for each OS version.

Furthermore, this clear separation simplifies lifecycle management. This is especially critical considering the typical lifecycle mismatch: operating systems often have support lifetimes of only 5-10 years, whereas accelerator control systems must remain operational and maintainable for potentially 25 years or more. By building the core EPICS components against the stable `linux-x86_64` architecture, we decouple them from specific, transient OS versions. The OS-specific folders then act as an adaptable layer, allowing the long-lived core system to integrate with whichever operating systems are current during its extended lifespan. Adding support for a new OS version primarily involves creating its specific folder, while decommissioning support for an end-of-life OS becomes a straightforward task of removing its folder. This approach ensures the control system's long-term viability and simplifies OS migration paths over decades, without requiring fundamental re-engineering of the core components within EPICS base and its IOC applications overall. This focus on a standardized core with isolated adaptations is key to maintaining a manageable and sustainable EPICS environment over its required operational lifetime.