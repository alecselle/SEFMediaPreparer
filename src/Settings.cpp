#include <Settings.hpp>

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;

namespace SuperEpicFuntime::MediaPreparer {

Settings::Settings() {
	init();
}

Settings::Settings(EventHandler *eventHandler) {
	_eventHandler = eventHandler;
	init();
}

void Settings::init() {
	if (!bf::exists(BASE_DIR)) {
		bf::create_directories(BASE_DIR);
	}
	if (!bf::exists(PRESET_DIR)) {
		bf::create_directories(PRESET_DIR);
	}
	if (!bf::exists(DEFAULT_PRESET)) {
		createDefaultPreset();
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

#define LOAD_SECTION {

std::string Settings::parsePath(std::string path) {
	std::string t {path};
	ba::ireplace_all(path, "%APPDATA%", APPDATA.c_str());
	ba::ireplace_all(path, "%USERPROFILE%", USERPROFILE.c_str());
	return t;
}

std::string Settings::parsePresetPath(std::string path) {
	std::string p {parsePath(path)};
	std::string t {""};
	if (p.find(":/") == p.npos && p.find(":\\") == p.npos) {
		t += PRESET_DIR + "\\";
	}
	t += p;
	if (p.capacity() > PRESET_EXTENSION.capacity() && p.substr(p.capacity() - 7).compare(PRESET_EXTENSION) != 0) {
		t += PRESET_EXTENSION;
	}
	return t;
}

std::string Settings::parsePresetName(std::string name) {
	bf::path p {parsePresetPath(name)};
	std::string t {p.filename().replace_extension().string()};
	return t;
}

void Settings::loadConfig() {
	bool save {false};
	FILE *fp {fopen(parsePath(CONFIG_FILE).c_str(), "rb")};
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document d;
	d.ParseStream(is);

	if (d.HasMember("preset") && d["preset"].IsString()) {
		presetPath = parsePresetPath(d["preset"].GetString());
	} else {
		presetPath = DEFAULT_PRESET;
		save	   = true;
	}
	presetName = bf::path(presetPath).filename().replace_extension().string();
	if (d.HasMember("tempDir") && d["tempDir"].IsString()) {
		tempDir = parsePath(d["tempDir"].GetString());
		if (!bf::exists(tempDir)) {
			bf::create_directories(tempDir);
		}
	} else {
		tempDir = DEFAULT_TEMP_DIR;
		save	= true;
	}
	if (d.HasMember("libraryDir") && d["libraryDir"].IsString()) {
		libraryDir = parsePath(d["libraryDir"].GetString());
	} else {
		libraryDir = DEFAULT_LIBRARY_DIR;
		save	   = true;
	}
	if (d.HasMember("outputDir") && d["outputDir"].IsString()) {
		outputDir = parsePath(d["outputDir"].GetString());
	} else {
		outputDir = libraryDir + DEFAULT_OUTPUT_FOLDER;
		save	  = true;
	}
	if (d.HasMember("preserveLog") && d["preserveLog"].IsBool()) {
		preserveLog = d["preserveLog"].GetBool();
	} else {
		preserveLog = DEFAULT_PRESERVE_LOG;
		save		= true;
	}
	if (d.HasMember("forceEncode") && d["forceEncode"].IsBool()) {
		preserveLog = d["forceEncode"].GetBool();
	} else {
		preserveLog = DEFAULT_FORCE_ENCODE;
		save		= true;
	}
	if (d.HasMember("fixMetadata") && d["fixMetadata"].IsBool()) {
		fixMetadata = d["fixMetadata"].GetBool();
	} else {
		fixMetadata = DEFAULT_FIX_METADATA;
		save		= true;
	}
	if (d.HasMember("subfolders") && d["subfolders"].IsBool()) {
		subfolders = d["subfolders"].GetBool();
	} else {
		subfolders = DEFAULT_SUBFOLDERS;
		save		= true;
	}
	if (d.HasMember("vCodecs") && d["vCodecs"].IsArray()) {
		vCodecList.clear();
		for (int i {0}; i < (int)d["vCodecs"].GetArray().Size(); i++) {
			vCodecList.push_back({});
			for (int j = 0; j < (int)d["vCodecs"].GetArray()[i].GetArray().Size(); j++) {
				vCodecList[i].push_back(d["vCodecs"].GetArray()[i].GetArray()[j].GetString());
			}
		}
	} else {
		vCodecList = DEFAULT_VCODECS;
		save	   = true;
	}
	if (d.HasMember("aCodecs") && d["aCodecs"].IsArray()) {
		aCodecList.clear();
		for (int i {0}; i < (int)d["aCodecs"].GetArray().Size(); i++) {
			aCodecList.push_back({});
			for (int j = 0; j < (int)d["aCodecs"].GetArray()[i].GetArray().Size(); j++) {
				aCodecList[i].push_back(d["aCodecs"].GetArray()[i].GetArray()[j].GetString());
			}
		}
	} else {
		aCodecList = DEFAULT_ACODECS;
		save	   = true;
	}
	if (d.HasMember("containers") && d["containers"].IsArray()) {
		for (int i {0}; i < (int)d["containers"].GetArray().Size(); i++) {
			containerList.push_back(d["containers"].GetArray()[i].GetString());
		}
	} else {
		containerList = DEFAULT_CONTAINERS;
		save		  = true;
	}
	fclose(fp);
	if (_eventHandler != NULL) {
		_eventHandler->newEvent(CONFIG_LOADED, "Loaded Config");
	}
	if (save) {
		saveConfig();
	}
}

void Settings::loadPreset() {
	loadPresetFile(presetPath);
}

void Settings::loadPreset(std::string name) {
	loadPresetFile(parsePresetPath(name));
}

void Settings::loadPresetFile(std::string path) {
	bf::path p(parsePresetPath(path).c_str());
	if (bf::exists(p) && bf::is_regular_file(p)) {

		FILE *fp {fopen(p.string().c_str(), "rb")};
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		rapidjson::Document d;
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
		fclose(fp);
		presetPath = parsePresetPath(path);
		presetName = parsePresetName(path);
	} else {
		if (!bf::exists(DEFAULT_PRESET)) {
			createDefaultPreset();
		}
		if (bf::exists(DEFAULT_PRESET)) {
			loadPresetFile(DEFAULT_PRESET);
		}
	}
	if (_eventHandler != NULL) {
		_eventHandler->newEvent(PRESET_LOADED, "Loaded Preset: " + presetName);
	}
}

void Settings::refreshPresets() {
	presetPathList.clear();
	presetNameList.clear();
	for (bf::directory_entry &x : bf::directory_iterator(PRESET_DIR)) {
		if (x.path().extension().compare(PRESET_EXTENSION) == 0) {
			presetPathList.push_back(parsePresetPath(x.path().string()));
			presetNameList.push_back(parsePresetName(x.path().string()));
		}
	}
}

#define END_LOAD_SECTION }
#define SAVE_SECTION {

void Settings::saveConfig() {
	std::string json {"{}"};
	rapidjson::StringStream s(json.c_str());

	rapidjson::Document d;
	rapidjson::Document::AllocatorType &alloc {d.GetAllocator()};
	d.ParseStream(s);

	d.AddMember(rapidjson::StringRef("preset"), rapidjson::Value(rapidjson::StringRef(presetPath.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("libraryDir"), rapidjson::Value(rapidjson::StringRef(libraryDir.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("tempDir"), rapidjson::Value(rapidjson::StringRef(tempDir.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("outputDir"), rapidjson::Value(rapidjson::StringRef(outputDir.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("preserveLog"), rapidjson::Value(preserveLog), alloc);
	d.AddMember(rapidjson::StringRef("forceEncode"), rapidjson::Value(forceEncode), alloc);
	d.AddMember(rapidjson::StringRef("fixMetadata"), rapidjson::Value(fixMetadata), alloc);
	d.AddMember(rapidjson::StringRef("subfolders"), rapidjson::Value(subfolders), alloc);
	d.AddMember(rapidjson::StringRef("vCodecs"), rapidjson::Value(), alloc);
	d["vCodecs"].SetArray();
	d.AddMember(rapidjson::StringRef("aCodecs"), rapidjson::Value(), alloc);
	d["aCodecs"].SetArray();
	d.AddMember(rapidjson::StringRef("containers"), rapidjson::Value(), alloc);
	d["containers"].SetArray();

	for (int i {0}; i < (int)vCodecList.size(); i++) {
		rapidjson::Value a(rapidjson::kArrayType);
		for (int j = 0; j < (int)vCodecList[i].size(); j++) {
			a.PushBack(rapidjson::Value().SetString(rapidjson::StringRef(vCodecList[i][j].c_str())), alloc);
		}
		d["vCodecs"].PushBack(a, alloc);
	}

	for (int i {0}; i < (int)aCodecList.size(); i++) {
		rapidjson::Value a(rapidjson::kArrayType);
		for (int j = 0; j < (int)aCodecList[i].size(); j++) {
			a.PushBack(rapidjson::Value().SetString(rapidjson::StringRef(aCodecList[i][j].c_str())), alloc);
		}
		d["aCodecs"].PushBack(a, alloc);
	}

	for (int i {0}; i < (int)containerList.size(); i++) {
		d["containers"].PushBack(rapidjson::Value().SetString(rapidjson::StringRef(containerList[i].c_str())), alloc);
	}

	FILE *fp {fopen(parsePath(CONFIG_FILE).c_str(), "wb")};
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);
	if (_eventHandler != NULL) {
		_eventHandler->newEvent(CONFIG_SAVED, "Saved Config");
	}
}

void Settings::createDefaultConfig() {
	presetName	= parsePresetName(DEFAULT_PRESET);
	presetPath	= DEFAULT_PRESET;
	libraryDir	= DEFAULT_LIBRARY_DIR;
	tempDir		  = DEFAULT_TEMP_DIR;
	outputDir	 = DEFAULT_OUTPUT_DIR;
	preserveLog   = DEFAULT_PRESERVE_LOG;
	vCodecList	= DEFAULT_VCODECS;
	aCodecList	= DEFAULT_ACODECS;
	containerList = DEFAULT_CONTAINERS;
	saveConfig();
}

void Settings::savePreset() {
	savePresetAs(presetName);
}

void Settings::savePresetAs(std::string name) {
	std::string json {"{}"};
	rapidjson::StringStream s(json.c_str());

	rapidjson::Document d;
	rapidjson::Document::AllocatorType &alloc {d.GetAllocator()};
	d.ParseStream(s);

	d.AddMember(rapidjson::StringRef("vCodec"), rapidjson::Value(rapidjson::StringRef(vCodec.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("vQuality"), rapidjson::Value(rapidjson::StringRef(vQuality.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("aCodec"), rapidjson::Value(rapidjson::StringRef(aCodec.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("aQuality"), rapidjson::Value(rapidjson::StringRef(aQuality.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("container"), rapidjson::Value(rapidjson::StringRef(container.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("subtitles"), rapidjson::Value(rapidjson::StringRef(subtitles.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("threads"), rapidjson::Value(rapidjson::StringRef(threads.c_str())), alloc);
	d.AddMember(rapidjson::StringRef("extraParams"), rapidjson::Value(rapidjson::StringRef(extraParams.c_str())), alloc);

	FILE *fp {fopen(parsePresetPath(name).c_str(), "wb")};
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);

	presetPath = parsePresetPath(name);
	presetName = parsePresetName(name);

	refreshPresets();
	if (_eventHandler != NULL) {
		_eventHandler->newEvent(PRESET_SAVED, "Saved Preset: " + presetName);
	}
}

void Settings::createDefaultPreset() {
	vCodec		= DEFAULT_VCODEC;
	vQuality	= DEFAULT_VQUALITY;
	aCodec		= DEFAULT_ACODEC;
	aQuality	= DEFAULT_AQUALITY;
	container   = DEFAULT_CONTAINER;
	subtitles   = DEFAULT_SUBTITLES;
	threads		= DEFAULT_THREADS;
	extraParams = DEFAULT_EXTRA_PARAMS;
	savePresetAs(DEFAULT_PRESET);
}

#define END_SAVE_SECTION }

void Settings::parseOverrideParams(std::string params) {
	boost::iostreams::array_source source(params.c_str(), std::strlen(params.c_str()));
	boost::iostreams::stream<boost::iostreams::array_source> iss(source);
	std::string s;

	overrideParams.clear();
	while (iss >> boost::io::quoted(s)) {
		overrideParams += ba::trim_copy(s).c_str();
	}
}

} // namespace SuperEpicFuntime::MediaPreparer
