#include "header/file_io.h"
#include "header/similarity.h"
#include "header/ranking.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

const string DICT = "../data/dict.txt";
const string CANDIDATES = "../data/candidates.txt";
const string BLENDS = "../data/blends.txt";
const string OUTPUT = "../data/output.txt";
const int THRESHOLD = 100;

/*
METHOD
0: local edit distance,
1: jaro winkler similarity,
2: global edit distance,
3: n-gram
*/
const string METHOD[4] = {"Local Edit Distance", "Jaro-Winkler Similarity", "Global Edit Distance", "2-Gram"};
const string METHOD_PATH[4] = {"../output/local-edit-distance.csv","../output/jaro-winkler.csv", "../output/global-edit-distance.csv", "../output/2-gram.csv"};

int main() {
    // Create instances
    file_io io;
    ranking ranking;

    // reading filess
    vector<string> dict = io.read(DICT);
    vector<vector<string> > blends = io.read_blends(BLENDS);
    // vector<string> candidates = io.read("../output/processed_candidates.txt");

    vector<string> candidates = io.filter_read(CANDIDATES, DICT);
    cout << "After filtering: " << candidates.size() << " words" << endl;
    ofstream fout("../data/processed_candidates.txt");
    for (int i = 0; i < candidates.size(); i++) {
        fout << candidates[i] << endl;
    }
    fout.close();

    for (int i = 0; i < 4; i++) {
        cout << endl << "===================================" << endl;
        cout << "METHOD: " << METHOD[i] << endl;
        cout << "===================================" << endl << endl;
        vector<pair<string, double> > result;
        vector<pair<string, double> > test;

        for (int j = 0; j < candidates.size(); j++) {
            if (j % 69 == 0) cout << j / 6844.0 * 100 << "% COMPLETED" << endl;

            string candidate = candidates[j];
            
            // get similarity rankings for prefix and suffix
            vector<Ranking> pre_ranking = ranking.get_prefix_ranking(candidate, dict, i);
            vector<Ranking> suf_ranking = ranking.get_suffix_ranking(candidate, dict, i);
            
            // skip if either of prefix or suffix ranking is size 0
            if (pre_ranking.size() == 0 || suf_ranking.size() == 0) continue;
            
            // get sum of (top prefix similarity + top suffix similarity) s.t. prefix word != suffix word
            double top_score;
            if (pre_ranking[0].word != suf_ranking[0].word) {
                top_score = pre_ranking[0].similarity + suf_ranking[0].similarity;
            } else {
                top_score = max(pre_ranking[0].similarity + suf_ranking[1].similarity, pre_ranking[1].similarity + suf_ranking[0].similarity);
            }

            // calculate final score
            double final_score = top_score / double(candidate.length());
            result.push_back(make_pair(candidate, final_score));

            // store true label
            auto itr = find_if(blends.begin(), blends.end(), [candidate](vector<string> vec){ return vec[0] == candidate; });
            if (itr != blends.end()) {
                test.push_back(make_pair(candidate, 1.0));
            } else {
                test.push_back(make_pair(candidate, 0.0));
            }
        }

        // save result
        ofstream fout(METHOD_PATH[i]);
        fout << "word" << "," << "score" << endl;;
        for (pair<string, double> p : result) {
            fout << p.first << "," << p.second << endl;
        }
        fout.close();

        // save true label
        ofstream test_out("../output/test.csv");
        test_out << "word" << "," << "label" << endl;;
        for (pair<string, double> p : test) {
            test_out << p.first << "," << p.second << endl;
        }
        fout.close();
    }

    return 0;
}