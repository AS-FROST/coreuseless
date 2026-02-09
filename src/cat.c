#include "utils.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 8192

const char* usage = "cat [FILE...]\nget the content of a/some file.s";

struct option options[] = {
};

int cat(const char* path) {
  FILE* file = fopen(path, "r");
  if(file == NULL) {
    fprintf(stderr, "cat: %s: %s\n", path, strerror(errno));
    return 1;
  }
  char c;
  while((c = fgetc(file)) != EOF) {
    putchar(c);
  }
  fclose(file);
  return 0;
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  parse_args(argc, argv, options, array_len(options));

  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-')continue;
    if(cat(argv[c])) return 1;
    if(c < argc-1)puts("---");
  }
}
