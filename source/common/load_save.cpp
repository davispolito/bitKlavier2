/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "load_save.h"

#include "sound_engine.h"
#include "midi_manager.h"
#include "synth_base.h"

#define QUOTE(x) #x
#define STRINGIFY(x) QUOTE(x)

namespace {
  const std::string kLinuxUserDataDirectory = "~/.local/share/vital/";
  const std::string kAvailablePacksFile = "available_packs.json";
  const std::string kInstalledPacksFile = "packs.json";

  Time getBuildTime() {
    StringArray date_tokens;
    date_tokens.addTokens(STRINGIFY(BUILD_DATE), true);
    if (date_tokens.size() != 5)
      return Time::getCompilationDate();

    int year = date_tokens[0].getIntValue();
    int month = date_tokens[1].getIntValue() - 1;
    int day = date_tokens[2].getIntValue();
    int hour = date_tokens[3].getIntValue();
    int minute = date_tokens[4].getIntValue();

    return Time(year, month, day, hour, minute);
  }
} // namespace

const std::string LoadSave::kUserDirectoryName = "User";
const std::string LoadSave::kPresetFolderName = "Presets";
const std::string LoadSave::kSampleFolderName = "Samples";




//void LoadSave::loadSample(SynthBase* mainSynth, const json& json_sample) {
////  vital::Sample* sample = mainSynth->getSample();
////  if (sample)
////    sample->jsonToState(json_sample);
//}
//


//
//void LoadSave::initSaveInfo(std::map<std::string, String>& save_info) {
//  save_info["preset_name"] = "";
//  save_info["author"] = "";
//  save_info["comments"] = "";
//  save_info["style"] = "";
//
////  for (int i = 0; i < vital::kNumMacros; ++i)
////    save_info["macro" + std::to_string(i + 1)] = "MACRO " + std::to_string(i + 1);
//}


String LoadSave::getAuthorFromFile(const File& file) {
  static constexpr int kMaxCharacters = 40;
  static constexpr int kMinSize = 60;
  FileInputStream file_stream(file);

  if (file_stream.getTotalLength() < kMinSize)
    return "";

  file_stream.readByte();
  file_stream.readByte();
  MemoryBlock author_memory_block;
  file_stream.readIntoMemoryBlock(author_memory_block, 6);

  char end_quote = file_stream.readByte();
  char colon = file_stream.readByte();
  char begin_quote = file_stream.readByte();
  if (author_memory_block.toString() != "author" || end_quote != '"' || colon != ':' || begin_quote != '"') {
    try {
      json parsed_json_state = json::parse(file.loadFileAsString().toStdString(), nullptr, false);
      return getAuthor(parsed_json_state);
    }
    catch (const json::exception& e) {
      return "";
    }
  }

  MemoryBlock name_memory_block;
  file_stream.readIntoMemoryBlock(name_memory_block, kMaxCharacters);
  String name = name_memory_block.toString();

  if (!name.contains("\""))
    return name.toStdString();

  StringArray tokens;
  tokens.addTokens(name, "\"", "");

  return tokens[0];
}

String LoadSave::getStyleFromFile(const File& file) {
  static constexpr int kMinSize = 5000;
  FileInputStream file_stream(file);

  if (file_stream.getTotalLength() < kMinSize)
    return "";

  MemoryBlock style_memory_block;
  file_stream.readIntoMemoryBlock(style_memory_block, kMinSize);

  StringArray tokens;
  tokens.addTokens(style_memory_block.toString(), "\"", "");
  bool found_style = false;
  for (String token : tokens) {
    if (found_style && token.trim() != ":")
      return token;
    if (token == "preset_style")
      found_style = true;
  }

  return "";
}



void LoadSave::writeCrashLog(String crash_log) {
  File data_dir = getDataDirectory();
  if (!data_dir.exists() || !data_dir.isDirectory())
    return;
  File file = data_dir.getChildFile("crash.txt");
  file.replaceWithText(crash_log);
}

void LoadSave::writeErrorLog(String error_log) {
  File data_dir = getDataDirectory();
  if (!data_dir.exists() || !data_dir.isDirectory())
    return; 

  File file = getDataDirectory().getChildFile("errors.txt");
  file.appendText(error_log + "\n");
}

File LoadSave::getFavoritesFile() {
#if defined(JUCE_DATA_STRUCTURES_H_INCLUDED)
  PropertiesFile::Options config_options;
  config_options.applicationName = "Vial";
  config_options.osxLibrarySubFolder = "Application Support";
  config_options.filenameSuffix = "favorites";

#ifdef LINUX
  config_options.folderName = "." + String(ProjectInfo::projectName).toLowerCase();
#else
  config_options.folderName = String(ProjectInfo::projectName).toLowerCase();
#endif

  return config_options.getDefaultFile();
#else
  return File();
#endif
}

