#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>

#define FLAG_DIRECTORY (1 << 0)

const char* usage = "mktemp\nmakes a temporary file";

struct option options[] = {
  opt("directory",'d',FLAG_DIRECTORY,"creates a temporary directory"),
};

int mktempdir(int flags) {
  char template[] = "/tmp/tmp.XXXXXX";
  char* tmp;
  if(flags & FLAG_DIRECTORY) tmp = mkdtemp(template);
  else {
    tmp = mktemp(template);
    if(tmp == NULL) {
      perror("mktemp");
      return 1;
    }
    FILE* f = fopen(tmp, "w");
    if(f == NULL) {
      perror("mktemp");
      return 1;
    }
  };
  if(tmp == NULL) {
    perror("mktemp: ");
    return 1;
  }
  puts(tmp);
  return 0;
}

int main(int argc, char** argv) {
  struct parsed parse = parse_args(argc, argv, options, array_len(options));
  if(mktempdir(parse.flags)) return 1;
}
