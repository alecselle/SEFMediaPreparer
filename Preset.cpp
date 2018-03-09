#include "Preset.h"
#include <Config.h>

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace bf = boost::filesystem;
using namespace nlohmann;
using namespace std;

Preset::Preset() { load(); }

Preset::Preset(bf::path preset) { load(preset); }

Preset::Preset(string preset) { save(preset); }

void Preset::createDefault() {
	json j = loadDefault();

	ofstream newPreset(config->baseDir.string() + "\\SEF Standard.preset");
	newPreset << setw(4) << j;
	newPreset.close();
}

json Preset::loadDefault() {
	json j;
	j["vCodec"] = DEFAULT_VCODEC;
	j["aCodec"] = DEFAULT_ACODEC;
	j["vQuality"] = DEFAULT_VQUALITY;
	j["aQuality"] = DEFAULT_AQUALITY;
	j["container"] = DEFAULT_CONTAINER;
	j["subtitles"] = DEFAULT_SUBTITLES;
	j["threads"] = DEFAULT_THREADS;
	j["extraParams"] = DEFAULT_EXTRA_PARAMS;

	return j;
}

void Preset::load() {
	json j;

	if (!bf::exists(config->preset)) {
		createDefault();
		if (!config->preset.string().compare(config->baseDir.string() + "\\SEF Standard.preset"))
			bf::copy_file(config->baseDir.string() + "\\SEF Standard.preset", config->preset);
	}
	if (bf::exists(config->preset)) {
		FILE *fp = fopen(bf::canonical(config->preset).string().c_str(), "r");
		j = json::parse(ifstream(fp));
		path = config->preset;
		name = config->preset.filename().replace_extension().string();
		fclose(fp);
	} else {
		j = loadDefault();
		path = config->baseDir.string() + "\\SEF Standard.preset";
		name = "SEF Standard";
	}

	load(j);
}

void Preset::load(boost::filesystem::path preset) {
	if (bf::exists(preset)) {

		FILE *fp = fopen(preset.string().c_str(), "r");
		json j = json::parse(ifstream(fp));
		path = preset;
		name = preset.filename().replace_extension().string();

		load(j);

		fclose(fp);

	} else {
		load();
	}
}

void Preset::load(json j) {
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

void Preset::save(string preset) {
	json j;
	j["vCodec"] = vCodec;
	j["aCodec"] = aCodec;
	j["vQuality"] = vQuality;
	j["aQuality"] = aQuality;
	j["container"] = container;
	j["subtitles"] = subtitles;
	j["threads"] = threads;
	j["extraParams"] = extraParams;

	string newPath = config->baseDir.string() + "\\" + preset + ".preset";

	ofstream newPreset(newPath);
	newPreset << setw(4) << j;
	newPreset.close();
	path = newPath.c_str();
	name = "preset";
}
