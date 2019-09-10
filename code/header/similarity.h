#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <string>
using namespace std;

class similarity {
    public:
        int global_edit(string s1, string s2);
        int local_edit(string s1, string s2);
        int n_gram(string s1, string s2, int n);
        double jaro_winkler(string s1, string s2, double p);
};

#endif