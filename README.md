# Cletyxo
Open-source userfriendly operating system.


## Installation

There is no stable release candidate for Cletyxo at the moment.

## Basic Documentation

Filetypes:
    - **.rck files**  These files, called Rocketfiles are the executables of Cletyxo.


Starter applications:
    - **Catapult**: This program allows you to launch any .exe from windows on your cletyxo machine.
    - **Notepad**: A basic text editor


## How to get started
[More comming soon :)]
First, make the gnu-efi bootloader by using the ``cd gnu-efi`` command. Then use ``make bootloader`` in order to make it. Now, go into the kernel
by using ``cd ../kernel`` and in it, there are a few commands that you could execute.
By using ``make kernel`` you make the kernel
Then follow up with ``make buildimg`` to make the img.
If you are on Linux, use the ``make run`` command in order to run it using QEMU.
On Windows, double-click the run.bat.

### Make-Shortcuts
Tired of using 2 commands to build the kernel?
Use ``make pfb`` to execute ``make kernel`` first and then ``make buildimg``. This saves you some time. Pfb stands for ``Prepare for boot``
If you are on Linux, use ``make boot`` in order to first make the kernel, then build the img and then also run it.