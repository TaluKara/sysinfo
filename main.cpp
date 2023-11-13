// include section
#include <iostream>
#include <fstream>
#include <array>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>
//include section end

//namespace section
using namespace std;
//namespace section end

//variables section
const string filePath = "sysinfo.txt";
//variables section end

// other functions section


// helper functions section
string execCommand(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// functions for gathering system info section

string getCpuInfo() {
    return execCommand("lscpu");
}

string getGpuInfo() {
    return execCommand("lspci | grep VGA");
}

string getMemoryInfo() {
    return execCommand("free -h");
}

string getStorageInfo() {
    return execCommand("df -h");
}

string getLinuxDistro() {
    return execCommand("cat /etc/os-release");
}

string getDesktopEnvironment() {
    const char* de = getenv("XDG_CURRENT_DESKTOP");
    return de ? string(de) : "Unknown";
}

string getWindowManager() {
    return execCommand("echo $XDG_SESSION_DESKTOP");
}

// file operations section
void writeSysInfoToFile(const string& filePath) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Unable to open file: " + filePath);
    }

    outFile << "CPU Information:\n" << getCpuInfo() << "\n";
    outFile << "GPU Information:\n" << getGpuInfo() << "\n";
    outFile << "Memory Information:\n" << getMemoryInfo() << "\n";
    outFile << "Storage Information:\n" << getStorageInfo() << "\n";
    outFile << "Linux Distribution:\n" << getLinuxDistro() << "\n";
    outFile << "Desktop Environment:\n" << getDesktopEnvironment() << "\n";
    outFile << "Window Manager:\n" << getWindowManager() << "\n";

    outFile.close();
}

void readSysInfoFromFile(const string& filePath) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        throw runtime_error("Unable to open file: " + filePath);
    }

    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }

    inFile.close();
}

// main function section
int main() {
    try {
        writeSysInfoToFile(filePath);
        readSysInfoFromFile(filePath);
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

//main function section end



