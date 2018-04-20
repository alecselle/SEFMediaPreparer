#include "src/Library.hpp"

namespace bf = boost::filesystem;
namespace bc = boost::container;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Library
 */
Library::Library(Settings *settings) {
	_settings = settings;
}

void Library::scan(bool scanRecursive) {
	_Library.clear();
	if (!scanRecursive) {
		for (bf::directory_entry &x : bf::directory_iterator(_settings->libraryDir)) {
			for (int i = 0; i < _extensions.size(); i++) {
				if (x.path().extension().string() == _extensions[i]) {
					File file = File(x.path().string());
					_Library.push_back(file);
				}
			}
		}
	} else {
		for (bf::directory_entry &x : bf::recursive_directory_iterator(_settings->libraryDir)) {
			if (x.path().parent_path().filename().string().compare("Converted") != 0) {
				for (int i = 0; i < _extensions.size(); i++) {
					if (x.path().extension().string().compare(_extensions[i]) == 0) {
						File file = File(x.path().string());
						_Library.push_back(file);
					}
				}
			}
		}
	}
}

int Library::size() {
	return _Library.size();
}

int Library::duration() {
	if (_duration == -1) {
		_duration = 0;
		for (int i = 0; i < size(); i++) {
			if (getFile(i).duration() > 0) {
				_duration += getFile(i).duration();
			}
		}
	}
	return _duration;
}

void Library::clear() {
	_Library.clear();
	clearEncode();
}

boost::container::vector<SuperEpicFuntime::File> &Library::getFiles() {
	return _Library;
}

SuperEpicFuntime::File &Library::getFile(int pos) {
	if (pos < _Library.size()) {
		return _Library[pos];
	}
}

int Library::findFile(File file) {
	for (int i = 0; i < size(); i++) {
		if (getFile(i).path() == file.path())
			return i;
	}
	return -1;
}

int Library::findFile(std::string file) {
	return findFile(File(file));
}

bool Library::addFile(File file) {
	if (bf::exists(file.path().c_str())) {
		_Library.push_back(file);
		return true;
	}
	return false;
}

bool Library::addFile(std::string file) {
	return addFile(File(file));
}

bool Library::checkEncode(SuperEpicFuntime::File file) {
	if (file.isLoaded()) {
		bool matches[3] = {false, false, false};
		for (int j = 0; j < _settings->vCodecList.size(); j++) {
			if (_settings->vCodecList[j][0].compare(_settings->vCodec) == 0) {
				for (int k = 0; k < _settings->vCodecList[j].size(); k++) {
					if (file.vcodec().compare(_settings->vCodecList[j][k]) == 0) {
						matches[0] = true;
					}
				}
			}
		}
		for (int l = 0; l < _settings->aCodecList.size(); l++) {
			if (_settings->aCodecList[l][0].compare(_settings->aCodec) == 0) {
				for (int m = 0; m < _settings->aCodecList[l].size(); m++) {
					if (file.acodec().compare(_settings->aCodecList[l][m]) == 0) {
						matches[1] = true;
					}
				}
			}
		}

		if (file.extension().compare("." + _settings->container) == 0) {
			matches[2] = true;
		}

		if ((!matches[0] || !matches[1] || !matches[2]) && findFileEncode(file) == -1) {
			return true;
		}
	}
	return false;
}

void Library::scanEncode() {
	_LibraryEncode.clear();
	for (int i = 0; i < size(); i++) {
		File &f = getFile(i);
		if (f.isLoaded()) {
			bool matches[3] = {false, false, false};
			for (int j = 0; j < _settings->vCodecList.size(); j++) {
				if (_settings->vCodecList[j][0].compare(_settings->vCodec) == 0) {
					for (int k = 0; k < _settings->vCodecList[j].size(); k++) {
						if (f.vcodec().compare(_settings->vCodecList[j][k]) == 0) {
							matches[0] = true;
						}
					}
				}
			}
			for (int l = 0; l < _settings->aCodecList.size(); l++) {
				if (_settings->aCodecList[l][0].compare(_settings->aCodec) == 0) {
					for (int m = 0; m < _settings->aCodecList[l].size(); m++) {
						if (f.acodec().compare(_settings->aCodecList[l][m]) == 0) {
							matches[1] = true;
						}
					}
				}
			}

			if (f.extension().compare("." + _settings->container) == 0) {
				matches[2] = true;
			}
			if ((!matches[0] || !matches[1] || !matches[2]) && findFileEncode(f) == -1) {
				_LibraryEncode.push_back(f);
			}
		}
	}
}

int Library::sizeEncode() {
	return _LibraryEncode.size();
}

int Library::durationEncode() {
	if (_duration == -1) {
		_duration = 0;
		for (int i = 0; i < sizeEncode(); i++) {
			if (getFileEncode(i).duration() > 0) {
				_duration += getFileEncode(i).duration();
			}
		}
	}
	return _duration;
}

void Library::clearEncode() {
	_LibraryEncode.clear();
}

boost::container::vector<SuperEpicFuntime::File> &Library::getFilesEncode() {
	return _LibraryEncode;
}

SuperEpicFuntime::File &Library::getFileEncode(int pos) {
	if (pos < _LibraryEncode.size()) {
		return _LibraryEncode[pos];
	}
}

int Library::findFileEncode(File file) {
	for (int i = 0; i < sizeEncode(); i++) {
		if (getFileEncode(i).path().compare(file.path()) == 0)
			return i;
	}
	return -1;
}

int Library::findFileEncode(std::string file) {
	return findFileEncode(File(file));
}

bool Library::forceEncode(File file) {
	int i;
	if ((i = findFile(file)) != NULL && findFileEncode(file) == NULL) {
		_LibraryEncode.push_back(file);
		return true;
	}
	return false;
}

bool Library::forceEncode(std::string file) {
	return forceEncode(File(file));
}

bool Library::skipEncode(File file) {
	int i;
	if ((i = findFileEncode(file)) != NULL) {
		_LibraryEncode.erase(_LibraryEncode.begin() + i);
		return true;
	}
	return false;
}

bool Library::skipEncode(std::string file) {
	return skipEncode(File(file));
}

} // namespace SuperEpicFuntime
