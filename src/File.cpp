#include "src/File.hpp"

namespace bf = boost::filesystem;
namespace ba = boost::algorithm;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) File
 */
File::File() {
}

File::File(std::string file) {
	if (bf::exists(bf::canonical(file.c_str()))) {
		bf::path path = bf::canonical(file.c_str());
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
	if (_loaded && _subtitles == 1) {
		return _pathSub;
	}
}

std::string File::name() {
	return _name;
}

std::string File::extension() {
	return _extension;
}

string File::vcodec() {
	if (_loaded) {
		return _vCodec;
	}
	return "error";
}

string File::acodec() {
	if (_loaded) {
		return _aCodec;
	}
	return "error";
}

int File::duration() {
	if (_loaded) {
		return _duration;
	}
	return 0;
}

int File::subtitles() {
	if (_loaded) {
		return _subtitles;
	}
	return -1;
}

string File::subtitlesStr() {
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

bool File::loadFileInfo(StringStream out) {
	Document d;
	d.ParseStream(out);
	if (!d.HasParseError()) {
		if (d.HasMember("streams") && d["streams"].IsArray()) {
			for (auto &stream : d["streams"].GetArray()) {
				string codec_type = stream["codec_type"].GetString();
				string codec_name = stream["codec_name"].GetString();

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
			string t = d["format"]["duration"].GetString();
			_duration = stoi(t) * 1000.0;
		} else {
			_duration = 0;
		}
	}
	if (!_vCodec.empty() && !_aCodec.empty()) {
		if (_subtitles == -1 && bf::exists(_pathSub)) {
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

bool File::isLoaded() {
	return _loaded;
}

} // namespace SuperEpicFuntime
