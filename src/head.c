#include "include/utils.h"
#include "types.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 2048

#define FLAG_BYTES (1 << 0)
#define FLAG_LINES (1 << 1)

const char* usage = "head [FILE...]\nget the 10 first lines of each FILE";

struct option options[] = {
    optv("bytes",'b',FLAG_BYTES,"get N bytes",TYPE_INT,(void*)(intptr_t)10),
    optv("lines",'n',FLAG_LINES,"get N lines",TYPE_INT,(void*)(intptr_t)10),
};


int head(const char* filepath, struct parsed flags) {
  FILE* file = fopen(filepath, "r");
  if(file == NULL) {
    printf("head: %s does not exist\n", filepath);
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
  if(!(flags.flags & FLAG_BYTES)) {
    char buf[MAX_LINE_LENGTH];
    while (fgets(buf, sizeof(buf), file) != NULL) {
      count++;
      printf("%s", buf);
      if(count == lines)break;
    }
  } else {
    char c;
    while ((c = fgetc(file)) != EOF) {
      count++;
      printf("%c", c);
      if(count == bytes) break;
    }
  }
  fclose(file);
  return 0;
}

int main(int argc, char** argv) {
  struct parsed flags = parse_args(argc, argv, options, array_len(options));
  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-') {c++; continue;};
    if(head(argv[c], flags)) return 1;
  }
}
