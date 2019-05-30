#ifndef LIBRARY_HPP
#define LIBRARY_HPP
#pragma once

#include <Global.hpp>

#include <Event.hpp>

#include <File.hpp>
#include <Settings.hpp>

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>

namespace SuperEpicFuntime::MediaPreparer {

/** ================================================================================================
 * (Class) Library
 */
class Library {
  private:
	EventHandler *_eventHandler;
	Settings *_settings;

	boost::container::vector<File> _Library = {};

	boost::container::vector<File> _LibraryEncode = {};

	const boost::container::vector<std::string> _extensions = {".wmv", ".avi", ".divx", ".mkv", ".mka", ".mks", ".webm", ".mp4", ".mpeg", ".mpg", ".mov", ".qt", ".flv"};

	int _duration = -1;

  public:
	bool cancelScan = false;

	Library(Settings *settings) {
		_settings = settings;
	}
	Library(Settings *settings, EventHandler *eventHandler) {
		_settings	 = settings;
		_eventHandler = eventHandler;
	}

	void scan(bool scanRecursive = true) {
		_Library.clear();
		if (boost::filesystem::exists(_settings->libraryDir)) {
			if (scanRecursive) {
				for (boost::filesystem::directory_entry &x : boost::filesystem::recursive_directory_iterator(_settings->libraryDir)) {
					if (x.path().parent_path().filename().string().compare("Converted") != 0) {
						for (int i = 0; i < _extensions.size(); i++) {
							if (x.path().extension().string().compare(_extensions[i]) == 0) {
								File file = File(x.path().string());
								_Library.push_back(file);
							}
						}
					}
				}
			} else {
				for (boost::filesystem::directory_entry &x : boost::filesystem::directory_iterator(_settings->libraryDir)) {
					for (int i = 0; i < _extensions.size(); i++) {
						if (x.path().extension().string() == _extensions[i]) {
							File file = File(x.path().string());
							_Library.push_back(file);
						}
					}
				}
			}
		}
	}
	int size() {
		return _Library.size();
	}
	int duration() {
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
	void clear() {
		_Library.clear();
		clearEncode();
	}

	boost::container::vector<File> &getFiles() {
		return _Library;
	}
	File &getFile(int filePosition) {
		if (filePosition < _Library.size()) {
			return _Library[filePosition];
		}
	}
	int findFile(File fileObject) {
		for (int i = 0; i < size(); i++) {
			if (getFile(i).path() == fileObject.path())
				return i;
		}
		return -1;
	}
	int findFile(std::string filePath) {
		return findFile(File(filePath));
	}

	bool addFile(File fileObject) {
		if (boost::filesystem::exists(fileObject.path().c_str())) {
			_Library.push_back(fileObject);
			return true;
		}
		return false;
	}
	bool addFile(std::string filePath) {
		return addFile(File(filePath));
	}

	bool removeFile(File fileObject);
	bool removeFile(std::string filePath);

	bool checkEncode(File fileObject) {
		if (fileObject.isLoaded()) {
			bool matches[3] = {false, false, false};
			for (int j = 0; j < _settings->vCodecList.size(); j++) {
				if (_settings->vCodecList[j][0].compare(_settings->vCodec) == 0) {
					for (int k = 0; k < _settings->vCodecList[j].size(); k++) {
						if (fileObject.vcodec().compare(_settings->vCodecList[j][k]) == 0) {
							matches[0] = true;
						}
					}
				}
			}
			for (int l = 0; l < _settings->aCodecList.size(); l++) {
				if (_settings->aCodecList[l][0].compare(_settings->aCodec) == 0) {
					for (int m = 0; m < _settings->aCodecList[l].size(); m++) {
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

	void scanEncode() {
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

	int sizeEncode() {
		return _LibraryEncode.size();
	}
	int durationEncode() {
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
	void clearEncode() {
		_LibraryEncode.clear();
	}

	boost::container::vector<File> &getFilesEncode() {
		return _LibraryEncode;
	}
	File &getFileEncode(int filePosition) {
		if (filePosition < _LibraryEncode.size()) {
			return _LibraryEncode[filePosition];
		}
	}
	int findFileEncode(File fileObject) {
		for (int i = 0; i < sizeEncode(); i++) {
			if (getFileEncode(i).path().compare(fileObject.path()) == 0)
				return i;
		}
		return -1;
	}
	int findFileEncode(std::string filePath) {
		return findFileEncode(File(filePath));
	}

	bool forceEncode(File fileObject) {
		int i = findFile(fileObject);
		if (i >= 0 && findFileEncode(fileObject) < 0) {
			_LibraryEncode.push_back(fileObject);
			return true;
		}
		return false;
	}
	bool forceEncode(std::string filePath) {
		return forceEncode(File(filePath));
	}

	bool skipEncode(File fileObject) {
		int i = findFileEncode(fileObject);
		if (i >= 0) {
			_LibraryEncode.erase(_LibraryEncode.begin() + i);
			return true;
		}
		return false;
	}
	bool skipEncode(std::string filePath) {
		return skipEncode(File(filePath));
	}
};

static Library *library;

} // namespace SuperEpicFuntime::MediaPreparer
#endif // LIBRARY_HPP
