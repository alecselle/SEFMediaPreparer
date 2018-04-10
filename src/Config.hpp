#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/filesystem.hpp>
#include <string>

namespace SuperEpicFuntime {
	class Config {
	private:
		static boost::filesystem::path configFile;
	public:
		static void load();
		static void save();
		static std::string value(std::string key);
	};
} //namespace SuperEpicFuntime
#endif //CONFIG_HPP
