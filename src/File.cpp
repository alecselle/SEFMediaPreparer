#include "src/File.hpp"

#include <QtWidgets/QWidget>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace bf = boost::filesystem;
namespace ba = boost::algorithm;
using namespace nlohmann;
using namespace std;

namespace SuperEpicFuntime {
	File::File() {
	}

	File::File(std::string file) {
		if (bf::exists(bf::canonical(file))) {
			bf::path path = bf::canonical(file);
			bf::path pathSub = file;
			pathSub.replace_extension(".srt");

			_path = path.string();

			_pathSub = pathSub.string();

			_name = path.filename().replace_extension().string();

			_extension = path.extension().string();
		}
	}

	std::string File::path() {
		return _path;
	}

	std::string File::pathSub() {
		if (_loaded && _subtitles == 1) return _pathSub;
	}

	std::string File::name() {
		return _name;
	}

	std::string File::extension() {
		return _extension;
	}

	string File::vcodec() {
		if (_loaded) return _vCodec;
		return "error";
	}

	string File::acodec() {
		if (_loaded) return _aCodec;
		return "error";
	}

	int File::duration() {
		if (_loaded) return _duration;
	}

	int File::subtitles() {
		if (_loaded) return _subtitles;
	}

	string File::subtitlesStr() {
		if (_loaded) {
			switch (_subtitles) {
			case 0:
				return "Not Found";
				break;
			case 1:
				return "Found";
				break;
			case 2:
				return "Embedded";
				break;
			default:
				return "error";
			}
		}
		return "error";
	}

	bool File::loadFileInfo(nlohmann::json j) { // @suppress("Member declaration not found")
		if (j.find("streams") != j.end()) { // @suppress("Method cannot be resolved")
			foreach (auto stream, j.find("streams").value()) // @suppress("Invalid arguments") // @suppress("Field cannot be resolved") // @suppress("Method cannot be resolved")
			{
				string codec_type = stream.find("codec_type").value(); // @suppress("Method cannot be resolved")
				string codec_name = stream.find("codec_name").value(); // @suppress("Method cannot be resolved")

				if (codec_type.compare("video") == 0 && codec_name.compare("png") != 0 && _vCodec.empty()) {
					_vCodec = codec_name;
				} else if (codec_type.compare("audio") == 0 && _aCodec.empty()) {
					_aCodec = codec_name;
				} else if (codec_type.compare("subtitle") == 0) {
					_subtitles = 2;
				}
			}
		}
		if (j.find("format") != j.end() && j.find("format").value().find("duration") != j.find("format").value().end()) { // @suppress("Method cannot be resolved")
			string duration = j.find("format").value().find("duration").value(); // @suppress("Method cannot be resolved")
			_duration = atof(duration.c_str()) * 1000.0;
		} else {
			_duration = 0;
		}

		if (!_vCodec.empty() && !_aCodec.empty() && _duration != NULL) {
			if (_subtitles == NULL && bf::exists(_pathSub)) {
				_subtitles = 1;
			} else {
				_subtitles = 0;
			}
			_loaded = true;
			return true;
		} else {
			_loaded = false;
			return false;
		}
	}

	bool File::isLoaded() {
		return _loaded;
	}
} // namespace SuperEpicFuntime
