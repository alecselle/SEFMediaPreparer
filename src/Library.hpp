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
	Library(Settings *settings);

	/**
	 * @brief Constructs library using settings and an eventhandler
	 * @param settings - Settings instance to use
	 * @param eventHandler - EventHandler instance to use
	 */
	Library(Settings *settings, EventHandler *eventHandler);

	/**
	 * @brief Scans the directory (provided by Settings) for media files
	 * @footnote Ignores files nested in 'Converted' folder
	 * @param scanRecursive - Whether to scan directory recursively (default: true)
	 */
	void scan(bool scanRecursive = true);

	/**
	 * @brief Returns the size of the library
	 * @return Number of media files
	 */
	unsigned int size();

	/**
	 * @brief Returns the duration of all files in the library
	 * @return Duration of all media files
	 */
	int duration();

	/**
	 * @brief Clears the library of all files
	 */
	void clear();

	/**
	 * @brief Returns all files in library
	 * @return Vector of Files in library
	 */
	boost::container::vector<File> &getFiles();

	/**
	 * @brief Returns the file at a specified index
	 * @param filePosition - Index of file
	 * @return File
	 */
	File &getFile(unsigned int filePosition);

	/**
	 * @brief Searches for the file index of a given File
	 * @param fileObject - File to search for
	 * @return Index of file or -1 if not found
	 */
	int findFile(File fileObject);

	/**
	 * @brief Searches for the file index of a given path
	 * @param filePath - Path of file to search for
	 * @return Index of file ot -1 if not found
	 */
	int findFile(std::string filePath);

	/**
	 * @brief Add a file to the library
	 * @param fileObject - File to add
	 * @return Whether file was successfully added
	 */
	bool addFile(File fileObject);

	/**
	 * @brief Add a file to the library using its path
	 * @param filePath - Path of file to add
	 * @return Whether file was successfully added
	 */
	bool addFile(std::string filePath);

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
	
	/**
	 * @brief Checks whether a file needs to be encoded, given options specified in Settings and file info
	 * @param fileObject - File to check
	 * @return Whether file needs to be encoded
	 */
	bool checkEncode(File fileObject);

	/**
	 * @brief Checks which files (if any) need to be encoded, given options specified in Settings and file info
	 */
	void scanEncode();

	/**
	 * @brief Returns the number of files that need to be encoded
	 * @return Number of files to encode
	 */
	int sizeEncode();

	/**
	 * @brief Returns the duration of all files that need to be encoded
	 * @return Duration of all files to encode
	 */
	int durationEncode();

	/**
	 * @brief Clears the list of files to encode
	 */
	void clearEncode();

	/**
	 * @brief Returns all files that need to be encoded in library
	 * @return Vector of Files to encode in library
	 */
	boost::container::vector<File> &getFilesEncode();

	/**
	 * @brief Returns the file at a specified index
	 * @param filePosition - Index of file
	 * @return File
	 */
	File &getFileEncode(unsigned int filePosition);

	/**
	 * @brief Searches for the file index of a given File
	 * @param fileObject - File to search for
	 * @return Index of file or -1 if not found
	 */
	int findFileEncode(File fileObject);

	/**
	 * @brief Searches for the file index of a given File
	 * @param fileObject - File to search for
	 * @return Index of file or -1 if not found
	 */
	int findFileEncode(std::string filePath);

	/**
	 * @brief Force a file to be encoded
	 * @param fileObject - File to encode
	 * @return Whether file was found and marked
	 */
	bool forceEncode(File fileObject);

	/**
	 * @brief Force a file to be encoded using its path
	 * @param filePath - Path of file to encode
	 * @return Whether file was found and marked
	 */
	bool forceEncode(std::string filePath);

	/**
	 * @brief Force a file to be skipped when encoding
	 * @param fileObject - File to skip
	 * @return Whether file was found and marked
	 */
	bool skipEncode(File fileObject);

	/**
	 * @brief Force a file to be skipped when encoding using its path
	 * @param filePath - Path of file to skip
	 * @return Whether file was found and marked
	 */
	bool skipEncode(std::string filePath);
};

/**
 * @brief Static member for global handling
 */
static Library *library;

} // namespace SuperEpicFuntime::MediaPreparer
#endif // LIBRARY_HPP
