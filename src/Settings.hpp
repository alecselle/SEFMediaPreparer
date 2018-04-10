#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#pragma once

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace SuperEpicFuntime {
	class Settings {
	private:
		const boost::filesystem::path APPDATA = getenv("APPDATA");
		const boost::filesystem::path USERPROFILE = getenv("USERPROFILE");

		const boost::filesystem::path BASE_DIR = APPDATA.string() + "\\SuperEpicFuntime\\SEFMediaPreparer";
		const boost::filesystem::path PRESET_DIR = BASE_DIR.string() + "\\presets";

		const boost::filesystem::path CONFIG_FILE = BASE_DIR.string() + "\\config.cfg";
		const boost::filesystem::path LOG_FILE = BASE_DIR.string() + "\\log.txt";
		const boost::filesystem::path DEFAULT_PRESET = PRESET_DIR.string() + "\\SEF Standard.preset";

		const boost::filesystem::path DEFAULT_TEMP_DIR = BASE_DIR.string() + "\\temp\\";
		const boost::filesystem::path DEFAULT_LIBRARY_DIR = USERPROFILE.string() + "\\Videos";
		const boost::filesystem::path DEFAULT_OUTPUT_DIR = DEFAULT_LIBRARY_DIR.string() + "\\Converted";
		const bool DEFAULT_PRESERVE_LOG = false;

		const boost::container::vector<boost::container::vector<std::string>> DEFAULT_VCODECS = { { "libx265", "hevc", "x265", "h265" }, { "libx264", "x264", "h264" }, { "libvpx-vp9", "vp9", "vpx" } };
		const boost::container::vector<boost::container::vector<std::string>> DEFAULT_ACODECS = { { "aac" }, { "ac3" }, { "libopus", "opus" }, { "libmp3lame", "mp3", "libmp3" } };
		const boost::container::vector<std::string> DEFAULT_CONTAINERS = { "mkv", "mp4", "mpeg", "avi", "mov", "webm" };

		const std::string DEFAULT_VCODEC = "libx265";
		const std::string DEFAULT_ACODEC = "aac";
		const std::string DEFAULT_VQUALITY = "24";
		const std::string DEFAULT_AQUALITY = "384";
		const std::string DEFAULT_CONTAINER = "mkv";
		const std::string DEFAULT_SUBTITLES = "Embed";
		const std::string DEFAULT_THREADS = "4";
		const std::string DEFAULT_EXTRA_PARAMS = "-x265-params pools=3";

	public:
		boost::filesystem::path baseDir, logPath, presetPath, tempDir, libraryDir, outputDir;
		boost::container::vector<boost::container::vector<std::string>> vCodecList, aCodecList;
		boost::container::vector<std::string> containerList;
		boost::container::vector<boost::filesystem::path> presetPathList;
		boost::container::vector<std::string> presetNameList;
		std::string presetName, vCodec, aCodec, vQuality, aQuality, subtitles, extraParams, threads, container;
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
} // namespace SuperEpicFuntime
#endif // SETTINGS_HPP
