#include "src/Config.hpp"

#include <QtCore/QSettings>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <fstream>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
namespace bpo = boost::program_options;
using namespace std;

namespace SuperEpicFuntime {
	static void Config::load() {
		std::ifstream config_file(CONFIG_FILE.string().c_str());
	}

	static void Config::save() {

	}

	static string Config::value(string key) {

	}
} // namespace SuperEpicFuntime
