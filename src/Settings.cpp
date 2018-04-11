#include "src/Settings.hpp"

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
	Settings::Settings() {
		if (!bf::exists(BASE_DIR)) {
			bf::create_directories(BASE_DIR);
		}
		if (!bf::exists(CONFIG_FILE)) {
			createDefaultConfig();
		}
		baseDir = BASE_DIR;
		logPath = LOG_FILE;
		for (bf::directory_entry &x : bf::directory_iterator(bf::path(PRESET_DIR))) {
			if (x.path().extension().compare(PRESET_EXTENSION) == 0) {
				presetPathList.push_back(x.path().string());
				presetNameList.push_back(x.path().filename().replace_extension().string());
			}
		}
		loadConfig();
		loadPreset();
	}

	void Settings::saveConfig() {
		json j;
		j["preset"] = presetPath;
		j["libraryDir"] = libraryDir;
		j["tempDir"] = tempDir;
		j["outputDir"] = outputDir;
		j["preserveLog"] = preserveLog;
		for (int i = 0; i < vCodecList.size(); i++) {
			j["vCodecs"][i] = vCodecList[i];
		}
		for (int i = 0; i < aCodecList.size(); i++) {
			j["aCodecs"][i] = aCodecList[i];
		}
		j["containers"] = containerList;

		ofstream newConfig(CONFIG_FILE.c_str());
		newConfig << setw(4) << j;
		newConfig.close();
	}

	void Settings::savePreset() {
		savePresetAs(presetName);
	}

	void Settings::savePresetAs(std::string name) {
		json j;
		j["vCodec"] = vCodec;
		j["aCodec"] = aCodec;
		j["vQuality"] = vQuality;
		j["aQuality"] = aQuality;
		j["container"] = container;
		j["subtitles"] = subtitles;
		j["threads"] = threads;
		j["extraParams"] = extraParams;

		string newPath = PRESET_DIR + "\\" + name + PRESET_EXTENSION;

		ofstream newPreset(newPath);
		newPreset << setw(4) << j;
		newPreset.close();
		presetPath = newPath.c_str();
		presetName = name;
	}

	void Settings::loadConfig() {
		auto j = json::parse(ifstream(CONFIG_FILE.c_str())); // @suppress("Function cannot be resolved")
		if (j.find("preset") != j.end()) { // @suppress("Method cannot be resolved")
			string presetStr = j.find("preset").value(); // @suppress("Method cannot be resolved")
			ba::ireplace_all(presetStr, "%APPDATA%", APPDATA.c_str());
			ba::ireplace_all(presetStr, "%USERPROFILE%", USERPROFILE.c_str());
			presetPath = presetStr.c_str();
		} else {
			presetPath = DEFAULT_PRESET;
		}
		if (j.find("tempDir") != j.end()) { // @suppress("Method cannot be resolved")
			string tempDirStr = j.find("tempDir").value(); // @suppress("Method cannot be resolved")
			ba::ireplace_all(tempDirStr, "%APPDATA%", APPDATA.c_str());
			ba::ireplace_all(tempDirStr, "%USERPROFILE%", USERPROFILE.c_str());
			tempDir = tempDirStr.c_str();
			if (!bf::exists(tempDir)) {
				bf::create_directories(tempDir);
			}
		} else {
			tempDir = DEFAULT_TEMP_DIR;
		}
		if (j.find("libraryDir") != j.end()) { // @suppress("Method cannot be resolved")
			string libraryDirStr = j.find("libraryDir").value(); // @suppress("Method cannot be resolved")
			ba::ireplace_all(libraryDirStr, "%APPDATA%", APPDATA.c_str());
			ba::ireplace_all(libraryDirStr, "%USERPROFILE%", USERPROFILE.c_str());
			libraryDir = libraryDirStr.c_str();
		} else {
			libraryDir = DEFAULT_LIBRARY_DIR;
		}
		if (j.find("outputDir") != j.end()) { // @suppress("Method cannot be resolved")
			string outputDirStr = j.find("outputDir").value(); // @suppress("Method cannot be resolved")
			ba::ireplace_all(outputDirStr, "%APPDATA%", APPDATA.c_str());
			ba::ireplace_all(outputDirStr, "%USERPROFILE%", USERPROFILE.c_str());
			outputDir = outputDirStr.c_str();
		} else {
			outputDir = DEFAULT_OUTPUT_DIR;
		}
		if (j.find("preserveLog") != j.end()) { // @suppress("Method cannot be resolved")
			preserveLog = j.find("preserveLog").value(); // @suppress("Method cannot be resolved")
		} else {
			preserveLog = DEFAULT_PRESERVE_LOG;
		}
		if (j.find("vCodecs") != j.end()) { // @suppress("Method cannot be resolved")
			for (int i = 0; i < j.find("vCodecs").value().size(); i++) { // @suppress("Method cannot be resolved")
				vCodecList.push_back(j.find("vCodecs").value()[i]); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
			}
		} else {
			vCodecList = DEFAULT_VCODECS;
		}
		if (j.find("aCodecs") != j.end()) { // @suppress("Method cannot be resolved")
			for (int i = 0; i < j.find("aCodecs").value().size(); i++) { // @suppress("Method cannot be resolved")
				aCodecList.push_back(j.find("aCodecs").value()[i]); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
			}
		} else {
			aCodecList = DEFAULT_ACODECS;
		}
		if (j.find("containers") != j.end()) { // @suppress("Method cannot be resolved")
			for (int i = 0; i < j.find("containers").value().size(); i++) { // @suppress("Method cannot be resolved")
				containerList.push_back(j.find("containers").value()[i]); // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
			}
		} else {
			containerList = DEFAULT_CONTAINERS;
		}
		saveConfig();
	}

	void Settings::loadPreset() {
		loadPresetFile(presetPath);
	}

	void Settings::loadPreset(std::string name) {
		loadPresetFile(PRESET_DIR + "\\" + name + PRESET_EXTENSION);
	}

	void Settings::loadPresetFile(std::string path) {
		bf::path p = bf::canonical(path);
		if (bf::exists(p)) {
			json j = json::parse(ifstream(path.c_str())); // @suppress("Function cannot be resolved")

			presetPath = p.string();
			presetName = p.filename().replace_extension().string();
			loadPresetJson(j); // @suppress("Invalid arguments")
		} else {
			if (!bf::exists(DEFAULT_PRESET)) {
				createDefaultPreset();
			}
			json j = json::parse(ifstream(DEFAULT_PRESET.c_str())); // @suppress("Function cannot be resolved")
			presetPath = p.string();
			presetName = p.filename().replace_extension().string();
			loadPresetJson(j); // @suppress("Invalid arguments")
			savePresetAs(presetName);
		}
	}

	void Settings::loadPresetJson(nlohmann::json j) { // @suppress("Member declaration not found")
		if (j.find("vCodec") != j.end()) { // @suppress("Method cannot be resolved")
			vCodec = j.find("vCodec").value(); // @suppress("Method cannot be resolved")
		} else {
			vCodec = DEFAULT_VCODEC;
		}
		if (j.find("vQuality") != j.end()) { // @suppress("Method cannot be resolved")
			vQuality = j.find("vQuality").value(); // @suppress("Method cannot be resolved")
		} else {
			vQuality = DEFAULT_VQUALITY;
		}
		if (j.find("aCodec") != j.end()) { // @suppress("Method cannot be resolved")
			aCodec = j.find("aCodec").value(); // @suppress("Method cannot be resolved")
		} else {
			aCodec = DEFAULT_ACODEC;
		}
		if (j.find("aQuality") != j.end()) { // @suppress("Method cannot be resolved")
			aQuality = j.find("aQuality").value(); // @suppress("Method cannot be resolved")
		} else {
			aQuality = DEFAULT_AQUALITY;
		}
		if (j.find("container") != j.end()) { // @suppress("Method cannot be resolved")
			container = j.find("container").value(); // @suppress("Method cannot be resolved")
		} else {
			container = DEFAULT_CONTAINER;
		}
		if (j.find("subtitles") != j.end()) { // @suppress("Method cannot be resolved")
			subtitles = j.find("subtitles").value(); // @suppress("Method cannot be resolved")
		} else {
			subtitles = DEFAULT_SUBTITLES;
		}
		if (j.find("threads") != j.end()) { // @suppress("Method cannot be resolved")
			threads = j.find("threads").value(); // @suppress("Method cannot be resolved")
		} else {
			threads = DEFAULT_THREADS;
		}
		if (j.find("extraParams") != j.end()) { // @suppress("Method cannot be resolved")
			extraParams = j.find("extraParams").value(); // @suppress("Method cannot be resolved")
		} else {
			extraParams = DEFAULT_EXTRA_PARAMS;
		}
	}

	void Settings::createDefaultConfig() {
		json j;
		j["preset"] = DEFAULT_PRESET;
		j["libraryDir"] = DEFAULT_LIBRARY_DIR;
		j["tempDir"] = DEFAULT_TEMP_DIR;
		j["outputDir"] = DEFAULT_OUTPUT_DIR;
		j["preserveLog"] = DEFAULT_PRESERVE_LOG;
		for (int i = 0; i < DEFAULT_VCODECS.size(); i++) {
			j["vCodecs"][i] = DEFAULT_VCODECS[i];
		}
		for (int i = 0; i < DEFAULT_ACODECS.size(); i++) {
			j["aCodecs"][i] = DEFAULT_ACODECS[i];
		}
		j["containers"] = DEFAULT_CONTAINERS;

		ofstream newConfig(CONFIG_FILE.c_str());
		newConfig << setw(4) << j;
		newConfig.close();
	}

	void Settings::createDefaultPreset() {
		json j;
		j["vCodec"] = DEFAULT_VCODEC;
		j["aCodec"] = DEFAULT_ACODEC;
		j["vQuality"] = DEFAULT_VQUALITY;
		j["aQuality"] = DEFAULT_AQUALITY;
		j["container"] = DEFAULT_CONTAINER;
		j["subtitles"] = DEFAULT_SUBTITLES;
		j["threads"] = DEFAULT_THREADS;
		j["extraParams"] = DEFAULT_EXTRA_PARAMS;

		ofstream newPreset(PRESET_DIR + "\\SEF Standard" + PRESET_EXTENSION);
		newPreset << setw(4) << j;
		newPreset.close();
	}
} // namespace SuperEpicFuntime
