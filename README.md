# Vizzium: Z80 Binary Debugger

Vizzium is a Z80 binary debugger designed to facilitate the debugging process of Z80 binary programs. It leverages the capabilities of the emuzeta80 emulator, providing a simple vi-like text-based interface (TUI) powered by ncurses.

## Features

- **Z80 Emulation:** Utilizes emuzeta80, a simple (and unfinished) Z80 emulator I developed in C++.
- **Load binary files:** Vizzium can load Z80 binary files, allowing you to inspect and debug your programs step by step.
- **Instruction Execution:** Execute Z80 instructions one at a time and observe the CPU status.
- **TUI Interface:** The text-based user interface is inspired by vi, providing a familiar environment for users comfortable with vi or vim.

## Getting Started

### Prerequisites

- c++ compiler (gcc)
- ncurses library
- emuzeta80 library (include as a submodule in git repository)

### Building

1. Clone the repository:

    ```bash
    git clone https://codeberg.com/iflumpi/vizzium
    cd vizzium
    ```

2. Build emuzeta80: 

    ```bash
    cd lib/emuzeta80
    make
    ```

3. Build vizzium: 

    ```bash
    cd ../..
    make
    ```

### Usage

1. Execute vizzium:

    ```bash
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib/emuzeta80/build ./build/vizzium
    ```

2. Load a binary file with z80 program

    ```
    :load examples/screen.bin
    ```

3. Execute Z80 instructions step by step

    ```
    :step (or press n in normal mode)
    ```

## License

This project is licensed under the LGPL3 License - see the [LICENSE.md](LICENSE.md) file for details.