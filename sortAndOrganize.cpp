#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unistd.h>
#include <limits.h>

namespace fs = std::filesystem;
using namespace std;

string get_hostname();
void github_repo_update();
map<string, string> file_and_path();
bool descriptive_enough(const string& s);
string input_from_user();
bool output_to_user(const string &output);
bool output_to_file(const string &output, const string &file);
vector<string> get_extension(vector<string> fileNames);

int main(int argc, char *argv[]) {
    // update code folder with gh repo 
    // this includes autogenerating repos as necessary
    github_repo_update();
    string programLocation = fs::current_path().string();
    map<string, string> extens = file_and_path();
    vector <string> filesInDownloads;
    string hostname = get_hostname();
    string path = hostname + "/Downloads";
    cout << path << endl;
    for (const auto & entry : fs::directory_iterator(path))
        filesInDownloads.push_back(entry.path().filename().string());
    vector<string> fileExt = get_extension(filesInDownloads);
    for (int i = 0; i < fileExt.size(); i++) {
        bool descriptiveEnough = false;
        if (filesInDownloads[i].find("Cert") != string::npos || filesInDownloads[i].find("cert")!= string::npos) {
            fs::rename(filesInDownloads[i],   + "/Documents/certs/" + filesInDownloads[i]);
        } else if (descriptive_enough(filesInDownloads[i])) {
            string new_file_name = filesInDownloads[i].substr(0, filesInDownloads[i].find_last_of("."));
            while (descriptive_enough(new_file_name) && descriptiveEnough == false){
                output_to_user("Your file name (" + filesInDownloads[i] + ") may not be descriptive enough. Would you like to rename it? (y/n): ");
                string input = input_from_user();
                if (input == "y" || input == "Y" || input == "\n") {
                    output_to_user("Please enter a new file name: ");
                    string new_file_name = input_from_user();
                    fs::rename(path + "/" + filesInDownloads[i], path + "/" + new_file_name);
                    filesInDownloads[i] = new_file_name + fileExt[i];
                } else {
                    break;
                }
            }
            filesInDownloads[i] = new_file_name + "."+ fileExt[i];
            descriptiveEnough = true;
        }
        if (fileExt[i] == "deb"){
            fs::remove(filesInDownloads[i]);
        }
        // moves all the files to the designated folder in the file_and_path.txt
        auto it = extens.find(fileExt[i]);
        if (it != extens.end() && filesInDownloads[i].length() > 0) {
            fs::rename(path + "/" + filesInDownloads[i], it->second + "/" + filesInDownloads[i]);
        } else {
            output_to_user("The file extension of " + fileExt[i] + " is not recognized. Please designate the default location for this file extension: ");
            string newPath = input_from_user();
            while (!fs::exists(newPath) ) {
                output_to_user("file path doest not exist. Would you like to create it? (y/n): ");
                if (input_from_user() == "y" || input_from_user() == "Y" || input_from_user() == "\n") {
                    try {
                        fs::create_directories(newPath);
                    }
                    catch (const fs::filesystem_error& e) {
                        output_to_user("Sorry can't seem to create the directory. Please designate a different path: ");
                    }
                }
            }
            if (filesInDownloads[i].length() > 0) {
                fs::rename(path + "/" + filesInDownloads[i], newPath + "/" + filesInDownloads[i]);
            }
            output_to_file(fileExt[i] + ":" + newPath, "file_and_path.txt");
            extens[fileExt[i]] = newPath;
        }
    }
    // organize the downloads folder

    // installables get deleted

    // executables get moved to the desktop folder
    // jars get automoved to the documents folder

    // detect files that do not have a very descriptive name, and ask user if they want to change the name
    

    // for unrecognized file extensions ask where to put and remember to put in place every time
    // if designated place is not an existing folder create said path. 
    return 0;
}

/**
 * Reads a file containing pairs of file names and paths from the given path, and returns a map of file names to paths.
 * 
 * The file should contain one line for each file, with the file name and path separated by a colon (:). The file name and path should be separated by a single space.
 * 
 * The file should be formatted as follows:
 * 
 * fileName1:path1
 * fileName2:path2
 * 
 * The function returns an empty map if the file cannot be opened or is empty.
 * 
 * @return A map of file names to paths, or an empty map if the file cannot be opened or is empty.
 */
map<string, string> file_and_path() {
    string path = "file_and_path.txt";
    map<string, string> file_and_path;
    ifstream in(path);
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string file = line.substr(0, pos);
                string path = line.substr(pos + 1);
                file_and_path[file] = path;
            }
        }
    }
    return file_and_path;
}

/**
 * Returns the hostname of the current user.
 *
 * This function uses the `getlogin_r` function to obtain the hostname of the current user.
 *
 * @return The hostname of the current user.
 */
std::string get_hostname() {
    char hostname[256];
    getlogin_r(hostname, sizeof(hostname));
    return "/home/" + string(hostname);
}

/**
 * Updates the code folder by pulling from the git repository and pushing to the remote repository.
 */
void github_repo_update() {
    system("git pull");
    system("git push");
}

/**
 * Returns whether a given string is descriptive enough.
 *
 * A string is considered descriptive if it is at least 10 characters long,
 * and contains at least one uppercase letter, one lowercase letter, and one digit.
 *
 * @param s The string to be evaluated.
 * @return Whether the string is descriptive enough.
 */
bool descriptive_enough(const string& s) {
    if (s.size() < 10) {
        return false;
    }
    int caps_count = 0, number_count = 0, letter_count = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z' || s[i] == '_') {
            caps_count++;
        } else if (s[i] >= 'a' && s[i] <= 'z') {
            letter_count++;
        } else if (s[i] >= '0' && s[i] <= '9') {
            number_count++;
        }
    }
    if (letter_count > 5) {
        return false;
    } else if (caps_count < letter_count / 10) {
        return false;
    }
    return true;
}

/**
 * Obtains input from the user.
*/
string input_from_user() {
    string input;
    getline(cin, input);
    return input;
}

/**
 * Outputs a text string to the user.
 * @param output the string to be displayed to the user.
*/
bool output_to_user(const string &output) {
    cout << output;
    return true;
}

/**
 * Writes a string to a file.
 *
 * @param output the string to be written to the file
 * @param file the name of the file to which the string should be written
 * @return true if the string was written to the file, false otherwise
 */
bool output_to_file(const string &output, const string &file) {
    ofstream out;
    out.open(file, std::ios_base::app);
    if (out.is_open()) {
        out << endl << output;
        out.close();
        return true;
    }
    return false;
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