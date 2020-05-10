//
// Created by TYTY on 2020-04-27 027.
//

#include <iostream>
#include <thread>
#include <chrono>

#include "misc.h"
#include "proc_schedule.h"

using namespace std::chrono_literals;

class MockSystem {
  PriorityScheduler *scheduler;
 public:
  void SetScheduler(PriorityScheduler *sche) {
    this->scheduler = sche;
  }

  void CreateProcess() const {
    size_t time = 0;
    ssize_t priority = 0;
    while (true) {
      std::cin >> priority >> time;
      if (std::cin.fail()) {
        break;
      }
      this->scheduler->AddProcess(CreatePCB(priority, time));
    }
    std::cin.clear();
  }

  [[noreturn]] void run() const {
    this->scheduler->status();
    while (true) {
      this->scheduler->Ticker();
      std::this_thread::sleep_for(1000ms);
    }
  }
};

int main() {
  const auto scheduler = new PriorityScheduler([](const std::wstring &info) {
    std::wcout << info;
    std::wcout.flush();
  });
  MockSystem system{};
  system.SetScheduler(scheduler);
  system.CreateProcess();
  system.run();
}