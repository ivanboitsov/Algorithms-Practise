#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

unordered_map<char, long> buildShiftTable(const string& pattern) {
    unordered_map<char, long> shiftTable;
    long m = pattern.size();
    for (long i = 0; i < m - 1; ++i) {
        shiftTable[pattern[i]] = m - i - 1;
    }
    for (char c = 'a'; c <= 'z'; ++c) {
        if (shiftTable.find(c) == shiftTable.end()) {
            shiftTable[c] = m;
        }
    }
    return shiftTable;
}

void searchApostolicoGiancarlo(const string& pattern, const vector<string>& text, vector<pair<long, long>>& result) {
    string lowerPattern = toLower(pattern);
    long m = lowerPattern.size();
    auto shiftTable = buildShiftTable(lowerPattern);

    long lineNum = 1;
    long poslongext = 0;
    string textBuffer;
    vector<long> lineStarts;


    for (const auto& line : text) {
        lineStarts.push_back(poslongext);
        textBuffer += toLower(line) + " ";
        poslongext += line.size() + 1;
    }

    long n = textBuffer.size();
    long i = 0;

    while (i <= n - m) {
        long j = m - 1;
        while (j >= 0 && lowerPattern[j] == textBuffer[i + j]) {
            j--;
        }

        if (j < 0) {
            auto lineIt = upper_bound(lineStarts.begin(), lineStarts.end(), i) - 1;
            long lineStart = *lineIt;
            lineNum = distance(lineStarts.begin(), lineIt) + 1;


            long wordNum = 1;
            for (long k = lineStart; k < i; ++k) {
                if (textBuffer[k] == ' ') {
                    wordNum++;
                }
            }
            result.emplace_back(lineNum, wordNum);


            i += m;
        }
        else {
            char badChar = textBuffer[i + j];
            long shift = shiftTable[badChar];
            i += shift;
        }
    }
}

int main() {
    string pattern;
    getline(cin, pattern);

    vector<string> text;
    string line;

    while (getline(cin, line) && !line.empty()) {
        text.push_back(line);
    }

    vector<pair<long, long>> result;
    searchApostolicoGiancarlo(pattern, text, result);

    // Вывод результатов
    for (const auto& entry : result) {
        cout << entry.first << ", " << entry.second << endl;
    }

    return 0;
}