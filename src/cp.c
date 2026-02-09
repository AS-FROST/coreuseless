#include "include/utils.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define FLAG_FORCE (1 << 0)
#define FLAG_RECURSIVE (1 << 1)
#define FLAG_VERBOSE (1 << 2)

const char* usage = "cp [SOURCE] [DESTINATION]\ncopy the content of SOURCE to DESTINATION\n-f, --force    force copy the SOURCE in DESTINATION, if it exists or not\n-r, --recursive    copy SOURCE folder and its content in DESTINATION\n-i, --verbose    get more information";

struct option options[] = {
  opt("force",'f',FLAG_FORCE,"force create file if exists"),
  opt("recursive",'r',FLAG_RECURSIVE,"copy an entire directory"),
  opt("verbose",'v',FLAG_VERBOSE,"get more information"),
};


int cp(const char* from, const char* to, int flags) {
  DIR* dir = opendir(from);
  if(dir != NULL) {
    if(!(flags & FLAG_RECURSIVE)) {
      printf("cp: cannot copy folder %s\n", from);
      return 1;
    }
    DIR* to_folder = opendir(to);
    if(to_folder == NULL) {
      int status = mkdir(to, 0777);
      if(status) {
        printf("cp: could not create directory %s\n", to);
        return 1;
      }
    }
    to_folder = opendir(to);
    if(to_folder != NULL) {
      if(flags & FLAG_VERBOSE) printf("cp: created directory %s\n", to);
      struct dirent* content;
      while((content = readdir(dir)) != NULL) {
        if(!strcmp(content->d_name, ".") || !strcmp(content->d_name, ".."))continue;
        if(content->d_type == DT_REG || content->d_type == DT_LNK) {
          char* from_path = strdup(from); strcat(from_path, "/"); strcat(from_path, content->d_name);
          char* to_path = strdup(to); strcat(to_path, "/"); strcat(to_path, content->d_name);
          
          cp(from_path, to_path, flags);
        }
      }
      if(flags & FLAG_VERBOSE) printf("cp: copied %s to %s\n", from, to);
      return 0;
    }

  }
  FILE* file = fopen(from, "r");
  if (file == NULL) {
    printf("cp: file %s not found\n", from);
    return 1;
  }
  
  FILE* new_file = fopen(to, "r");
  if(new_file != NULL && !(flags & FLAG_FORCE)) {
    printf("cp: file %s already exists\n", to);
    return 1;
  }
  new_file = fopen(to, "w");
  int c;
  while((c = fgetc(file)) != EOF) {
    fputc(c, new_file);
  }

  if(flags & FLAG_VERBOSE) {
    printf("cp: copied %s to %s\n", from, to);
  }

  fclose(file);
  fclose(new_file);
  return 0;
}

int main(int argc, char** argv) {
  struct parsed flags = parse_args(argc, argv, options, array_len(options));

  char* from = NULL;
  char* to = NULL;
  
  for(int c = 1; c < argc; c++) {
    if(argv[c][0] == '-') continue;
    if(from == NULL) {
      from = argv[c];
    } else if ((from != NULL) && (to == NULL)) {
      to = argv[c];
      break;
    }
  }
  if (from == NULL || to == NULL) {
    puts("cp: missing arguments");
    return 1;
  }
  return cp(from, to, flags.flags);
}
