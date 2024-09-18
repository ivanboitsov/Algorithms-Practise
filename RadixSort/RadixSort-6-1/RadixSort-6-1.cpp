#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Вспомогательная функция для удаления символов '+' и '-'
// Нормализует телефонный номер, возвращает только цифры
unsigned long long normalizePhone(const string& phone) {
    unsigned long long normalized = 0;
    for (char c : phone) {
        if (isdigit(c)) {
            normalized = normalized * 10 + (c - '0'); // Добавляем только цифры
        }
    }
    return normalized;
}

// Функция сортировки подсчетом для одного разряда (поразрядная сортировка)
void countingSort(vector<pair<unsigned long long, int>>& pairs, unsigned long long pos) {
    const int base = 10; // Возможные цифры от 0 до 9
    vector<int> count(base, 0);
    vector<pair<unsigned long long, int>> output(pairs.size());

    // Подсчёт количества вхождений каждой цифры на позиции pos
    for (const auto& p : pairs) {
        int charIdx = (p.first / pos) % 10; // Получаем цифру на позиции pos
        count[charIdx]++;
    }

    // Накопительное суммирование для определения позиций
    for (int i = 1; i < base; i++) {
        count[i] += count[i - 1];
    }

    // Построение отсортированного массива
    for (int i = pairs.size() - 1; i >= 0; i--) {
        int charIdx = (pairs[i].first / pos) % 10;
        output[--count[charIdx]] = pairs[i];
    }

    // Копирование отсортированных данных обратно
    pairs = output;
}

// Поразрядная сортировка для пар ключ-значение
void radixSort(vector<pair<unsigned long long, int>>& pairs) {
    // Определяем максимальное значение нормализованного телефонного номера
    unsigned long long maxVal = 0;
    for (const auto& p : pairs) {
        maxVal = max(maxVal, p.first);
    }

    // Выполняем поразрядную сортировку с конца номера
    for (unsigned long long pos = 1; maxVal / pos > 0; pos *= 10) {
        countingSort(pairs, pos);
    }
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    vector<pair<string, string>> pairs;
    unordered_map<unsigned long long, vector<int>> originalPhones; // Для маппинга нормализованных телефонов к оригинальным
    string line;

    // Чтение пар ключ-значение из входных данных
    while (getline(cin, line)) {
        if (line.empty()) continue;

        size_t tabPos = line.find('\t');
        if (tabPos == string::npos) continue;

        string key = line.substr(0, tabPos);
        string value = line.substr(tabPos + 1);

        unsigned long long normalizedKey = normalizePhone(key);

        pairs.emplace_back(key, value); // Сохраняем оригинальные пары ключ-значение
        originalPhones[normalizedKey].push_back(pairs.size() - 1); // Сохраняем индексы для восстановления оригинальных данных
    }

    // Подготовка данных для сортировки: нормализованные телефоны + индексы
    vector<pair<unsigned long long, int>> pairs_sort(pairs.size());
    for (int i = 0; i < pairs.size(); i++) {
        pairs_sort[i] = { normalizePhone(pairs[i].first), i }; // Нормализованный телефон и индекс
    }

    // Применение поразрядной сортировки
    radixSort(pairs_sort);

    // Вывод отсортированных пар
    for (const auto& p : pairs_sort) {
        int originalIndex = p.second;
        cout << pairs[originalIndex].first << '\t' << pairs[originalIndex].second << endl;
    }

    return 0;
}