// Kenny Sexton
// CIS 3207
// Project 4
// FileSystem

#include <stdio.h>
#include <stdlib.h> // for use of filesystem commands

#define DRIVE "Drive2MB"


	/* Functions */

	/* Structs */
struct file{
	int file_id;
	char fname[50];
	char dcreated[50];
	char dmodified[50];
};

// void initiateFileSystem(){
// 	printf("setting up file system ...\n");
// 	setup();
// }



void setup(){ // Sets up the file system

	FILE *fp;

	fp = fopen(DRIVE, "r+");  // open drive that will act as filesystem
	//fwrite(fp, 512, )
	// createRoot(fp)
	
	//fputs("a", fp);
	printf("hurray!\n");

	//return *fp;
}


// int closefs(FILE *fp){
// 	fclose(fp);
// }