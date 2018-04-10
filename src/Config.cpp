#include "src/Config.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace nlohmann;
using namespace std;

namespace SuperEpicFuntime {
	static void Config::load() {
		QSettings settings(CONFIG_FILE.string().c_str(), QSettings::IniFormat);
	}

	static void Config::save() {

	}

	static string Config::value(string key) {
		return settings.value(key.c_str(), "null").toString();
	}
} // namespace SuperEpicFuntime
