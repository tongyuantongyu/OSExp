//
// Created by TYTY on 2020-05-04 004.
//

#include <iostream>
#include <sstream>

#include "disk_fs.h"

int main() {
  auto fs = new ContinuousFileSystem();
  std::wstringstream buf;
  size_t address;
  size_t size;
  File file{};
  while (true) {
    buf.clear();
    buf.str(std::wstring());
    const auto input_ptr = new wchar_t[256];
    std::wcin.getline(input_ptr, 256);
    buf << input_ptr;
    switch (buf.get()) {
      case L'q':
        goto CLEAN;
      case L'a':
        buf >> size;
        std::wcout << FileInfo(fs->Alloc(size)) << std::endl;
        break;
      case L'f':
        buf >> address >> size;
        file = File{address, size};
        fs->Free(file);
        break;
      case L'l':
        std::wcout << fs->Status();
        std::wcout.flush();
      default: ;
    }
  }
CLEAN:
  delete fs;
}