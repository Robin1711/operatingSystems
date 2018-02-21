/* CELEBO01

   This example opens a directory.

 */
#define _POSIX_SOURCE
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>

void traverse(char *fn, int indent) {
  DIR *dir;
  struct dirent *entry;
  int count;
  char path[1025];
  struct stat info;

  printf("%*s %s\n", indent, "", fn);

  if ((dir = opendir(fn)) == NULL)
    perror("opendir() error");
  else {
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] != '.') {
        strcpy(path, fn);
        strcat(path, "/");
        strcat(path, entry->d_name);
        if (stat(path, &info) != 0)
          fprintf(stderr, "stat() error on %s: %s\n", path,
                  strerror(errno));
        else if (S_ISDIR(info.st_mode))
               traverse(path, indent+1);
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
  puts("Directory structure:");
  traverse(argv[1], 0);
  return 0;
}
//main() {
//  puts("Directory structure:");
//  traverse("/home/s2696320/Documents/operatingSystems", 0);
//}
