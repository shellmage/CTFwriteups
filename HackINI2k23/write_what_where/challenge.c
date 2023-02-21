#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define NAME_SIZE 0x160 //352

void input_str(char *msg, char *str, size_t size) ;
void disable_buffering(){
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

void header(){
    puts("Welcome !");
}

void menu(){
    puts("1) Enter Your Name");
    puts("2) Print your Name");
    puts("3) Execute Random function"); 
    puts("4) Exit");
    printf("Choice: ");
}

void get_name(char name[NAME_SIZE]){
    input_str("Name: ",name,NAME_SIZE);
}

void print_name(char name[NAME_SIZE]){
    printf(name); // format string :)
}

void func(){}

int main(int argc, char* argv[]){
    char name[NAME_SIZE];
    int choice ;

    disable_buffering();

    mprotect((void*)(((unsigned long)&func >> 12) << 12),0x1000,PROT_READ | PROT_WRITE | PROT_EXEC); //This is a gift for you

    puts("Welcome\nTry to find out how to pwn me!\nIt's not hard, I swear!");
    
    while (1)
    {
        menu();
        scanf("%d",&choice);
        switch (choice)
        {
            case 1:
                get_name(name);
                break;
            case 2:
                print_name(name);
                break;
            case 3:
                func();
                break;
            default:
                exit(0);
                break;
        }
        
    }
    exit(0);
}

void input_str(char *msg, char *str, size_t size)
{
    ssize_t num_bytes;

    fputs(msg, stdout);
    num_bytes = read(STDIN_FILENO, str, size - 1);
    if (num_bytes == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    } else {
        str[num_bytes] = '\0';
    }
    return;
}

