#include "header/ranking.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// sort in descending order
bool ranking::descending (const Ranking &a, const Ranking &b) {
    return a.similarity > b.similarity;
}

double ranking::calc_sim(string word, string candidate, int method) {
    // create instance
    similarity similarity;
    
    // get similarity between word and candidate
    switch (method)
    {
    case 0:
        return similarity.local_edit(word, candidate);
        break;

    case 1:
        return similarity.jaro_winkler(word, candidate, 0.1);
        break;
    
    case 2:
        return similarity.global_edit(word, candidate);
        break;

    case 3:
        return similarity.n_gram(word, candidate, 2);
        break;
    
    default:
        break;
    }

    return -1;
}

// calculate similarity between word and prefix of candidate and sort in descending order
vector<Ranking> ranking::get_prefix_ranking(string candidate, vector<string> dict, int method) {
    const int cl = candidate.length();
    vector<Ranking> pre_info;
    // store word, similarity to lexical word, and total count of word appearance 
    vector<string> pre_word;
    vector<double> pre_similarity;
    vector<int> pre_count;
    
    // prefix length should be at least 2 and suffix length should be at least 3
    for (int pre_len = 2; pre_len < cl - 2; pre_len++) {
        string prefix = candidate.substr(0, pre_len);

        for (string word : dict) {
            // part of word word should not be the same as prefix
            if (word.length() > pre_len && word.substr(0, pre_len) == prefix) {
                auto itr = find(pre_word.begin(), pre_word.end(), word);
                
                if (itr == pre_word.end()) {
                    // if word doesn't exist in pre_word, push it into pre_word
                    double pre_sim = calc_sim(word, candidate, method);
                    pre_word.push_back(word);
                    pre_similarity.push_back(pre_sim);
                    pre_count.push_back(1);
                } else {
                    // if word is already exist in pre_word, increment count
                    int idx = itr - pre_word.begin();
                    pre_count[idx]++;
                } 
            }
        }
    }

    // calculate similarity * word appearance count
    for (string word : pre_word) {
        auto itr = find(pre_word.begin(), pre_word.end(), word);
        int idx = itr - pre_word.begin();
        double freq_sim = pre_similarity[idx] * pre_count[idx];
        Ranking info = { word, freq_sim };
        pre_info.push_back(info);
    }

    // sort in descending order
    sort(pre_info.begin(), pre_info.end(), descending);
    
    return pre_info;
}

// calculate similarity between word and prefix of candidate and sort in descending order
vector<Ranking> ranking::get_suffix_ranking(string candidate, vector<string> dict, int method) {
    const int cl = candidate.length();
    vector<Ranking> suf_info;
    // store word, similarity to lexical word, and total count of word appearance 
    vector<string> suf_word;
    vector<double> suf_similarity;
    vector<int> suf_count;
    
    // prefix length should be at least 2 and suffix length should be at least 3
    for (int suf_len = 3; suf_len < cl - 1; suf_len++) {
        string suffix = candidate.substr(cl - suf_len, suf_len);
        
        for (string word : dict) {
            // part of word word should not be the same as suffix
            if (word.length() > suf_len && word.substr(word.length() - suf_len, suf_len) == suffix) {
                auto itr = find(suf_word.begin(), suf_word.end(), word);
                
                if (itr == suf_word.end()) {
                    // if word doesn't exist in suf_word, push it into pre_word
                    double suf_sim; 
                    if (method == 1) {
                        // if method is Jaro-Winkler Similarity, then reverse word and candidate
                        reverse(word.begin(), word.end());
                        reverse(candidate.begin(), candidate.end());
                        suf_sim = calc_sim(word, candidate, method);
                        reverse(word.begin(), word.end());
                        reverse(candidate.begin(), candidate.end());
                    } else {
                        suf_sim = calc_sim(word, candidate, method);
                    }

                    suf_word.push_back(word);
                    suf_similarity.push_back(suf_sim);
                    suf_count.push_back(1);
                } else {
                    // if word is already exist in suf_word, increment count
                    int idx = itr - suf_word.begin();
                    suf_count[idx]++;
                } 
            }
        }
    }

    // calculate similarity * word appearance count
    for (string word : suf_word) {
        auto itr = find(suf_word.begin(), suf_word.end(), word);
        int idx = itr - suf_word.begin();
        double freq_sim = suf_similarity[idx] * suf_count[idx];
        Ranking info = { word, freq_sim };
        suf_info.push_back(info);
    }

    // sort in descending order
    sort(suf_info.begin(), suf_info.end(), descending);;
    
    return suf_info;
}