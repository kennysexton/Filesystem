# Filesystem
A CIS 3207 Project

## Design

**FAT** - contains filename and pointers to other memory locations

**meta** - holds the metadata for files

**data** - raw 512 blocks that are used for storing info

![img1](/img1.jpg)

### FAT
```
valid[1];	// 1 byte
fileName[12];   // 12 bytes
metaPtr[6];	// 6 bytes
dataPtr[6]; 	// 6 bytes
nextPtr[6];	// 6 bytes
 ```

### meta
```
fileName[12];  	// 12 bytes
ext[3]; 	// 3 bytes
create_year[4];	// 4 bytes
create_month[2]; // 2 bytes
create_day[2]; 	// 2 bytes
create_hour[2]; // 2 bytes
create_min[2]; 	// 2 bytes
create_sec[2]; 	// 2 bytes
modify_year[4];	 // 4 bytes
modify_month[2]; // 2 bytes
modify_day[2]; 	// 2 bytes
modify_hour[2]; // 2 bytes
modify_min[2]; 	// 2 bytes
modify_sec[2]; 	// 2 bytes
file_size[6];	 // 6 bytes
```

### data
```
data[512];   // 512 bytes
```

## User Commands
**create** - makes a new empty file

**delete** - removes a file or directory

**read** - reads back the data blocks assoicated with a file

**write** - writes data to a file

**mkdir** - same as create, but makes a file of type 'DIR'

**cd** - used to navigate directories

**info** - prints meta information about a file

**clear** - clears the screen

**list** - lists the files in the current directory

**exit** - exits the program
