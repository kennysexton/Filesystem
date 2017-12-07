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
#define BLOCK_SIZE_LINES
#define FILE_MAX 64   	// files can take up a max of 64 blocks  == 32768 MAX BYTES / 512 BLOCK_SIZE
#define START_OF_FAT 0  // same as SEEK_SET
#define START_OF_ROOT 1 // # of blocks until start of root
#define START_OF_DATA 2	// # of block until start of data 


	/* Globals */
FILE *fp; // same file pointer used by all functions
char digit2[3];
char digit4[5];
char digit6[7]; // Used for internal ptrs
char *blank32[32] = { NULL };
char *blank64[64] = { NULL };

	/* Functions */
void createRoot();
char *getcurrentTime(int i);
char *getcurrentDate(int i);
void clear32bytes();
void clear64bytes();
void printTime();


	/* Structs */
struct FAT{		// fat entries are 32 byte (16 fat entries per block)
	unsigned char valid[1];		// 1 byte
	unsigned char fileName[12]; // 12 bytes
	unsigned char rootPtr[6];	// 6 bytes
	unsigned char dataPtr[6]; 	// 6 bytes
	unsigned char nextPtr[6];	// 6 bytes
};

struct rootDir{		// Each file has a 64 byte root directory  (8 files can be stored in 1 block)
	unsigned char fileName[12];  	// 12 bytes
	unsigned char ext[3]; 			// 3 bytes
	unsigned char create_year[4];	// 4 bytes
	unsigned char create_month[2]; 	// 2 bytes
	unsigned char create_day[2]; 	// 2 bytes
	unsigned char create_hour[2]; 	// 2 bytes
	unsigned char create_min[2]; 	// 2 bytes
	unsigned char create_sec[2]; 	// 2 bytes
	unsigned char modify_year[4];	// 4 bytes
	unsigned char modify_month[2]; 	// 2 bytes
	unsigned char modify_day[2]; 	// 2 bytes
	unsigned char modify_hour[2]; 	// 2 bytes
	unsigned char modify_min[2]; 	// 2 bytes
	unsigned char modify_sec[2]; 	// 2 bytes
	unsigned char starting_block[2]; 	// 2 bytes
	unsigned char file_size[4];		// 4 bytes
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

		/* --------------------- Init FAT --------------------- */
	clear32bytes();
	fseek(fp, 0, SEEK_SET);		// START OF FAT

	if (i == 0){  // prints debug information
		printf("SYSTEM| FAT created\n");
	}
	struct FAT FATroot;

	//strcpy(FATroot.valid, "1");
	//strcpy(FATroot.fileName, "/");

	sprintf(digit6, "%u", BLOCK_SIZE * START_OF_ROOT / 16 + 1); 	// 512 * START_OF_ROOT gives the bit starting number dividing by /16 + 1 gives line number
	printf("%s\n", digit6);
	//strcpy(FATroot.rootPtr,digit6);

	sprintf(digit6, "%u", BLOCK_SIZE * START_OF_DATA / 16 + 1);	// returns the index of data as a string
	printf("%s\n", digit6);
	//strcpy(FATroot.dataPtr, digit6);

	sprintf(digit6, "%u", 3);	// returns the index of data as a string
	printf("%s\n", digit6);
	//strcpy(FATroot.nextPtr, digit6);

	fwrite(&FATroot, sizeof(struct FAT), 1, fp);
	printf("SYSTEM| FAT populated\n");	

	/* --------------------- Init rootDir --------------------- */  
	i = fseek(fp, BLOCK_SIZE * START_OF_ROOT, SEEK_SET); // START OF ROOT
	//fputs("11111111", fp);

	if (i == 0){  // prints debug information
		printf("SYSTEM| blank root created\n");
	}
	struct rootDir root; // declare root Directory

	strcpy(root.fileName, "/");
	strcpy(root.ext, "DIR");

	strcpy(root.create_year, getcurrentDate(1));
	strcpy(root.create_month, getcurrentDate(2));
	strcpy(root.create_day, getcurrentDate(3));	

	strcpy(root.create_hour, getcurrentTime(1));
	strcpy(root.create_min, getcurrentTime(2));
	strcpy(root.create_sec, getcurrentTime(3));

	strcpy(root.modify_year, getcurrentDate(1));
	strcpy(root.modify_month, getcurrentDate(2));
	strcpy(root.modify_day, getcurrentDate(3));	

