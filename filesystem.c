// Kenny Sexton
// CIS 3207
// Project 4
// FileSystem

#include <stdio.h>
#include <stdlib.h> // for use of filesystem commands
#include <string.h>
#include <time.h> 	// get current time

#define DRIVE "Drive2MB"
#define BLOCK_SIZE 512  //bytes
#define FILE_MAX 64   	// files can take up a max of 64 blocks  == 32768 MAX BYTES / 512 BLOCK_SIZE
#define START_OF_FAT 0  // same as SEEK_SET
#define START_OF_ROOT 1 // How many blocks in the root starts 


	/* Globals */
FILE *fp; // same file pointer used by all functions


	/* Functions */
void createRoot();
short getcurrentTime();


	/* Structs */
struct rootDir{		// Each file has a 32 byte root directory  (16 files can be stored in 1 block)
	unsigned char fileName[8];  // 8 bytes
	unsigned char ext[3]; 		// 3 bytes
	unsigned short create_time;	// 2 bytes
	unsigned short create_date; // 2 bytes
	unsigned short modify_time; // 2 bytes
	unsigned short modify_date; // 2 bytes
	unsigned short starting_block; // 2 bytes
	unsigned long file_size;	// 4 bytes
	unsigned long empty;		// 6 bytes
};

/*------------------------------ File Setup ------------------------------*/
FILE setup(){ // Sets up the file system
	fp = fopen(DRIVE, "r+");  // open drive that will act as filesystem
	//fwrite(fp, 512, )
	createRoot();
	
	//fputs("a", fp);
	return *fp;
}

void createRoot() {
	int i = 0;

	i = fseek(fp, BLOCK_SIZE * START_OF_ROOT, SEEK_SET); // START OF ROOT
	fputs("11111111", fp);

	if (i == 0){  // prints debug information
		printf("SYSTEM| root created\n");
	}

	struct rootDir root; // declare root Directory

	strcpy(root.fileName, "/");
	strcpy(root.ext, "DIR");
	//root.create_date
	getcurrentTime();
	
	//printf("hello");

	//root.create_time = 
	

}

short getcurrentTime(){  // retrieves current time EST (UTC -5)
	
	/* github_user: paxdiablo
     * url: stackoverflow.com/questions/2242963/get-the-current-time-in-seconds
	*/
	time_t now;
	struct tm *tm;

	now = time(0);

	if((tm = localtime(&now)) != NULL){
		printf ("%04d %02d %02d %02d %02d %02d\n", 
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	}
	return 0;
}

/*------------------------------ Called by user ------------------------------*/
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

void fs_exit(){ // closes the disk,  and exits
	fclose(fp);
	printf("SYSTEM| shutdown\n");
	exit(0);
}