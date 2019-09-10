#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <vector>
#include <string>
using namespace std;

class file_io {
    public:
        vector<string> read(string path);
        vector<string> filter_read(string path, string filter_path);
        vector<vector<string> > read_blends(string path);
};

#endif