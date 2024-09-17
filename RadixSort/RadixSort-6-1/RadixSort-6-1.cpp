#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int VALUE_LENGTH = 64; // Фиксированная длина строки значения

// Вспомогательная функция для удаления символов '+' и '-'
string normalizePhone(const string& phone) {
    string normalized;
    for (char c : phone) {
        if (isdigit(c)) {
            normalized += c; // Добавляем только цифры
        }
    }
    return normalized;
}

// Функция сортировки подсчетом для одного разряда
void countingSort(vector<pair<string, string>>& pairs, int pos) {
    const int base = 10; // Возможные цифры от 0 до 9
    vector<int> count(base, 0);
    vector<pair<string, string>> output(pairs.size());

    // Подсчёт количества вхождений каждого символа на позиции pos
    for (const auto& p : pairs) {
        string normalized = normalizePhone(p.first);
        int charIdx = (pos < normalized.size()) ? (normalized[normalized.size() - 1 - pos] - '0') : 0;
        count[charIdx]++;
    }

    // Накопительное суммирование для определения позиций
    for (int i = 1; i < base; i++) {
        count[i] += count[i - 1];
    }

    // Построение отсортированного массива
    for (int i = pairs.size() - 1; i >= 0; i--) {
        string normalized = normalizePhone(pairs[i].first);
        int charIdx = (pos < normalized.size()) ? (normalized[normalized.size() - 1 - pos] - '0') : 0;
        output[--count[charIdx]] = pairs[i];
    }

    // Копирование отсортированных данных обратно
    pairs = output;
}

// Поразрядная сортировка для пар ключ-значение
void radixSort(vector<pair<string, string>>& pairs) {
    // Определяем максимальную длину нормализованного телефонного номера
    int maxLength = 0;
    for (const auto& p : pairs) {
        maxLength = max(maxLength, (int)normalizePhone(p.first).size());
    }

    // Выполняем поразрядную сортировку с конца номера
    for (int pos = 0; pos < maxLength; ++pos) {
        countingSort(pairs, pos);
    }
}

// Функция для дополнения строки до 64 символов нулями
string padValue(const string& value) {
    string padded = value;
    while (padded.size() < VALUE_LENGTH) {
        padded += '\0'; // Дополнение нулевыми символами
    }
    return padded;
}

int main() {
    vector<pair<string, string>> pairs;
    string line;

    // Чтение пар ключ-значение из входных данных
    while (getline(cin, line)) {
        if (line.empty()) continue;

        size_t tabPos = line.find('\t');
        if (tabPos == string::npos) continue;

        string key = line.substr(0, tabPos);
        string value = line.substr(tabPos + 1);

        // Дополняем значение до 64 символов
        value = padValue(value);

        pairs.emplace_back(key, value);
    }

    // Применение поразрядной сортировки
    radixSort(pairs);

    // Вывод отсортированных пар
    for (const auto& p : pairs) {
        cout << p.first << '\t';
        cout << p.second.substr(0, p.second.find('\0')) << endl; // Вывод без нулевых символов
    }

    return 0;
}