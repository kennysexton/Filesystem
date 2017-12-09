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
#define START_OF_META 1 // # of blocks until start of meta
#define START_OF_DATA 2	// # of block until start of data 


	/* Globals */
FILE *fp; // same file pointer used by all functions
char digit2[3];
char digit4[5];
char digit6[6]; // Used for internal ptrs
char *blank32[32] = { NULL };
char *blank64[64] = { NULL };


	/* Structs */
struct fat{		// fat entries are 32 byte (16 fat entries per block)
	unsigned char valid[1];		// 1 byte
	unsigned char fileName[12]; // 12 bytes
	unsigned char rootPtr[6];	// 6 bytes
	unsigned char dataPtr[6]; 	// 6 bytes
	unsigned char nextPtr[6];	// 6 bytes
};

struct meta{		// Each file has a 64 byte meta sequence  (8 files can be stored in 1 block)
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
	unsigned char file_size[6];		// 6 bytes
};

	/* Functions */
void createRoot();
char *findFatfree();
char *findMetafree();
char *getcurrentTime(int i);
char *getcurrentDate(int i);
void clear32bytes();
void clear64bytes();
void printTime();
void printfat( struct fat fatentry);

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
	//clear32bytes();
	fseek(fp, 0, SEEK_SET);		// START OF FAT

	if (i == 0){  // prints debug information
		printf("SYSTEM| FAT created\n");
	}
	struct fat FATroot = {""}; // declare as empty

	strcpy(FATroot.valid, "1");
	strcpy(FATroot.fileName, "/");

	sprintf(digit6, "%u", BLOCK_SIZE * START_OF_META / 16 + 1); 	// 512 * START_OF_META gives the bit starting number dividing by /16 + 1 gives line number
	//printf("%s\n", digit6);
	strcpy(FATroot.rootPtr,digit6);

	sprintf(digit6, "%u", BLOCK_SIZE * START_OF_DATA / 16 + 1);	// returns the index of data as a string
	//printf("%s\n", digit6);
	strcpy(FATroot.dataPtr, digit6);

	sprintf(digit6, "%u", 3);	// returns the index of data as a string
	//printf("%s\n", digit6);
	strcpy(FATroot.nextPtr, digit6);

	//printfat(FATroot);
	fwrite(&FATroot, sizeof(struct fat), 1, fp);
	printf("SYSTEM| FAT populated\n");	

	/* --------------------- Init meta --------------------- */  
	i = fseek(fp, BLOCK_SIZE * START_OF_META, SEEK_SET); // START OF META
	//fputs("11111111", fp);

	if (i == 0){  // prints debug information
		printf("SYSTEM| blank root created\n");
	}
	struct meta root = {""}; // declare root Directory

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

	strcpy(root.file_size, "000000");

	fwrite(&root, sizeof(struct meta), 1, fp);
	printf("SYSTEM| root populated\n");
}

char *findFatfree(){  // finds a free spot in the file allocation table
	char valid[1];
	char buffer[6];
	
	fseek(fp, 0, SEEK_SET);	// nav to start of fat
	for(int i = 0; i < (START_OF_META * BLOCK_SIZE); i+= 32){
		fseek(fp, i, SEEK_SET);
		fread(valid, 1, 1, fp);
		//printf("read in bit:  %s\n", valid);
		if (strcmp(valid, "") == 0){
			printf("SYSTEM| found free fat\n");
			sprintf(digit6, "%u", i / 16 + 1); // convert #bits into line #
			return digit6;
		}
	}
	return "-1"; // Couldn't find a free FAT entry
}

char *findMetafree(){
	char cmpName[12];

	fseek(fp, BLOCK_SIZE * START_OF_META , SEEK_SET); // nav to start of meta
	for(int i = 0; i < START_OF_DATA * BLOCK_SIZE; i+= 64){
		fseek(fp, BLOCK_SIZE * START_OF_META + i, SEEK_SET);
		fread(cmpName, 12, 1, fp);
		printf("Read in: %s\n", cmpName);
		if (strcmp(cmpName, "") == 0){
			printf("SYSTEM| found free meta\n");
			printf("%d\n", i);
			sprintf(digit6, "%u", (i / 64 * 4) + (BLOCK_SIZE * START_OF_META / 16) + 1);
			return digit6;
		}
	}
	return "-1"; // couldn't find any free meta entries
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
void printfat( struct fat fatentry){ // for debugging
	printf("Valid bit: %s\n", fatentry.valid);
	printf("File Name: %s\n", fatentry.fileName);
	printf("rootPtr: %s\n", fatentry.rootPtr);
	printf("dataPtr: %s\n", fatentry.dataPtr);
	printf("nextPtr: %s\n", fatentry.nextPtr);
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

void clear32bytes(){  //  resets a row of 32 bytes BORKEN ATM
	fwrite(blank32, 1, sizeof(blank32), fp);
	//TODO: may add a fp reset
}

void clear64bytes(){  //  resets a row of 64 bytes 
	fwrite(blank64, 1, sizeof(blank64), fp);
	//TODO: may add a fp reset
}

/*------------------------------ Called by user ------------------------------*/
void fs_create(char *fileName){
	printf("filename: %s\n", fileName);
	char *fatIndex;
	char *metaIndex; 
	//fputs(fileName, fp);

		/* Find free fat entry */
	fatIndex = findFatfree();	// returns line number of free fat sequence
	printf("fatIndex: %s\n", fatIndex);
	if (strcmp(fatIndex, "-1")  == 0){  // findFatfree() retruns -1 on error
		printf("SYSTEM| Unable to locate free FAT \n");
	}

		/* Find free meta */
	metaIndex = findMetafree();	// returns line number of free meta sequence
	printf("metaIndex: %s\n", metaIndex);
	if (strcmp(metaIndex, "-1")  == 0){	// findMetafree() retruns -1 on error
		printf("SYSTEM| Unable to locate free meta \n");
	}
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

	fseek(fp, BLOCK_SIZE * START_OF_META, SEEK_SET);  // navigate to root

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