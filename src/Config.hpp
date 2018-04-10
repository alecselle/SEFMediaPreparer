#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/filesystem.hpp>
#include <string>

namespace SuperEpicFuntime {
	class Config {
	private:

	public:
		Config(boost::filesystem::path configFile);
	};
} //namespace SuperEpicFuntime
#endif //CONFIG_HPP
