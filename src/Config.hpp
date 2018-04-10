#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string>


namespace SuperEpicFuntime {
	class Config {
	private:

	public:
		static const boost::filesystem::path APPDATA = getenv("APPDATA");
		static const boost::filesystem::path USERPROFILE = getenv("USERPROFILE");

		static const boost::filesystem::path BASE_DIR = APPDATA.string() + "\\SuperEpicFuntime\\SEFMediaPreparer";

		static const boost::filesystem::path CONFIG_FILE = BASE_DIR + "\\config.ini";


	};
} //namespace SuperEpicFuntime
#endif //CONFIG_HPP
