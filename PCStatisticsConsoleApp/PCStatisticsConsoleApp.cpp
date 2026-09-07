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

struct Spinner {
    int row;
    int col;
    int frame = 0;
    const char symbols[4] = { '|', '/', '-', '\\' };

    void update() {
        setCursor(row, col);
        std::cout << symbols[frame % 4] << std::flush;
        frame++;
    }
};

std::string getCurrentTime() {
    char strTime[26];
    time_t now = time(NULL);
    ctime_s(strTime, sizeof strTime, &now);

    std::string stringTime = strTime;

    if (!stringTime.empty() && stringTime.back() == '\n') {
        stringTime.pop_back();
    }


    return stringTime;
}

struct LiveField {
    int row; 
    int col;
    std::string lastValue = "";

    void update(const std::string& newValue) {
        if (newValue != lastValue) {
            setCursor(row, col);
            std::cout << newValue << " " << std::flush;
            lastValue = newValue;
        }
    }
};

int main()
{
    system("cls");

    std::cout << "\x1B[?25l"; 
    std::cout << "\x1B[2J\x1B[H"; 

    std::cout << "+-------------------------------------------+            +-----------------------------------------------------------+\n";
    std::cout << "|                  System             [ ]   |            |                        General Info                       |\n";
    std::cout << "+-------------------------------------------+            +-----------------------------------------------------------+\n";
    std::cout << "| RAM Usage(%)    [                         |            | Date & Time  [                          ]                 |\n";
    std::cout << "| Total Uptime    [                         |            | Example      [                                            |\n";
    std::cout << "| Example         [           ]             |            | Example      [                                            |\n";
    std::cout << "| Example         [           ]             |            | Example      [                                            |\n";
    std::cout << "+-------------------------------------------+            +-----------------------------------------------------------+\n";

    //System Fields
    LiveField ramField = { 4, 21 };
    LiveField runTimeField = { 5, 21 };
    LiveField exampleField2 = { 6, 21 };
    LiveField exampleField3 = { 7, 21 };
    Spinner mySpinner = { 2, 40 }; 

    //General Info Fields
    LiveField timeField = { 4, 75 };

    ram.dwLength = sizeof(MEMORYSTATUSEX);

    while (true) {
        GlobalMemoryStatusEx(&ram);
        mySpinner.update();
        ramField.update(std::to_string(ram.dwMemoryLoad) + "%" + " ]");
        runTimeField.update(getUptime());
        timeField.update(getCurrentTime());

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}


