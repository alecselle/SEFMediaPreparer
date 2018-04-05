#include "src/File.hpp"

#include <QtCore/QProcess>
#include <QtCore/QTime>
#include <QtWidgets/QWidget>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string.h>

namespace bf = boost::filesystem;
namespace ba = boost::algorithm;
using namespace nlohmann;
using namespace std;

File::File() {
}

File::File(boost::filesystem::path file) {
	if (bf::exists(bf::canonical(file))) {

		_path = bf::canonical(file);

		_pathSub = _path;
		_pathSub.replace_extension(".srt");

		_name = bf::canonical(file).filename().replace_extension();

		_extension = bf::canonical(file).extension();
	}
}

bf::path File::path() {
	return _path;
}

string File::pathStr() {
	string str = _path.string();
	ba::replace_all(str, "/", "\\");
	return str;
}

bf::path File::pathSub() {
	if (_loaded && _subtitles == 1) return _pathSub;
}

string File::pathSubStr() {
	if (_loaded && _subtitles == 1) return _pathSub.string();
}

bf::path File::name() {
	return _name;
}

string File::nameStr() {
	string str = _name.string();
	ba::replace_all(str, "/", "\\");
	return str;
}

bf::path File::extension() {
	return _extension;
}

string File::extensionStr() {
	string str = _extension.string();
	ba::replace_all(str, "/", "\\");
	return str;
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

bool File::loadFileInfo(nlohmann::json j) {
	if (j.find("streams") != j.end()) {
		foreach (auto stream, j.find("streams").value())
		{
			string codec_type = stream.find("codec_type").value();
			string codec_name = stream.find("codec_name").value();

			if (codec_type.compare("video") == 0 && codec_name.compare("png") != 0 && _vCodec.empty()) {
				_vCodec = codec_name;
			} else if (codec_type.compare("audio") == 0 && _aCodec.empty()) {
				_aCodec = codec_name;
			} else if (codec_type.compare("subtitle") == 0) {
				_subtitles = 2;
			}
		}
	}
	if (j.find("format") != j.end() && j.find("format").value().find("duration") != j.find("format").value().end()) {
		string duration = j.find("format").value().find("duration").value();
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
