#include "src/Config.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <string>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
namespace bpo = boost::program_options;
using namespace std;

namespace SuperEpicFuntime {
	Config::Config(boost::filesystem::path configFile) {
		boost::program_options::variables_map vm;
	}

} // namespace SuperEpicFuntime
