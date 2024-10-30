#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Функция для вычисления Z-массива
vector<int> computeZ(const string& s) {
    int n = s.size();
    vector<int> z(n, 0);
    cout << s << endl;
    int left = 0, right = 0;
    for (int i = 1; i < n; i++) {
        if (i <= right) {
            z[i] = min(right - i + 1, z[i - left]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }
    return z;
}

// Функция для поиска всех позиций вхождений образца в тексте
vector<int> findPatternPositions(const string& text, const string& pattern) {
    string combined = pattern + "$" + text; // объединение строки и образца с разделителем
    int patternLength = pattern.size();
    vector<int> z = computeZ(combined);
    vector<int> positions;

    // Проход по Z-массиву для нахождения позиций совпадений
    for (int i = patternLength + 1; i < z.size(); i++) {
        if (z[i] == patternLength) {
            positions.push_back(i - patternLength - 1);
        }
    }
    return positions;
}

int main() {
    string text, pattern;
    cin >> text >> pattern;

    vector<int> positions = findPatternPositions(text, pattern);

    // Вывод позиций вхождений
    for (int pos : positions) {
        cout << pos << endl;
    }

    return 0;
}