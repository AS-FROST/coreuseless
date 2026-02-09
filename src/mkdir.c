#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#define FLAG_PARENTS (1 << 0)
#define FLAG_VERBOSE (1 << 1)
#define FLAG_MODE (1 << 2)

const char* usage = "mkdir [DIRECTORY...]\nmake a/multiple DIRECTORY.IES";

struct option options[] = {
  opt("mode",'m',FLAG_MODE,"change mode of the new directory.ies"),
  opt("parents",'p',FLAG_PARENTS,"create parents if needed"),
  opt("verbose",'v',FLAG_VERBOSE,"get more infos"),
};


int mkdirectory(char* path, struct parsed flags) {
  mode_t mode = 0777;
  for(size_t c = 0; c < flags.value_count; c++) {
    if(!strcmp(flags.value[c].flag_name, "mode")) {
      mode = (mode_t)(intptr_t)flags.value[c].value;
    }
  }

  if(!(flags.flags & FLAG_PARENTS)) {
    DIR* first_dir = opendir(path);
    if(first_dir != NULL) {
      closedir(first_dir);
      errno = EEXIST;
      fprintf(stderr, "mkdir: %s: %s", path, strerror(errno)) ;
      return 1;
    }
    if(mkdir(path, mode)) {
      fprintf(stderr, "mkdir: %s", strerror(errno));
      return 1;
    }
    if(flags.flags & FLAG_VERBOSE) printf("mkdir: created %s\n", path);
  } else {
    char* first_path = strtok(path, "/");
    DIR* first_dir = opendir(first_path);
    if(first_dir == NULL) {
      if(mkdir(first_path, mode)) {
        fprintf(stderr, "mkdir: %s: %s", first_path, strerror(errno));
      }
      if(flags.flags & FLAG_VERBOSE) printf("mkdir: created %s\n", first_path);
    }
    char* next_path = strdup(first_path);
    strcat(next_path, "/");
    while((first_path = strtok(NULL, "/")) != NULL) {
      strcat(next_path, first_path);
      if(strlen(path) != strlen(next_path)) strcat(next_path, "/");
      if(mkdir(next_path, mode)) {
        fprintf(stderr, "mkdir: %s: %s\n", next_path, strerror(errno));
      }
      if(flags.flags & FLAG_VERBOSE) printf("mkdir: created %s\n", next_path);
    }
  }

  return 0;
}

int main(int argc, char** argv) {
  struct parsed flags = parse_args(argc, argv, options, array_len(options));

  if(argc < 2) {
    puts("mkdir: need at least 1 argument");
    return 1;
  }
  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-') {c++; continue;};
    if(mkdirectory(argv[c], flags)) return 1;
  }
}
