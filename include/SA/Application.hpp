#pragma once

#include <future>
#include <sstream>
#include <filesystem>

#include <SA/sa.h>
#include <SA/Timer.hpp>
#include <SA/lineCounter.hpp>
#include <SA/wordCounter.hpp>
#include <SA/Statistics.hpp>

namespace fs = std::filesystem;

struct Settings {
    bool onlyTxtfiles = true;
    bool merge_uppercase = false;
    bool countWords = false;
    bool printDebug = false;
    bool useThreads = true;
};

class Application {
SA_Private:
    std::vector<std::string> filesContent;
    std::vector<fs::path> foundFiles;
    fs::directory_entry dir;
    fs::path searchPath;

    Statistics stats;
    Settings settings;

    bool finishedJob = false;

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
                                std::cout << "found file! : " << d.path() << '\n';
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
            } else {
                std::cout << "Provided file path is invalid. Exiting..." << '\n';
                return EXIT_FAILURE;
            }
        } catch (const std::system_error& e) {
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

    void singleCountLines() {
        for (auto& f : filesContent) {
            lineCounter l;
            l.countLines(f);

            stats.addemptyLineCount(l.getEmptyLineCount());
            stats.addnonemptyLineCount(l.getnonemptyLineCount());
            stats.addTotalLineCount(l.getTotalLineCount());
        }

        finishedJob = true;
    }

    std::mutex linesMutex;
    std::vector<std::future<void>> futures;

    void job(std::string s) {
        lineCounter l;
        l.countLines(s);

        std::lock_guard<std::mutex> lock(linesMutex);

        stats.addemptyLineCount(l.getEmptyLineCount());
        stats.addnonemptyLineCount(l.getnonemptyLineCount());
        stats.addTotalLineCount(l.getTotalLineCount());
    }

    void threadCountLines() {

        for (auto& f : filesContent) {
            futures.push_back(std::async(std::launch::async, &Application::job, this, f));
        }

        finishedJob = true;

    }
public:
    bool isFinished() {
        return finishedJob;
    }

    void deleteLoadedData() {
        filesContent.clear();
        foundFiles.clear();
    }

    Application() {
    }

    /// @brief get refrence to Statistics object
    /// @return 
    Statistics& getStats() {
        return stats;
    }

    /// @brief start application with default search_path (current binary dir) and perform chosen operations 
    /// @param customize search path by changing path parameter
    /// @return 
    void start(fs::path path = "") {
        if (path.empty()) {
            searchPath = fs::current_path();
        } else {
            searchPath = path;
        }

        scanForFiles();
        loadAllFileContents();
        stats.setfileCount(foundFiles.size());

        lines();
    }

    /// @brief set sustom settings
    /// @param refrence to Settings struct
    /// @return 
    void setSettings(Settings& settings) {
        this->settings = settings;
    }

    /// @brief count lines in all loaded files
    void lines() {
        if (settings.useThreads) {
            threadCountLines();
        } else {
            singleCountLines();
        }
    }
};