	strcpy(root.modify_hour, getcurrentTime(1));
	strcpy(root.modify_min, getcurrentTime(2));
	strcpy(root.modify_sec, getcurrentTime(3));

	strcpy(root.starting_block, "00");
	strcpy(root.file_size, "0000");

	fwrite(&root, sizeof(struct rootDir), 1, fp);
	printf("SYSTEM| root populated\n");
}



char *getcurrentDate(int i){ // retrieves curretn date EST (UTC - 5)
	time_t now;
	struct tm *tm;

	now = time(0);

	if((tm = localtime(&now)) != NULL){
		if (i == 1){	// 1 === YEAR
			sprintf(digit4, "%04d", tm->tm_year + 1900);
			return digit4;
		}
		else if (i == 2){ // 2 == MONTH
			sprintf(digit2, "%02d", tm->tm_mon + 1);
			return digit2;
		}
		else if (i == 3){	// 3 == DAY
			sprintf(digit2, "%02d", tm->tm_mday);
			return digit2;
		}
		else {
			printf("invalid call to getcurrentDate(int i)\n");
		}
	}

}

char *getcurrentTime(int i){  // retrieves current time EST (UTC -5)
	
	time_t now;
	struct tm *tm;
	now = time(0);

	if((tm = localtime(&now)) != NULL){
		// sprintf (tm_String, "%02d %02d %02d\n", 
		// 	tm->tm_hour, tm->tm_min, tm->tm_sec);
		if (i == 1){	// 1 == HOUR
			sprintf(digit2, "%02d", tm->tm_hour);
			return digit2;
		}
		else if (i == 2){	// 2 == MIN
			sprintf(digit2, "%02d", tm->tm_min);
			return digit2;
		}
		else if (i == 3){	// 3 == SEC
			sprintf(digit2, "%02d", tm->tm_sec);
			return digit2;
		}
		else {
			printf("invalid call to getcurrentTime(int i)\n");
		}
	}
}

void printTime(){ // prints current time formatted   || FOR DEBUGGING
	/*	Getting formatted date 
		github_user: paxdiablo
     	url: stackoverflow.com/questions/2242963/get-the-current-time-in-seconds
	*/
	time_t now;
	struct tm *tm;

	now = time(0);

	if((tm = localtime(&now)) != NULL){
		printf ("%04d %02d %02d %02d %02d %02d\n", 
			tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
	}
}

void clear32bytes(){  //  resets a row of 32 bytes 
	fwrite(blank32, 1, sizeof(blank32), fp);
	//TODO: may add a fp reset
}

void clear64bytes(){  //  resets a row of 64 bytes 
	fwrite(blank64, 1, sizeof(blank64), fp);
	//TODO: may add a fp reset
}

/*------------------------------ Called by user ------------------------------*/
void fs_create(char *fileName){
	printf("%s", fileName);

	fseek(fp, 0, SEEK_SET);	
	fputs(fileName, fp);

}

void fs_read(char *fileName){
	char buffer[12];

	fseek(fp, 0, SEEK_SET);	
	fread(buffer, 1, 12, fp);

	for (int i = 0; i < sizeof(buffer); ++i)
	{
		printf("%c", buffer[i]);
	}
	puts("");
}

void fs_info(char *fileName){

	char fName[13] = {'\0'};
	char fext[4] = {'\0'};
	char cyear[5] = {'\0'};
	char cmon[3] = {'\0'};
	char cday[3]= {'\0'};
	char chour[3]= {'\0'};
	char cmin[3]= {'\0'};
	char csec[3]= {'\0'};

	fseek(fp, BLOCK_SIZE * START_OF_ROOT, SEEK_SET);  // navigate to root

	// if (filefound)
	
	fread(fName, 1, 12, fp);
	fread(fext, 1, 3, fp);
	fread(cyear, 1, 4, fp);
	fread(cmon, 1, 2, fp);
	fread(cday, 1, 2, fp);
	fread(chour, 1, 2, fp);
	fread(cmin, 1, 2, fp);
	fread(csec, 1, 2, fp);

	printf("Name: %s\n", fName);
	printf("Ext: %3s\n", fext);
	printf("Created: %s-%s-%s at %s:%s:%s \n", cmon, cday, cyear, chour, cmin, csec);
	printf("Modified: %s-%s-%s at %s:%s:%s \n", cmon, cday, cyear, chour, cmin, csec);

}

void fs_exit(){ // closes the disk,  and exits
	fclose(fp);
	printf("SYSTEM| shutdown\n");
	exit(0);
}