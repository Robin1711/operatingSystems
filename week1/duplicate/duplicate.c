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
#define TRUE 1
#define FALSE 0

// Allocate and return a new file with the length of the contents and the location.
file createFile(char *fpath, char *fname) {
  file f = malloc(sizeof(struct filedata));

  // Get the path of the given file (Add one more to the length for the terminating character)
  int length = (int)strlen(fpath) + (int)strlen(fname) + 1;
  f->path = malloc(length * sizeof(char));
  strcpy(f->path, fpath);
  strcat(f->path, fname);

  // Get the size of the given file
  FILE *fp = fopen(f->path, "r");
  assert(fp != NULL);
  fseek(fp, 0, SEEK_END);
  f->size = ftell(fp);
  fclose(fp);

  return f;
}

// Collect all the files from a folder and put them in *files.
void collectFiles(char *folderName, file *files, int *curPos, int *size) {
  DIR *dir;
  struct dirent *entry;
  char path[1025];
  struct stat info;
  if ((dir = opendir(folderName)) == NULL)
    perror("opendir() error");
  else {
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] != '.') {
        strcpy(path, folderName);
        strcat(path, entry->d_name);
        if (stat(path, &info) != 0) {
          fprintf(stderr, "stat() error on %s: %s\n", path,
                  strerror(errno));
        } else if (S_ISDIR(info.st_mode)) {
          strcat(path, "/");
          collectFiles(path, files, curPos, size);
        } else if (S_ISREG(info.st_mode)) {
          if (*curPos >= *size) {
            (*size) += 10;
            files = realloc(files, (*size) * sizeof(struct filedata));
            assert(files != NULL);
          }
          file f = createFile(folderName, entry->d_name);
          files[*curPos] = f;
          (*curPos)++;
        }
      }
    }
    closedir(dir);
  }
}

// Compare two files (given is the same length) and return TRUE or FALSE based on whether they have the same contents
int compareFiles(file f1, file f2) {
  long lSize1 = f1->size;
  long lSize2 = f2->size;

  FILE *pFile1,*pFile2;
  pFile1 = fopen(f1->path,"r");
  pFile2 = fopen(f2->path,"r");

  char tmp1, tmp2;
  for (int i = 0; i < lSize1; i++) {
    fread(&tmp1, 1, 1, pFile1);
    fread(&tmp2, 1, 1, pFile2);
    if (tmp1 != tmp2) {
      fclose(pFile1);
      fclose(pFile2);
      return FALSE;
    }
  }

  fclose(pFile1);
  fclose(pFile2);
  return TRUE;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <directory>\n", argv[0]);
    return 0;
  }

  // Define size and allocate the array in which the files are stored.
  int size = 2000;
  int elements = 0;
  file* files = malloc(size * sizeof(struct filedata));
  assert(files != NULL);
  collectFiles(argv[1], files, &elements, &size);

  // Compare all files on size. If the size is equal, compare the contents
  for (int i = 0; i < elements; i++) {
    if (files[i] == NULL) continue;
    for (int j = i+1; j < elements; j++) {
      if (files[i]->size == files[j]->size) {
        if (compareFiles(files[i], files[j])) {
          for (int c = (int)strlen(argv[1]); c < (int)strlen(files[i]->path); c++) printf("%c", files[i]->path[c]);
          printf(" and ");
          for (int c = (int)strlen(argv[1]); c < (int)strlen(files[j]->path); c++) printf("%c", files[j]->path[c]);
          printf(" are the same file\n");
        }
      }
    }
    free(files[i]);
  }

  return 0;
}
