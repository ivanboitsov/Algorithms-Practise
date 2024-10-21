#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Функция для разделения строки на слова и приведения их к нижнему регистру
vector<string> splitAndLowercase(const string& str) {
    vector<string> words;
    stringstream ss(str);
    string word;
    while (ss >> word) {
        // Приводим слово к нижнему регистру
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.push_back(word);
    }
    return words;
}

// Функция для построения таблицы плохого символа (для строк)
vector<int> buildBadCharTable(const vector<string>& pattern) {
    vector<int> badCharTable(256, -1); // Таблица для символов ASCII
    for (int i = 0; i < pattern.size(); i++) {
        for (char c : pattern[i]) {
            badCharTable[(int)c] = i; // Для каждого символа в слове
        }
    }
    return badCharTable;
}

// Основная функция поиска алгоритмом Апостолико-Джанкарло
void apostolicoGiancarloSearch(const vector<vector<string>>& textLines, const vector<string>& pattern) {
    int patternSize = pattern.size();

    // Строим таблицу плохих символов
    vector<int> badCharTable = buildBadCharTable(pattern);

    // Проходим по каждой строке текста
    for (int i = 0; i < textLines.size(); i++) {
        const vector<string>& words = textLines[i];
        int n = words.size();
        int shift = 0;

        while (shift <= (n - patternSize)) {
            int j = patternSize - 1;

            // Сравниваем паттерн с текстом (справа налево)
            while (j >= 0 && pattern[j] == words[shift + j]) {
                j--;
            }

            // Если весь паттерн совпал с последовательностью слов
            if (j < 0) {
                cout << i + 1 << ", " << shift + 1 << endl;  // Выводим 1-индексную позицию строки и слова
                shift += 1; // Смещаем паттерн на 1 слово для поиска следующих вхождений (для перекрытий)
            }
            else {
                // Используем таблицу плохого символа для сдвига
                shift += max(1, j - badCharTable[(int)words[shift + j][0]]);
            }
        }
    }
}

int main() {
    vector<string> patterns;  // Паттерн (первая строка)
    vector<vector<string>> text;  // Текст (остальные строки)
    string line;

    // Считываем первую строку как паттерн и приводим к нижнему регистру
    getline(cin, line);
    patterns = splitAndLowercase(line);

    // Считываем оставшиеся строки как текст и приводим к нижнему регистру
    while (getline(cin, line)) {
        text.push_back(splitAndLowercase(line));
    }

    // Поиск паттерна в тексте
    apostolicoGiancarloSearch(text, patterns);

    return 0;
}