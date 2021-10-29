#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>

#include <SA/sa.h>

class Statistics {
private:
    int fileCount,
        emptyLineCount,
        nonemptyLineCount,
        totalLineCount;
  
public:

    void printStats() {
        printWordInfo();
        printFileInfo();
    }

    void setfileCount(int fileCount) {
        this->fileCount = fileCount;
    }
    void setemptyLineCount(int emptyLineCount) {
        this->emptyLineCount= emptyLineCount;
    }
    void setnonemptyLineCount(int emptyLineCount) {
        this->nonemptyLineCount = emptyLineCount;
    }
    void setTotalLineCount(int totalLineCount) {
        this->totalLineCount = totalLineCount;
    }


    int getfileCount() {
        return fileCount;
    }
    int getemptyLineCount() {
        return emptyLineCount;
    }
    int getnonemptyLineCount() {
        return nonemptyLineCount;
    }

    int getTotalLineCount() {
        return totalLineCount;
    }


    friend std::ostream& operator<<(std::ostream& os, const Statistics& s);

SA_Private:

    void printWordInfo() {
        //for (auto& l : wordCount) {
        //    for (auto& w : l) {
        //        std::cout << w.first << " " << w.second << "\n";
        //    }
        //}

    }

    void printFileInfo() {
        std::cout << "\nfiles : " << fileCount << '\n' <<
            "empty lines : " << emptyLineCount << '\n' <<
            "non empty lines : " << nonemptyLineCount << '\n';
    }

};

std::ostream& operator<<(std::ostream& os, const Statistics& s) {
    //for (auto& l : s.wordCount) {
    //    for (auto& w : l) {
    //        os << w.first << " " << w.second << "\n";
    //    }
    //}

    os << "files : " << s.fileCount << '\n' <<
        "empty lines : " << s.emptyLineCount << '\n' <<
        "all lines : " << s.totalLineCount << '\n' <<
        "non empty lines : " << s.nonemptyLineCount << '\n';

    return os;
}