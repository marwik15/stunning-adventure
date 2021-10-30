#include <string>
#include <iostream>

#include <SA/Application.hpp>


void startNormal() {
    Application app;

    app.start("benchF");
    std::cout << app.getStats();
}

void benchmarch(bool useThreads) {
    Settings s;
    s.useThreads = useThreads;

    Application app;
    app.setSettings(s);

    Timer timer;

    app.start("benchF");
    std::cout << app.getStats();
    app.deleteLoadedData();
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