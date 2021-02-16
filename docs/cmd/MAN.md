# The 'man' Command

### Description

> Shows the manual for a specific command.

### Short for

> man is short for manual

### Usage

- `man <command> --flags`


### Definitions:

- `command`: Can be any existing command. It will show the manual for this command

### Possible Parameters

- Command-Options:
  - Can be any command as long as it exists.


### Flags

- -force: Forces the action


### Errors & Fixes

- `man001` | `You do not have the permission to do this`:
  - Description:
    - Happens when you do not have enough permission to execute a command.
  - Fix:
    - If you have permission to do so, execute the command with `sudo` infront of it..
- `man002` | `The entered command does not exist`:
  - Description:
    - Happens when the command in `command` does not exist.
  - Fix:
    - Does the command have a typo? Does it actually exist?

### Examples

- `man sudo` | Will show the manual for the `sudo` command.
- `man man` | Will show the manual for the `man` command.
- `man cd` | Will show the manual for the `cd`command.