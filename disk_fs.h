//
// Created by TYTY on 2020-05-03 003.
//

#ifndef OSEXP__DISK_FS_H_
#define OSEXP__DISK_FS_H_

#include <map>

const size_t TRACK = 200;
const size_t CYLINDER = 20;
const size_t SECTOR = 6;
const size_t SECTOR_SIZE = 1u << 10u;
const size_t CAPACITY = TRACK * CYLINDER * SECTOR;

struct PhysicalAddress {
  size_t track;
  size_t cylinder;
  size_t sector;
};

struct File {
  size_t address;
  size_t size;
};

const File BAD_FILE = File{.address = size_t(-1), .size = 0};

PhysicalAddress Logical2Physical(size_t logical);

size_t Physical2Logical(const PhysicalAddress &physical);

std::wstring FileInfo(const File &file);

typedef std::map<size_t, size_t> SpareRecord;

class ContinuousFileSystem {
  SpareRecord spare{{0, CAPACITY}};
 public:
  File Alloc(size_t size);
  bool Free(File &file);
  std::wstring Status();
};

#endif //OSEXP__DISK_FS_H_
