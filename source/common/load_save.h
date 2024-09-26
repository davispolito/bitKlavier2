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

#pragma once

#include "JuceHeader.h"


#include <map>
#include <set>
#include <string>

using json = nlohmann::json;

namespace bitklavier {
    const std::string kPresetExtension = "bk2";
    const std::string kSampleExtensionsList = "*.wav;*.flac";
  class StringLayout;
}

class MidiManager;
class SynthBase;

class LoadSave {
  public:
    class FileSorterAscending {
      public:
        FileSorterAscending() { }

        static int compareElements(File a, File b) {
          return a.getFullPathName().toLowerCase().compareNatural(b.getFullPathName().toLowerCase());
        }

      private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileSorterAscending)
    };

    enum PresetStyle {
//      kBass,
//      kLead,
//      kKeys,
//      kPad,
//      kPercussion,
//      kSequence,
//      kExperimental,
//      kSfx,
      kTemplate,
      kNumPresetStyles
    };

    static const int kMaxCommentLength = 500;
    static const std::string kUserDirectoryName;
    static const std::string kPresetFolderName;
    static const std::string kWavetableFolderName;
    static const std::string kSkinFolderName;
    static const std::string kSampleFolderName;
    static const std::string kLfoFolderName;
    static const std::string kAdditionalWavetableFoldersName;
    static const std::string kAdditionalSampleFoldersName;

    static String getAuthorFromFile(const File& file);
    static String getStyleFromFile(const File& file);
    static std::string getAuthor(json file);
    static std::string getLicense(json state);

    static File getConfigFile();
    static void writeCrashLog(String crash_log);
    static void writeErrorLog(String error_log);
    static File getFavoritesFile();
    static File getDefaultSkin();
    static std::string getAuthor();
    static std::vector<std::string> getAdditionalFolders(const std::string& name);

    static File getDataDirectory();
    static std::vector<File> getDirectories(const String& folder_name);
    static std::vector<File> getPresetDirectories();
    static std::vector<File> getSampleDirectories();
    static File getUserDirectory();
    static File getUserPresetDirectory();
    static File getUserSampleDirectory();
    static void getAllFilesOfTypeInDirectories(Array<File>& files, const String& extensions,
                                               const std::vector<File>& directories);
    static void getAllSamples(Array<File>& samples);
    static void getAllUserPresets(Array<File>& presets);
    static void getAllUserSamples(Array<File>& samples);

    static File getShiftedFile(const String directory_name, const String& extensions,
                               const std::string& additional_folders_name, const File& current_file, int shift);

  private:
    LoadSave() { }
};

