#ifndef LIBRARY_HPP
#define LIBRARY_HPP
#pragma once

#include <src/File.hpp>
#include <src/Settings.hpp>

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>

#include <string.h>

class Library {
private:
	bool _hasSettings = false;
	bool _isInitialized = false;
	bool _isChecked = false;
	bool _isRecursive = false;
	boost::filesystem::path _directory;
	boost::container::vector<File> _Library = { };
	boost::container::vector<File> _LibraryEncode = { };
	boost::container::vector<std::string> _extensions = { ".wmv", ".avi", ".divx", ".mkv", ".mka", ".mks", ".webm", ".mp4", ".mpeg", ".mpg", ".mov", ".qt", ".flv" };
	int _duration = -1;

	Settings *_settings = new Settings();

public:
	Library(Settings *settingsObject);
	Library();
	bool Init(boost::filesystem::path directoryPath, Settings *settingsObject, bool scanRecursive = false);
	bool Init(boost::filesystem::path, bool scanRecursive = false);
	bool setDirectory(boost::filesystem::path directoryPath, bool scanRecursive = true, bool scanNow = false);
	void setSettings(Settings *settingsObject);
	void setRecursive(bool scanRecursive = true, bool scanNow = false);

	bool isValid(bool hasSettings = true, bool isInitialized = true, bool isChecked = true);

	void scan(bool scanRecursive = false);
	int size();
	int duration();
	void clear();

	boost::container::vector<File> &getFiles();
	File &getFile(int filePosition);
	int findFile(File fileObject);
	int findFile(boost::filesystem::path filePath);

	bool addFile(File fileObject);
	bool addFile(boost::filesystem::path filePath);

	bool removeFile(File fileObject);
	bool removeFile(boost::filesystem::path filePath);

	bool checkEncode(File fileObject);

	void scanEncode();
	int sizeEncode();
	int durationEncode();
	void clearEncode();

	boost::container::vector<File> &getFilesEncode();
	File &getFileEncode(int filePosition);
	int findFileEncode(File fileObject);
	int findFileEncode(boost::filesystem::path filePath);

	bool forceEncode(File fileObject);
	bool forceEncode(boost::filesystem::path filePath);

	bool skipEncode(File fileObject);
	bool skipEncode(boost::filesystem::path filePath);
};

#endif // LIBRARY_HPP
