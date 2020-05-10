//
// Created by TYTY on 2020-04-27 027.
//

#include <sstream>
#include "misc.h"
#include "proc_schedule.h"

PCB *CreatePCB(ssize_t priority, size_t time) {
  return new PCB{
      .pid = ++PID_COUNTER,
      .priority = priority,
      .time = time
  };
}

void DisposePCB(PCB *head) {
  while (head != nullptr) {
    const auto tmp = head;
    head = head->next;
    delete tmp;
  }
}

PriorityScheduler::PriorityScheduler(std::function<void(std::wstring)> logger) {
  this->logger = std::move(logger);
}

PCB *PriorityScheduler::ready() const {
  if (UNLIKELY(this->running == nullptr)) {
    return nullptr;
  }
  return this->running->next;
}

void PriorityScheduler::status() const {
  std::wstringstream buffer;
  buffer << L"Running: ";
  if (this->running != nullptr) {
    buffer << this->running->pid
           << L":<P:" << this->running->priority
           << L",T:" << this->running->time
           << L">";
  }
  buffer << std::endl;
  buffer << "Ready: ";
  auto _ready = this->ready();
  while (_ready != nullptr) {
    buffer << _ready->pid
           << L":<P:" << _ready->priority
           << L",T:" << _ready->time
           << L"> ";
    _ready = _ready->next;
  }
  buffer << std::endl << L"End: ";
  auto _end = this->end;
  while (_end != nullptr) {
    buffer << _end->pid << L" ";
    _end = _end->next;
  }
  buffer << std::endl;
  this->logger(buffer.str());
}

void PriorityScheduler::schedule(PCB *process) {
  if (UNLIKELY(this->running == nullptr)) {
    process->next = nullptr;
    this->running = process;
    return;
  }
  auto before = this->running;
  if (process->priority > before->priority) {
    process->next = this->running;
    this->running = process;
    return;
  }
  while (UNLIKELY(before->next != nullptr) && before->next->priority >= process->priority) {
    before = before->next;
  }
  process->next = before->next;
  before->next = process;
}

void PriorityScheduler::AddProcess(PCB *process) {
  if (UNLIKELY(process->status == END)) {
    process->next = this->end;
    this->end = process;
    return;
  }
  this->schedule(process);
}

void PriorityScheduler::Ticker() {
  if (UNLIKELY(this->running == nullptr)) {
    status();
    return;
  }
  if (UNLIKELY(--this->running->time == 0)) {
    this->running->status = END;
    const auto _tmp = this->running->next;
    this->running->next = this->end;
    this->end = this->running;
    this->running = _tmp;
    status();
    return;
  }
  --this->running->priority;
  const auto process = this->running;
  this->running = this->running->next;
  this->schedule(process);
  status();
}

PriorityScheduler::~PriorityScheduler() {
  DisposePCB(this->running);
  DisposePCB(this->end);
}

