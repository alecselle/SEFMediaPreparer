#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#pragma once

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <string>

class Settings {
  private:
	boost::filesystem::path APPDATA = getenv("APPDATA");
	boost::filesystem::path USERPROFILE = getenv("USERPROFILE");

	boost::filesystem::path BASE_DIR = APPDATA.string() + "\\SuperEpicFuntime\\SEFMediaPreparer";
	boost::filesystem::path PRESET_DIR = BASE_DIR.string() + "\\presets";

	boost::filesystem::path CONFIG_FILE = BASE_DIR.string() + "\\config.cfg";
	boost::filesystem::path LOG_FILE = BASE_DIR.string() + "\\log.txt";
	boost::filesystem::path DEFAULT_PRESET = PRESET_DIR.string() + "\\SEF Standard.preset";

	boost::filesystem::path DEFAULT_TEMP_DIR = BASE_DIR.string() + "\\temp\\";
	boost::filesystem::path DEFAULT_LIBRARY_DIR = USERPROFILE.string() + "\\Videos";
	boost::filesystem::path DEFAULT_OUTPUT_DIR = DEFAULT_LIBRARY_DIR.string() + "\\Converted";
	bool DEFAULT_PRESERVE_LOG = false;

	boost::container::vector<boost::container::vector<std::string>> DEFAULT_VCODECS = {{"libx265", "hevc", "x265",
																						"h265"},
																					   {"libx264", "x264", "h264"},
																					   {"libvpx-vp9", "vp9", "vpx"}},
																	DEFAULT_ACODECS = {{"aac"},
																					   {"ac3"},
																					   {"libopus", "opus"},
																					   {"libmp3lame", "mp3", "libmp3"}};
	boost::container::vector<std::string> DEFAULT_CONTAINERS = {"mkv", "mp4", "mpeg", "avi", "mov", "webm"};

	std::string DEFAULT_VCODEC = "libx265", DEFAULT_ACODEC = "aac", DEFAULT_VQUALITY = "24", DEFAULT_AQUALITY = "384",
				DEFAULT_CONTAINER = "mkv", DEFAULT_SUBTITLES = "Embed", DEFAULT_THREADS = "4",
				DEFAULT_EXTRA_PARAMS = "-x265-params pools=3";

  public:
	boost::filesystem::path baseDir, logPath, presetPath, tempDir, libraryDir, outputDir;
	std::string presetName, vCodec, aCodec, vQuality, aQuality, subtitles, extraParams, threads, container;
	boost::container::vector<boost::container::vector<std::string>> vCodecList, aCodecList;
	boost::container::vector<std::string> containerList;
	boost::container::vector<boost::filesystem::path> presetPathList;
	boost::container::vector<std::string> presetNameList;
	bool preserveLog;

	Settings();

	void saveConfig();
	void savePreset();
	void savePresetAs(std::string presetName);

	void loadConfig();
	void loadPreset();
	void loadPreset(std::string presetName);
	void loadPresetFile(boost::filesystem::path presetFilePath);
	void loadPresetJson(nlohmann::json presetJsonObject);

	void createDefaultConfig();
	void createDefaultPreset();
};

#endif // SETTINGS_HPP
