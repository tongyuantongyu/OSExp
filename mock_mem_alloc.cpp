//
// Created by TYTY on 2020-05-03 003.
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

#include "mem_alloc.h"

int main() {
  auto alloc = new PageMemoryAllocator();
  std::wstringstream buf;
  PID pid;
  size_t alloc_size;
  std::wstring free_mask;
  while (true) {
    buf.clear();
    buf.str(std::wstring());
    const auto input_ptr = new wchar_t[256];
    std::wcin.getline(input_ptr, 256);
    buf << input_ptr;
    switch (buf.get()) {
      case L'q':
        goto CLEAN;
      case L'c':
        buf >> pid;
        alloc->Create(pid);
        break;
      case L'a':
        buf >> pid;
        buf >> alloc_size;
        std::wcout << L"Allocated page: " << std::bitset<64>(alloc->Alloc(pid, alloc_size)) << std::endl;
        break;
      case L'f':
        buf >> pid;
        buf >> free_mask;
        alloc->Free(pid, std::bitset<64>(free_mask).to_ullong());
        break;
      case L'r':
        buf >> pid;
        alloc->Clean(pid);
        break;
      case L's':
        buf >> pid;
        std::wcout << L"Page of PID " << pid << L":" << std::endl << ShowPage(alloc->Status(pid));
        break;
      case L'l':
        std::wcout << alloc->ShowMap();
    }
  }
  CLEAN:
  delete alloc;
}