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
	EMBEDDED = 0x0002
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
	File() {
	}

	/**
	 * @brief Constructor
	 * @param file - Path to file
	 */
	File(std::string file) {
		if (boost::filesystem::exists(boost::filesystem::canonical(file.c_str()))) {
			boost::filesystem::path path {boost::filesystem::canonical(file.c_str())};
			boost::filesystem::path pathSub {file};
			pathSub.replace_extension(".srt");
			_path	  = path.string();
			_pathSub   = pathSub.string();
			_name	  = path.filename().replace_extension().string();
			_extension = path.extension().string();
		}
	}

	/**
	 * @brief Returns path of the file
	 * @return Path
	 */
	std::string path() {
		return _path;
	}

	/**
	 * @brief Returns path of subtitle file (if found)
	 * @return Path of srt
	 */
	std::string pathSub() {
		return _pathSub;
	}

	/**
	 * @brief Returns name of the file
	 * @return File name (no extension)
	 */
	std::string name() {
		return _name;
	}

	/**
	 * @brief Returns extension of the file
	 * @return File extension
	 */
	std::string extension() {
		return _extension;
	}

	/**
	 * @brief Returns detected video codec
	 * @return Video Codec
	 */
	std::string vcodec() {
		if (_loaded) {
			return _vCodec;
		}
		return "error";
	}

	/**
	 * @brief Returns detected audio codec
	 * @return Audio Codec
	 */
	std::string acodec() {
		if (_loaded) {
			return _aCodec;
		}
		return "error";
	}

	/**
	 * @brief Returns detected duration
	 * @return Video duration
	 */
	int duration() {
		if (_loaded) {
			return _duration;
		}
		return 0;
	}

	/**
	 * @brief Returns detected subtitle status
	 * @return -1: Error, 0: Not found, 1: Found, 2: Embedded
	 */
	int subtitles() {
		if (_loaded) {
			return _subtitles;
		}
		return -1;
	}

	/**
	 * @brief Returns detected subtitle status string
	 * @return Subtitle status string
	 */
	std::string subtitlesStr() {
		if (_loaded) {
			if (_subtitles == NOT_FOUND) {
				return "Not Found";
			} else if (_subtitles == FOUND) {
				return "Found";
			} else if (_subtitles == EMBEDDED) {
				return "Embedded";
			}
		}
		return "error";
	}

	/**
	 * @brief Checks if file data has been loaded
	 * @return Whether file data has been loaded
	 */
	bool isLoaded() {
		return _loaded;
	}

	/**
	 * @brief Loads file info from ffprobe output
	 * @param out - Ffprobe output
	 * @return Whether data was loaded successfully
	 */
	bool loadFileInfo(rapidjson::StringStream out) {
		rapidjson::Document d {};
		d.ParseStream(out);
		if (!d.HasParseError()) {
			if (d.HasMember("streams") && d["streams"].IsArray()) {
				for (auto &stream : d["streams"].GetArray()) {
					std::string codec_type = stream["codec_type"].GetString();
					std::string codec_name = stream["codec_name"].GetString();

					if (codec_type.compare("video") == 0 && codec_name.compare("png") != 0 && _vCodec.empty()) {
						_vCodec = codec_name;
					} else if (codec_type.compare("audio") == 0 && _aCodec.empty()) {
						_aCodec = codec_name;
					} else if (codec_type.compare("subtitle") == 0) {
						_subtitles = EMBEDDED;
					}
				}
			}
			if (d.HasMember("format") && d["format"].HasMember("duration")) {
				std::string t {d["format"]["duration"].GetString()};
				_duration	 = stoi(t) * 1000.0;
			} else {
				_duration = 0;
			}
		}
		if (!_vCodec.empty() && !_aCodec.empty()) {
			if (_subtitles == -1 && boost::filesystem::exists(_pathSub)) {
				_subtitles = FOUND;
			} else if (_subtitles == -1) {
				_subtitles = NOT_FOUND;
			}
			_loaded = true;
		} else {
			_loaded = false;
		}
		return _loaded;
	}
};

} // namespace SuperEpicFuntime
#endif // FILE_HPP
