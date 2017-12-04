// Kenny Sexton
// CIS 3207
// Project 4
// FileSystem

#include <stdio.h>
#include <stdlib.h> // for use of filesystem commands

#define DRIVE "Drive2MB"
#define BLOCK_SIZE 512   //bytes


	/* Globals */
FILE *fp;


	/* Functions */

	/* Structs */
struct file{
	int file_id;
	char fname[50];
	char dcreated[50];
	char dmodified[50];
};


FILE setup(){ // Sets up the file system

	

	fp = fopen(DRIVE, "r+");  // open drive that will act as filesystem
	//fwrite(fp, 512, )
	// createRoot(fp)
	
	//fputs("a", fp);
	printf("hurray!\n");

	return *fp;
}

void fs_create(char *fileName){
	printf("%s", fileName);

	fputs(fileName, fp);

}


void fs_read(char *fileName){
	char buffer[8];

	fseek(fp, 0, SEEK_SET);	
	fread(buffer, 1, 8, fp);

	for (int i = 0; i < sizeof(buffer); ++i)
	{
		printf("%c", buffer[i]);
	}
	puts("");
}



int closefs(FILE *fp){
	fclose(fp);
}