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
#define COLOR_RESET  "\033[0m"

	/* Functions */
void welcome();
void displayOptions();
void commandChoice();


	/* Main Function */
int main(){
	
	FILE *fp;
	char str[20];
	
	setup();
	welcome();
	displayOptions();
	printf("type a command: ");
	scanf("%20s", str);

	commandChoice(str);

	//closefs(fp);
}

void welcome(){
	system("clear");  // clear the screen
	printf(CYAN_BOLD "\tWelcome to Kenny's filesystem \n\n" COLOR_RESET); // welcome message
}

void displayOptions(){
	printf(YELLOW_BOLD "Here are your options:\n" COLOR_RESET);
	printf("\"create\"\t\"delete\" \n");
	printf("\"open\"  \t\"close\" \n");
	printf("\"read\"  \t\"write\" \n");
	printf("\"up\"    \t\"down\" \n");
	printf("\"info\"  \t\"path\" \n");
}

void commandChoice(char str[]){

	if (strcmp(str, "create") == 0){
		printf("create selected\n");
	}
	else if (strcmp(str, "delete") == 0){
		printf("delete selected\n");
	}
	
}
