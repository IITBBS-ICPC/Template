# CP Theory

## Compilation commands

Scripts for compilation and setting stack size.

```shell
# Settings
# CF_CP_STACK_SIZE contains the stack size used by Codeforces (in KiB)
: ${CF_CP_STACK_SIZE:=262144}

function cprep() {
  ulimit -s unlimited
}

alias m++="cprep && g++ -static -DMYCODER_JUDGE -O2 -std=c++17"
```
