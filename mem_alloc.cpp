//
// Created by TYTY on 2020-05-03 003.
//

#include <sstream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <bit>
#include "misc.h"
#include "mem_alloc.h"

bool PageMemoryAllocator::Create(const PID pid) {
  if (UNLIKELY(this->page_table.contains(pid))) {
    return false;
  }
  page_table.insert_or_assign(pid, Page());
  return true;
}

bool PageMemoryAllocator::Clean(const PID pid) {
  if (UNLIKELY(!this->page_table.contains(pid))) {
    return false;
  }
  const auto page = page_table[pid];
  this->free += page.size();
  for (const auto entry : page) {
    this->bitmap &= ~(size_t{1} << entry.second);
  }
  return true;
}

size_t PageMemoryAllocator::Alloc(const PID pid, const size_t size) {
  if (UNLIKELY(!this->page_table.contains(pid) || size == 0)) {
    return 0;
  }
  auto count = ((size - 1) >> PAGE_LENGTH) + 1;
  if (UNLIKELY(count > this->free)) {
    return 0;
  }
  std::vector<std::pair<size_t, size_t>> alloc;
  auto &page = page_table[pid];
  ssize_t begin = -1;
  auto bitmap_offset = this->scan_start;
  size_t alloc_map = 0;
  for (const auto entry : page) {
    if (UNLIKELY(count == 0)) {
      break;
    }
    auto span = entry.first - begin - 1;
    if (span == 0) {
      begin = entry.first;
      continue;
    }
    if (span > count) {
      span = count;
    }
    for (size_t i = 0; i < span; ++i) {
      while (this->bitmap & (size_t{1} << bitmap_offset)) {
        ++bitmap_offset;
      }
      auto page_order = begin + i + 1;
      alloc_map |= size_t{1} << page_order;
      this->bitmap |= size_t{1} << bitmap_offset;
      --this->free;
      alloc.emplace_back(page_order, bitmap_offset);
      ++bitmap_offset;
    }
    count -= span;
    begin += span + 1;
  }
  if (count != 0) {
    for (unsigned i = 0; i < count; ++i) {
      while (this->bitmap & size_t{1} << bitmap_offset) {
        ++bitmap_offset;
      }
      unsigned page_order = begin + i + 1;
      alloc_map |= size_t{1} << page_order;
      this->bitmap |= size_t{1} << bitmap_offset;
      --this->free;
      alloc.emplace_back(page_order, bitmap_offset);
      ++bitmap_offset;
    }
  }
  for (const auto alloc_item : alloc) {
    page.insert(alloc_item);
  }
  this->scan_start = bitmap_offset;
  return alloc_map;
}

bool PageMemoryAllocator::Free(const PID pid, size_t pos) {
  if (UNLIKELY(!this->page_table.contains(pid))) {
    return false;
  }
  auto &page = page_table[pid];
  while (pos != 0) {
    const auto p_mask = b_floor(pos);
    pos -= p_mask;
    const auto p_num = std::log2(p_mask);

    try {
      auto b_offset = page.at(p_num);
      this->bitmap &= ~(size_t{1} << b_offset);
      ++this->free;
      page.erase(p_num);
      this->scan_start = std::min(this->scan_start, b_offset);
    }
    catch (const std::out_of_range &) {
    }
  }
  return true;
}

const Page &PageMemoryAllocator::Status(const PID pid) const {
  try {
    return page_table.at(pid);
  }
  catch (const std::out_of_range &) {
    return EMPTY_PAGE;
  }
}

std::wstring PageMemoryAllocator::ShowMap() const {
  std::wstringstream buffer;
  buffer << L"Bitmap: " << std::bitset<64>(this->bitmap)
         << L" Free: " << std::dec << this->free << std::endl;
  return buffer.str();
}

std::wstring ShowPage(const Page &page) {
  std::wstringstream buffer;
  for (const auto &entry : page) {
    buffer << entry.first << L"->" << entry.second << std::endl;
  }
  return buffer.str();
}
