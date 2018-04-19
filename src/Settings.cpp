#include "src/Settings.hpp"

#include "rapidjson/filereadstream.h"
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/filesystem.hpp>
#include <cstdio>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <string>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {
Settings::Settings() {
	if (!bf::exists(BASE_DIR)) {
		bf::create_directories(BASE_DIR);
	}
	if (!bf::exists(PRESET_DIR)) {
		bf::create_directories(PRESET_DIR);
	}
	if (!bf::exists(CONFIG_FILE)) {
		createDefaultConfig();
	}
	baseDir = BASE_DIR;
	logPath = LOG_FILE;
	refreshPresets();
	loadConfig();
	loadPreset();
}

string Settings::parsePath(string path) {
}

void Settings::loadConfig() {
	FILE *fp = fopen(CONFIG_FILE.c_str(), "rb");
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	Document d;
	d.ParseStream(is);

	if (d.HasMember("preset") && d["preset"].IsString()) {
		string presetStr = d["preset"].GetString();
		ba::ireplace_all(presetStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(presetStr, "%USERPROFILE%", USERPROFILE.c_str());
		presetPath = presetStr.c_str();
	} else {
		presetPath = DEFAULT_PRESET;
	}
	presetName = bf::path(presetPath).filename().replace_extension().string();
	if (d.HasMember("tempDir") && d["tempDir"].IsString()) {
		string tempDirStr = d["tempDir"].GetString();
		ba::ireplace_all(tempDirStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(tempDirStr, "%USERPROFILE%", USERPROFILE.c_str());
		tempDir = tempDirStr.c_str();
		if (!bf::exists(tempDir)) {
			bf::create_directories(tempDir);
		}
	} else {
		tempDir = DEFAULT_TEMP_DIR;
	}
	if (d.HasMember("libraryDir") && d["libraryDir"].IsString()) {
		string libraryDirStr = d["libraryDir"].GetString();
		ba::ireplace_all(libraryDirStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(libraryDirStr, "%USERPROFILE%", USERPROFILE.c_str());
		libraryDir = libraryDirStr.c_str();
	} else {
		libraryDir = DEFAULT_LIBRARY_DIR;
	}
	if (d.HasMember("outputDir") && d["outputDir"].IsString()) {
		string outputDirStr = d["outputDir"].GetString();
		ba::ireplace_all(outputDirStr, "%APPDATA%", APPDATA.c_str());
		ba::ireplace_all(outputDirStr, "%USERPROFILE%", USERPROFILE.c_str());
		outputDir = outputDirStr.c_str();
	} else {
		outputDir = DEFAULT_OUTPUT_DIR;
	}
	if (d.HasMember("preserveLog") && d["preserveLog"].IsBool()) {
		preserveLog = d["preserveLog"].GetBool();
	} else {
		preserveLog = DEFAULT_PRESERVE_LOG;
	}
	if (d.HasMember("vCodecs") && d["vCodecs"].IsArray()) {
		vCodecList.clear();
		for (int i = 0; i < d["vCodecs"].GetArray().Size(); i++) {
			vCodecList.push_back({});
			for (int j = 0; j < d["vCodecs"].GetArray()[i].GetArray().Size(); j++) {
				vCodecList[i].push_back(d["vCodecs"].GetArray()[i].GetArray()[j].GetString());
			}
		}
	} else {
		vCodecList = DEFAULT_VCODECS;
	}
	if (d.HasMember("aCodecs") && d["aCodecs"].IsArray()) {
		aCodecList.clear();
		for (int i = 0; i < d["aCodecs"].GetArray().Size(); i++) {
			aCodecList.push_back({});
			for (int j = 0; j < d["aCodecs"].GetArray()[i].GetArray().Size(); j++) {
				aCodecList[i].push_back(d["aCodecs"].GetArray()[i].GetArray()[j].GetString());
			}
		}
	} else {
		aCodecList = DEFAULT_ACODECS;
	}
	if (d.HasMember("containers") && d["containers"].IsArray()) {
		for (int i = 0; i < d["containers"].GetArray().Size(); i++) {
			containerList.push_back(d["containers"].GetArray()[i].GetString());
		}
	} else {
		containerList = DEFAULT_CONTAINERS;
	}
	saveConfig();
	fclose(fp);
}

void Settings::saveConfig() {
	string json = "{}";
	StringStream s(json.c_str());

	Document d;
	Document::AllocatorType &alloc = d.GetAllocator();
	d.ParseStream(s);

	d.AddMember(StringRef("preset"), Value(StringRef(presetPath.c_str())), alloc);
	d.AddMember(StringRef("libraryDir"), Value(StringRef(libraryDir.c_str())), alloc);
	d.AddMember(StringRef("tempDir"), Value(StringRef(tempDir.c_str())), alloc);
	d.AddMember(StringRef("outputDir"), Value(StringRef(outputDir.c_str())), alloc);
	d.AddMember(StringRef("preserveLog"), Value(preserveLog), alloc);
	d.AddMember(StringRef("vCodecs"), Value(), alloc);
	d["vCodecs"].SetArray();
	d.AddMember(StringRef("aCodecs"), Value(), alloc);
	d["aCodecs"].SetArray();
	d.AddMember(StringRef("containers"), Value(), alloc);
	d["containers"].SetArray();

	for (int i = 0; i < vCodecList.size(); i++) {
		Value a(kArrayType);
		for (int j = 0; j < vCodecList[i].size(); j++) {
			a.PushBack(Value().SetString(StringRef(vCodecList[i][j].c_str())), alloc);
		}
		d["vCodecs"].PushBack(a, alloc);
	}

	for (int i = 0; i < aCodecList.size(); i++) {
		Value a(kArrayType);
		for (int j = 0; j < aCodecList[i].size(); j++) {
			a.PushBack(Value().SetString(StringRef(aCodecList[i][j].c_str())), alloc);
		}
		d["aCodecs"].PushBack(a, alloc);
	}

	for (int i = 0; i < containerList.size(); i++) {
		d["containers"].PushBack(Value().SetString(StringRef(containerList[i].c_str())), alloc);
	}

	FILE *fp = fopen(CONFIG_FILE.c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);
}

void Settings::createDefaultConfig() {
	string json = "{}";
	StringStream s(json.c_str());

	Document d;
	Document::AllocatorType &alloc = d.GetAllocator();
	d.ParseStream(s);

	if (d.HasParseError()) {
		cout << "faq";
	}

	d.AddMember(StringRef("preset"), Value(StringRef(DEFAULT_PRESET.c_str())), alloc);
	d.AddMember(StringRef("libraryDir"), Value(StringRef(DEFAULT_LIBRARY_DIR.c_str())), alloc);
	d.AddMember(StringRef("tempDir"), Value(StringRef(DEFAULT_TEMP_DIR.c_str())), alloc);
	d.AddMember(StringRef("outputDir"), Value(StringRef(DEFAULT_OUTPUT_DIR.c_str())), alloc);
	d.AddMember(StringRef("preserveLog"), Value(DEFAULT_PRESERVE_LOG), alloc);
	d.AddMember(StringRef("vCodecs"), Value(), alloc);
	d["vCodecs"].SetArray();
	d.AddMember(StringRef("aCodecs"), Value(), alloc);
	d["aCodecs"].SetArray();
	d.AddMember(StringRef("containers"), Value(), alloc);
	d["containers"].SetArray();

	for (int i = 0; i < DEFAULT_VCODECS.size(); i++) {
		Value a(kArrayType);
		for (int j = 0; j < DEFAULT_VCODECS[i].size(); j++) {
			a.PushBack(Value().SetString(StringRef(DEFAULT_VCODECS[i][j].c_str())), alloc);
		}
		d["vCodecs"].PushBack(a, alloc);
	}

	for (int i = 0; i < DEFAULT_ACODECS.size(); i++) {
		Value a(kArrayType);
		for (int j = 0; j < DEFAULT_ACODECS[i].size(); j++) {
			a.PushBack(Value().SetString(StringRef(DEFAULT_ACODECS[i][j].c_str())), alloc);
		}
		d["aCodecs"].PushBack(a, alloc);
	}

	for (int i = 0; i < DEFAULT_CONTAINERS.size(); i++) {
		d["containers"].PushBack(Value().SetString(StringRef(DEFAULT_CONTAINERS[i].c_str())), alloc);
	}

	FILE *fp = fopen(CONFIG_FILE.c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);
}

void Settings::loadPreset() {
	loadPresetFile(presetPath);
}

void Settings::loadPreset(std::string name) {
	string newPath;
	if (name.find(":/") == name.npos && name.find(":\\") == name.npos) {
		newPath += PRESET_DIR + "\\";
	}
	newPath += name;
	if (name.capacity() > PRESET_EXTENSION.capacity() &&
		name.substr(name.capacity() - 7).compare(PRESET_EXTENSION) != 0) {
		newPath += PRESET_EXTENSION;
	}
	loadPresetFile(newPath);
}

void Settings::loadPresetFile(std::string path) {
	bf::path p(path.c_str());
	if (bf::exists(p) && bf::is_regular_file(p)) {

		FILE *fp = fopen(p.string().c_str(), "rb");
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document d;
		d.ParseStream(is);

		if (d.HasMember("vCodec") && d["vCodec"].IsString()) {
			vCodec = d["vCodec"].GetString();
		} else {
			vCodec = DEFAULT_VCODEC;
		}
		if (d.HasMember("vQuality") && d["vQuality"].IsString()) {
			vQuality = d["vQuality"].GetString();
		} else {
			vQuality = DEFAULT_VQUALITY;
		}
		if (d.HasMember("aCodec") && d["aCodec"].IsString()) {
			aCodec = d["aCodec"].GetString();
		} else {
			aCodec = DEFAULT_ACODEC;
		}
		if (d.HasMember("aQuality") && d["aQuality"].IsString()) {
			aQuality = d["aQuality"].GetString();
		} else {
			aQuality = DEFAULT_AQUALITY;
		}
		if (d.HasMember("container") && d["container"].IsString()) {
			container = d["container"].GetString();
		} else {
			container = DEFAULT_CONTAINER;
		}
		if (d.HasMember("subtitles") && d["subtitles"].IsString()) {
			subtitles = d["subtitles"].GetString();
		} else {
			subtitles = DEFAULT_SUBTITLES;
		}
		if (d.HasMember("threads") && d["threads"].IsString()) {
			threads = d["threads"].GetString();
		} else {
			threads = DEFAULT_THREADS;
		}
		if (d.HasMember("extraParams") && d["extraParams"].IsString()) {
			extraParams = d["extraParams"].GetString();
		} else {
			extraParams = DEFAULT_EXTRA_PARAMS;
		}
		presetPath = path;
		presetName = bf::path(presetPath).filename().replace_extension().string();
	} else {
		if (!bf::exists(DEFAULT_PRESET)) {
			createDefaultPreset();
		}
		if (bf::exists(DEFAULT_PRESET)) {
			loadPresetFile(DEFAULT_PRESET);
		}
	}
}

void Settings::savePreset() {
	savePresetAs(presetName);
}

void Settings::savePresetAs(std::string name) {
	string json = "{}";
	StringStream s(json.c_str());

	Document d;
	Document::AllocatorType &alloc = d.GetAllocator();
	d.ParseStream(s);

	d.AddMember(StringRef("vCodec"), Value(StringRef(vCodec.c_str())), alloc);
	d.AddMember(StringRef("vQuality"), Value(StringRef(vQuality.c_str())), alloc);
	d.AddMember(StringRef("aCodec"), Value(StringRef(aCodec.c_str())), alloc);
	d.AddMember(StringRef("aQuality"), Value(StringRef(aQuality.c_str())), alloc);
	d.AddMember(StringRef("container"), Value(StringRef(container.c_str())), alloc);
	d.AddMember(StringRef("subtitles"), Value(StringRef(subtitles.c_str())), alloc);
	d.AddMember(StringRef("threads"), Value(StringRef(threads.c_str())), alloc);
	d.AddMember(StringRef("extraParams"), Value(StringRef(extraParams.c_str())), alloc);

	string newPath;
	if (name.find(":/") == name.npos && name.find(":\\") == name.npos) {
		newPath += PRESET_DIR + "\\";
	}
	newPath += name;
	if (name.capacity() > PRESET_EXTENSION.capacity() &&
		name.substr(name.capacity() - 7).compare(PRESET_EXTENSION) != 0) {
		newPath += PRESET_EXTENSION;
	}
	FILE *fp = fopen(newPath.c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);

	presetPath = newPath.c_str();
	presetName = bf::path(newPath).filename().replace_extension().string();

	refreshPresets();
}

void Settings::refreshPresets() {
	presetPathList.clear();
	presetNameList.clear();
	for (bf::directory_entry &x : bf::directory_iterator(PRESET_DIR)) {
		if (x.path().extension().compare(PRESET_EXTENSION) == 0) {
			presetPathList.push_back(x.path().string());
			presetNameList.push_back(x.path().filename().replace_extension().string());
		}
	}
}

void Settings::createDefaultPreset() {
	string json = "{}";
	StringStream s(json.c_str());

	Document d;
	Document::AllocatorType &alloc = d.GetAllocator();
	d.ParseStream(s);

	d.AddMember(StringRef("vCodec"), Value(StringRef(DEFAULT_VCODEC.c_str())), alloc);
	d.AddMember(StringRef("vQuality"), Value(StringRef(DEFAULT_VQUALITY.c_str())), alloc);
	d.AddMember(StringRef("aCodec"), Value(StringRef(DEFAULT_ACODEC.c_str())), alloc);
	d.AddMember(StringRef("aQuality"), Value(StringRef(DEFAULT_AQUALITY.c_str())), alloc);
	d.AddMember(StringRef("container"), Value(StringRef(DEFAULT_CONTAINER.c_str())), alloc);
	d.AddMember(StringRef("subtitles"), Value(StringRef(DEFAULT_SUBTITLES.c_str())), alloc);
	d.AddMember(StringRef("threads"), Value(StringRef(DEFAULT_THREADS.c_str())), alloc);
	d.AddMember(StringRef("extraParams"), Value(StringRef(DEFAULT_EXTRA_PARAMS.c_str())), alloc);

	FILE *fp = fopen(DEFAULT_PRESET.c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);
	refreshPresets();
}
} // namespace SuperEpicFuntime
