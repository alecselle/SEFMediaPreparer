#ifndef FILE_HPP
#define FILE_HPP
#pragma once

#include <Global.hpp>

#include <boost/filesystem.hpp>
#include <rapidjson/document.h>

namespace SuperEpicFuntime::MediaPreparer {

enum SubtitleStatus {
	NOT_LOADED = -0x0001,
	NOT_FOUND = 0x0000,
	FOUND = 0x0001,
    EMBEDDED = 0x0002,
    BOTH = 0x0003
};

/**
 * @brief Media file container that stores and parses data from ffprobe
 * @author Alec S.
 */
class File {
  private:
	// File path
	std::string _path;
	// Subtitle path
	std::string _pathSub;
	// File name
	std::string _name;
	// File extension (container)
	std::string _extension;
	// Video codec
	std::string _vCodec;
	// Audio codec
	std::string _aCodec;
	// Duration
	int _duration {-1};
	// Subtitle status
	SubtitleStatus _subtitles {NOT_LOADED};
	// Load status
	bool _loaded {false};

  public:
	/**
	 * @brief Default contructor for compatibility only
	 * @bold DO NOT USE
	 */
	File();

	/**
	 * @brief Constructor
	 * @param file - Path to file
	 */
	File(std::string file);

	/**
	 * @brief Returns path of the file
	 * @return Path
	 */
	std::string path();

	/**
	 * @brief Returns path of subtitle file (if found)
	 * @return Path of srt
	 */
	std::string pathSub();

	/**
	 * @brief Returns name of the file
	 * @return File name (no extension)
	 */
	std::string name();

	/**
	 * @brief Returns extension of the file
	 * @return File extension
	 */
	std::string extension();

	/**
	 * @brief Returns detected video codec
	 * @return Video Codec
	 */
	std::string vcodec();

	/**
	 * @brief Returns detected audio codec
	 * @return Audio Codec
	 */
	std::string acodec();

	/**
	 * @brief Returns detected duration
	 * @return Video duration
	 */
	int duration();

	/**
	 * @brief Returns detected subtitle status
	 * @return -1: Error, 0: Not found, 1: Found, 2: Embedded
	 */
	int subtitles();

	/**
	 * @brief Returns detected subtitle status string
	 * @return Subtitle status string
	 */
	std::string subtitlesStr();

	/**
	 * @brief Checks if file data has been loaded
	 * @return Whether file data has been loaded
	 */
	bool isLoaded();

	/**
	 * @brief Loads file info from ffprobe output
	 * @param out - Ffprobe output
	 * @return Whether data was loaded successfully
	 */
	bool loadFileInfo(rapidjson::StringStream out);
};

} // namespace SuperEpicFuntime::MediaPreparer
#endif // FILE_HPP
