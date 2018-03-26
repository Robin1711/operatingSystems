#include <lib.h>
#include <unistd.h>

int utctime() {
  message m;
  int return_systemcall = _syscall(PM_PROC_NR, UTCTIME, &m);
  if (return_systemcall < 0) {
    return return_systemcall;
  } else {
    return m.m2_l1;
  }
}
