#ifndef FILE_HPP
#define FILE_HPP
#pragma once

#include <QtCore/QTime>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string.h>

class File {
private:
	boost::filesystem::path _path;
	boost::filesystem::path _pathSub;
	boost::filesystem::path _name;
	boost::filesystem::path _extension;

	std::string _vCodec;
	std::string _aCodec;
	int _duration = -1;

	int _subtitles = -1;
	bool _loaded = false;

public:
	File();
	File(boost::filesystem::path);
	boost::filesystem::path path();
	std::string pathStr();
	boost::filesystem::path pathSub();
	std::string pathSubStr();
	boost::filesystem::path name();
	std::string nameStr();
	boost::filesystem::path extension();
	std::string extensionStr();
	std::string vcodec();
	std::string acodec();
	int duration();
	int subtitles();
	std::string subtitlesStr();
	bool loadFileInfo(nlohmann::json jsonObject);
	bool isLoaded();
};

#endif // FILE_HPP
