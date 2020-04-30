#define n_elems(array) sizeof(array)/sizeof(array[0])
#define CMD_LS 0
#define CMD_ID 1
#define CMD_WHOAMI 2
#define CMD_ECHO 3
#define CMD_PRINTF 4
#define CMD_CAT 5
#define CMD_PWD 6
#define CMD_CD 7
#define CMD_HELP 8
#define CMD_EXIT 9
#define CMD_UNAME 10

#define COMMAND_NOT_FOUND 512
#define ARITY_ERROR 1024
#define ARG_ERROR 2048

#define LISTING_1 "secretfolder\n"
#define LONG_LISTING_1 "drwxr-xr-x 2 root root 4096 janv.  3 00:54 secretfolder\n"

#define LISTING_2 "flag.txt\n"
#define LONG_LISTING_2 "-rw-r--r-- 1 root mate 462 janv.  3 00:41 flag.txt\n"
#define FLAG_CONTENT "\n"\
"############################################################################\n"\
"# Of course, you will not find the flag in here ;)                         #\n" \
"# The real flag of this challenge is in "BLUE("/challenge/flag.txt")"                #\n" \
"# You forgot that you were in a fake terminal? what did you expect? xD     #\n" \
"#                                                                          #\n" \
"############################################################################\n"

#define RED(s) "\e[01;91m"s"\e[00m"
#define BLUE(s) "\e[01;34m"s"\e[00m"

#define UNAME_OUTPUT "Linux"
#define UNAME_LONG_OUTPUT "Linux pwnable 2.6.32-573.3.1.el6.x86_64 #1 SMP Mon Aug 10 09:44:54 EDT 2015 x86_64 x86_64 x86_64 GNU/Linux"

typedef struct command_ll {
    unsigned int command;
    char* args;
} command_ll;


unsigned int ls(char*);
unsigned int id(char*);
unsigned int whoami(char*);
unsigned int echo(char*);
unsigned int cat(char*);
unsigned int pwd(char*);
unsigned int help(char*);
unsigned int cd(char*);
unsigned int uname(char*);
void read_cmd(char*, size_t);
unsigned int parse_cmd(char*, command_ll*);
unsigned int process_cmd(command_ll*);
void clean_cmd(command_ll*);

