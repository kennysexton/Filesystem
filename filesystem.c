// Kenny Sexton
// CIS 3207
// Project 4
// FileSystem

#include <stdio.h>
#include <stdlib.h> // for use of filesystem commands
#include <string.h>
#include <time.h> 	// get current time

#define DRIVE "Drive2MB"
#define TOTAL_BLOCKS 244 	// number of avalible blocks at 2MB
#define BLOCK_SIZE 512  	//bytes
#define BLOCK_SIZE_LINES
#define FILE_MAX 64   	// files can take up a max of 64 blocks  == 32768 MAX BYTES / 512 BLOCK_SIZE
#define START_OF_FAT 0  // same as SEEK_SET
#define START_OF_META 1 // # of blocks until start of meta
#define START_OF_DATA 3	// # of block until start of data 

	/* Colors */
#define BRIGHT_RED "\033[1m\033[31m"
#define COLOR_RESET  "\033[0m"

	/* Globals */
FILE *fp; // same file pointer used by all functions
char digit2[3];
char digit4[5];
char digit6[6]; // Used for internal ptrs
char *blank32[32] = { NULL };
char *blank64[64] = { NULL };

long CURRENT_DIR = 0;
long PREV_DIR = 0;
long PREV_PREV_DIR = 0;

	/* Structs */
struct fat{		// fat entries are 32 byte (16 fat entries per block)
	unsigned char valid[1];		// 1 byte
	unsigned char fileName[12]; // 12 bytes
	unsigned char metaPtr[6];	// 6 bytes
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

struct data{		// used to delete data blocks (via fs_delete)
	unsigned char blank[512];
};

struct line{
	unsigned char blank[16];
};

	/* Functions */
void createRoot();
char *findFatfree();
char *findMetafree();
int findFileByName(char *fileName);
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

	sprintf(digit6, "%u", BLOCK_SIZE * START_OF_META / 16 + 1); // 512 * START_OF_META gives the bit starting number dividing by /16 + 1 gives line number
	strcpy(FATroot.metaPtr,digit6);

	sprintf(digit6, "%u", BLOCK_SIZE * START_OF_DATA / 16 + 1);	// returns the index of data as a string
	strcpy(FATroot.dataPtr, digit6);

	//sprintf(digit6, "%u", 3);	// returns the index of data as a string
	strcpy(FATroot.nextPtr, "-1");

	//printfat(FATroot);
	fwrite(&FATroot, sizeof(struct fat), 1, fp);
	printf("SYSTEM| FAT populated\n");

