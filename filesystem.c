// Kenny Sexton
// CIS 3207
// Project 4
// FileSystem

#include <stdio.h>
#include <stdlib.h>


	/* Colors */
#define CYAN_BOLD  "\033[1m\033[36m" 
#define COLOR_RESET  "\033[0m"


#define DRIVE "Drive2MB"

	/* Functions */
char welcome();

struct file{
	int file_id;
	char fname[50];
};


int main(){


	FILE *fp;
	fp = fopen(DRIVE, "r+");  // open drive that will act as filesystem

	// createRoot(fp)
	
	welcome();

	

	fputs("a", fp);




	fclose(fp); // close filesystem
}


char welcome(){

	system("clear");  // clear the screen
	printf(CYAN_BOLD "\tWelcome to Kenny's filesystem \n" COLOR_RESET); // welcome message

	return 'a';
}
