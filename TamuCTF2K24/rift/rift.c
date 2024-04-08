#include <stdio.h>

int upkeep() {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
}

char buf[64];

void vuln() {
    int always_true = 1;
    while (always_true) {
        fgets(buf, sizeof(buf), stdin);
        printf(buf);
    }
}

int main() {
	upkeep();
    vuln();
}
