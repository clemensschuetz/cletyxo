# The 'cd' Command

### Description

> Changes the directory allowing you to execute commands in it.

### Short for

> cd is short for 'change directory'

### Usage

- `cd <directory-chain>`


### Definitions:

- `directory-chain>`: Can be any directory / A chain of directories


### Possible Parameters

- DirectoryChain-Options:
  - Any directory. The path has to be relative to the current location. 
    - The following tricks could be helpful.
      - If you want to go **back** one directory, use  `..` as the directory.
      - You can use **variables** in this command. To learn more about variables, please read  [this](../environment/VARIABLES.md).
      - A cd statement will be executed in a chronological order. This means that, if you are in the folder `documentation` you can use `cd ../gnu-efi/../documentation` and you would end up in the same location. Why? Let's go through this. First, you go **out** of the `documentation` folder by using `..`. Next, you **go into** the `gnu-efi` folder. In the next step you **go back** one directory. After this, you **enter** the `documentation` folder again.


### Flags

- None

### Errors & Fixes

- `cd001` | `You do not have the permission to do this`:
  - Description:
    - Happens if you do not have enough permission to execute a command. 
  - Fix:
    - If you have permission to do so, execute the command with `sudo` infront of it.
    - 
- `cd002` | `The directory does not exist`:
  - Description:
    - Happens when you try to enter a directory that does not exist.
  - Fix:
    - Try to find the mistake in you command.

### Examples

- `cd ../gnu-efi/../documentation` | Assuming you are in `documentation`, this will chain directories so that you will end up in documentation again.
- `cd $HOME` | It does not matter where you are, this will send you into the `home` directory, as you use the **variable** `$HOME` which stands for the home directory.
- `cd /dev/cletyxo` | This will send you into a folder.
