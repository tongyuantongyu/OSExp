//
// Created by TYTY on 2020-05-04 004.
//

#include <Windows.h>
#include "process_create_work.cpp"

int main(const int argc, char *argv[]) {

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  if (argc == 1) {
    HANDLE children[2];
    // Solve C6335, make VS happy
    HANDLE thread[2];

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(nullptr, (std::string(*argv) + " 0").data(), nullptr,
                  nullptr, false, 0, nullptr, nullptr, &si, &pi);
    children[0] = pi.hProcess;
    thread[0] = pi.hThread;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(nullptr, (std::string(*argv) + " 1").data(), nullptr,
                  nullptr, false, 0, nullptr, nullptr, &si, &pi);
    children[1] = pi.hProcess;
    thread[1] = pi.hThread;
    parent();

    WaitForMultipleObjects(2, children, true, INFINITE);

    CloseHandle(children[0]);
    CloseHandle(children[1]);
    CloseHandle(thread[0]);
    CloseHandle(thread[1]);
  }
  else if (strcmp(*(argv + 1), "0") != 0) {
    child0();
  }
  else if (strcmp(*(argv + 1), "1") != 0) {
    child1();
  }
}