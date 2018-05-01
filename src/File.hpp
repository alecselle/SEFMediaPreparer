#ifndef FILE_HPP
#define FILE_HPP
#pragma once

#include <Global.hpp>

#include <boost/filesystem.hpp>
#include <rapidjson/document.h>

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) File
 */
class File {
  private:
	std::string _path;
	std::string _pathSub;
	std::string _name;
	std::string _extension;

	std::string _vCodec;
	std::string _aCodec;
	int _duration = -1;

	int _subtitles = -1;
	bool _loaded   = false;

  public:
	File() {
	}

	File(std::string file) {
		if (boost::filesystem::exists(boost::filesystem::canonical(file.c_str()))) {
			boost::filesystem::path path	= boost::filesystem::canonical(file.c_str());
			boost::filesystem::path pathSub = file;
			pathSub.replace_extension(".srt");
			_path	  = path.string();
			_pathSub   = pathSub.string();
			_name	  = path.filename().replace_extension().string();
			_extension = path.extension().string();
		}
	}

	std::string path() {
		return _path;
	}
	std::string pathSub() {
		return _pathSub;
	}
	std::string name() {
		return _name;
	}
	std::string extension() {
		return _extension;
	}
	std::string vcodec() {
		if (_loaded) {
			return _vCodec;
		}
		return "error";
	}
	std::string acodec() {
		if (_loaded) {
			return _aCodec;
		}
		return "error";
	}
	int duration() {
		if (_loaded) {
			return _duration;
		}
		return 0;
	}
	int subtitles() {
		if (_loaded) {
			return _subtitles;
		}
		return -1;
	}
	std::string subtitlesStr() {
		if (_loaded) {
			if (_subtitles == 0) {
				return "Not Found";
			} else if (_subtitles == 1) {
				return "Found";
			} else if (_subtitles == 2) {
				return "Embedded";
			}
		}
		return "error";
	}
	bool isLoaded() {
		return _loaded;
	}

	bool loadFileInfo(rapidjson::StringStream out) {
		rapidjson::Document d;
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
						_subtitles = 2;
					}
				}
			}
			if (d.HasMember("format") && d["format"].HasMember("duration")) {
				std::string t = d["format"]["duration"].GetString();
				_duration	 = stoi(t) * 1000.0;
			} else {
				_duration = 0;
			}
		}
		if (!_vCodec.empty() && !_aCodec.empty()) {
			if (_subtitles == -1 && boost::filesystem::exists(_pathSub)) {
				_subtitles = 1;
			} else if (_subtitles == -1) {
				_subtitles = 0;
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
