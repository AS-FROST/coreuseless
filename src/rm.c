#include "include/utils.h"
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define FLAG_RECURSIVE (1 << 0)
#define FLAG_VERBOSE (1 << 1)

const char* usage = "rm [FILES...]\nremove FILES from the filesystem\n-r, --recursive   delete folders\n-i, --verbose   program gets verbose";

struct option options[] = {
  opt("recursive",'r',FLAG_RECURSIVE,"delete folders"),
  opt("verbose",'v',FLAG_VERBOSE,"program gets verbose"),
};

int rm(const char* path, int flag) {
  DIR* dir = opendir(path);
  if ((flag & FLAG_RECURSIVE) && dir != NULL) {
    struct dirent* content;
    while((content = readdir(dir)) != NULL) {
      if(!strcmp(content->d_name, ".") || !strcmp(content->d_name, ".."))continue;
      char* file_path = strdup(path);
      strcat(file_path, "/");
      strcat(file_path, content->d_name);
      rm(file_path, flag);
    }
    return remove(path);
  }
  closedir(dir);
  FILE* file = fopen(path, "r");

  if (file == NULL) {
    fprintf(stderr, "rm: %s: %s", path, strerror(errno));
    return 1;
  }
  
  fclose(file);

  if((flag & FLAG_VERBOSE)) {
    printf("rm: deleted %s\n", path);
  }

  return remove(path);
}

int main(int argc, char** argv) {

  struct parsed flags = parse_args(argc, argv, options, array_len(options));

  int count = 0;
  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-')continue;
    if(rm(argv[c], flags.flags)) {
      return 1;
    }
    count++;
  }
  if(count == 0) {
    puts("rm: need paths to delete");
    return 1;
  }
  return 0;
}
