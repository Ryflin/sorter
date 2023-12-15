#include <limits.h>
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
namespace fs = std::filesystem;

using namespace std;

int main(int argc, char *argv[]) {
    char hostyNamey[256];
    getlogin_r(hostyNamey, sizeof(hostyNamey));
    cout << hostyNamey << endl;
    string hostName = string(hostyNamey);
    string path = "/home/" + hostName + "/code/test";
    vector<string> filesInDownloads;
    for (const auto & entry : fs::directory_iterator(path))
        filesInDownloads.push_back(entry.path().filename().string());
    for (const auto & file : filesInDownloads) {
        cout << file << endl;
    }
    cout << "Now let us try writing to a file" << endl;
    string contents;
    return 0;
}

// write to a file using ofstream
void writeToFile(const string & fileName, const string & data) {
    ofstream file;
    file.open(fileName);
    file << data;
    file.close();
}