#ifndef FILE_HPP
#define FILE_HPP
#pragma once

#include <rapidjson/document.h>
#include <string>

namespace SuperEpicFuntime {
class File {
  private:
	std::string _path;
	std::string _pathSub;
	std::string _name;
	std::string _extension;

	std::string _vCodec;
	std::string _aCodec;
	int _duration = -1;

	int _subtitles = -1;
	bool _loaded = false;

  public:
	File();
	File(std::string);
	std::string path();
	std::string pathSub();
	std::string name();
	std::string extension();
	std::string vcodec();
	std::string acodec();
	int duration();
	int subtitles();
	std::string subtitlesStr();
	bool loadFileInfo(rapidjson::StringStream ss);
	bool isLoaded();
};
} // namespace SuperEpicFuntime
#endif // FILE_HPP
