#ifndef RANKING_H_
#define RANKING_H_

#include "structure.h"
#include "similarity.h"
#include <string>
#include <unordered_map>
using namespace std;

class ranking {
    public:
        vector<Ranking> get_prefix_ranking(string candidates, vector<string> dict, int method);
        vector<Ranking> get_suffix_ranking(string candidates, vector<string> dict, int method);

    private:
        static bool descending (const Ranking &a, const Ranking &b);
        static double calc_sim(string word, string candidate, int method);
};

#endif