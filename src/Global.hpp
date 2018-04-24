#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#pragma once

#include <product_info.hpp>

namespace SuperEpicFuntime {

class MediaPreparer;
template <class S, class T> class EventHandler;
class Library;
class Settings;

static MediaPreparer *mediaPreparer;
static EventHandler<void, MediaPreparer> *eventHandler;
static Library *library;
static Settings *settings;

enum WorkerType { NONE = 0, SCAN = 1, ENCODE = 2 };

static const int RETRY_COUNT = 5;

} // namespace SuperEpicFuntime
#endif // GLOBAL_HPP