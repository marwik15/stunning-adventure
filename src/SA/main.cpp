#include <string>
#include <iostream>

#include <SA/Application.hpp>


void startNormal() {
    Application app;

    app.start("bench");
    Timer timer;
    //counter runs in async mode
    while (1) {
        if (app.isFinished()) {
            std::cout << app.getStats();
            break;
        }
    }
}


void benchmarch(bool useThreads) {
    Settings s;
    s.useThreads = useThreads;

    Application app;
    app.setSettings(s);

    app.start("bench");
    Timer timer;
    //counter runs in async mode
    while (1) {
        if (app.isFinished()) {
            std::cout << app.getStats();
            break;
        }
    }
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
        startBenchmark(2);
    } else {
        startNormal();
    }
    
    return 0;
}