//
// Created by TYTY on 2020-05-03 003.
//

#ifndef OSEXP__MEM_ALLOC_H_
#define OSEXP__MEM_ALLOC_H_

#include <vector>
#include <map>
#include <unordered_map>
#include <cmath>
#include <bit>

const size_t PAGE_LENGTH = 10;

typedef uint16_t PID;

typedef std::map<size_t, size_t> Page;
const Page EMPTY_PAGE = Page();

std::wstring ShowPage(const Page &page);

class PageMemoryAllocator {
  std::unordered_map<PID, Page> page_table;
  uint64_t bitmap = 0;
  size_t free = 64;
  size_t scan_start = 0;
 public:
  bool Create(PID pid);
  bool Clean(PID pid);
  size_t Alloc(PID pid, size_t size);
  bool Free(PID pid, size_t pos);
  [[nodiscard]] const Page &Status(PID pid) const;
  [[nodiscard]] std::wstring ShowMap() const;
};

#endif //OSEXP__MEM_ALLOC_H_
