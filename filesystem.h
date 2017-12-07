#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

FILE setup();
int closefs(FILE *fp);
void fs_create(char *fileName);
void fs_read(char *fileName);
void fs_info(char *fileName);
void fs_exit();

#endif // FILESYSTEM_H_
