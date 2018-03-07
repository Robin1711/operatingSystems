#ifndef DUPLICATE_DUPLICATE_H
#define DUPLICATE_DUPLICATE_H

typedef struct filedata *file;

typedef struct filedata {
  long size;
  char* path;
};

void singledir(char *argv);
void collectFiles(char *folderName, file *files, int *curPos, int *size);

#endif //DUPLICATE_DUPLICATE_H
