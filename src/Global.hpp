#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#pragma once

#include <product_info.hpp>

namespace SuperEpicFuntime::MediaPreparer {

enum WorkerType { NONE = 0, SCAN = 1, ENCODE = 2 };

static const int RETRY_COUNT = 5;

static bool cancelWorker = false;

} // namespace SuperEpicFuntime
#endif // GLOBAL_HPP
