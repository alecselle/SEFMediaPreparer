#ifndef LIBRARY_HPP
#define LIBRARY_HPP
#pragma once

#include <src/File.hpp>
#include <src/Settings.hpp>

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>

#include <string>

namespace SuperEpicFuntime {
	class Library {
	private:
		bool _hasSettings = false;
		bool _isInitialized = false;
		bool _isChecked = false;
		bool _isRecursive = false;
		std::string _directory;
		boost::container::vector<File> _Library = { };
		boost::container::vector<File> _LibraryEncode = { };
		boost::container::vector<std::string> _extensions = { ".wmv", ".avi", ".divx", ".mkv", ".mka", ".mks", ".webm", ".mp4", ".mpeg", ".mpg", ".mov", ".qt", ".flv" };
		int _duration = NULL;

		SuperEpicFuntime::Settings *_settings = new Settings();

	public:
		Library(SuperEpicFuntime::Settings *settingsObject);
		Library();
		bool Init(std::string directoryPath, SuperEpicFuntime::Settings *settingsObject, bool scanRecursive = false);
		bool Init(std::string, bool scanRecursive = false);
		bool setDirectory(std::string directoryPath, bool scanRecursive = true, bool scanNow = false);
		void setSettings(SuperEpicFuntime::Settings *settingsObject);
		void setRecursive(bool scanRecursive = true, bool scanNow = false);

		bool isValid(bool hasSettings = true, bool isInitialized = true, bool isChecked = true);

		void scan(bool scanRecursive = false);
		int size();
		int duration();
		void clear();

		boost::container::vector<SuperEpicFuntime::File> &getFiles();
		SuperEpicFuntime::File &getFile(int filePosition);
		int findFile(SuperEpicFuntime::File fileObject);
		int findFile(std::string filePath);

		bool addFile(SuperEpicFuntime::File fileObject);
		bool addFile(std::string filePath);

		bool removeFile(File fileObject);
		bool removeFile(std::string filePath);

		bool checkEncode(SuperEpicFuntime::File fileObject);

		void scanEncode();
		int sizeEncode();
		int durationEncode();
		void clearEncode();

		boost::container::vector<SuperEpicFuntime::File> &getFilesEncode();
		SuperEpicFuntime::File &getFileEncode(int filePosition);
		int findFileEncode(SuperEpicFuntime::File fileObject);
		int findFileEncode(std::string filePath);

		bool forceEncode(SuperEpicFuntime::File fileObject);
		bool forceEncode(std::string filePath);

		bool skipEncode(SuperEpicFuntime::File fileObject);
		bool skipEncode(std::string filePath);
	};
} // namespace SuperEpicFuntime
#endif // LIBRARY_HPP
