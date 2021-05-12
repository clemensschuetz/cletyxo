# Cletyxo - An operating system in Rust

## Roadmap

1. Cletyxo Hubble (aka Cletyxo 0.5) :: The first release of Cletyxo. Planned: Text outputs and other basic OS stuff :)
2. Cletyxo Andromeda (aka Cletyxo 1.0) :: The first "real" release of Cletyxo. Planned: Very basic ui (very, very basic)
As you can see, these release are quite frequent, this is because it will motivate me. 

## Components of Cletyxo
`/kernel`: The kernel
`/drivers`: Drivers for the kernel
`/arch`: Architecture specific things
`/boot`: The bootloader
`/usr`: Userland specific things
    - `/apps`: The default apps for Cletyxo
    - `/de`: The Destkop Environment for Cletyxo
`/clib`: C**l**etyxo **Lib**rary - A library for cletyxo!
    - `/gphl` (LibGraphical): Widgets, Windows and more
    - `/std`  (LibStd): STD-like functions (FS, ...)
    - `/cmd`  (LibCmd): Cmd-utils like ArgParse and more
`/targets`: Targets to build Cletyxo for
`/fonts`: Default fonts
`/patches`: Patches to make software run on cletyxo
`/scripts`: Scripts!
`/thirdparty`: Thirdparty things
`/config`: Configs for the OS (Example: startup.nsh)
`/OVMFbin`: Binaries for QEMU

Created during development:
`/build`: Here, all of the objects and more get copied to. Do not look at it, it looks awful.
`/bin`: Here, the final image ends up.