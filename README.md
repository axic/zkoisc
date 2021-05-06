# zkOISC

### unileq32

We call this machine the unileq32, which is inspired by [unileq](https://alecdee.github.io/unileq/index.html)
and [subleq](https://en.wikipedia.org/wiki/One-instruction_set_computer#Subtract_and_branch_if_less_than_or_equal_to_zero).

[One-instruction set computers (OISC)](https://en.wikipedia.org/wiki/One-instruction_set_computer), as the name states,
have a single instruction with usually three or four operands.
They have unlimited access to memory, and there is no code and data separation, allowing for self-modifying code.
Execution starts at `pc = 0` and operands are read in the order of `a`, `b`, `c` from memory at `pc`, `pc + 1`, `pc + 2`,
respectively.

The complete interpreter loop in pseudocode:
```python
// Indexing the memory extends it with zero-filled new elements,
// i.e. out-of-bounds read returns 0.
let mem: u32[]
let pc: u32
while true:
    let a = mem[pc]
    let b = mem[pc + 1]
    let c = mem[pc + 2]
    if a == 0xffffffff:
        # syscall, args are in b and c
        if c == 0:
            stop()
        elif c == 1:
            putchar(mem[b])
        elif c == 2:
            mem[b] = getchar()
        else:
            panic()
        pc = pc + 3
    else:
        if mem[a] <= mem[b]:
            pc = c
        else:
            pc = pc + 3
        mem[a] = mem[a] - mem[b]
```

This matches exactly what unileq does with the exception of being entirely 32-bits and not 64-bits.

We have modified the unileq assembler/interpreter so that it dumps the assembled program.

The following code is based on unileq's "print usage" example:
```
loop: len ?+4  neg  #if [len]=0, exit
      0-1 data 1    #print a letter
      ?-2 neg  loop #increment pointer and loop
data: 72 101 108 108 111 32 122 107 79 73 83 67 33
neg:  0-1
len:  len-data
```

It compiles to the following (24 cells, the remaining 40 are padding zeroes):
```
"0x00000017", "0x00000005", "0x00000016", "0xffffffff", "0x00000009", "0x00000001", "0x00000004", "0x00000016",
"0x00000000", "0x00000048", "0x00000065", "0x0000006c", "0x0000006c", "0x0000006f", "0x00000020", "0x0000007a",
"0x0000006b", "0x0000004f", "0x00000049", "0x00000053", "0x00000043", "0x00000021", "0xffffffff", "0x0000000e",
"0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000",
"0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000",
"0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000",
"0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000",
"0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000", "0x00000000"
```

It will print the "Hello zkOISC!" message.

### Build

```sh
zokrates compile -i oisc.zok
```

### Run

```sh
cat helloworld.input | ./zokrates compute-witness --stdin --abi
```
