// Kenny Sexton
// CIS 3207
// Project 4
// Filetester

#include <stdio.h>
#include <stdlib.h>
#include "filesystem.h"

	/* Colors */
#define CYAN_BOLD  "\033[1m\033[36m" 
#define COLOR_RESET  "\033[0m"

	/* Functions */
char welcome();


	/* Main Function */
int main(){
	
	welcome();
	setup();
}

char welcome(){

	system("clear");  // clear the screen
	printf(CYAN_BOLD "\tWelcome to Kenny's filesystem \n" COLOR_RESET); // welcome message

	return 'a';
}
