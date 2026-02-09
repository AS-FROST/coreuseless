#include "utils.h"
#include <stdio.h>
#include <sys/utsname.h>

#define ALL_FLAG (1 << 0)
#define OS_TYPE_FLAG (1 << 1)
#define NODENAME_FLAG (1 << 2)
#define KERNEL_RELEASE_FLAG (1 << 3)
#define KERNEL_VERSION_FLAG (1 << 4)
#define ARCH_FLAG (1 << 5)

const char* usage = "uname\nget informations on the system";

struct option options[] = {
  opt("all",'a',ALL_FLAG,"get every information"),
  opt("os-type",'o',OS_TYPE_FLAG,"get the os type"),
  opt("nodename",'n',NODENAME_FLAG,"get the nodename"),
  opt("kernel-release",'r',KERNEL_RELEASE_FLAG,"get the kernel release"),
  opt("kernel-version",'v',KERNEL_VERSION_FLAG,"get the kernel version"),
  opt("arch",'A',ARCH_FLAG,"get the architecture of the system"),
};

int main(int argc, char** argv) {
  struct parsed parsed_args = parse_args(argc, argv, options, array_len(options));
  struct utsname sys_infos;
  if(uname(&sys_infos) != 0) {
    perror("uname: ");
    return 1;
  }
  if(parsed_args.flags & ALL_FLAG) {
    printf("%s %s %s %s %s\n", sys_infos.sysname, sys_infos.nodename, sys_infos.release, sys_infos.version, sys_infos.machine);
    return 0;
  }
  bool should_space = false;
  if(parsed_args.flags & OS_TYPE_FLAG) {
    printf("%s", sys_infos.sysname);
    should_space = true;
  }
  if(parsed_args.flags & NODENAME_FLAG) {
    if(should_space) putchar(' ');
    printf("%s", sys_infos.nodename);
    should_space = true;
  }
  if(parsed_args.flags & KERNEL_RELEASE_FLAG) {
    if(should_space) putchar(' ');
    printf("%s", sys_infos.release);
    should_space = true;
  }
  if(parsed_args.flags & KERNEL_VERSION_FLAG) {
    if(should_space) putchar(' ');
    printf("%s", sys_infos.version);
    should_space = true;
  }
  if(parsed_args.flags & ARCH_FLAG) {
    if(should_space) putchar(' ');
    printf("%s", sys_infos.machine);
  }
  putchar('\n');
}
