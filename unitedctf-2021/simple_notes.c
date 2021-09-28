#include <stdio.h>
#include <stdlib.h>

#define MAX_NOTES   256
#define MIN_SIZE    8
#define MAX_SIZE    1024

typedef struct Note {
    int importance;
    char *content;
}Note;

typedef struct Notebook {
    Note notes[MAX_NOTES];
    int current;
}Notebook;

Notebook notebook;

int read_int() {
    char buffer[20];
    fgets(buffer, sizeof(buffer), stdin);
    return atoi(buffer);
}

int read_index() {
    printf("Index: ");
    unsigned int index = read_int();
    if (index >= notebook.current+1) {
        puts("Invalid index");
        return -1;
    }
    return index;
}

unsigned int read_size() {
    unsigned int size;
    while(1) {
        printf("Size: ");
        size = read_int();
        if (size <= MIN_SIZE)
            puts("Too small");
        else if (size > MAX_SIZE)
            puts("Max size exceeded");
        else
            return size;
    }
}

void create_note() {
    unsigned int size;
    int current;
    if (notebook.current < MAX_NOTES) {
        current = notebook.current + 1;
        printf("Importance: ");
        notebook.notes[current].importance = read_int();
        size = read_size();
        notebook.notes[current].content = malloc(size+1);
        if (notebook.notes[current].content == NULL) {
            puts("malloc error");
            exit(1);
        }
        fgets(notebook.notes[current].content, size+1, stdin);
        notebook.current++;
    }
    else {
        puts("Maximum number of notes exceeded");
        puts("You need to delete other notes first");
    }
}

void delete_note() {
    int index = read_index();
    if (index != -1) {
        free(notebook.notes[index].content);
        if (index < notebook.current) {
            for (int i=index; i<notebook.current; i++) {
                notebook.notes[i].importance = notebook.notes[i+1].importance;
                notebook.notes[i].content = notebook.notes[i+1].content;
            }
        }
        notebook.current--;
    }
}

void read_note() {
    int index = read_index();
    if (index != -1) {
        printf("Note %d\nImportance: %u\n%s\n", index, notebook.notes[index].importance, notebook.notes[index].content);
    }
}

void edit_note() {
    unsigned int size;
    int index = read_index();
    if (index != -1) {
        printf("Importance: ");
        notebook.notes[index].importance = read_int();
        size = read_size();
        notebook.notes[index].content = realloc(notebook.notes[index].content, size+1);
        if (notebook.notes[index].content == NULL) {
            puts("malloc error");
            exit(1);
        }
        fgets(notebook.notes[index].content, size+1, stdin);
    }
}

void print_menu() {
    puts("1. Create note");
    puts("2. Delete note");
    puts("3. Read note");
    puts("4. Edit note");
    puts("5. Quit");
}

int main(int argc, char** argv) {
    char *pos, *token;
    int choice, index;

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    notebook.current = -1;
    puts("Simple note taking app");
    while(1) {
        printf("You currently have %d notes\n", notebook.current+1);
        print_menu();
        printf("> ");
        choice = read_int();
        switch (choice) {
            case 1:
                create_note();
                break;
            case 2:
                delete_note();
                break;
            case 3:
                read_note();
                break;
            case 4:
                edit_note();
                break;
            case 5:
                exit(0);
                break;
            default:
                puts("Invalid choice");
                break;
        }
    }
}
