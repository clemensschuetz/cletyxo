# The 'sudo' Command

### Description

> Will force the execution of any command

### Short for

> sudo stands for 'super user do' which means that you will execute this command as the root.

### Usage

- `sudo <full command>`


### Definitions:

- `full command`: Use any **full** command ('cmd options flags').


### Possible Parameters

- Full Command-Options:
  - Everything can go here

### Flags

- No flags, use the flags of the wanted command.

### Errors & Fixes

- `sudo001` | `You do not have the permission to do this`:
  - Description:
    - Happens when you do not have enough permission to execute a command. 
  - Fix:
    - No fix available, this only happens if you do not have the permissions to do it.

- `sudo002` | `Command chaining is not allowed`:
  - Description:
    - Happens when you chain sudos. (e.g.: `sudo sudo sudo sudo ...`)
  - Fix:
    - No fix available.

- Any other error:
  - These errors are made by the system. Sudo does not throw any errors except for 001 & 002.


### Examples

- `sudo man cd` | Will force the execution of the `man cd` command
- `sudo mng set display-size true --auto-convert -verbose` | This will force the mng command, which will set the display-size to 1 (`--auto-convert` forces conversion of `true` (`boolean`) to the required type, `int` (This would be `1`)). As you can see, you can use flags and more
- `sudo sudo sudo sudo sudo sudo sudo sudo sudo sudo sudo mgn check` | This throw an error as the sudo002 is triggered.

