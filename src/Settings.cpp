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

Settings::Settings() {
	if (!bf::exists(BASE_DIR)) {
		bf::create_directories(BASE_DIR);
	}
	if (!bf::exists(CONFIG_FILE)) {
		createDefaultConfig();
	}
	baseDir = BASE_DIR;
	logPath = LOG_FILE;
	for (bf::directory_entry &x : bf::directory_iterator(PRESET_DIR)) {
		if (x.path().extension().string().compare(".preset") == 0) {
			presetPathList.push_back(x.path());
			presetNameList.push_back(x.path().filename().replace_extension().string());
		}
	}
	loadConfig();
	loadPreset();
}

void Settings::saveConfig() {
	json j;
	j["preset"] = presetPath.string();
	j["libraryDir"] = libraryDir.string();
	j["tempDir"] = tempDir.string();
	j["outputDir"] = outputDir.string();
	j["preserveLog"] = preserveLog;
	for (int i = 0; i < vCodecList.size(); i++) {
		j["vCodecs"][i] = vCodecList[i];
	}
	for (int i = 0; i < aCodecList.size(); i++) {
		j["aCodecs"][i] = aCodecList[i];
	}
	j["containers"] = containerList;

	ofstream newConfig(CONFIG_FILE.string().c_str());
	newConfig << setw(4) << j;
	newConfig.close();
}

void Settings::savePreset() {
	savePresetAs(presetName);
}

void Settings::savePresetAs(string name) {
	json j;
	j["vCodec"] = vCodec;
	j["aCodec"] = aCodec;
	j["vQuality"] = vQuality;
	j["aQuality"] = aQuality;
	j["container"] = container;
	j["subtitles"] = subtitles;
	j["threads"] = threads;
	j["extraParams"] = extraParams;

	string newPath = PRESET_DIR.string() + "\\" + name + ".preset";

	ofstream newPreset(newPath);
	newPreset << setw(4) << j;
	newPreset.close();
	presetPath = newPath.c_str();
	presetName = name;
}

void Settings::loadConfig() {
	auto j = json::parse(ifstream(CONFIG_FILE.string().c_str()));
	if (j.find("preset") != j.end()) {
		string presetStr = j.find("preset").value();
		ba::ireplace_all(presetStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(presetStr, "%USERPROFILE%", USERPROFILE.c_str());
		presetPath = presetStr.c_str();
	} else {
		presetPath = DEFAULT_PRESET;
	}
	if (j.find("tempDir") != j.end()) {
		string tempDirStr = j.find("tempDir").value();
		ba::ireplace_all(tempDirStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(tempDirStr, "%USERPROFILE%", USERPROFILE.c_str());
		tempDir = tempDirStr.c_str();
		if (!bf::exists(tempDir)) {
			bf::create_directories(tempDir);
		}
	} else {
		tempDir = DEFAULT_TEMP_DIR;
	}
	if (j.find("libraryDir") != j.end()) {
		string libraryDirStr = j.find("libraryDir").value();
		ba::ireplace_all(libraryDirStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(libraryDirStr, "%USERPROFILE%", USERPROFILE.c_str());
		libraryDir = libraryDirStr.c_str();
	} else {
		libraryDir = DEFAULT_LIBRARY_DIR;
	}
	if (j.find("outputDir") != j.end()) {
		string outputDirStr = j.find("outputDir").value();
		ba::ireplace_all(outputDirStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(outputDirStr, "%USERPROFILE%", USERPROFILE.c_str());
		outputDir = outputDirStr.c_str();
	} else {
		outputDir = DEFAULT_OUTPUT_DIR;
	}
	if (j.find("preserveLog") != j.end()) {
		preserveLog = j.find("preserveLog").value();
	} else {
		preserveLog = DEFAULT_PRESERVE_LOG;
	}
	if (j.find("vCodecs") != j.end()) {
		for (int i = 0; i < j.find("vCodecs").value().size(); i++) {
			vCodecList.push_back(j.find("vCodecs").value()[i]);
		}
	} else {
		vCodecList = DEFAULT_VCODECS;
	}
	if (j.find("aCodecs") != j.end()) {
		for (int i = 0; i < j.find("aCodecs").value().size(); i++) {
			aCodecList.push_back(j.find("aCodecs").value()[i]);
		}
	} else {
		aCodecList = DEFAULT_ACODECS;
	}
	if (j.find("containers") != j.end()) {
		for (int i = 0; i < j.find("containers").value().size(); i++) {
			containerList.push_back(j.find("containers").value()[i]);
		}
	} else {
		containerList = DEFAULT_CONTAINERS;
	}
	saveConfig();
}

void Settings::loadPreset() {
	loadPresetFile(presetPath);
}

void Settings::loadPreset(string name) {
	loadPresetFile(PRESET_DIR.string() + "\\" + name + ".preset");
}

void Settings::loadPresetFile(bf::path path) {
	if (bf::exists(path)) {
		json j = json::parse(ifstream(path.string().c_str()));
		presetPath = path;
		presetName = path.filename().replace_extension().string();
		loadPresetJson(j);
	} else {
		if (!bf::exists(DEFAULT_PRESET)) {
			createDefaultPreset();
		}
		json j = json::parse(ifstream(DEFAULT_PRESET.string().c_str()));
		presetPath = path;
		presetName = path.filename().replace_extension().string();
		loadPresetJson(j);
		savePresetAs(presetName);
	}
}

void Settings::loadPresetJson(json j) {
	if (j.find("vCodec") != j.end()) {
		vCodec = j.find("vCodec").value();
	} else {
		vCodec = DEFAULT_VCODEC;
	}
	if (j.find("vQuality") != j.end()) {
		vQuality = j.find("vQuality").value();
	} else {
		vQuality = DEFAULT_VQUALITY;
	}
	if (j.find("aCodec") != j.end()) {
		aCodec = j.find("aCodec").value();
	} else {
		aCodec = DEFAULT_ACODEC;
	}
	if (j.find("aQuality") != j.end()) {
		aQuality = j.find("aQuality").value();
	} else {
		aQuality = DEFAULT_AQUALITY;
	}
	if (j.find("container") != j.end()) {
		container = j.find("container").value();
	} else {
		container = DEFAULT_CONTAINER;
	}
	if (j.find("subtitles") != j.end()) {
		subtitles = j.find("subtitles").value();
	} else {
		subtitles = DEFAULT_SUBTITLES;
	}
	if (j.find("threads") != j.end()) {
		threads = j.find("threads").value();
	} else {
		threads = DEFAULT_THREADS;
	}
	if (j.find("extraParams") != j.end()) {
		extraParams = j.find("extraParams").value();
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

	ofstream newConfig(CONFIG_FILE.string().c_str());
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

	ofstream newPreset(PRESET_DIR.string() + "\\SEF Standard.preset");
	newPreset << setw(4) << j;
	newPreset.close();
}