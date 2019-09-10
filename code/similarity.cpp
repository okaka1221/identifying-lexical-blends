#include "header/similarity.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Global Edit Distance, Match(+1), Insert/Delete/Replace(-1)
int similarity::global_edit(string s1, string s2) {
    const int l1 = s1.length(); 
    const int l2 = s2.length();
    vector<vector<int> > A(l1 + 1, vector<int>(l2 + 1));

    for (int i = 0; i <= l1; i++){
        A[i][0] = -i;
    }

    for (int j = 0; j <= l2; j++){
        A[0][j] = -j;
    }

    for (int i = 1; i <= l1; i++) {
        for (int j = 1; j <= l2; j++) {
            A[i][j] = max({
                A[i][j - 1] - 1,
                A[i - 1][j] - 1,
                A[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 1 : -1)
            });
        }
    }

    return A[l1][l2];
}

// Local Edit Distance, Match(+1), Insert/Delete/Replace(-1)
int similarity::local_edit(string s1, string s2) {
    const int l1 = s1.length();
    const int l2 = s2.length();
    int res = 0;
    vector<vector<int> > A(l1 + 1, vector<int>(l2 + 1));

    for (int i = 0; i <= l1; i++){
        A[i][0] = 0;
    }

    for (int j = 0; j <= l2; j++){
        A[0][j] = 0;
    }

    for (int i = 1; i <= l1; i++) {
        for (int j = 1; j <= l2; j++) {
            A[i][j] = max({
                0,
                A[i][j - 1] - 1,
                A[i - 1][j] - 1,
                A[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 1 : -1)
            });

            res = max(res, A[i][j]);
        }
    }

    return res;
}

// N-gram
int similarity::n_gram(string s1, string s2, int n) {
    vector<string> n_gram1;
    vector<string> n_gram2;
    int l1 = 0;
    int l2 = 0;
    int common = 0;

    s1 = "#" + s1 + "#";
    s2 = "#" + s2 + "#";

    for (int i = 0; i < s1.length() - n + 1; i++) {
        n_gram1.push_back(s1.substr(i, n));
        l1++;
    }

    for (int i = 0; i < s2.length() - n + 1; i++) {
        l2++;
        string item = s2.substr(i, n);
        auto itr = find(n_gram1.begin(), n_gram1.end(), item);

        if (itr != n_gram1.end()) {
            common++;
            int idx = itr - n_gram1.begin();
            swap(n_gram1[idx], n_gram1.back());
            n_gram1.pop_back();
        }
    }

    // negeate n-gram distance so that greater value indicates more relevance
    return -(l1 + l2 - 2 * common);
}

// Jaro-Winkler Similarity
double similarity::jaro_winkler(string s1, string s2, double p) {
    const int l1 = s1.length();
    const int l2 = s2.length();
    const int match_dist = max(l1, l2) / 2 - 1;
    bool match1[l1];
    bool match2[l2];
    fill(match1, match1 + l1, false);
    fill(match2, match2 + l2, false);
    double match = 0;
    double t = 0;
    int l = 0;

    for (int i = 0; i < l1; i++) {
        const int end = min(i + match_dist, l2);
        for (int j = max(0, i - match_dist); j < end; j++) {
            if (!match2[j] && s1[i] == s2[j]) {
                match1[i] = true;
                match2[j] = true;
                match++;
                break;
            }
        }
    }

    if (match == 0) {
        return 0;
    }

    int k = 0;
    for (int i = 0; i < l1; i++) {
        if (match1[i]) {
            while (!match2[k]) k++;

            if (s1[i] != s2[k]) t += 0.5;
            k++;
        }
    }

    for (int i = 0; i < s1.length(); i++) {
        if (s1[i] != s2[i]) break;
        l++;
    }

    double sim = (match / l1 + match / l2 + (match - t) / match) / 3.0;
    return sim + min(l, 4) * p * (1 - sim);
}