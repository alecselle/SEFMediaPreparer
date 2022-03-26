#include "File.hpp"

namespace SuperEpicFuntime::MediaPreparer {

File::File() {}

File::File(std::string file) {
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

std::string File::path() {
	return _path;
}

std::string File::pathSub() {
	return _pathSub;
}

std::string File::name() {
	return _name;
}

std::string File::extension() {
	return _extension;
}

std::string File::vcodec() {
	if (_loaded) {
		return _vCodec;
	}
	return "error";
}

std::string File::acodec() {
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

std::string File::subtitlesStr() {
	if (_loaded) {
		if (_subtitles == NOT_FOUND) {
			return "Not Found";
		} else if (_subtitles == FOUND) {
            return "External";
		} else if (_subtitles == EMBEDDED) {
			return "Embedded";
        } else if (_subtitles == BOTH) {
            return "Emb.&Ext.";
        }
	}
	return "error";
}

bool File::isLoaded() {
	return _loaded;
}

bool File::loadFileInfo(rapidjson::StringStream out) {
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
        boost::filesystem::path pathSubAlt = _pathSub;
        pathSubAlt.replace_extension(".eng.srt");
        if (_subtitles == EMBEDDED && (boost::filesystem::exists(_pathSub) || boost::filesystem::exists(pathSubAlt))) {
            _subtitles = BOTH;
        } else if (_subtitles == NOT_LOADED && (boost::filesystem::exists(_pathSub) || boost::filesystem::exists(pathSubAlt))) {
			_subtitles = FOUND;
        } else if (_subtitles == NOT_LOADED) {
			_subtitles = NOT_FOUND;
		}
		_loaded = true;
	} else {
		_loaded = false;
	}
	return _loaded;
}


} // namespace SuperEpicFuntime::MediaPreparer
