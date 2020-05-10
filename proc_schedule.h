//
// Created by TYTY on 2020-04-27 027.
//

#ifndef OSEXP__PROC_SCHEDULE_H_
#define OSEXP__PROC_SCHEDULE_H_

#include <cstddef>
#include <functional>
#include <string>

enum ProcessStatus { READY, END };

static unsigned short PID_COUNTER = 0;

struct PCB {
  unsigned short pid = 0;
  PCB *next = nullptr;
  ProcessStatus status = READY;
  ssize_t priority = 0;
  size_t time = 0;
};

PCB *CreatePCB(ssize_t priority, size_t time);

void DisposePCB(PCB *head);

class PriorityScheduler {
  PCB *running = nullptr;
  PCB *end = nullptr;
  std::function<void(std::wstring)> logger;
  void schedule(PCB *process);
 public:
  void status() const;
  explicit PriorityScheduler(std::function<void(std::wstring)> logger);
  [[nodiscard]] PCB *ready() const;
  void AddProcess(PCB *process);
  void Ticker();
  ~PriorityScheduler();
};

#endif //OSEXP__PROC_SCHEDULE_H_
