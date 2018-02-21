#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

void listdir(char *argv) {
  DIR *mydirhandle;
  struct dirent *mydirent;

//  for (int i = 0; argv[i] != '\0'; i ++) {
//    printf("%c\n", argv[i]);
//  }
  printf("\nTrying to open %s\n", argv);
  if ((mydirhandle = opendir(argv)) == NULL) {
    perror("opendir ");
    exit(1);
  }
  printf("%s/\n", argv);
  while ((mydirent = readdir(mydirhandle)) != NULL) {
    if ((strcmp(mydirent->d_name, ".") == 0)
        || (strcmp(mydirent->d_name, "..") == 0)
        || mydirent->d_name[0] == '.') {
      continue;
    } else {
      printf("\t%s\n", mydirent->d_name);

      //check if next entry is a directory
      if (mydirent->d_type == DT_DIR) {
        int size = (int)sizeof(argv) + (int)sizeof(char) + mydirent->d_namlen;
        char *x = malloc(size*sizeof(char));
        strcpy(x, argv);
        strcat(x, "/");
        strcat(x, mydirent->d_name);
        listdir(x);
      }
    }
  }
  closedir(mydirhandle);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <directory>\n", argv[0]);
    return 0;
  }

  listdir(argv[1]);
  return 0;
}