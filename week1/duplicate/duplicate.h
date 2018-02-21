#ifndef DUPLICATE_DUPLICATE_H
#define DUPLICATE_DUPLICATE_H

typedef struct filedata *file;

typedef struct filedata {
  long size;
  char path[1024];
};

void singledir(char *argv);
void traverse(char *fn, int indent, file* files, int* curPos, int size);

#endif //DUPLICATE_DUPLICATE_H
