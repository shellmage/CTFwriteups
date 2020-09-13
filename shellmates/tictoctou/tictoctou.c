#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "header.h"
	 	 	 			  	 				  	 		 	   	
int main(int argc, char** argv) {
    unsigned char choice;
	  		 						  			  	  	 		
    setup();
    clear();
    welcome();
    print_menu();
		  				 	  	  			  	  			  	
    choice = menu_choice();
    while (choice != '0') {
        if (choice == '1') {
            gameloop();
	 	 	  	   		  		   	  		 	 		 		
        } else {
            how_to_play();
            wait_for_user();
        }
				   	   	  				 	   		  		 	
        clear();
        print_menu();
        choice = menu_choice();
    }
					  	  	 	  				 	  		 	
    goodbye();
		  		 	 		 						  			
    return 0;
}
 		    		 		 						 		 		
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    srand(time(NULL));
}
		  				 	  		  		  	 		 	  	
void clear() {
    puts("\e[1;1H\e[2J");
}
	 			  	 					 						
void welcome() {
    char *msg = ".--------------------------------------------------------------------------------.\n"
                "| Welcome to the stupidest version of the classic Tic-Tac-Toe game: Tic-Toc-Tou! |\n"
                "|  No artificial intelligence behind the bot's moves or anything close to that   |\n"
                "|                      It's just, pure pseudo randomness!                        |\n"
                "|                 That way you'll have a chance to win, maybe.                   |\n"
                "|              If you lose, however, you'll be a certified boomer                |\n"
                "|           and you can make a post about it on LinkedIn. (haha lol)             |\n"
                "|   Oh, and before I forget, there's a file on my home directory that contains   |\n"
                "|   some highly sensitive information, everyone is racing in order to get to it  |\n"
                "|             first. Will you be the first one? It's up to you pal.              |\n"
                "`--------------------------------------------------------------------------------'";
 	  	 		  	    	   	 	 	  				 	  	 	
    puts(msg);
}
			  	 		 		 							 	
void goodbye() {
    puts(byemsg);
}
 				  	 				  		  		    	 		
void print_menu() {
    char *menu = ".-------------- Game menu --------------.\n"
                 "|                                       |\n"
                 "| 1. Start new game                     |\n"
                 "| 2. How to play                        |\n"
                 "| 0. Exit                               |\n"
                 "`.......................................'\n"
                 ">>> ";
    printf("%s", menu);
}
	 	 		 		 	  				 					
unsigned char getchoice() {
    unsigned char c = getchar();
    while (getchar() != '\n');
	   		  	   	 			   	 	 	  		 			
    return c;
}
  		 	 	  	   		   	 				 				
void draw_board() {
    char *fmt = ".---.---.---.\n"
                "| %c | %c | %c |\n"
                ":---:---:---:\n"
                "| %c | %c | %c |\n"
                ":---:---:---:\n"
                "| %c | %c | %c |\n"
                "`---'---'---'\n";
		  				 	   	 				 				
    printf(fmt, board[0], board[1], board[2],
                board[3], board[4], board[5],
                board[6], board[7], board[8]);
}
		 			 	 	 	 		  	 		 		 	
unsigned char menu_choice() {
    unsigned char choice;
	 						 					 	  	  		
    choice = getchoice();
	  		   	  	 		 	  		 	 	   		 		
    while (choice < 48 || choice > 50) {
        puts("Invalid option. Please select a valid option from the menu");
        print_menu();
    }
   		  		 						  				 	  	
    return choice;
}
   		  		 				 						  	  	
int checkwin() {
    unsigned char winner = 0;
 	  		 	 	  	  	 	  			 		  		 	
    if (board[0] != ' ' && board[0] == board[1] && board[0] == board[2]){
        winner = board[0];
    } else if (board[3] != ' ' && board[3] == board[4] && board[3] == board[5]) {
        winner = board[3];
    } else if (board[6] != ' ' && board[6] == board[7] && board[6] == board[8]) {
        winner = board[6];
    } else if (board[0] != ' ' && board[0] == board[3] && board[0] == board[6]) {
        winner = board[0];
    } else if (board[1] != ' ' && board[1] == board[4] && board[1] == board[7]) {
        winner = board[1];
    } else if (board[2] != ' ' && board[2] == board[5] && board[2] == board[8]) {
        winner = board[2];
    } else if (board[0] != ' ' && board[0] == board[4] && board[0] == board[8]) {
        winner = board[0];
    } else if (board[2] != ' ' && board[2] == board[4] && board[2] == board[6]) {
        winner = board[2];
    }
 	   		 			 						  	    	  		
    if (winner == 'O') winner = 1;
    else if (winner == 'X') winner = 2;
  			 						 	 		  	  	 		
    return (int)winner;
}
		  		 	 	  	  			  	  			  	  	
