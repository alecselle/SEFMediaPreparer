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

/**
 * @brief Stores, scans, and probes files in a given directory
 * @author Alec S.
 */
class Library {
  private:
	// Supported extensions
	const boost::container::vector<std::string> _extensions {".wmv", ".avi", ".divx", ".mkv", ".mka", ".mks", ".webm", ".mp4", ".mpeg", ".mpg", ".mov", ".qt", ".flv"};
	// EventHandler
	EventHandler *_eventHandler;
	// Settings
	Settings *_settings;
	// File list
	boost::container::vector<File> _Library {};
	// File to encode list
	boost::container::vector<File> _LibraryEncode {};
	// Duration of files
	int _duration {-1};
	// Duration of files to encode
	int _durationEncode {-1};

  public:
	// Override to interrupt scan
	bool cancelScan {false};

	/**
	 * @brief Constructs library using settings
	 * @param settings - Settings instance to use
	 */
	Library(Settings *settings) {
		_settings = settings;
	}

	/**
	 * @brief Constructs library using settings and an eventhandler
	 * @param settings - Settings instance to use
	 * @param eventHandler - EventHandler instance to use
	 */
	Library(Settings *settings, EventHandler *eventHandler) {
		_settings	 = settings;
		_eventHandler = eventHandler;
	}

	/**
	 * @brief Scans the directory (provided by Settings) for media files
	 * @footnote Ignores files nested in 'Converted' folder
	 * @param scanRecursive - Whether to scan directory recursively (default: true)
	 */
	void scan(bool scanRecursive = true) {
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

#define LIBRARY_SECTION {

	/**
	 * @brief Returns the size of the library
	 * @return Number of media files
	 */
	unsigned int size() {
		return _Library.size();
	}

	/**
	 * @brief Returns the duration of all files in the library
	 * @return Duration of all media files
	 */
	int duration() {
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

	/**
	 * @brief Clears the library of all files
	 */
	void clear() {
		_Library.clear();
		clearEncode();
	}

	/**
	 * @brief Returns all files in library
	 * @return Vector of Files in library
	 */
	boost::container::vector<File> &getFiles() {
		return _Library;
	}

	/**
	 * @brief Returns the file at a specified index
	 * @param filePosition - Index of file
	 * @return File
	 */
	File &getFile(unsigned int filePosition) {
		if (filePosition < _Library.size()) {
			return _Library[filePosition];
		}
	}

	/**
	 * @brief Searches for the file index of a given File
	 * @param fileObject - File to search for
	 * @return Index of file or -1 if not found
	 */
	int findFile(File fileObject) {
		for (unsigned int i {0}; i < size(); i++) {
			if (getFile(i).path() == fileObject.path())
				return i;
		}
		return -1;
	}

	/**
	 * @brief Searches for the file index of a given path
	 * @param filePath - Path of file to search for
	 * @return Index of file ot -1 if not found
	 */
	int findFile(std::string filePath) {
		return findFile(File(filePath));
	}

	/**
	 * @brief Add a file to the library
	 * @param fileObject - File to add
	 * @return Whether file was successfully added
	 */
	bool addFile(File fileObject) {
		if (boost::filesystem::exists(fileObject.path().c_str())) {
			_Library.push_back(fileObject);
			return true;
		}
		return false;
	}

	/**
	 * @brief Add a file to the library using its path
	 * @param filePath - Path of file to add
	 * @return Whether file was successfully added
	 */
	bool addFile(std::string filePath) {
		return addFile(File(filePath));
	}

	/**
	 * @note UNIMPLEMENTED
	 * @brief Remove a file from the library
	 * @param fileObject - File to remove
	 * @return Whether file was found and removed
	 */
	bool removeFile(File fileObject);

	/**
	 * @note UNIMPLEMENTED
	 * @brief Remove a file from the library using its path
	 * @param filePath - Path of file to remove
	 * @return Whether file was found and removed
	 */
	bool removeFile(std::string filePath);

#define END_LIBRARY_SECTION }

	/**
	 * @brief Checks whether a file needs to be encoded, given options specified in Settings and file info
	 * @param fileObject - File to check
	 * @return Whether file needs to be encoded
	 */
	bool checkEncode(File fileObject) {
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

	/**
	 * @brief Checks which files (if any) need to be encoded, given options specified in Settings and file info
	 */
	void scanEncode() {
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

#define ENCODE_SECTION {

	/**
	 * @brief Returns the number of files that need to be encoded
	 * @return Number of files to encode
	 */
	int sizeEncode() {
		return _LibraryEncode.size();
	}

	/**
	 * @brief Returns the duration of all files that need to be encoded
	 * @return Duration of all files to encode
	 */
	int durationEncode() {
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

	/**
	 * @brief Clears the list of files to encode
	 */
	void clearEncode() {
		_LibraryEncode.clear();
		_durationEncode = -1;
	}

	/**
	 * @brief Returns all files that need to be encoded in library
	 * @return Vector of Files to encode in library
	 */
	boost::container::vector<File> &getFilesEncode() {
		return _LibraryEncode;
	}

	/**
	 * @brief Returns the file at a specified index
	 * @param filePosition - Index of file
	 * @return File
	 */
	File &getFileEncode(unsigned int filePosition) {
		if (filePosition < _LibraryEncode.size()) {
			return _LibraryEncode[filePosition];
		}
	}

	/**
	 * @brief Searches for the file index of a given File
	 * @param fileObject - File to search for
	 * @return Index of file or -1 if not found
	 */
	int findFileEncode(File fileObject) {
		for (int i {0}; i < sizeEncode(); i++) {
			if (getFileEncode(i).path().compare(fileObject.path()) == 0)
				return i;
		}
		return -1;
	}

	/**
	 * @brief Searches for the file index of a given File
	 * @param fileObject - File to search for
	 * @return Index of file or -1 if not found
	 */
	int findFileEncode(std::string filePath) {
		return findFileEncode(File(filePath));
	}

	/**
	 * @brief Force a file to be encoded
	 * @param fileObject - File to encode
	 * @return Whether file was found and marked
	 */
	bool forceEncode(File fileObject) {
		int i = findFile(fileObject);
		if (i >= 0 && findFileEncode(fileObject) < 0) {
			_LibraryEncode.push_back(fileObject);
			return true;
		}
		return false;
	}

	/**
	 * @brief Force a file to be encoded using its path
	 * @param filePath - Path of file to encode
	 * @return Whether file was found and marked
	 */
	bool forceEncode(std::string filePath) {
		return forceEncode(File(filePath));
	}

	/**
	 * @brief Force a file to be skipped when encoding
	 * @param fileObject - File to skip
	 * @return Whether file was found and marked
	 */
	bool skipEncode(File fileObject) {
		int i = findFileEncode(fileObject);
		if (i >= 0) {
			_LibraryEncode.erase(_LibraryEncode.begin() + i);
			return true;
		}
		return false;
	}

	/**
	 * @brief Force a file to be skipped when encoding using its path
	 * @param filePath - Path of file to skip
	 * @return Whether file was found and marked
	 */
	bool skipEncode(std::string filePath) {
		return skipEncode(File(filePath));
	}

#define END_ENCODE_SECTION }

};

/**
 * @brief Static member for global handling
 */
static Library *library;

} // namespace SuperEpicFuntime::MediaPreparer
#endif // LIBRARY_HPP
