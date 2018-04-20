#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#pragma once

#include <EventHandler.hpp>
#include <Library.hpp>
#include <Settings.hpp>

namespace SuperEpicFuntime {
enum EventType { ERROR = 0, PROGRESS_UPDATED = 1, WORKER_STARTED = 2, WORKER_FINISHED = 3 };
enum WorkerType { SCAN = 1, ENCODE = 2, CLOSE = 0 };
enum ProgressBar { PRIMARY = 1, SECONDARY = 2 };

// inline static void newEvent(EventType type, std::string message, int data = NULL) {
//	eventHandler->addEvent(type, message, data);
//}

} // namespace SuperEpicFuntime

#endif // GLOBAL_HPP