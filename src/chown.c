#include "utils.h"
#include <pwd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char* usage = "chown [USER] [FILE]\nChange file user ownership";

struct option options[] = {
  
};

int my_chown(char* username, char* file_path) {
  struct passwd* pwd = getpwnam(username);
  if(pwd == NULL) {
    fprintf(stderr, "chown: %s: %s\n", username, strerror(errno));
    return 1;
  }
  uid_t uid = pwd->pw_uid;
  FILE* file = fopen(file_path, "r");
  if(file == NULL) {
    errno = ENOENT;
    fprintf(stderr, "chown: %s: %s\n", file_path, strerror(errno));
    return 1;
  }
  return chown(file_path, uid, -1);
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
  if(my_chown(user, file_path)) return 1;
}
