#include "VideoLibrary.h"
#include <File.h>
#include <Settings.h>

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <string>

namespace bf = boost::filesystem;
namespace bc = boost::container;
using namespace std;

VideoLibrary::VideoLibrary(Settings *settings) {
	_settings = settings;
	_hasSettings = true;
	_isInitialized = false;
	_isChecked = false;
}

VideoLibrary::VideoLibrary() {
	_hasSettings = false;
	_isInitialized = false;
	_isChecked = false;
}

bool VideoLibrary::Init(bf::path libraryDirectory, Settings *settings, bool scanRecursive) {
	if (bf::exists(libraryDirectory) && bf::is_directory(libraryDirectory)) {
		_settings = settings;
		_directory = libraryDirectory;
		_isRecursive = scanRecursive;
		_videoLibrary.clear();
		_videoLibraryEncode.clear();

		_hasSettings = true;
		_isInitialized = true;
		scan(_isRecursive);
		_isChecked = false;

		return true;
	}
	return false;
}

bool VideoLibrary::Init(bf::path libraryDirectory, bool scanRecursive) {
	if (isValid(true, false, false) && bf::exists(libraryDirectory) && bf::is_directory(libraryDirectory)) {
		_directory = libraryDirectory;
		_isRecursive = scanRecursive;
		_videoLibrary.clear();
		_videoLibraryEncode.clear();

		_isInitialized = true;
		scan(_isRecursive);
		_isChecked = false;

		return true;
	}
	return false;
}

bool VideoLibrary::isValid(bool settings, bool initialized, bool loaded) {
	if (settings && !_hasSettings) return false;
	if (initialized && !_isInitialized) return false;
	if (loaded && !_isChecked) return false;
	return true;
}

void VideoLibrary::scan(bool scanRecursive) {
	if (isValid(true, true, false)) {
		_videoLibrary.clear();
		if (!scanRecursive) {
			for (bf::directory_entry &x : bf::directory_iterator(_directory)) {
				for (int i = 0; i < _extensions.size(); i++) {
					if (x.path().extension().string() == _extensions[i]) {
						File file = File(x.path());
						_videoLibrary.push_back(file);
					}
				}
			}
		} else {
			for (bf::directory_entry &x : bf::recursive_directory_iterator(_directory)) {
				if (x.path().parent_path().filename().string().compare("Converted") != 0) {
					for (int i = 0; i < _extensions.size(); i++) {
						if (x.path().extension().string().compare(_extensions[i]) == 0) {
							File file = File(x.path());
							_videoLibrary.push_back(file);
						}
					}
				}
			}
		}
	}
}

int VideoLibrary::size() {
	if (isValid(true, true, false)) return _videoLibrary.size();
	return NULL;
}

int VideoLibrary::duration() {
	if (isValid(true, true, false)) {
		if (_duration == NULL) {
			_duration = 0;
			for (int i = 0; i < size(); i++) {
				_duration += getFile(i).duration();
			}
		}
		return _duration;
	}
	return NULL;
}

void VideoLibrary::clear() {
	_videoLibrary.clear();
	clearEncode();
}

bc::vector<File> &VideoLibrary::getFiles() {
	if (isValid(true, true, false)) return _videoLibrary;
}

File &VideoLibrary::getFile(int pos) {
	if (isValid(true, true, false) && pos < _videoLibrary.size()) return _videoLibrary[pos];
}

int VideoLibrary::findFile(File file) {
	if (isValid(true, true, false)) {
		for (int i = 0; i < size(); i++) {
			if (getFile(i).path() == file.path()) return i;
		}
	}
	return NULL;
}

int VideoLibrary::findFile(boost::filesystem::path file) {
	if (isValid(true, true, false)) return findFile(File(file));
}

bool VideoLibrary::addFile(File file) {
	if (isValid(true, true, false) && bf::exists(file.path())) {
		_videoLibrary.push_back(file);
		if (_isChecked) scanEncode();
		return true;
	}
	return false;
}

bool VideoLibrary::addFile(boost::filesystem::path file) {
	if (isValid(true, true, false)) return addFile(File(file));
}

bool VideoLibrary::checkEncode(File file) {
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

		if (file.extensionStr().compare("." + _settings->container) == 0) {
			matches[2] = true;
		}

		if (!matches[0] || !matches[1] || !matches[2] && findFileEncode(file) == NULL) {
			return true;
		}
	}
	return false;
}

void VideoLibrary::scanEncode() {
	if (isValid(true, true, false)) {
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

				if (f.extensionStr().compare("." + _settings->container) == 0) {
					matches[2] = true;
				}
				if ((!matches[0] || !matches[1] || !matches[2]) && findFileEncode(f) == NULL) {
					_videoLibraryEncode.push_back(f);
				}
			}
		}
		if (_videoLibraryEncode.size() > 0) {
			_isChecked = true;
		} else {
			_isChecked = false;
		}
	}
}

int VideoLibrary::sizeEncode() {
	if (isValid(true, true, true)) return _videoLibraryEncode.size();
}

int VideoLibrary::durationEncode() {
	if (isValid(true, true, true)) {
		if (_duration == NULL) {
			_duration = 0;
			for (int i = 0; i < sizeEncode(); i++) {
				_duration += getFileEncode(i).duration();
			}
		}
		return _duration;
	}
}

void VideoLibrary::clearEncode() { _videoLibraryEncode.clear(); }

bc::vector<File> &VideoLibrary::getFilesEncode() {
	if (isValid(true, true, true)) return _videoLibraryEncode;
}

File &VideoLibrary::getFileEncode(int pos) {
	if (isValid(true, true, true) && pos < _videoLibraryEncode.size()) return _videoLibraryEncode[pos];
}

int VideoLibrary::findFileEncode(File file) {
	if (isValid(true, true, true)) {
		for (int i = 0; i < sizeEncode(); i++) {
			if (getFileEncode(i).path() == file.path()) return i;
		}
	}
	return NULL;
}

int VideoLibrary::findFileEncode(boost::filesystem::path file) {
	if (isValid(true, true, true)) return findFileEncode(File(file));
}

bool VideoLibrary::forceEncode(File file) {
	int i;
	if (isValid(true, true, true) && (i = findFile(file)) != NULL && findFileEncode(file) == NULL) {
		_videoLibraryEncode.push_back(file);
		return true;
	}
	return false;
}

bool VideoLibrary::forceEncode(boost::filesystem::path file) {
	if (isValid(true, true, true)) return forceEncode(File(file));
}

bool VideoLibrary::skipEncode(File file) {
	int i;
	if (isValid(true, true, true) && (i = findFileEncode(file)) != NULL) {
		_videoLibraryEncode.erase(_videoLibraryEncode.begin() + i);
		return true;
	}
	return false;
}

bool VideoLibrary::skipEncode(boost::filesystem::path file) {
	if (isValid(true, true, true)) return skipEncode(File(file));
}
