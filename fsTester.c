// Kenny Sexton
// CIS 3207
// Project 4
// Filetester

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

	/* Colors */
#define CYAN_BOLD  "\033[1m\033[36m"
#define YELLOW_BOLD   "\033[1m\033[33m"
#define BRIGHT_RED "\033[1m\033[31m"

#define COLOR_RESET  "\033[0m"

	/* Functions */
void welcome();
void displayOptions();
void commandChoice();


	/* Main Function */
int main(){
	
	FILE *fp;
	char str[20];
	
	setup(); // start up the filesystem
	welcome();
	
	while (1){
		displayOptions();
		printf("$ ");
		scanf("%20s", str);

		commandChoice(str);
	}
}

void welcome(){
	// system("clear");  // clear the screen
	printf(CYAN_BOLD "\tWelcome to Kenny's filesystem \n\n" COLOR_RESET); // welcome message
}

void displayOptions(){
	printf(YELLOW_BOLD "Here are your options:\n" COLOR_RESET);
	printf("\"create\"\t\"delete\" \n");
	printf("\"read\"  \t\"write\" \n");
	printf("\"up\"    \t\"down\" \n");
	printf("\"info\"  \t\"path\" \n");
	printf("\"list\"  \t\"exit\" \n");
}

void commandChoice(char str[]){

	char fileName[9];

	if (strcmp(str, "create") == 0){ // needs extension handling
		printf("create selected\n");
		printf("Enter the name of file: ");
		scanf("%8s", fileName);

		fs_create(fileName);

		//printf("%s created\n", fileName);

		//divide filename and .txt if applicable
		//fs_create(char *filename, char *extension)
	}
	else if (strcmp(str, "delete") == 0){
		printf("delete selected\n");
	}
	else if (strcmp(str, "read") == 0){
		fs_read(fileName);
	}
	else if (strcmp(str, "write") == 0){
		
	}
	else if (strcmp(str, "clear") == 0){ // clears screen
		system("clear");	
	}
	else if (strcmp(str, "exit") == 0){ // exit program
		fs_exit();	
	}
	else if (strcmp(str, "quit") == 0){ // exit program
		fs_exit();	
	}
	else {
		printf(BRIGHT_RED "Invalid Command\n" COLOR_RESET);
	}
	
}
