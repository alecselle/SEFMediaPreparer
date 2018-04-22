#include "src/Config.hpp"

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Config
 */
Config::Config() {
	if (!bf::exists(BASE_DIR)) {
		bf::create_directories(BASE_DIR);
	}
	if (!bf::exists(PRESET_DIR)) {
		bf::create_directories(PRESET_DIR);
	}
	if (!bf::exists(CONFIG_FILE)) {
		createDefault();
	}
	baseDir = BASE_DIR;
	logPath = LOG_FILE;
	refreshPresets();
	load();
}

string Config::parsePath(string path) {
	string t = path;
	ba::ireplace_all(path, "%APPDATA%", APPDATA.c_str());
	ba::ireplace_all(path, "%USERPROFILE%", USERPROFILE.c_str());
	return t;
}

string Config::parsePresetPath(string path) {
	string p = parsePath(path);
	string t = "";
	if (p.find(":/") == p.npos && p.find(":\\") == p.npos) {
		t += PRESET_DIR + "\\";
	}
	t += p;
	if (p.capacity() > PRESET_EXTENSION.capacity() && p.substr(p.capacity() - 7).compare(PRESET_EXTENSION) != 0) {
		t += PRESET_EXTENSION;
	}
	return t;
}

string Config::parsePresetName(string name) {
	bf::path p = parsePath(name);
	string t = p.filename().replace_extension().string();
	return t;
}

void Config::load() {
	FILE *fp = fopen(parsePath(CONFIG_FILE).c_str(), "rb");
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	Document d;
	d.ParseStream(is);

	if (d.HasMember("preset") && d["preset"].IsString()) {
		preset = new Preset(parsePresetPath(d["preset"].GetString()));
	} else {
		preset = new Preset();
	}
	if (d.HasMember("tempDir") && d["tempDir"].IsString()) {
		tempDir = parsePath(d["tempDir"].GetString());
		if (!bf::exists(tempDir)) {
			bf::create_directories(tempDir);
		}
	} else {
		tempDir = DEFAULT_TEMP_DIR;
	}
	if (d.HasMember("libraryDir") && d["libraryDir"].IsString()) {
		libraryDir = parsePath(d["libraryDir"].GetString());
	} else {
		libraryDir = DEFAULT_LIBRARY_DIR;
	}
	if (d.HasMember("outputDir") && d["outputDir"].IsString()) {
		outputDir = parsePath(d["outputDir"].GetString());
	} else {
		outputDir = libraryDir + DEFAULT_OUTPUT_FOLDER;
	}
	if (d.HasMember("preserveLog") && d["preserveLog"].IsBool()) {
		preserveLog = d["preserveLog"].GetBool();
	} else {
		preserveLog = DEFAULT_PRESERVE_LOG;
	}
	if (d.HasMember("vCodecs") && d["vCodecs"].IsArray()) {
		vCodecList.clear();
		for (int i = 0; i < (int)d["vCodecs"].GetArray().Size(); i++) {
			vCodecList.push_back({});
			for (int j = 0; j < (int)d["vCodecs"].GetArray()[i].GetArray().Size(); j++) {
				vCodecList[i].push_back(d["vCodecs"].GetArray()[i].GetArray()[j].GetString());
			}
		}
	} else {
		vCodecList = DEFAULT_VCODECS;
	}
	if (d.HasMember("aCodecs") && d["aCodecs"].IsArray()) {
		aCodecList.clear();
		for (int i = 0; i < (int)d["aCodecs"].GetArray().Size(); i++) {
			aCodecList.push_back({});
			for (int j = 0; j < (int)d["aCodecs"].GetArray()[i].GetArray().Size(); j++) {
				aCodecList[i].push_back(d["aCodecs"].GetArray()[i].GetArray()[j].GetString());
			}
		}
	} else {
		aCodecList = DEFAULT_ACODECS;
	}
	if (d.HasMember("containers") && d["containers"].IsArray()) {
		for (int i = 0; i < (int)d["containers"].GetArray().Size(); i++) {
			containerList.push_back(d["containers"].GetArray()[i].GetString());
		}
	} else {
		containerList = DEFAULT_CONTAINERS;
	}
	save();
	fclose(fp);
}

void Config::save() {
	string json = "{}";
	StringStream s(json.c_str());

	Document d;
	Document::AllocatorType &alloc = d.GetAllocator();
	d.ParseStream(s);

	d.AddMember(StringRef("preset"), Value(StringRef(preset->presetPath.c_str())), alloc);
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

	for (int i = 0; i < (int)vCodecList.size(); i++) {
		Value a(kArrayType);
		for (int j = 0; j < (int)vCodecList[i].size(); j++) {
			a.PushBack(Value().SetString(StringRef(vCodecList[i][j].c_str())), alloc);
		}
		d["vCodecs"].PushBack(a, alloc);
	}

	for (int i = 0; i < (int)aCodecList.size(); i++) {
		Value a(kArrayType);
		for (int j = 0; j < (int)aCodecList[i].size(); j++) {
			a.PushBack(Value().SetString(StringRef(aCodecList[i][j].c_str())), alloc);
		}
		d["aCodecs"].PushBack(a, alloc);
	}

	for (int i = 0; i < (int)containerList.size(); i++) {
		d["containers"].PushBack(Value().SetString(StringRef(containerList[i].c_str())), alloc);
	}

	FILE *fp = fopen(parsePath(CONFIG_FILE).c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);
}

void Config::createDefault() {
	libraryDir = DEFAULT_LIBRARY_DIR;
	tempDir = DEFAULT_TEMP_DIR;
	outputDir = DEFAULT_OUTPUT_DIR;
	preserveLog = DEFAULT_PRESERVE_LOG;
	vCodecList = DEFAULT_VCODECS;
	aCodecList = DEFAULT_ACODECS;
	containerList = DEFAULT_CONTAINERS;
	save();
}

void Config::refreshPresets() {
	presetList.clear();
	for (bf::directory_entry &x : bf::directory_iterator(PRESET_DIR)) {
		if (x.path().extension().compare(PRESET_EXTENSION) == 0) {
			presetList.push_back(new Preset(x.path().string()));
		}
	}
}

} // namespace SuperEpicFuntime
