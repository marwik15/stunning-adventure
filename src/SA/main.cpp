#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <SA/stats.hpp>
#include <SA/Lcounter.hpp>

namespace fs = std::filesystem;

int main(){

    Statistics stat;
   // std::cout<<stat;

   std::vector<fs::path> foundFiles;

    fs::directory_entry dir(u8"C:/users/marwik15/desktop/bench");

    //prevent generating exception by non ascii characters
    std::setlocale(LC_ALL, "en_US.UTF-8");

    bool onlyTxtfiles = true;

    try {
        if (dir.is_directory()) {
            //scan for files
            for (auto& d : fs::recursive_directory_iterator(dir)) {

                //save path only to .txt files
                if (onlyTxtfiles) {
                    if (d.path().extension() == ".txt") {
                        foundFiles.push_back(d.path());
                        std::cout << d.path() << '\n';
                    }
                }
                //save path to all files
                else {
                    foundFiles.push_back(d.path());
                    std::cout << d.path() << '\n';
                
                }
            }
        }
        else {
            std::cout <<"Provided file path is invalid. Exiting..." << '\n';
            return -1;
        }
    }
    catch (const std::system_error& e) {
        std::cerr << "Caught system_error with code " << e.code()
            << " meaning " << e.what() << '\n';
    }
   
    std::ifstream t(foundFiles[0]);
    std::stringstream buffer;
    buffer << t.rdbuf();
    t.close();

    Lcounter lineCounter;
    lineCounter.countLines(buffer);

    return 0;
}