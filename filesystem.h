#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

FILE setup();
int closefs(FILE *fp);
void fs_create(char *fileName, int flag);
void fs_delete(char *fileName);
void fs_read(char *fileName);
void fs_write(char *fileName, char *writeData);
void fs_cd(char *fileName);
void fs_info(char *fileName);
void fs_list();
void fs_exit();

#endif // FILESYSTEM_H_
