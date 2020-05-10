//
// Created by TYTY on 2020-05-04 004.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "process_create_work.cpp"

int main() {
  auto pid1 = fork();
  if (pid1 == 0) {
    child0();
    return 0;
  }
  auto pid2 = fork();
  if (pid2 == 0) {
    child1();
    return 0;
  }
  parent();
  waitpid(pid1, nullptr, 0);
  waitpid(pid2, nullptr, 0);
}