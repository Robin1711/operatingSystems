#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "duplicate.h"

file createFile(char *fpath, char *fname) {
  file f = malloc(sizeof(struct filedata));
  f->size = NULL;
//  int length = (int)sizeof(fpath) + (int)strlen(fname);
//  (*newFile)->path = malloc(length * sizeof(char));
  strcpy(f->path, fpath);
  strcat(f->path, "/");
  strcat(f->path, fname);

  FILE *fp = fopen(f->path, "r");
  fseek(fp, 0, SEEK_END);
  f->size = ftell(fp);
  fclose(fp);

  return f;
}

void traverse(char *fn, int indent, file *files, int *curPos, int size) {
  DIR *dir;
  struct dirent *entry;
  char path[1025];
  struct stat info;
  if ((dir = opendir(fn)) == NULL)
    perror("opendir() error");
  else {
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] != '.') {
        strcpy(path, fn);
        strcat(path, "/");
        strcat(path, entry->d_name);
        if (stat(path, &info) != 0) {
          fprintf(stderr, "stat() error on %s: %s\n", path,
                  strerror(errno));
        } else if (S_ISDIR(info.st_mode)) {
          traverse(path, indent + 1, files, curPos, size);
        } else if (S_ISREG(info.st_mode)) {
//          file* files = malloc(8 * sizeof(file));
          file f = createFile(fn, entry->d_name);
          printf("%d\n", *curPos);
          files[*curPos] = f;
          (*curPos)++;
          printf("%*s %s\n", indent, "", entry->d_name);
        }
      }
    }
    closedir(dir);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <directory>\n", argv[0]);
    return 0;
  }
  printf("%s\n", argv[1]);
  int size = 8;
  int position = 0;
  file *files = malloc(size * sizeof(file));
  traverse(argv[1], 0, files, &position , size);
  for (int i = 0; i < position; i++) {
    printf("%s => ", files[i]->path);
    printf("%ld\n", files[i]->size);
  }
  return 0;
}
