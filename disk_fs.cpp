//
// Created by TYTY on 2020-05-03 003.
//

#include <sstream>
#include "misc.h"
#include "disk_fs.h"

PhysicalAddress Logical2Physical(const size_t logical) {
  return PhysicalAddress{
      .track = logical / SECTOR / CYLINDER,
      .cylinder = size_t(logical / SECTOR) % CYLINDER,
      .sector = logical % SECTOR
  };
}

size_t Physical2Logical(const PhysicalAddress &physical) {
  return physical.track * CYLINDER * SECTOR + physical.cylinder * SECTOR + physical.sector;
}

std::wstring FileInfo(const File &file) {
  if (file.address == size_t(-1) && file.size == 0) {
    return L"Bad file.";
  }
  const auto physical = Logical2Physical(file.address);
  std::wstringstream buffer;
  buffer << L"File sized " << file.size << " bytes located at logical address:" << file.address
         << ", physical address: track" << physical.track << L",cylinder" << physical.cylinder
         << L",sector" << physical.sector;
  return buffer.str();
}

File ContinuousFileSystem::Alloc(const size_t size) {
  const auto blocks = size_t((size - 1) / SECTOR_SIZE) + 1;
  if (UNLIKELY(size == 0 || blocks > CAPACITY)) {
    // 0 byte file or too big file
    return BAD_FILE;
  }
  size_t selected = 0;
  auto selected_size = size_t(-1);
  for (const auto &span : this->spare) {
    if (span.second < blocks) {
      continue;
    }
    if (span.second < selected_size) {
      selected = span.first;
      selected_size = span.second;
    }
  }
  if (UNLIKELY(selected_size == size_t(-1))) {
    // can't find long enough spare span
    return BAD_FILE;
  }
  this->spare.erase(selected);
  if (selected_size > blocks) {
    spare.insert({selected + blocks, selected_size - blocks});
  }
  return File{.address = selected, .size = blocks * SECTOR_SIZE};
}

inline bool overlap(const size_t begin1, const size_t len1, const size_t begin2, const size_t len2) {
  return begin1 + len1 != begin2 &&
         begin2 + len2 != begin1 &&
         ssize_t((begin1 + len1 - begin2) ^ (begin1 - begin2 - len2)) < 0;
}

bool ContinuousFileSystem::Free(File &file) {
  auto address = file.address;
  auto blocks = size_t((file.size - 1) / SECTOR_SIZE) + 1;
  file = BAD_FILE;
  if (UNLIKELY(blocks == 0 ||
               address > CAPACITY ||
               blocks > CAPACITY ||
               address + blocks > CAPACITY)) {
    return false;
  }
  auto it = spare.begin();
  for (; it != spare.end(); ++it) {
    if (UNLIKELY(overlap(address, blocks, it->first, it->second))) {
      return false;
    }
    if (UNLIKELY(it->first >= address + blocks)) {
      break;
    }
  }
  if (it != spare.end()) {
    if (address + blocks == it->first) {
      // concat behind
      blocks += it->second;
      // erase current and move before
      spare.erase(it--);
    }
    --it;
  }
  else if (UNLIKELY(it == spare.begin() && it == spare.end())) {
    // all space has been allocated.
    spare.insert({address, blocks});
    return true;
  }
  else {
    --it;
  }
  if (it->first + it->second == address) {
    // concat before
    address = it->first;
    blocks += it->second;
    // remove before and move to suitable hint position
#if __cplusplus >= 201103L
    spare.erase(it++);
#else
    spare.erase(it--);
#endif
  }
  // insert free span with position hint
  spare.insert(it, {address, blocks});
  return true;
}

std::wstring ContinuousFileSystem::Status() {
  std::wstringstream buffer;
  buffer << L"Spare span status: \n";
  for (const auto span : this->spare) {
    buffer << span.first << L'[' << span.second << L']' << std::endl;
  }
  return buffer.str();
}
