// Kenny Sexton
// CIS 3207
// Project 4
// FileSystem

#include <stdio.h>
#include <stdlib.h> // for use of filesystem commands

#define DRIVE "Drive2MB"
#define BLOCK_SIZE 512   //bytes
#define FILE_MAX 64   // files can take up a max of 64 blocks  == 32768 MAX BYTES / 512 BLOCK_SIZE


	/* Globals */
FILE *fp;


	/* Functions */

	/* Structs */
typedef struct {
	unsigned char filename[8];  // 8 bytes
	unsigned char ext[3]; 		// 3 bytes
	unsigned short create_time;	// 2 bytes
	unsigned short create_date; // 2 bytes
	unsigned short modify_time; // 2 bytes
	unsigned short modify_date; // 2 bytes
	unsigned short starting_block; // 2 bytes
	unsigned long file_size;	// 4 bytes
} FATrootEntry;


FILE setup(){ // Sets up the file system
	fp = fopen(DRIVE, "r+");  // open drive that will act as filesystem
	//fwrite(fp, 512, )
	createRoot();
	
	//fputs("a", fp);
	return *fp;
}

void createRoot() {
	fseek(fp, BLOCK_SIZE * , SEEK_SET)
}

void fs_create(char *fileName){
	printf("%s", fileName);

	fseek(fp, 0, SEEK_SET);	
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