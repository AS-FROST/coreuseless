#include "include/utils.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FLAG_SYMBOLIC (1 << 0)
#define FLAG_FORCE (1 << 1)
#define FLAG_VERBOSE (1 << 2)

const char* usage = "ln [SOURCE] [DESTINATION]\nlink DESTINATION to SOURCE";

struct option options[] = {
  opt("symbolic",'s',FLAG_SYMBOLIC,"symbolically link files"),
  opt("force",'f',FLAG_FORCE,"force link file"),
  opt("verbose",'v',FLAG_VERBOSE,"get more information"),
};

int ln(const char* from, const char* to, int flags) {
  FILE* file_from = fopen(from, "r");

  if(file_from == NULL) {
    fprintf(stderr, "ln: %s: %s", from, strerror(errno));
    return 1;
  }

  FILE* file_to = fopen(to, "r");
  if(file_to != NULL && !(flags & FLAG_FORCE)) {
    errno = EEXIST;
    fprintf(stderr, "ln: %s: %s", to, strerror(errno));
    return 1;
  }

  if(!(flags & FLAG_SYMBOLIC)) link(from, to);
  else symlink(from, to);

  if(flags & FLAG_VERBOSE) printf("ln: linked %s to %s\n", to, from);

  return 0;
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");
  struct parsed flags = parse_args(argc, argv, options, array_len(options));

  char* from = NULL; char* to = NULL;
  if(argc < 3) {
    puts("ln: need 2 arguments");
    return 1;
  }
  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-')continue;
    if(from == NULL) {
      from = argv[c];
    } else {
      to = argv[c];
      break;
    }
  }
  if(from == NULL || to == NULL) {
    puts("ln: need 2 arguments");
    return 1;
  }

  return ln(from, to, flags.flags);
}
