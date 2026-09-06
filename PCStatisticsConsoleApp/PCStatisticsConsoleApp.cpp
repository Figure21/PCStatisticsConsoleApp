// PCStatisticsConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>

MEMORYSTATUSEX ram;





void setCursor(int row, int col) {
    std::cout << "\x1B[" << row << ";" << col << "H";
}

std::string getUptime() {
    uint64_t totalSeconds = GetTickCount64() / 1000;

    uint64_t hours = totalSeconds / 3600;
    uint64_t minutes = (totalSeconds % 3600) / 60;
    uint64_t seconds = totalSeconds % 60;

    std::string h = (hours < 10 ? "0" : "") + std::to_string(hours);
    std::string m = (minutes < 10 ? "0" : "") + std::to_string(minutes);
    std::string s = (seconds < 10 ? "0" : "") + std::to_string(seconds);

    return h + "h " + m + "m " + s + "s" + " ]";
}

struct LiveField {
    int row; 
    int col;
    std::string lastValue = "";

    void update(const std::string& newValue) {
        if (newValue != lastValue) {
            setCursor(row, col);
            std::cout << newValue << "   " << std::flush;
            lastValue = newValue;
        }
    }
};

int main()
{
    system("cls");

    std::cout << "\x1B[?25l"; 
    std::cout << "\x1B[2J\x1B[H"; 

    std::cout << "+-------------------------------------------+\n";
    std::cout << "|                  System                   |\n";
    std::cout << "+-------------------------------------------+\n";
    std::cout << "| RAM Usage(%)    [                         |\n";
    std::cout << "| Total Uptime    [           ]             |\n";
    std::cout << "| Example         [           ]             |\n";
    std::cout << "| Example         [           ]             |\n";
    std::cout << "+-------------------------------------------+\n";

    LiveField ramField = { 4, 21 };
    LiveField runTimeField = { 5, 21 };
    LiveField exampleField2 = { 6, 21 };
    LiveField exampleField3 = { 7, 21 };
    ram.dwLength = sizeof(MEMORYSTATUSEX);
    

    while (true) {
        GlobalMemoryStatusEx(&ram);
        //std::string currentRamTest = ram.dwMemoryLoad << "%\n";

        ramField.update(std::to_string(ram.dwMemoryLoad) + "%" + " ]");
        runTimeField.update(getUptime());
        

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }


}