File LoadSave::getDefaultSkin() {
#if defined(JUCE_DATA_STRUCTURES_H_INCLUDED)
  PropertiesFile::Options config_options;
  config_options.applicationName = "Vial";
  config_options.osxLibrarySubFolder = "Application Support";
  config_options.filenameSuffix = "skin";

#ifdef LINUX
  config_options.folderName = "." + String(ProjectInfo::projectName).toLowerCase();
#else
  config_options.folderName = String(ProjectInfo::projectName).toLowerCase();
#endif

  return config_options.getDefaultFile();
#else
  return File();
#endif
}


//
//bool LoadSave::isInstalled() {
//  return getDataDirectory().exists();
////}
////
//
//std::string LoadSave::getAuthor() {
//  json data = getConfigJson();
//
//  if (data.count("author"))
//    return data["author"];
//
//  return "";
//}



std::vector<File> LoadSave::getPresetDirectories() {
  return getDirectories(kPresetFolderName);
}


std::vector<File> LoadSave::getSampleDirectories() {
  return getDirectories(kSampleFolderName);
}

File LoadSave::getUserDirectory() {
  File directory = getDataDirectory().getChildFile(kUserDirectoryName);
  if (!directory.exists())
    directory.createDirectory();
  return directory;
}

File LoadSave::getUserPresetDirectory() {
  File directory = getUserDirectory().getChildFile(kPresetFolderName);
  if (!directory.exists())
    directory.createDirectory();
  return directory;
}

File LoadSave::getUserSampleDirectory() {
  File directory = getUserDirectory().getChildFile(kSampleFolderName);
  if (!directory.exists())
    directory.createDirectory();
  return directory;
}

void LoadSave::getAllFilesOfTypeInDirectories(Array<File>& files, const String& extensions,
                                              const std::vector<File>& directories) {
  files.clear();
  for (const File& directory : directories) {
    if (directory.exists() && directory.isDirectory())
      directory.findChildFiles(files, File::findFiles, true, extensions);
  }
}
std::vector<std::string> LoadSave::getAdditionalFolders(const std::string& name) {
//    json data = getConfigJson();
//    std::vector<std::string> folders;
//
//    if (data.count(name)) {
//        json folder_list = data[name];
//        for (json& folder : folder_list)
//            folders.push_back(folder);
//    }
//
//    return folders;
}

File LoadSave::getDataDirectory() {
//    json data = getConfigJson();
//    if (data.count("data_directory")) {
//        std::string path = data["data_directory"];
//        File folder(path);
//        if (folder.exists() && folder.isDirectory())
//            return folder;
//    }
//
//#ifdef LINUX
//    File directory = File(kLinuxUserDataDirectory);
//  String xdg_data_home = SystemStats::getEnvironmentVariable ("XDG_DATA_HOME", {});
//
//  if (!xdg_data_home.trim().isEmpty())
//    directory = File(xdg_data_home).getChildFile("vial");
//
//#elif defined(__APPLE__)
//    File home_directory = File::getSpecialLocation(File::userHomeDirectory);
//    File directory = home_directory.getChildFile("Music").getChildFile("Vial");
//#else
//    File documents_dir = File::getSpecialLocation(File::userDocumentsDirectory);
//  File directory = documents_dir.getChildFile("Vial");
//#endif
//
//    return directory;
}

std::vector<File> LoadSave::getDirectories(const String& folder_name) {
    File data_dir = getDataDirectory();
    std::vector<File> directories;

    if (!data_dir.exists() || !data_dir.isDirectory())
        return directories;

    Array<File> sub_folders;
    sub_folders.add(data_dir);
    data_dir.findChildFiles(sub_folders, File::findDirectories, false);
    for (const File& sub_folder : sub_folders) {
        File directory = sub_folder.getChildFile(folder_name);
        if (directory.exists() && directory.isDirectory())
            directories.push_back(directory);
    }

    return directories;
}

void LoadSave::getAllSamples(Array<File>& samples) {
  getAllFilesOfTypeInDirectories(samples, "*.wav", getSampleDirectories());
}

void LoadSave::getAllUserPresets(Array<File>& presets) {
  std::vector<File> directories = {
    getDataDirectory().getChildFile(kPresetFolderName),
    getUserPresetDirectory()
  };
  getAllFilesOfTypeInDirectories(presets, String("*.") + bitklavier::kPresetExtension, directories);
}

void LoadSave::getAllUserSamples(Array<File>& samples) {
  std::vector<File> directories = {
    getDataDirectory().getChildFile(kSampleFolderName),
    getUserSampleDirectory()
  }; 
  getAllFilesOfTypeInDirectories(samples, "*.wav", directories);
}



File LoadSave::getShiftedFile(const String directory_name, const String& extensions,
                              const std::string& additional_folders_name, const File& current_file, int shift) {
  FileSorterAscending file_sorter;

  std::vector<File> directories = getDirectories(directory_name);
  std::vector<std::string> additional = getAdditionalFolders(additional_folders_name);
  for (const std::string& path : additional)
    directories.push_back(File(path));

  Array<File> all_files;
  getAllFilesOfTypeInDirectories(all_files, extensions, directories);
  if (all_files.isEmpty())
    return File();

  all_files.sort(file_sorter);
  int index = all_files.indexOf(current_file);
  if (index < 0)
    return all_files[0];
  return all_files[(index + shift + all_files.size()) % all_files.size()];
}
