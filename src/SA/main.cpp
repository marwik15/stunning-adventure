#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <SA/stats.hpp>
#include <SA/lineCounter.hpp>
#include <SA/wordCounter.hpp>

namespace fs = std::filesystem;

struct Settings {
    bool onlyTxtfiles = true;
    bool merge_uppercase = false;
    bool countWords = false;
};

class Application : public lineCounter, public wordCounter{
SA_Private:
    std::vector<std::string> filesContent;
    std::vector<fs::path> foundFiles;
    fs::directory_entry dir;
    bool onlyTxtfiles;

    Statistics stats;
    Settings& settings;

    int scanForFiles() {
        dir.assign(u8"C:/users/marwik15/desktop/bench");
        try {
            if (dir.is_directory()) {
                //scan for files
                for (auto& d : fs::recursive_directory_iterator(dir)) {

                    //save path only to .txt files
                    if (onlyTxtfiles) {
                        if (d.path().extension() == ".txt") {
                            foundFiles.push_back(d.path());
                            //std::cout << d.path() << '\n';
                        }
                    }
                    //save path to all files
                    else {
                        foundFiles.push_back(d.path());
                        //std::cout << d.path() << '\n';

                    }
                }
            }
            else {
                std::cout << "Provided file path is invalid. Exiting..." << '\n';
                return -1;
            }
        }
        catch (const std::system_error& e) {
            std::cerr << "Caught system_error with code " << e.code()
                << " meaning " << e.what() << '\n';
        }
    }

    void loadAllFileContents() {
        filesContent.reserve(foundFiles.size());
        for (auto& r : foundFiles) {
            std::ifstream t(r);
            std::stringstream buffer;
            buffer << t.rdbuf();
            filesContent.push_back(buffer.str());
            t.close();
        }
    }
public:
    Application() : settings(Settings()){
    }

    void start() {
        scanForFiles();
        loadAllFileContents();
        lines();
        updateStats();

        std::cout << stats;
    }

    void setSettings(Settings& settings) {
        this->settings = settings;
    }

    void lines() {
        for (auto& f : filesContent) {
            countLines(f);
        }
    }

    void updateStats() {
        stats.setfileCount(foundFiles.size());
        stats.setemptyLineCount(getEmptyLineCount());
        stats.setnonemptyLineCount(getnonemptyLineCount());
    }

};

int main(){
    Settings Settings;
    //prevent generating exception by non ascii characters
    std::setlocale(LC_ALL, "en_US.UTF-8");

    Application app;
    app.start();
    return 0;
}