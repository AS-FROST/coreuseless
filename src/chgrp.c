#include "utils.h"
#include <grp.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char* usage = "chgrp [GROUP] [FILE]\nChange file group ownership";

struct option options[] = {
  
};

int my_chgrp(char* group_name, char* file_path) {
  struct group* grp = getgrnam(group_name);
  if(grp == NULL) {
    fprintf(stderr, "chgrp: %s: %s\n", group_name, strerror(errno));
    return 1;
  }
  gid_t gid = grp->gr_gid;
  FILE* file = fopen(file_path, "r");
  if(file == NULL) {
    errno = ENOENT;
    fprintf(stderr, "chgrp: %s: %s\n", file_path, strerror(errno));
    return 1;
  }
  return chown(file_path, -1, gid);
}

int main(int argc, char** argv) {
  char* user = NULL; char* file_path = NULL;
  for(int c = 1; c < argc; c++){
    if(user == NULL) {
      user = argv[c];
    } else {
      file_path = argv[c];
      break;
    }
  }
  if(my_chgrp(user, file_path)) return 1;
}
