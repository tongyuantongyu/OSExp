//
// Created by TYTY on 2020-05-04 004.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>

#include "misc.h"

typedef std::vector<size_t> Resource;
typedef uint16_t PID;

bool lesser(const Resource &first, const Resource &second) {
  if (UNLIKELY(first.size() != second.size())) {
    return false;
  }
  return std::mismatch(first.begin(), first.end(), second.begin(), second.end(), std::less_equal<size_t>{}).first == first.end();
}

Resource &add(Resource &base, const Resource &adder) {
  std::transform(base.begin(), base.end(), adder.begin(), base.begin(), std::plus<size_t>{});
  return base;
}

Resource &sub(Resource &base, const Resource &minus) {
  std::transform(base.begin(), base.end(), minus.begin(), base.begin(), std::minus<size_t>{});
  return base;
}

class Banker {
  Resource resource{9, 3, 6};
  std::unordered_map<PID, Resource> claim;
  std::unordered_map<PID, Resource> holding;
 public:
  const size_t length = resource.size();
  void init();
  bool acquire(PID, const Resource &);
  void status();
};

void Banker::init() {
  std::wstringstream buf;
  PID pid;
  while (true) {
  BREAK_TO_WHILE:
    Resource claimed;
    claimed.reserve(this->length);
    std::wcout << L"Init: ";
    std::wcout.flush();
    buf.clear();
    buf.str(std::wstring());
    const auto input_ptr = new wchar_t[256];
    std::wcin.getline(input_ptr, 256);
    buf << input_ptr;
    buf >> pid;
    if (pid == 0) {
      break;
    }
    for (const auto &i : this->resource) {
      size_t c;
      buf >> c;
      if (c > i) {
        std::wcout << "Claimed recourse over available." << std::endl;
        goto BREAK_TO_WHILE;
      }
      claimed.push_back(c);
    }
    claim.insert({pid, std::move(claimed)});
    holding.insert({pid, Resource(this->length)});
  }
}

bool Banker::acquire(const PID pid, const Resource &request) {
  if (UNLIKELY(!this->claim.contains(pid))) {
    return false;
  }
  auto holding_copy = this->holding;
  auto &pid_holding_copy = holding_copy.at(pid);
  auto resource_copy = this->resource;
  auto x = request.begin();
  auto y = pid_holding_copy.begin(), z = resource_copy.begin(), w = this->claim.at(pid).begin();
  for (; x != request.end(); ++x, ++y, ++z, ++w) {
    if (*x > *z || UNLIKELY(*x + *y > *w)) {
      return false;
    }
    *z -= *x;
    *y += *x;
  }
  while (!holding_copy.empty()) {
    size_t selected = 0;
    for (const auto &p : holding_copy) {
      auto require = claim.at(p.first);
      sub(require, p.second);
      if (lesser(require, resource_copy)) {
        selected = p.first;
        break;
      }
    }
    if (selected == 0) {
      return false;
    }
    add(resource_copy, holding_copy.at(selected));
    holding_copy.erase(selected);
  }
  sub(this->resource, request);
  add(this->holding.at(pid), request);
  return true;
}

void Banker::status() {
  std::wcout << "Available: ";
  auto i = this->resource.begin();
  const auto last = --this->resource.end();
  while (true) {
    if (UNLIKELY(i == last)) {
      std::wcout << *i << std::endl;
      return;
    }
    std::wcout << *i << L", ";
    ++i;
  }
}

int main() {
  std::wcout << std::boolalpha;
  auto banker = new Banker();
  banker->init();
  std::wstringstream buf;
  PID pid;
  while (true) {
    Resource request(banker->length);
    std::wcout << L"Acquire: ";
    std::wcout.flush();
    buf.clear();
    buf.str(std::wstring());
    const auto input_ptr = new wchar_t[256];
    std::wcin.getline(input_ptr, 256);
    buf << input_ptr;
    buf >> pid;
    if (pid == 0) {
      break;
    }
    for (auto &i : request) {
      size_t _tmp;
      buf >> _tmp;
      i = _tmp;
    }
    std::wcout << L"Result: " << banker->acquire(pid, request) << std::endl;
    banker->status();
  }
  delete banker;
}