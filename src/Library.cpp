#include "Library.hpp"

namespace SuperEpicFuntime::MediaPreparer {

Library::Library(Settings *settings) {
	_settings = settings;
}

Library::Library(Settings *settings, EventHandler *eventHandler) {
	_settings	 = settings;
	_eventHandler = eventHandler;
}

void Library::scan(bool scanRecursive) {
	_Library.clear();
	if (boost::filesystem::exists(_settings->libraryDir)) {
		if (scanRecursive) {
			for (boost::filesystem::directory_entry &x : boost::filesystem::recursive_directory_iterator(_settings->libraryDir)) {
				if (x.path().parent_path().filename().string().compare("Converted") != 0) {
					for (unsigned int i {0}; i < _extensions.size(); i++) {
						if (x.path().extension().string().compare(_extensions[i]) == 0) {
							File file = File(x.path().string());
							_Library.push_back(file);
						}
					}
				}
			}
		} else {
			for (boost::filesystem::directory_entry &x : boost::filesystem::directory_iterator(_settings->libraryDir)) {
				for (unsigned int i {0}; i < _extensions.size(); i++) {
					if (x.path().extension().string() == _extensions[i]) {
						File file = File(x.path().string());
						_Library.push_back(file);
					}
				}
			}
		}
	}
}

unsigned int Library::size() {
	return _Library.size();
}

int Library::duration() {
	if (_duration == -1) {
		_duration = 0;
		for (unsigned int i {0}; i < size(); i++) {
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

boost::container::vector<File> &Library::getFiles() {
	return _Library;
}

File &Library::getFile(unsigned int filePosition) {
	if (filePosition < _Library.size()) {
		return _Library[filePosition];
	}
}

int Library::findFile(File fileObject) {
	for (unsigned int i {0}; i < size(); i++) {
		if (getFile(i).path() == fileObject.path())
			return i;
	}
	return -1;
}

int Library::findFile(std::string filePath) {
	return findFile(File(filePath));
}

bool Library::addFile(File fileObject) {
	if (boost::filesystem::exists(fileObject.path().c_str())) {
		_Library.push_back(fileObject);
		return true;
	}
	return false;
}

bool Library::addFile(std::string filePath) {
	return addFile(File(filePath));
}

bool Library::checkEncode(File fileObject) {
	if (fileObject.isLoaded()) {
		bool matches[3] = {false, false, false};
		for (unsigned int j = 0; j < _settings->vCodecList.size(); j++) {
			if (_settings->vCodecList[j][0].compare(_settings->vCodec) == 0) {
				for (unsigned int k = 0; k < _settings->vCodecList[j].size(); k++) {
					if (fileObject.vcodec().compare(_settings->vCodecList[j][k]) == 0) {
						matches[0] = true;
					}
				}
			}
		}
		for (unsigned int l = 0; l < _settings->aCodecList.size(); l++) {
			if (_settings->aCodecList[l][0].compare(_settings->aCodec) == 0) {
				for (unsigned int m = 0; m < _settings->aCodecList[l].size(); m++) {
					if (fileObject.acodec().compare(_settings->aCodecList[l][m]) == 0) {
						matches[1] = true;
					}
				}
			}
		}

		if (fileObject.extension().compare("." + _settings->container) == 0) {
			matches[2] = true;
		}

		if ((!matches[0] || !matches[1] || !matches[2]) && findFileEncode(fileObject) == -1) {
			return true;
		}
	}
	return false;
}

void Library::scanEncode() {
	_LibraryEncode.clear();
	for (unsigned int i {0}; i < size(); i++) {
		File &f {getFile(i)};
		if (f.isLoaded()) {
			bool matches[3] = {false, false, false};
			for (unsigned int j = 0; j < _settings->vCodecList.size(); j++) {
				if (_settings->vCodecList[j][0].compare(_settings->vCodec) == 0) {
					for (unsigned int k = 0; k < _settings->vCodecList[j].size(); k++) {
						if (f.vcodec().compare(_settings->vCodecList[j][k]) == 0) {
							matches[0] = true;
						}
					}
				}
			}
			for (unsigned int l = 0; l < _settings->aCodecList.size(); l++) {
				if (_settings->aCodecList[l][0].compare(_settings->aCodec) == 0) {
					for (unsigned int m = 0; m < _settings->aCodecList[l].size(); m++) {
						if (f.acodec().compare(_settings->aCodecList[l][m]) == 0) {
							matches[1] = true;
						}
					}
				}
			}

			if (f.extension().compare("." + _settings->container) == 0) {
				matches[2] = true;
			}
			if (((!matches[0] || !matches[1] || !matches[2]) && findFileEncode(f) == -1) || _settings->forceEncode == true || _settings->override == true) {
				_LibraryEncode.push_back(f);
			}
		}
	}
}

int Library::sizeEncode() {
	return _LibraryEncode.size();
}

int Library::durationEncode() {
	if (_durationEncode == -1) {
		_durationEncode = 0;
		for (int i {0}; i < sizeEncode(); i++) {
			if (getFileEncode(i).duration() > 0) {
				_durationEncode += getFileEncode(i).duration();
			}
		}
	}
	return _durationEncode;
}

void Library::clearEncode() {
	_LibraryEncode.clear();
	_durationEncode = -1;
}

boost::container::vector<File> &Library::getFilesEncode() {
	return _LibraryEncode;
}

File &Library::getFileEncode(unsigned int filePosition) {
	if (filePosition < _LibraryEncode.size()) {
		return _LibraryEncode[filePosition];
	}
}

int Library::findFileEncode(File fileObject) {
	for (int i {0}; i < sizeEncode(); i++) {
		if (getFileEncode(i).path().compare(fileObject.path()) == 0)
			return i;
	}
	return -1;
}

int Library::findFileEncode(std::string filePath) {
	return findFileEncode(File(filePath));
}

bool Library::forceEncode(File fileObject) {
	int i = findFile(fileObject);
	if (i >= 0 && findFileEncode(fileObject) < 0) {
		_LibraryEncode.push_back(fileObject);
		return true;
	}
	return false;
}

bool Library::forceEncode(std::string filePath) {
	return forceEncode(File(filePath));
}

bool Library::skipEncode(File fileObject) {
	int i = findFileEncode(fileObject);
	if (i >= 0) {
		_LibraryEncode.erase(_LibraryEncode.begin() + i);
		return true;
	}
	return false;
}

bool Library::skipEncode(std::string filePath) {
	return skipEncode(File(filePath));
}

} // namespace SuperEpicFuntime::MediaPreparer