void gameloop() {
    int result, i, j, n;
    unsigned char choice;
 	  				 	   	 			  		 	 	   		
    memset(board, 32, 9);
  		 						 				  	  	  		
    for (i = 0; !(result = checkwin()) && i < 9; i++) {
        if (i % 2) {
            bot_move(9 - i);
            continue;
        }
	   	 	 	  			 			 	   			 		
        clear();
        draw_board();
				 					 		 					
        printf("Your next move: ");
	   					   	   	  	   			 				
        choice = getchoice();
		  			  	  	 				  				 	
        while (choice < 49 || choice > 57 || board[choice - 49] != ' '){
            printf("Invalid identifier or case occupied, please enter a valid number: ");
            choice = getchoice();
        }
  	  			  	  			  		 	 	  	   		  	
        board[choice - 49] = 'O';
        history[i] = choice;
    }
	   	  		 	 		 						  		  	
    clear();
    draw_board();
	  	    	   		 			 						 	 	
    switch (result){
        case 0:
            puts("Tie!");
            break;
        case 1:
            puts("You WON! But at what cost?");
            break;
        case 2:
            puts("You lost against a brainless bot, boomer.");
            break;
    }
	  	  	 				  		 	 	  	  			  	
    while (choice != 'y' && choice != 'n') {
        printf("Do you wish to save the game stats? [y/n] ");
        choice = getchoice();
    }
  			  	  	 	  				 	   	 			  	
    if (choice == 'y') {
        char nickname[128];
        char filepath[128];
	 	 	   		  		 		   		 					
        printf("Enter your nickname: ");
        n = read(0, nickname, sizeof(nickname));
	 		 		 	  	   		  		  		  	    	   	
        for (j = n - 1; j > -1 && nickname[j] != '\n'; j--);
        nickname[j] = '\0';
	  	  		 	 	  			  		 					
        printf("Enter the file where you'd like to save the game stats (must be a file YOU own): ");
        n = read(0, filepath, sizeof(filepath));
	 	 	   	  		 	 	  		 	 	  	 	  		
        for (j = n - 1; j > -1 && filepath[j] != '\n'; j--);
        filepath[j] = '\0';
 						  		 	 	   	  		  		 	
        if (save_stats(nickname, filepath, result, i) == 0) puts("Stats saved successfully.");
        else puts("An error have occured, make sure you entered the correct file path.");
        wait_for_user();
    }
}
 	  	   		   	 				 	   	
void how_to_play() {
    for (int i = 0; i < 9; i++) board[i] = i + 49;
    puts("Cases are numbered like shown: ");
    draw_board();
    puts("Just pick the number of the case you want to mark, ez pz.");
}

void wait_for_user() {
    printf("Press enter to continue... ");
    while (getchar() != '\n');
}

void bot_move(int empty_cells) {
    int i = rand() % empty_cells;
    int c = -1, j = 0;

    for (j = 0; j < 9 && c != i; j++){
        if (board[j] == ' ') c++;
    }

    board[j - 1] = 'X';
    history[9 - empty_cells] = j + 48;
}

int save_stats(char *nickname, char *filepath, int result, int num_moves) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Check that the real user have write permissions on that file
    if (access(filepath, W_OK) != 0) {
        perror("access check failed");
        return -1;
    }

    // Since the real user can write to that file, we proceed safely
    FILE* f = fopen(filepath, "a");
    if (f == NULL) return -1;

    switch (result){
        case 0:
            fprintf(f, "%s TIED against brainless bot on %d-%02d-%02d at %02d:%02d:%02d! Still a boomer...\n", nickname, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            break;
        case 1:
            fprintf(f, "%s WON against brainless bot on %d-%02d-%02d at %02d:%02d:%02d! Not surprising however.\n", nickname, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            break;
        case 2:
            fprintf(f, "%s LOST against brainless bot on %d-%02d-%02d at %02d:%02d:%02d! What a shame to lose against pseudo randomness, go play minecraft.\n", nickname, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            break;
    }

    fprintf(f, "################# Game stats #################\n");
    for (int j = 0; j < num_moves; j++) {
        if (j % 2) {
            fprintf(f, "# Brainless Bot (X) plays on case %c          #\n", history[j]);
        } else {
            fprintf(f, "# Human (O) plays on case %c                  #\n", history[j]);
        }
    }
    fprintf(f, "##############################################\n\n");

    fclose(f);

    return 0;
}
