#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#pragma once

#include <Event.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

namespace SuperEpicFuntime::MediaPreparer {

/** ================================================================================================
 * (Class) Settings
 */
class Settings {
  private:
	const std::string APPDATA {getenv("APPDATA")};
	const std::string USERPROFILE {getenv("USERPROFILE")};

	const std::string BASE_DIR {APPDATA + "\\SuperEpicFuntime\\SEFMediaPreparer"};
	const std::string PRESET_DIR {BASE_DIR + "\\presets"};

	const std::string CONFIG_FILE {BASE_DIR + "\\config.cfg"};
	const std::string LOG_FILE {BASE_DIR + "\\log.txt"};
	const std::string PRESET_EXTENSION {".preset"};
	const std::string DEFAULT_PRESET {PRESET_DIR + "\\SEF Standard" + PRESET_EXTENSION};

	const std::string DEFAULT_TEMP_DIR {BASE_DIR + "\\temp"};
	const std::string DEFAULT_LIBRARY_DIR {USERPROFILE + "\\Videos"};
	const std::string DEFAULT_OUTPUT_FOLDER {"\\Converted"};
	const std::string DEFAULT_OUTPUT_DIR {DEFAULT_LIBRARY_DIR + DEFAULT_OUTPUT_FOLDER};
	const bool DEFAULT_PRESERVE_LOG	{false};
	const bool DEFAULT_FORCE_ENCODE {false};
	const bool DEFAULT_FIX_METADATA {false};
	const bool DEFAULT_SUBFOLDERS {false};

	const boost::container::vector<boost::container::vector<std::string>> DEFAULT_VCODECS {
		{
			{"libx265", "hevc", "x265", "h265"},
			{"hevc_amf", "libx265", "hevc", "x265", "h265"},
			{"hevc_nvenc", "libx265", "hevc", "x265", "h265"},
			{"libx264", "x264", "h264"},
			{"h264_amf", "libx264", "x264", "h264"},
			{"h264_nvenc", "libx264", "x264", "h264"},
			{"libvpx", "libvpx-vp9", "vp9", "vpx"}
		}
	};

	const boost::container::vector<boost::container::vector<std::string>> DEFAULT_ACODECS {
		{
			{"aac"}, {"ac3"},
			{"libopus", "opus"},
			{"libmp3lame", "mp3", "libmp3"}
		}
	};

	const boost::container::vector<std::string> DEFAULT_CONTAINERS {
		{"mkv", "mp4", "mpeg", "avi", "mov", "webm"}
	};

	const std::string DEFAULT_VCODEC {"libx265"};
	const std::string DEFAULT_ACODEC {"aac"};
	const std::string DEFAULT_VQUALITY {"24"};
	const std::string DEFAULT_AQUALITY {"384"};
	const std::string DEFAULT_CONTAINER {"mkv"};
	const std::string DEFAULT_SUBTITLES {"Embed"};
	const std::string DEFAULT_THREADS {"4"};
	const std::string DEFAULT_EXTRA_PARAMS {"-x265-params pools=3"};

	EventHandler *_eventHandler {};

	std::string parsePath(std::string path);
	std::string parsePresetPath(std::string path);
	std::string parsePresetName(std::string path);
	std::string parsePresetNameFull(std::string name);
	void init();

  public:
	const boost::container::vector<std::string> SUBTITLE_CONTAINERS {
		{"mkv", "mp4", "mov"}
	};
	std::string baseDir, logPath, presetPath, tempDir, libraryDir, outputDir;
	boost::container::vector<boost::container::vector<std::string>> vCodecList, aCodecList;
	boost::container::vector<std::string> containerList;
	boost::container::vector<std::string> presetPathList;
	boost::container::vector<std::string> presetNameList;
	std::string presetName, vCodec, aCodec, vQuality, aQuality, subtitles, extraParams, threads, container;
	bool preserveLog, forceEncode, fixMetadata, subfolders;

	Settings();
	Settings(EventHandler *eventHandler);

	void loadConfig();
	void saveConfig();
	void createDefaultConfig();

	void loadPreset();
	void loadPreset(std::string presetName);
	void loadPresetFile(std::string presetFilePath);
	void savePreset();
	void savePresetAs(std::string presetName);
	void refreshPresets();
	void createDefaultPreset();
};

static Settings *settings;

} // namespace SuperEpicFuntime
#endif // SETTINGS_HPP
