#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <SA/Statistics.hpp>
#include <SA/lineCounter.hpp>
#include <SA/wordCounter.hpp>

namespace fs = std::filesystem;

struct Settings {
    bool onlyTxtfiles = true;
    bool merge_uppercase = false;
    bool countWords = false;
    bool printDebug = true;
};

class Application : public lineCounter, public wordCounter{
SA_Private:
    std::vector<std::string> filesContent;
    std::vector<fs::path> foundFiles;
    fs::directory_entry dir;
    fs::path searchPath;

    Statistics stats;
    Settings settings;

    /// @brief save all found file's path in provided path
    /// @return 
    int scanForFiles() {
        dir.assign(searchPath);
        try {
            if (dir.is_directory()) {
              
                //scan for files
                for (auto& d : fs::recursive_directory_iterator(dir)) {

                    //save path only to .txt files
                    if (settings.onlyTxtfiles) {
                        if (d.path().extension() == ".txt") {

                            foundFiles.push_back(d.path());

                            if (settings.printDebug) {
                                std::cout <<"found file! : "<< d.path() << '\n';
                            }
                        }
                    }
                    //save path to all files
                    else {
                        foundFiles.push_back(d.path());

                        if (settings.printDebug) {
                            std::cout << "found file! : " << d.path() << '\n';
                        }
                    }
                }
            }
            else {
                std::cout << "Provided file path is invalid. Exiting..." << '\n';
                return EXIT_FAILURE;
            }
        }
        catch (const std::system_error& e) {
            std::cerr << "Caught system_error with code " << e.code()
                << " meaning " << e.what() << '\n';
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
    /// @brief load all found files 
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
    Application(){
    }
    
    /// @brief get refrence to Statistics object
    /// @return 
    Statistics& getStats() {
        return stats;
    }

    /// @brief start application with default search_path (current binary dir) and perform chosen operations 
    /// @param customize search path by changing path parameter
    /// @return 
    void start(fs::path path ="") {
        if (path.empty()) {
            searchPath = fs::current_path();
        }
        else {
            searchPath = path;
        }

        scanForFiles();
        loadAllFileContents();
        lines();
        updateStats();
    }
    
    /// @brief set sustom settings
    /// @param refrence to Settings struct
    /// @return 
    void setSettings(Settings& settings) {
        this->settings = settings;
    }

    /// @brief cout lines in all loaded files
    void lines() {
        for (auto& f : filesContent) {
            countLines(f);
        }
    }
    
    /// @brief update statists 
    void updateStats() {
        stats.setfileCount(foundFiles.size());
        stats.setemptyLineCount(getEmptyLineCount());
        stats.setnonemptyLineCount(getnonemptyLineCount());
        stats.setTotalLineCount(getTotalLineCount());
    }
};

int main(){
    //prevent generating exception by non ascii characters
    std::setlocale(LC_ALL, "en_US.UTF-8");

    Application app;
    app.start();
    std::cout << app.getStats();
 
    return 0;
}