# c64vm - A 64-bit Register-Based Virtual Machine in C

Welcome to the **c64vm** project! This repository hosts an upgraded version of the previously established [c16vm](https://github.com/noah1400/c16vm), now equipped with 64-bit capabilities. This virtual machine is being developed to provide a powerful and versatile platform for executing code within a simulated 64-bit environment.

## Table of Contents

- [Introduction](#introduction)
- [Build](#build)
- [Usage](#usage)
- [License](#license)

## Introduction

The **c64vm** project aims to create a 64-bit register-based virtual machine. This virtual machine is designed to execute code written in a custom assembly-like language within its simulated environment. As an upgrade to the [c16vm](https://github.com/noah1400/c16vm) project, **c64vm** extends its capabilities by using a 64bit address range.

Please note that this project is currently a work in progress. As such, expect ongoing updates, enhancements, and potentially some rough edges. Contributions, suggestions, and feedback are greatly appreciated as i strive to refine and improve the virtual machine.

## Build

To build **c64vm**, follow these steps:

1. Clone this repository to your local machine.
2. Navigate to the repository directory.
3. Run the following command to compile the project:

    ```sh
    gcc -o c64vm c64mem.c c64cpu.c c64mm.c c64util.c c64vm.c c64main.c -Iinclude -std=c99 -Wall -Wextra -Wpedantic
    ```

Please keep in mind that this project is a work in progress, and there might be changes to the build process as development progresses.

## Usage

As the project is currently under active development, detailed usage instructions are forthcoming. However, once complete, you can expect a comprehensive guide on how to use the **c64vm** virtual machine, including assembling code, running programs, and interacting with the simulated environment.

## License

This project is licensed under the [MIT License](LICENSE), meaning you are free to use, modify, and distribute the software according to the terms outlined in the license.

---

**Disclaimer:** As a work in progress, the **c64vm** project is continuously evolving. While i am striving to provide accurate information, some details may change over time.
