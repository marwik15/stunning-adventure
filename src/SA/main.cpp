#include <string>
#include <iostream>

#include <SA/Application.hpp>


void generateTestData(int fileCount,int emptylineCount,int nonemptyLineCount, std::string Path = "") {
   
    if (!fs::exists(Path))
        fs::create_directories(Path);

    std::string path;
    for (int i = 0; i < fileCount; i++) {
        path = Path + "/testfile" + std::to_string(i) + ".txt";

        std::ofstream ofs(path);
        
        if (ofs.is_open()) {

            for (int e = 0; e < nonemptyLineCount; e++) {
                ofs << "non empty\n";
            }

            for (int n = 0; n < emptylineCount; n++) {
                ofs << "\n";
            }
         
            ofs.close();
        } else {
            std::cout << "Error opening file\n";
        }
    }
}

void startNormal() {
    Application app;

    app.start("bench");
    std::cout << app.getStats();
}

void benchmarch(bool useThreads) {
    std::string benchmarkCachePath = "benchmarkFiles/";
    generateTestData(100, 1032, 1045, benchmarkCachePath);

    Settings s;
    s.useThreads = useThreads;

    Application app;
    app.setSettings(s);

    Timer timer;

    app.start(benchmarkCachePath);
    std::cout << app.getStats();
    app.deleteLoadedData();

    int n = std::filesystem::remove_all(benchmarkCachePath);
}

void startBenchmark(int loops){

    for (int i = 0; i < loops; i++) {
        std::cout << "---\nbenchmarking without async : \n";
        benchmarch(false);
        std::cout << "---\nbenchmarking with async : \n";
        benchmarch(true);
    }
}

int main(){
    bool useBenchmark = false;

    //prevent generating exception by non ascii characters
    std::setlocale(LC_ALL, "en_US.UTF-8");

    if (useBenchmark) {
        startBenchmark(4);
    } else {
        startNormal();
    }
    
    return 0;
}