#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#pragma once

#include <product_info.hpp>

namespace SuperEpicFuntime::MediaPreparer {

/**
 * @brief Worker type enumeration
 */
enum WorkerType { NONE = 0, SCAN = 1, ENCODE = 2 };

// Max number of tries to scan item information
static const int RETRY_COUNT {5};

// Override to interrupt workers
static bool cancelWorker {false};

} // namespace SuperEpicFuntime
#endif // GLOBAL_HPP
