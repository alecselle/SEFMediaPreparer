#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#pragma once

#include <EventHandler.hpp>
#include <File.hpp>
#include <Library.hpp>
#include <Settings.hpp>
#include <product_info.hpp>

namespace SuperEpicFuntime {

static const int RETRY_COUNT = 5;

enum WorkerType { NONE = 0, SCAN = 1, ENCODE = 2 };

} // namespace SuperEpicFuntime
#endif // GLOBAL_HPP