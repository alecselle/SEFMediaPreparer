#ifndef PRESET_HPP
#define PRESET_HPP
#pragma once

#include <Config.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <cstdio>
#include <iostream>
#include <product_info.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <string>

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Settings
 */
class Preset {
  private:
	const std::string DEFAULT_VCODEC = "libx265";
	const std::string DEFAULT_ACODEC = "aac";
	const std::string DEFAULT_VQUALITY = "24";
	const std::string DEFAULT_AQUALITY = "384";
	const std::string DEFAULT_CONTAINER = "mkv";
	const std::string DEFAULT_SUBTITLES = "Embed";
	const std::string DEFAULT_THREADS = "4";
	const std::string DEFAULT_EXTRA_PARAMS = "-x265-params pools=3";

  public:
	std::string presetName, presetPath, vCodec, aCodec, vQuality, aQuality, subtitles, extraParams, threads, container;

	Preset();
	Preset(std::string);

	void load();
	void load(std::string presetName);
	void loadFile(std::string presetFilePath);
	void save();
	void save(std::string presetName);
	void createDefault();
};

static Preset *preset;

} // namespace SuperEpicFuntime
#endif // PRESET_HPP
