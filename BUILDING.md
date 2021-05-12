# How to build Cletyxo

## Dependencies

- Rust Toolchain Nightly
- Make
- Qemu for testing

## Build

You can build Cletyxo using the `make` command.

### Building for specific architectures

By default, you will build for x86_64 architecture. To change this,
just use the flag `ARCH`. 
As an Example, to build for RISC-V, use:
`make ARCH=riscv`