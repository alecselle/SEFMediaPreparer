#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#pragma once

#include <Event.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/filesystem.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

namespace SuperEpicFuntime::MediaPreparer {

/**
 * @brief Stores user settings and saves/loads them to/from files
 */
class Settings {
  private:
	// User's Appdata
	const std::string APPDATA {getenv("APPDATA")};
	// User's Profile
	const std::string USERPROFILE {getenv("USERPROFILE")};
	// Install Directory
	const std::string BASE_DIR {APPDATA + "\\SuperEpicFuntime\\SEFMediaPreparer"};
	// Preset Directory
	const std::string PRESET_DIR {BASE_DIR + "\\presets"};
	// Config path
	const std::string CONFIG_FILE {BASE_DIR + "\\config.cfg"};
	// Log path
	const std::string LOG_FILE {BASE_DIR + "\\log.txt"};
	// Preset extension
	const std::string PRESET_EXTENSION {".preset"};

#define DEFAULT_SECTION {
	// Default preset path
	const std::string DEFAULT_PRESET {PRESET_DIR + "\\SEF Standard" + PRESET_EXTENSION};
	// Default processing path
	const std::string DEFAULT_TEMP_DIR {BASE_DIR + "\\temp"};
	// Default library directory
	const std::string DEFAULT_LIBRARY_DIR {USERPROFILE + "\\Videos"};
	// Default output folder
	const std::string DEFAULT_OUTPUT_FOLDER {"\\Converted"};
	// Default output directory
	const std::string DEFAULT_OUTPUT_DIR {DEFAULT_LIBRARY_DIR + DEFAULT_OUTPUT_FOLDER};
	// Default preserve log option
	const bool DEFAULT_PRESERVE_LOG	{false};
	// Default force encode option
	const bool DEFAULT_FORCE_ENCODE {false};
	// Default fix metadata option
	const bool DEFAULT_FIX_METADATA {false};
	// Default subfolders option
	const bool DEFAULT_SUBFOLDERS {false};
	// Default video codecs
	const boost::container::vector<boost::container::vector<std::string>> DEFAULT_VCODECS {
		{
			{"libx265", "hevc", "x265", "h265"},
			{"hevc_amf", "libx265", "hevc", "x265", "h265"},
			{"hevc_nvenc", "libx265", "hevc", "x265", "h265"},
			{"libx264", "x264", "h264"},
			{"h264_amf", "libx264", "x264", "h264"},
			{"h264_nvenc", "libx264", "x264", "h264"},
			{"libvpx", "libvpx-vp9", "vp9", "vpx"},
			{"copy"}
		}
	};
	// Default audio codecs
	const boost::container::vector<boost::container::vector<std::string>> DEFAULT_ACODECS {
		{
			{"aac"}, {"ac3"},
			{"libopus", "opus"},
			{"libmp3lame", "mp3", "libmp3"},
			{"copy"}
		}
	};
	// Default containers
	const boost::container::vector<std::string> DEFAULT_CONTAINERS {
		{"mkv", "mp4", "mpeg", "avi", "mov", "webm"}
	};
	// Default video codec
	const std::string DEFAULT_VCODEC {"libx265"};
	// Default audio codec
	const std::string DEFAULT_ACODEC {"aac"};
	// Default video quality
	const std::string DEFAULT_VQUALITY {"24"};
	// Default audio quality
	const std::string DEFAULT_AQUALITY {"384"};
	// Default container
	const std::string DEFAULT_CONTAINER {"mkv"};
	// Default subtitle setting
	const std::string DEFAULT_SUBTITLES {"Embed"};
	// Default threads
	const std::string DEFAULT_THREADS {"4"};
	// Default extra parameters
	const std::string DEFAULT_EXTRA_PARAMS {"-x265-params pools=3"};

#define END_DEFAULT_SECTION }
	// EventHandler
	EventHandler *_eventHandler {};

	std::string parsePath(std::string path);
	std::string parsePresetPath(std::string path);
	std::string parsePresetName(std::string path);
	std::string parsePresetNameFull(std::string name);
	// Checks for config files, creates new if none exist
	void init();

  public:
	// Containers that support embedded subtitles
	const boost::container::vector<std::string> SUBTITLE_CONTAINERS {
		{"mkv", "mp4", "mov"}
	};
	std::string baseDir, logPath, presetPath, tempDir, libraryDir, outputDir;
	boost::container::vector<boost::container::vector<std::string>> vCodecList, aCodecList;
	boost::container::vector<std::string> containerList;
	boost::container::vector<std::string> presetPathList;
	boost::container::vector<std::string> presetNameList;
	std::string presetName, vCodec, aCodec, vQuality, aQuality, subtitles, extraParams, threads, container;
	QStringList overrideParams;
	bool preserveLog, forceEncode, fixMetadata, subfolders, override;

	/**
	 * @brief Default constructor
	 */
	Settings();

	/**
	 * @brief Constructs using an existing event handler
	 * @param eventHandler - EventHandler instance
	 */
	Settings(EventHandler *eventHandler);

	/**
	 * @brief Load config from file
	 */
	void loadConfig();
	/**
	 * @brief Save config to file
	 */
	void saveConfig();
	/**
	 * @brief Create config using defaults
	 */
	void createDefaultConfig();

	/**
	 * @brief Load current profile
	 */
	void loadPreset();
	/**
	 * @brief Load profile using its name
	 * @param presetName - Name of preset
	 */
	void loadPreset(std::string presetName);
	/**
	 * @brief Load profile using its path
	 * @param presetFilePath - Path of preset file
	 */
	void loadPresetFile(std::string presetFilePath);
	/**
	 * @brief Update existing preset
	 */
	void savePreset();
	/**
	 * @brief Save new preset
	 * @param presetName - Name of preset
	 */
	void savePresetAs(std::string presetName);
	/**
	 * @brief Refresh list of presets
	 */
	void refreshPresets();
	/**
	 * @brief Create preset using defaults
	 */
	void createDefaultPreset();
	/**
	 * @brief Parse parameter string into list
	 * @param params - Parameter string
	 */
	void parseOverrideParams(std::string params);
};

static Settings *settings;

} // namespace SuperEpicFuntime
#endif // SETTINGS_HPP
