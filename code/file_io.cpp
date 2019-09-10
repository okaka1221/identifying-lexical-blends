#include "header/file_io.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

vector<string> file_io::read(string path) {
    vector<string> vec;
    ifstream fin(path);

    if (fin.fail()) {
        cout << "File Open Error" << endl;
    }

    cout << "Loading..." << endl;

    string line;
    while (getline(fin, line)){
        vec.push_back(line);
    }
    
    fin.close();
    cout << "Finish loading from " + path << endl;

    return vec;
}

vector<string> file_io::filter_read(string path, string filter_path) {
    vector<string> vec;
    vector<string> filter;
    ifstream fin(path);
    ifstream filter_in(filter_path);

    vector<string> repeat_1;
    vector<string> repeat_2;
    string prev;

    if (fin.fail() || filter_in.fail()) {
        cout << "File Open Error" << endl;
    }

    cout << "Filtering..." << endl;

    string line;
    while (getline(filter_in, line)){
        filter.push_back(line);
    }

    filter_in.close();

    int i = 0;
    while (getline(fin, line)){
        i++;

        // remove if length of word is < 4;
        if (line.length() < 6) { 
            continue;
        }


        // remove if word has less than 3 different chars
        unordered_set<char> set;
        for (char c : line) {
            set.insert(c);
        }
        if (set.size() < 4) { 
            continue;
        }

        // remove if previous word is same after addint 's' at the end
        if (line[line.length() - 1] ==  's' && line == prev + "s") {
            continue;
        }
        prev = line;

        // remove if word has more than 3 repeated chars in a row
        string processed_1 = line.substr(0, 1);
        int count_1 = 0;
        int max_1 = 0;
        char prev_1 = line[0];
        for (int i = 1; i < line.length(); i++) {
            if (line[i] == line[i - 1]) {
                count_1++;
            } else {
                processed_1 += line[i];
                count_1 = 0;
            }
            max_1 = max(max_1, count_1);
        }

        if (max_1 > 1) { 
            continue;
        }

        // remove if new string without repeated chars is found in filer file
        auto itr1 = find(filter.begin(), filter.end(), processed_1);
        if (itr1 != filter.end()) { 
            continue;
        }


        // remove if word has more than 3 repeated sequences of string in a row such as ahahaha
        string processed_2 = line.substr(0, 2);
        int count_2 = 0;
        int max_2 = 0;
        string prev_2 = line.substr(0, 2);

        string last;
        if (line.length() % 2) {
            last = line.substr(line.length() - 1, 1);
        }

        for (int i = 2; i < line.length() - 1; i ++) {
            if (line[i] != line[i - 2] || line[i + 1] != line[i - 1]) {
                processed_2 += line.substr(i, 2);
            } else {
                count_2++;
                prev_2 = line.substr(i, 2);
                i++;
            }
            max_2 = max(max_2, count_2);
        }

        if (max_2 > 1) { 
            continue;
        }

        processed_2 += last;

        // remove if new string without repeated sequences is found in filer file
        auto itr2 = find(filter.begin(), filter.end(), processed_2);
        if (itr2 != filter.end()) { 
            continue;
        }


        // add 'g' at the end of word which ends with "in" and remove if new string is found in filter file
        string add_g = line;
        if (line[line.length() - 2] == 'i' && line[line.length() - 1] == 'n') {
            add_g = line + 'g';
        }

        auto itr3 = find(filter.begin(), filter.end(), add_g);
        if (itr3 != filter.end()) { 
            continue;
        }

        vec.push_back(line);
    }
    
    fin.close();

    cout << "Finished" << endl;
    cout << "Before filtering: " << i << " words" << endl;

    return vec;
}

// read blends.txt
vector<vector<string> > file_io::read_blends(string path) {
    vector<vector<string> > vec;
    ifstream fin(path);

    if (fin.fail()) {
        cout << "File Open Error" << endl;
    }

    cout << "Loading..." << endl;
    
    string line;
    while (getline(fin, line)){
        vector<string> blend;
        stringstream ss(line);
        string item;

        while (getline(ss, item, '\t') && !item.empty()) {
            blend.push_back(item);
        }

        vec.push_back(blend);
    }
    
    fin.close();
    cout << "Finish loading from " + path << endl;

    return vec;
}