	/* --------------------- Init data --------------------- */ 
	fseek(fp, BLOCK_SIZE * START_OF_DATA, SEEK_SET);
	CURRENT_DIR = START_OF_DATA * BLOCK_SIZE;
	fputs(".", fp);										// this is done so that findDataFree() will see that this block is reserved

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

/*------------------------------ Find Free ------------------------------*/
char *findFatfree(){  // finds a free spot in the file allocation table
	char valid[1];
	
	fseek(fp, 0, SEEK_SET);	// nav to start of fat
	for(int i = 0; i < (START_OF_META * BLOCK_SIZE); i+= 32){
		fseek(fp, i, SEEK_SET);
		fread(valid, 1, 1, fp);
		//printf("read in bit:  %s\n", valid);
		if (strcmp(valid, "") == 0){
			sprintf(digit6, "%u", i / 16 + 1); // convert #bits into line #
			printf("SYSTEM| found free fat @ %s\n", digit6);
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
		//printf("Read in: %s\n", cmpName);
		if (strcmp(cmpName, "") == 0){
			sprintf(digit6, "%u", (i / 64 * 4) + (BLOCK_SIZE * START_OF_META / 16) + 1);
			printf("SYSTEM| found free meta @ %s\n", digit6);
			return digit6;
		}
	}
	return "-1"; // couldn't find any free meta entries
}

char *findDatafree(){
	char valid[1]; // data block is empty if first bit is NULL

	fseek(fp, BLOCK_SIZE * START_OF_DATA, SEEK_SET);
	for(int i= 0; i <= (TOTAL_BLOCKS - 3) * 512 ; i+= 512){
		fseek(fp, BLOCK_SIZE * START_OF_DATA + i, SEEK_SET);
		//printf("%u\n", i );
		fread(valid, 1, 1, fp); // read in first byte of data block
		if(strcmp(valid, "") == 0){ // if first byte is null,  then block is empty
			sprintf(digit6, "%u", (i / 512 * 32) + (BLOCK_SIZE * START_OF_DATA / 16) + 1);
			printf("SYSTEM| found free data @ %s\n", digit6);
			return digit6;
		}
	}
	return "-1";	// couldn't find any free data blocks
}

int findFileByName(char *fileName){  // finds a free spot in the file allocation table
	char cmpName[12];
	int fileIndex = 0;
	
	fseek(fp, 0, SEEK_SET);	// nav to start of fat
	for(int i = 0; i < (START_OF_META * BLOCK_SIZE); i+= 32){
		fseek(fp, i + 1, SEEK_SET);
		fread(cmpName,12, 1, fp);
		//printf("read in bit:  %s\n", valid);
		if (strcmp(cmpName, fileName) == 0){
			fileIndex = i / 16 + 1; // convert #bits into line #
			printf("SYSTEM| found file @ %d\n", fileIndex);
			return fileIndex;
		}
	}
	return -1; // Couldn't find file
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
	printf("metaPtr: %s\n", fatentry.metaPtr);
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

/*------------------------------------------------------------------- Called by user -------------------------------------------------------------------*/
/* ------------------------------ Create ------------------------------ */
void fs_create(char *fileName, int flag){
	char *retrunIndex;
	char *fatIndex;
	char *metaIndex;
	char *dataIndex; 

		/* Find free fat entry */
	fatIndex = strdup(findFatfree());	// returns line number of free fat sequence
	if (strcmp(fatIndex, "-1")  == 0){  // findFatfree() retruns -1 on error
		printf("SYSTEM| Unable to locate free FAT \n");
	}

		/* Find free meta */
	metaIndex = strdup(findMetafree());	// returns line number of free meta sequence
	if (strcmp(metaIndex, "-1")  == 0){	// findMetafree() retruns -1 on error
		printf("SYSTEM| Unable to locate free meta \n");
	}

		/* Find free meta */
	dataIndex = strdup(findDatafree());
	if (strcmp(dataIndex, "-1")  == 0){	// findMetafree() retruns -1 on error
		printf("SYSTEM| Unable to locate a free data block \n");
	}
	fseek(fp, (atol(fatIndex) - 1) * 16, SEEK_SET); // convert line number back to bit seek value

	struct fat newFile = {""};

	strcpy(newFile.valid, "1");
	strcpy(newFile.fileName, fileName);
	strcpy(newFile.metaPtr, metaIndex);
	strcpy(newFile.dataPtr, dataIndex);
	strcpy(newFile.nextPtr, "-1");

	fwrite(&newFile, sizeof(struct fat), 1, fp);
	printf("SYSTEM| New FAT entry\n");

		/* Create meta entry */
	fseek(fp, (atol(metaIndex) - 1) * 16, SEEK_SET); 				// convert String -> number

	struct meta newMeta = {""};										// create an empty meta struct

	strcpy(newMeta.fileName, fileName);

	if (flag == 1){
		strcpy(newMeta.ext, "DIR"); // gives dir ext
	}
	else {
		strcpy(newMeta.ext, "");
	}

	strcpy(newMeta.create_year, getcurrentDate(1));
	strcpy(newMeta.create_month, getcurrentDate(2));
	strcpy(newMeta.create_day, getcurrentDate(3));	

	strcpy(newMeta.create_hour, getcurrentTime(1));
	strcpy(newMeta.create_min, getcurrentTime(2));
	strcpy(newMeta.create_sec, getcurrentTime(3));

	strcpy(newMeta.modify_year, getcurrentDate(1));
	strcpy(newMeta.modify_month, getcurrentDate(2));
	strcpy(newMeta.modify_day, getcurrentDate(3));	

	strcpy(newMeta.modify_hour, getcurrentTime(1));
	strcpy(newMeta.modify_min, getcurrentTime(2));
	strcpy(newMeta.modify_sec, getcurrentTime(3));
	strcpy(newMeta.file_size, "000000");

	fwrite(&newMeta, sizeof(struct meta), 1, fp);
	printf("SYSTEM| meta populated\n");

		/* Reserve Data */
	fseek(fp, (atol(dataIndex) - 1) * 16, SEEK_SET); 	// place a 1 at the start of data bit
	fputs(".", fp);										// this is done so that findDataFree() will see that this block is reserved

		/* Put File in DIR listing */
	printf("CURRENT_DIR: %li\n", CURRENT_DIR);
	fseek(fp, CURRENT_DIR, SEEK_SET);
	int i = 0;
	int found = 0;
	char valid[1];

	while(i < 32){
		fseek(fp, CURRENT_DIR + (i * 16), SEEK_SET);
		fread(valid, 1, 1, fp); // read in first byte 
		if(strcmp(valid, "") == 0){ 	// if first byte is null,  then block is empty
			fseek(fp, -1, SEEK_CUR);	// move back one
			fwrite(fileName, 12, 1, fp);
			break;	
		}
		i++;
	}

		/* If dir set as new current dir */
	if (flag == 1){
		PREV_PREV_DIR = PREV_DIR;	// update grandparent
		PREV_DIR = CURRENT_DIR;  // update parent
		CURRENT_DIR = (atol(dataIndex) - 1) * 16 ;
	}
}

/* ------------------------------ Delete ------------------------------ */
void fs_delete(char *fileName){
	int fileIndex = 0;
	char dataPtr[6];
	char metaPtr[6];
	char entryName[13] = {'\0'};

	fileIndex = findFileByName(fileName); //returns the FAT index of this file
	if (fileIndex == -1){	// file does not exist
		printf(BRIGHT_RED "FILE NOT FOUND: %s\n" COLOR_RESET, fileName);
		return;
	}
	
	/* Delete Data Block */
	fseek(fp, (fileIndex - 1) * 16, SEEK_SET);	// seek to the FAT index
	fseek(fp, 19, SEEK_CUR);					// offset of dataPtr
	fread(dataPtr, 1, 6, fp);
	fseek(fp, (atol(dataPtr) - 1) * 16, SEEK_SET);
	struct data clearBlock = {""};  // write an empty 512 blocks at file pointer
	fwrite(&clearBlock, sizeof(struct data), 1, fp);

	/* Delete meta data */
	fseek(fp, (fileIndex - 1) * 16, SEEK_SET);	// seek to the FAT index
	fseek(fp, 13, SEEK_CUR);					// offset of metaPtr
	fread(metaPtr, 1, 6, fp);
	fseek(fp, (atol(metaPtr) - 1) * 16, SEEK_SET);

	struct meta clearMeta = {""}; // write an empty 64 byte bank to clear the meta data for file
	fwrite(&clearMeta, sizeof(struct meta), 1, fp);

	/* Delete FAT data */
	fseek(fp, (fileIndex - 1) * 16, SEEK_SET);	// seek to the FAT index
	struct fat clearfat = {""};
	fwrite(&clearfat, sizeof(struct fat), 1, fp);

	/* Delete from dir */
	for(int i = 0; i < 32; i++){
		fseek(fp, CURRENT_DIR + (i * 16), SEEK_SET);
		fread(entryName, 1, 12, fp);
		if (strcmp(entryName, fileName) == 0){
			fseek(fp, CURRENT_DIR + (i * 16), SEEK_SET);
			struct line clearline = {""};
			fwrite(&clearline, sizeof(struct line), 1, fp);
		}
	}
}
/* ------------------------------ Read ------------------------------ */
void fs_read(char *fileName){
	int fileIndex = 0;
	char buffer[512];
	char dataPtr[6];
	long dataPtrAsNum;

	fileIndex = findFileByName(fileName); //returns the FAT index of this file
	if (fileIndex == -1){	// file does not exist
		printf(BRIGHT_RED "FILE NOT FOUND: %s\n" COLOR_RESET, fileName);
		return;
	}
	fseek(fp, (fileIndex - 1) * 16, SEEK_SET);	// seek to the FAT index

	fseek(fp, 19, SEEK_CUR); 					// seek to the offset of dataPtr
	fread(dataPtr , 1, 6, fp); 					// read back dataPtr
	dataPtrAsNum = atol(dataPtr); 				// convert string -> number

	fseek(fp, (dataPtrAsNum - 1) * 16, SEEK_SET);
	fread(buffer, 1, 512, fp);

	for (int i = 0; i < 512; i++){
		printf("%c", buffer[i]);
	}
	printf("\n");
}
/* ------------------------------ Write ------------------------------ */
void fs_write(char *fileName, char *writeData){
	int fileIndex = 0;
	int inputLength = 0;
	char dataPtr[6];
	long dataPtrAsNum; 

	fileIndex = findFileByName(fileName); //returns the FAT index of this file
	//printf("fileIndex: %d\n", fileIndex);
	if (fileIndex == -1){	// file does not exist
		printf("FILE NOT FOUND: %s\n", fileName);
		return;
	}
	inputLength = strlen(writeData);

	if(inputLength >= 512){	// data will have to take more than one block
		printf("SYSTEM| allocating more blocks\n");
		return;	// Not yet implemented
	}

	fseek(fp, (fileIndex - 1) * 16, SEEK_SET);	// seek to the FAT index
	fseek(fp, 19, SEEK_CUR); 					// seek to the offset of dataPtr

	fread(dataPtr , 1, 6, fp); 					// read back dataPtr
	dataPtrAsNum = atol(dataPtr); 				// convert string -> number

	fseek(fp, (dataPtrAsNum - 1) * 16, SEEK_SET);
	fwrite(writeData, 1, BLOCK_SIZE, fp);
}
/* ------------------------------ up ------------------------------ */
void fs_up(){	// same as cd ..
	CURRENT_DIR = PREV_DIR;
}
void fs_cd(char *fileName){
	
}
/* ------------------------------ Info ------------------------------ */
void fs_info(char *fileName){
	char fName[13] = {'\0'};
	char fext[4] = {'\0'};
	char cyear[5] = {'\0'};
	char cmon[3] = {'\0'};
	char cday[3]= {'\0'};
	char chour[3]= {'\0'};
	char cmin[3]= {'\0'};
	char csec[3]= {'\0'};

	int fileIndex = 0;
	char metaPtr[6];
	long metaPtrAsNum;

	fileIndex = findFileByName(fileName); //returns the FAT index of this file
	if (fileIndex == -1){	// file does not exist
		printf(BRIGHT_RED "FILE NOT FOUND: %s\n" COLOR_RESET, fileName);
		return;
	}
	fseek(fp, (fileIndex - 1) * 16, SEEK_SET);	// seek to the FAT index
	fseek(fp, 13, SEEK_CUR); 					// seek to the offset of metaPtr

	fread(metaPtr , 1, 6, fp); 					// read back metaPtr
	metaPtrAsNum = atol(metaPtr); 				// convert string -> number

	fseek(fp,(metaPtrAsNum - 1) * 16, SEEK_SET);	// Seek to files metaPtr block

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
/* ------------------------------ List ------------------------------ */
void fs_list(){
	char fileName[13] = {'\0'};

	for(int i = 0; i < 32; i++){
		fseek(fp, CURRENT_DIR + (i * 16), SEEK_SET);
		// fileName = {'\0'};  // clear out fileName 
		fread(fileName, 1, 12, fp);
		printf("%s   ", fileName);
		}
	printf("\n");
}

/* ------------------------------ Exit ------------------------------ */
void fs_exit(){ // closes the disk,  and exits
	fclose(fp);
	printf("SYSTEM| shutdown\n");
	exit(0);
}