#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>
#include <stdlib.h>
#include <string.h>
#include <cstring>

#include "func.h"

unsigned int factorial(unsigned int number)
{
    return number <= 1 ? 1 : factorial(number - 1) * number;
}


vector<int> computeLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int containsCode(const string& transmissionFile, const string& mcodeFile) {
    ifstream transmission(transmissionFile);
    ifstream mcode(mcodeFile);

    string transmissionContent((istreambuf_iterator<char>(transmission)), istreambuf_iterator<char>());
    string mcodeContent((istreambuf_iterator<char>(mcode)), istreambuf_iterator<char>());

    int n = transmissionContent.length();
    int m = mcodeContent.length();

    vector<int> lps = computeLPS(mcodeContent);

    int i = 0; // index for transmissionContent
    int j = 0; // index for mcodeContent

    while (i < n) {
        if (mcodeContent[j] == transmissionContent[i]) {
            j++;
            i++;
        }

        if (j == m) {
            return i - j + 1; // mcode found, return the starting index
        } else if (i < n && mcodeContent[j] != transmissionContent[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return -1; // mcode not found
}

tuple<int, int, string> findLongestPalindrome(const string& transmissionFile) {
    ifstream transmission(transmissionFile);

    string transmissionContent((istreambuf_iterator<char>(transmission)),
                                     istreambuf_iterator<char>());

    int maxLength = 0;
    int start = 0;
    int end = 0;
    
    for (int i = 0; i < transmissionContent.size(); i++){
        // odd
        int left = i;
        int right = i;
        while (left >= 0 && right < transmissionContent.size() && transmissionContent[left] == transmissionContent[right]){
            int length = right - left + 1;
            if (length > maxLength){
                maxLength = length;
                start = left;
                end = right;
            }
            left--;
            right++;
        }

        // even 
        left = i;
        right = i + 1;
        while (left >= 0 && right < transmissionContent.size() && transmissionContent[left] == transmissionContent[right]){
            int length = right - left + 1;
            if (length > maxLength){
                maxLength = length;
                start = left;
                end = right;
            }
            left--;
            right++;
        }
    }
   return make_tuple(start + 1, end + 1, transmissionContent.substr(start, maxLength));
}

tuple<int, int, string> findLongestCommonSubstring(const string& transmission1File, const string& transmission2File) {
    ifstream transmission1(transmission1File);
    ifstream transmission2(transmission2File);

    string transmission1ContentS((istreambuf_iterator<char>(transmission1)), istreambuf_iterator<char>());
    string transmission2ContentS((istreambuf_iterator<char>(transmission2)), istreambuf_iterator<char>());

    int len1 = transmission1ContentS.size();
    int len2 = transmission2ContentS.size();
    vector<vector<int>> longest(len1+1, vector<int>(len2+1,0));
    int maxLen = 0;
    int endRow = -1, endCol = -1;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (transmission1ContentS[i - 1] == transmission2ContentS[j - 1]) {
                longest[i][j] = longest[i - 1][j - 1] + 1;
                if (maxLen < longest[i][j]) {
                    maxLen = longest[i][j];
                    endRow = i;
                    endCol = j;
                }
            } else {
                longest[i][j] = 0;
            }
        }
    }

    if (maxLen == 0) {
        cout << "There exists no common substring" << endl;
        return make_tuple(-1, -1, "");
    }

    string final_str(maxLen, '\0');
    int row = endRow;
    int col = endCol;

    while (longest[row][col] != 0 && row > 0 && col > 0) {
        final_str[--maxLen] = transmission1ContentS[row - 1];
        row--;
        col--;
    }
    return make_tuple(row+1, row+final_str.length(), final_str); 
}
