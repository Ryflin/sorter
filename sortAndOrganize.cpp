#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

void github_repo_update() {
    system("git pull");
    system("git push");
}
map<string, string> file_and_path() {
    map<string, string> file_and_path;
    ifstream in  = ifstream("file_and_path.txt");
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string file;
            string path;
            getline(ss, file, ':');
            getline(ss, path);
            file_and_path[file] = path;
        }
    }
    return file_and_path;
    
}
/**
 * Returns the file extension of a given file name.
 *
 * @param fileNames A vector of file names
 * @return A vector of file extensions
 */
vector<string> get_extension(vector<string> fileNames) {
    vector<string> temp;
    for (int i = 0; i < fileNames.size(); i++) {
        auto pos = fileNames[i].find_last_of(".");
        if (pos != string::npos) {
            temp.push_back(fileNames[i].substr(pos + 1));
        }
    }
    return temp;
}

int main(int argc, char *argv[]) {
    // update code folder with gh repo 
    // this includes autogenerating repos as necessary
    github_repo_update();
    map<string, string> extens = file_and_path();

    for (auto pair : extens) {
        pair.second;
        pair.first;
    }
    vector <string> filesInDownloads;
    string path = "/home/sam/code/test";
    for (const auto & entry : fs::directory_iterator(path))
        filesInDownloads.push_back(entry.path().filename().string());

    fs::path changeDirectory = path;
    fs::current_path(changeDirectory);
    vector<string> fileExt = get_extension(filesInDownloads);
    cout << fs::current_path() << endl;

    for (int i = 0; i < fileExt.size(); i++) {
        if (fileExt[i] == "deb"){
            fs::remove(filesInDownloads[i]);
        } else if (fileExt[i] == "zip"){
            fs::rename(filesInDownloads[i], "/home/sam/")
    }
    // for (auto extensions : fileExtensions) {
    //     if (extensions == "") 
    //     cout << endl;
    // }
    // organize the downloads folder

    // installables get deleted

    // executables get moved to the desktop folder
    // jars get automoved to the documents folder

    // detect files that do not have a very descriptive name, and ask user if they want to change the name
    

    // for unrecognized file extensions ask where to put and remember to put in place every time
    // if designated place is not an existing folder create said path. 
    return 0;
}