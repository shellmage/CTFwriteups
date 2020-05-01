#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"



char* supported_command_strings[] = { "ls", "id", "whoami", "echo", "printf", "cat", "pwd", "cd", "help", "exit", "uname" };
unsigned int supported_command_consts[] = { CMD_LS, CMD_ID, CMD_WHOAMI, CMD_ECHO, CMD_PRINTF, CMD_CAT, CMD_PWD, CMD_CD, CMD_HELP, CMD_EXIT, CMD_UNAME };
int arity[] = { 1, 0, 0, 1, 1, 1, 0 ,1, 0, 0, 1 }; // 0 or 1 arguments to be passed to the command
unsigned int(*command_handlers[])(void*) = { ls, id, whoami, echo, printf, cat, pwd, cd, help, exit, uname };

char* paths[] = { "/", "/secretfolder" };

char* cwd; // current working directory

char* user = "mate";
char* hostname = "pwnable";

unsigned int uname(char* param)
{
    if (param == NULL)
        puts(UNAME_OUTPUT);
    else if (!strcmp(param, "-a"))
        puts(UNAME_LONG_OUTPUT);
}
unsigned int ls(char* param)
{
    char l = 0;
    if (param != NULL)
    {
        if (!strcmp(param, "-l"))
            l = 1;
        else
            return ARG_ERROR;
    }    
    if (cwd == paths[0]) printf(l ? LONG_LISTING_1 : LISTING_1);
    else printf(l ? LONG_LISTING_2 : LISTING_2);
    return 0;
}

unsigned int id(char* param)
{
    if (param != NULL) return ARITY_ERROR;
    puts("uid=1000(mate) gid=1000(mate) groupes=1000(mate),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),118(lpadmin),128(sambashare)");
    return 0;
}

unsigned int help(char* param)
{
    if (param != NULL) return ARITY_ERROR;
    puts("Supported commands\n"BLUE("ls\tid\twhoami\techo\tprintf\ncat\tpwd\tcd\thelp\texit"));
}

unsigned int whoami(char* param)
{
    if (param != NULL) return ARITY_ERROR;
    puts(user);
    return 0;
}

unsigned int echo(char* param)
{
    puts(param);
    return 0;
}

unsigned int cat(char* params)
{
    if (params == NULL) return ARITY_ERROR;
    int i;
    for (i = 0; params[i]; i++);
    for (; i >= 0 && params[i] != '/'; i--);
    i++;
    char* fname = params+i;
    if (!strcmp(params+i, "flag.txt"))
    {
        params[i] = '\0';
        char* current = cwd;
        cd(params);
        if (cwd == paths[1])
            puts(FLAG_CONTENT);
        else
            printf("[-] cat "RED("%s/flag.txt")" : NO SUCH FILE OR DIRECTORY", params);
        cwd = current;
    }
    else
        printf("[-] cat "RED("%s")" : NO SUCH FILE OR DIRECTORY\n", params);
    return 0;
}

unsigned int pwd(char* param)
{
    if (param != NULL) return ARITY_ERROR;
    puts(cwd);
    return 0;
}

unsigned int cd(char* param)
{
    if (param == NULL)
    {
        cwd = paths[0];
        return 0;
    }
    size_t i = 0;
    size_t start = (cwd == paths[1]);
    if (param[i] == '/')
    {
        start = 0;
        i = 1;
    }
    while (param[i] != '\0')
    {
        if (!strncmp(param+i, "..", 2))
        {
            start = 0;
            i += 2;
        }
        else if (!strncmp(param+i, "secretfolder", 12))
        {
            start = 1;
            i += 12;
        }
        else
        {
            puts("[-] No such directory");
            return ARG_ERROR;
        }
        if (param[i] == '/') i++;
    }
    cwd = paths[start];
    return 0;
}

void read_cmd(char* buffer, size_t size)
{
    fgets(buffer, size, stdin);
}

unsigned int parse_cmd(char* buffer, command_ll* head)
{
    size_t i = 0;
    for (i = 0; i < 1024 && buffer[i]; i++)
        if (buffer[i] == '\n') buffer[i] = '\0';
    i = 0;
    for (size_t j = 0; j < n_elems(supported_command_consts); j++)
    {
        size_t len = strlen(supported_command_strings[j]);
        if (!strncmp(buffer, supported_command_strings[j], len))
        {
            head->command = supported_command_consts[j];
            i = len;
            break;
        }
    }
    //head->args = NULL;
    if (i == 0) return COMMAND_NOT_FOUND;
    for (; i < 1024 && buffer[i]; i++)
        if (buffer[i] != ' ' && buffer[i-1] == ' ')
        {
            buffer[i-1] = '\0';
            head->args = buffer+i;
            break;
        }
        else if (buffer[i-1] == ' ')
            buffer[i-1] = '\0';
    return 0;
}

unsigned int process_cmd(command_ll* cmd)
{
    return command_handlers[cmd->command](cmd->args);
}

void clean_cmd(command_ll* cmd)
{
    cmd->args = NULL;
}

void print_prompt()
{
    printf(RED("%s@%s")":"BLUE("%s")" $ ", user, hostname, cwd);
}

void disable_buffering()
{
    setvbuf(stdout, NULL, _IONBF, 0);
}

int main()
{
    disable_buffering();
    cwd = paths[0];
    command_ll* current_command = malloc(sizeof(command_ll));
    char command_buffer[1024];
//    print_banner();
    while (1)
    {
        print_prompt();
        read_cmd(command_buffer, sizeof(command_buffer));
        if (command_buffer[0] == '\n') continue;
        switch (parse_cmd(command_buffer, current_command))
        {
            case COMMAND_NOT_FOUND:
            {
                printf(RED("%s")": command not found, type "BLUE("help")" for available commands\n", command_buffer);
                break;
            }
            default:
            {
                switch (process_cmd(current_command))
                {
                    case ARITY_ERROR:
                    {
                        printf("[-] Wrong number of arguments for the command "BLUE("%s")"\n", supported_command_strings[current_command->command]);
                        break;
                    }
                    case ARG_ERROR:
                    {
                        printf("[-] Invalid argument given to the command "BLUE("%s")"\n", supported_command_strings[current_command->command]);
                        break;
                    }
                }
            }
        }
        clean_cmd(current_command);
    }
}

