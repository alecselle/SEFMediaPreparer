#ifndef LIBRARY_HPP
#define LIBRARY_HPP
#pragma once

#include <File.hpp>
#include <Settings.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <src/File.hpp>
#include <src/Settings.hpp>
#include <string>

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Library
 */
class Library {
  private:
	boost::container::vector<File> _Library = {};
	boost::container::vector<File> _LibraryEncode = {};
	boost::container::vector<std::string> _extensions = {".wmv", ".avi",  ".divx", ".mkv", ".mka", ".mks", ".webm",
														 ".mp4", ".mpeg", ".mpg",  ".mov", ".qt",  ".flv"};
	int _duration = -1;

  public:
	Library();
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

static Library *library;

} // namespace SuperEpicFuntime
#endif // LIBRARY_HPP
