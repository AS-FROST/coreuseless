#include "include/utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include <stdlib.h>

#define FLAG_NO_CREATE (1 << 0)
#define FLAG_VERBOSE (1 << 1)
#define FLAG_ACCSS_DATE_ONLY (1 << 2)
#define FLAG_MODIF_DATE_ONLY (1 << 3)

const char* usage = "touch [FILENAME...]\ncreates FILENAME(S) or edit the modification/access date of FILENAME(S)";

struct option options[] = {
  opt("no-create",'c',FLAG_NO_CREATE,"do not create a file"),
  opt("verbose",'v',FLAG_VERBOSE,"get more informations"),
  opt("access-date",'a',FLAG_ACCSS_DATE_ONLY,"edit the access date only"),
  opt("modification-date",'m',FLAG_MODIF_DATE_ONLY,"edit the modification date only"),
};

int touch(const char* filename, int flags) {
  FILE* file = fopen(filename, "r");
  if(file == NULL) {
    if(flags & FLAG_NO_CREATE) return 0;
    fopen(filename, "w");
    if(flags & FLAG_VERBOSE) printf("touch: created %s\n", filename);
    return 0;
  }
  struct utimbuf* buf = malloc(sizeof(struct utimbuf*));
  if(flags & FLAG_ACCSS_DATE_ONLY) {
    buf->actime = time(0);
    struct stat stats;
    stat(filename, &stats);
    buf->modtime = stats.st_mtim.tv_sec;
    utime(filename, buf);
    return 0;
  }
  if(flags & FLAG_MODIF_DATE_ONLY) {
    buf->modtime = time(0);
    struct stat stats;
    stat(filename, &stats);
    buf->actime = stats.st_atim.tv_sec;
    utime(filename, buf);
    return 0;
  }

  time_t t = time(0);

  buf->actime = t;
  buf->modtime = t;

  utime(filename, buf);

  return 0;
}

int main(int argc, char** argv) {
  struct parsed flags = parse_args(argc, argv, options, array_len(options));

  int count = 0;
  for(int c = 1; c < argc; c++) {
    if(argv[c][0]=='-')continue;
    if(touch(argv[c], flags.flags)) return 1;
    count++;
  }
  if (count == 0) {
    puts("touch: no arguments given");
    return 1;
  }
}
