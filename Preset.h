#ifndef PRESET_H
#define PRESET_H

#include <Config.h>

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string>

class Preset {
  private:
	const std::string DEFAULT_VCODEC = "libx265";
	const std::string DEFAULT_ACODEC = "aac";
	const std::string DEFAULT_VQUALITY = "24";
	const std::string DEFAULT_AQUALITY = "384";
	const std::string DEFAULT_CONTAINER = "mkv";
	const std::string DEFAULT_SUBTITLES = "Embed";
	const std::string DEFAULT_THREADS = "4";
	const std::string DEFAULT_EXTRA_PARAMS = "-x265-params pools=3";

	nlohmann::json loadDefault();
	void createDefault();

  public:
	Preset();
	Preset(boost::filesystem::path);
	Preset(std::string);

	void load();
	void load(boost::filesystem::path);
	void load(nlohmann::json);
	void save(std::string);

	std::string name, vCodec, aCodec, vQuality, aQuality, subtitles, extraParams, threads, container;
	boost::filesystem::path path;
};

#endif // PRESET_H
