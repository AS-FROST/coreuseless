#include "include/utils.h"
#include "types.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 2048

#define FLAG_BYTES (1 << 0)
#define FLAG_LINES (1 << 1)

const char* usage = "head [FILE...]\nget the 10 first lines of each FILE";

struct option options[] = {
    optv("bytes",'b',FLAG_BYTES,"get N bytes",TYPE_INT,(void*)(intptr_t)10),
    optv("lines",'n',FLAG_LINES,"get N lines",TYPE_INT,(void*)(intptr_t)10),
};


int head(const char* filepath, struct parsed flags, int argc) {
  FILE* file = fopen(filepath, "r");
  if(file == NULL) {
    fprintf(stderr, "head: %s: %s", filepath, strerror(errno));
    return 1;
  }
  int bytes = 10; int lines = 10; int count = 0;
  for(size_t c = 0; c < flags.value_count; c++) {
      if(flags.value[c].flag_name && !strcmp(flags.value[c].flag_name, "bytes")) {
        bytes = (int)(intptr_t)flags.value[c].value;
      } else if (flags.value[c].flag_name && !strcmp(flags.value[c].flag_name, "lines")) {
        lines = (int)(intptr_t)flags.value[c].value;
      }
  }
  if(argc > 0) {
    printf("==> %s <==\n", filepath);
  }
  if(!(flags.flags & FLAG_BYTES)) {
    char buf[MAX_LINE_LENGTH];
    while (fgets(buf, sizeof(buf), file) != NULL) {
      count++;
      printf("%s", buf);
      if(count == lines)break;
    }
  } else {
    int c;
    while ((c = fgetc(file)) != EOF) {
      count++;
      printf("%c", c);
      if(count == bytes) break;
    }
  }
  fclose(file);
  putchar('\n');
  return 0;
}

int main(int argc, char** argv) {
  struct parsed flags = parse_args(argc, argv, options, array_len(options));
  char** str = {0}; int ind = 0;
  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-') {c++; continue;};
    // if(head(argv[c], flags, argc)) return 1;
    str = realloc(str, (ind+1)*sizeof(char*));
    str[ind]=argv[c];
    ind++;
  }
  for(int c = 0; c < ind; c++) {
    if(head(str[c], flags, ind)) return 1;
  }
}
