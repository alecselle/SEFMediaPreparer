#include "src/Preset.hpp"

namespace bf = boost::filesystem;
namespace bc = boost::container;
namespace ba = boost::algorithm;
using namespace rapidjson;
using namespace std;

namespace SuperEpicFuntime {

/** ================================================================================================
 * (Class) Preset
 */
Preset::Preset() {
	load(config->DEFAULT_PRESET);
}

Preset::Preset(string path) {
	load(path);
}

void Preset::load() {
	load(presetPath);
}

void Preset::load(std::string path) {
	bf::path p(config->parsePath(path).c_str());
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
		presetPath = config->parsePresetPath(path);
		presetName = config->parsePresetName(path);
	} else {
		if (!bf::exists(config->DEFAULT_PRESET)) {
			createDefault();
		}
		if (bf::exists(config->DEFAULT_PRESET)) {
			load(config->DEFAULT_PRESET);
		}
	}
}

void Preset::save() {
	save(presetName);
}

void Preset::save(std::string name) {
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

	FILE *fp = fopen(config->parsePresetPath(name).c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);

	presetPath = config->parsePresetPath(name);
	presetName = config->parsePresetName(name);
}

void Preset::createDefault() {
	vCodec = DEFAULT_VCODEC;
	vQuality = DEFAULT_VQUALITY;
	aCodec = DEFAULT_ACODEC;
	aQuality = DEFAULT_AQUALITY;
	container = DEFAULT_CONTAINER;
	subtitles = DEFAULT_SUBTITLES;
	threads = DEFAULT_THREADS;
	extraParams = DEFAULT_EXTRA_PARAMS;
	save(config->DEFAULT_PRESET);
}
} // namespace SuperEpicFuntime
