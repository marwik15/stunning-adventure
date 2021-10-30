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
    bool merge_uppercase = false; // to do
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

    /// @brief wait for async job to complete
    /// @param future futures vector to watch for
    void waitForAsync(std::vector<std::future<void>>& future) {
        for (auto& f : future) {
            f.wait();
        }
    }

    std::mutex contentMutex;
    std::vector<std::future<void>> futuresContent;

    /// @brief (job) load file's content
    /// @param r path to file
    void getContentJob(fs::path r) {
        std::ifstream t(r);
        std::stringstream buffer;
        buffer << t.rdbuf();
        t.close();
        std::lock_guard<std::mutex> lock(contentMutex);
        filesContent.push_back(buffer.str());
    }

    /// @brief load file's content in single thread
    void getContentSingle() {
        for (auto& r : foundFiles) {
            getContentJob(r);
        }
    }

    /// @brief load file's content in async
    void getContentAsync() {
        for (auto& f : foundFiles) {
            futuresContent.push_back(std::async(std::launch::async, &Application::getContentJob, this, f));
        }
    }

    /// @brief load all found files 
    void loadAllFileContents() {
        filesContent.reserve(foundFiles.size());

        if (settings.useThreads) {
            getContentAsync();
        } else {
            getContentSingle();
        }
    }

    std::mutex linesMutex;
    std::vector<std::future<void>> futuresLineCount;

    /// @brief count lines job
    /// @param s buffer with lines to count
    void countLinesJob(std::string s) {
        lineCounter l;
        l.countLines(s);

        std::lock_guard<std::mutex> lock(linesMutex);

        stats.addemptyLineCount(l.getEmptyLineCount());
        stats.addnonemptyLineCount(l.getnonemptyLineCount());
        stats.addTotalLineCount(l.getTotalLineCount());
    }

    /// @brief count lines in single thread
    void countLinesSingle() {
        for (auto& f : filesContent) {
            countLinesJob(f);
        }
    }

    /// @brief count lines in async
    void countLinesAsync() {
        for (auto& f : filesContent) {
            futuresLineCount.push_back(std::async(std::launch::async, &Application::countLinesJob, this, f));
        }
    }
 
public:

    void deleteLoadedData() {
        filesContent.clear();
        foundFiles.clear();
        futuresLineCount.clear();
        futuresContent.clear();
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
        waitForAsync(futuresContent);

        stats.setfileCount(foundFiles.size());

        lines();
        waitForAsync(futuresLineCount);
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
           countLinesAsync();
        } else {
            countLinesSingle();
        }
    }
};