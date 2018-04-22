#ifndef CONFIG_HPP
#define CONFIG_HPP
#pragma once

#include <Preset.hpp>

#include "rapidjson/filereadstream.h"
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <cstdio>
#include <iostream>
#include <product_info.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <string>

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Config
 */
class Config {
  private:
	const bool DEFAULT_PRESERVE_LOG = false;

	const boost::container::vector<boost::container::vector<std::string>> DEFAULT_VCODECS = {
		{"libx265", "hevc", "x265", "h265"}, {"libx264", "x264", "h264"}, {"libvpx", "libvpx-vp9", "vp9", "vpx"}};
	const boost::container::vector<boost::container::vector<std::string>> DEFAULT_ACODECS = {
		{"aac"}, {"ac3"}, {"libopus", "opus"}, {"libmp3lame", "mp3", "libmp3"}};
	const boost::container::vector<std::string> DEFAULT_CONTAINERS = {"mkv", "mp4", "mpeg", "avi", "mov", "webm"};

  public:
	const std::string APPDATA = getenv("APPDATA");
	const std::string USERPROFILE = getenv("USERPROFILE");
	const std::string BASE_DIR = APPDATA + "\\SuperEpicFuntime\\SEFMediaPreparer";
	const std::string PRESET_DIR = BASE_DIR + "\\presets";
	const std::string CONFIG_FILE = BASE_DIR + "\\config.cfg";
	const std::string LOG_FILE = BASE_DIR + "\\log.txt";
	const std::string PRESET_EXTENSION = ".preset";
	const std::string DEFAULT_PRESET = PRESET_DIR + "\\SEF Standard" + PRESET_EXTENSION;
	const std::string DEFAULT_TEMP_DIR = BASE_DIR + "\\temp";
	const std::string DEFAULT_LIBRARY_DIR = USERPROFILE + "\\Videos";
	const std::string DEFAULT_OUTPUT_FOLDER = "\\Converted";
	const std::string DEFAULT_OUTPUT_DIR = DEFAULT_LIBRARY_DIR + DEFAULT_OUTPUT_FOLDER;

	std::string parsePath(std::string path);
	std::string parsePresetPath(std::string path);
	std::string parsePresetName(std::string path);

	std::string baseDir, logPath, tempDir, libraryDir, outputDir;
	boost::container::vector<boost::container::vector<std::string>> vCodecList, aCodecList;
	boost::container::vector<std::string> containerList;
	Preset *preset;
	boost::container::vector<Preset *> presetList;
	bool preserveLog;

	Config();

	void load();
	void save();
	void createDefault();

	void refreshPresets();
};

static Config *config;

} // namespace SuperEpicFuntime
#endif // CONFIG_HPP
