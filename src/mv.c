#include "include/utils.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

#define FLAG_FORCE (1 << 0)
#define FLAG_VERBOSE (1 << 1)

const char* usage = "mv [SOURCE] [DESTINATION]\nmove SOURCE to DESTINATION";

struct option options[] = {
  opt("force",'f',FLAG_FORCE,"force move file"),
  opt("verbose",'v',FLAG_VERBOSE,"get more infos"),
};


int mv(const char* from, const char* to, int flags) {
  FILE* file_from = fopen(from, "r");

  if(file_from == NULL) {
    fprintf(stderr, "mv: %s: %s\n", from, strerror(errno));
    return 1;
  }
  
  FILE* file_to = fopen(to, "r");

  if(file_to != NULL && !(flags & FLAG_FORCE)) {
    errno = EEXIST;
    fprintf(stderr, "mv: %s: %s\n", to, strerror(errno));
    return 1;
  }

  int status = rename(from, to);
  if (status != 0) {
    perror("mv");
    return status;
  }

  if(flags & FLAG_VERBOSE) printf("mv: moved file %s to %s\n", from, to);

  fclose(file_from);
  if(file_to != NULL) fclose(file_to);

  return 0;
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  struct parsed flags = parse_args(argc, argv, options, array_len(options));

  const char* from = NULL; const char* to = NULL;
  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-')continue;
    if(!from){
      from = argv[c];
    } else {
      to = argv[c];
      break;
    }
  }

  if(!from || !to) {
    puts("mv: need SOURCE and DESTINATION args");
    return 1;
  }

  return mv(from, to, flags.flags);
}
