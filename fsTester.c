// Kenny Sexton
// CIS 3207
// Project 4
// Filetester

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
	printf("\"mkdir\" \t\"up\" \n");
	printf("\"cd\" 	  \t\"\" \n");
	printf("\"info\"  \t\"clear\" \n");
	printf("\"list\"  \t\"exit\" \n");
}

void commandChoice(char str[]){

	char fileName[9];

	if (strcmp(str, "create") == 0){ // needs extension handling
		printf("create selected\n");
		printf("Enter the name of file: ");
		scanf("%12s", fileName);

		fs_create(fileName, 0);		// 0 flag means file
	}
	else if (strcmp(str, "delete") == 0){
		printf("Enter the name of file: ");
		scanf("%12s", fileName);
		fs_delete(fileName);
	}
	else if (strcmp(str, "read") == 0){					// read data blocks of a file
		printf("Enter the name of file: ");
		scanf("%12s", fileName);
		fs_read(fileName);
	}
	else if (strcmp(str, "write") == 0){
		
		char *dataBlock = calloc(32768, sizeof(char));	// create an empty larger array

		printf("Enter the name of file: ");
		scanf("%12s", fileName);
		printf("--- Write --- \n");
		
		if(read(STDIN_FILENO, dataBlock, 32768) < 0){
		}
		fs_write(fileName, dataBlock);
		free(dataBlock);
	}
	else if (strcmp(str, "mkdir") == 0){  // create a directory
		printf("Enter the name of directory: ");
		scanf("%12s", fileName);

		fs_create(fileName, 1); 	// 1 flag means directory
	}
	else if (strcmp(str, "up") == 0){ 
		fs_up();
	}
	else if (strcmp(str, "cd") == 0){
		printf("Enter the name of directory: ");
		scanf("%12s", fileName);
		fs_cd(fileName);
	}
	else if (strcmp(str, "info") == 0){  // get file metadata
		printf("Enter the name of file: ");
		scanf("%12s", fileName);

		fs_info(fileName);
	}
	else if (strcmp(str, "clear") == 0){ // clears screen
		system("clear");	
	}
	else if (strcmp(str, "list") == 0){ // exit program
		fs_list();	
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
