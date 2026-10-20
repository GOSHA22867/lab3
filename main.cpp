#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <limits>

using namespace std;

// === 1. СТРУКТУРА (Вариант 22: Электронные ресурсы) ===
struct Resource {
    int id;             // Числовое 1
    string name;        // Строковое 1
    string type;        // Строковое 2
    string url;         // Строковое 3
    float size;         // Числовое 2 (дробное)
    int year;           // Числовое 3
};

// Вспомогательная функция для очистки ввода (чтобы cin не зацикливался при ошибках)
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// === 2. ФУНКЦИЯ ЧТЕНИЯ ИЗ ФАЙЛА В ДИНАМИЧЕСКИЙ МАССИВ ===
Resource* loadDataFromFile(const string& filename, int& count) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << endl;
        count = 0;
        return nullptr;
    }

    // Сначала считаем количество строк в файле, чтобы выделить память
    count = 0;
    string tempLine;
    while (getline(fin, tempLine)) {
        count++;
    }

    if (count == 0) {
        cout << "Файл пуст." << endl;
        fin.close();
        return nullptr;
    }

    // Выделяем динамический массив
    Resource* arr = new Resource[count];

    // Возвращаемся в начало файла и читаем данные
    fin.clear();
    fin.seekg(0, ios::beg);

    for (int i = 0; i < count; ++i) {
        fin >> arr[i].id >> arr[i].name >> arr[i].type >> arr[i].url >> arr[i].size >> arr[i].year;
    }
    fin.close();
    cout << "Успешно загружено " << count << " записей." << endl;
    return arr;
}

// === 3. ФУНКЦИЯ ПРОСМОТРА ДАННЫХ (ТАБЛИЦЕЙ) ===
void printTable(Resource* arr, int count) {
    if (count == 0 || arr == nullptr) {
        cout << "База данных пуста.\n";
        return;
    }
    cout << "\n" << string(80, '-') << "\n";
    cout << left << setw(5) << "ID"
         << setw(20) << "Название"
         << setw(15) << "Тип"
         << setw(20) << "URL"
         << setw(10) << "Размер"
         << setw(6) << "Год" << "\n";
    cout << string(80, '-') << "\n";
    for (int i = 0; i < count; ++i) {
        cout << left << setw(5) << arr[i].id
             << setw(20) << arr[i].name
             << setw(15) << arr[i].type
             << setw(20) << arr[i].url
             << setw(10) << arr[i].size
             << setw(6) << arr[i].year << "\n";
    }
    cout << string(80, '-') << "\n";
}

// === 4. ФУНКЦИЯ КОРРЕКТИРОВКИ ДАННЫХ ===
void editRecord(Resource* arr, int count) {
    int searchId;
    cout << "Введите ID записи для редактирования: ";
    cin >> searchId;

    for (int i = 0; i < count; ++i) {
        if (arr[i].id == searchId) {
            cout << "Текущее название: " << arr[i].name << ". Введите новое: ";
            cin >> arr[i].name;
            cout << "Текущий URL: " << arr[i].url << ". Введите новый: ";
            cin >> arr[i].url;
            cout << "Запись обновлена.\n";
            return;
        }
    }
    cout << "Запись с таким ID не найдена.\n";
}

// === 5. ФУНКЦИЯ ВЫЧИСЛЕНИЯ ===
void calculateStats(Resource* arr, int count) {
    if (count == 0) return;
    float totalSize = 0;
    int modernCount = 0;

    for (int i = 0; i < count; ++i) {
        totalSize += arr[i].size;
        if (arr[i].year > 2020) {
            modernCount++;
        }
    }
    cout << "\n--- ВЫЧИСЛЕНИЯ ---\n";
    cout << "Общий размер всех ресурсов: " << totalSize << " МБ\n";
    cout << "Количество ресурсов, созданных после 2020 года: " << modernCount << "\n";
}

// === 6. ФУНКЦИЯ СОХРАНЕНИЯ В НОВЫЙ ФАЙЛ ===
void saveToFile(Resource* arr, int count) {
    string newFilename;
    cout << "Введите имя нового файла (например, newdata.txt): ";
    cin >> newFilename;

    ofstream fout(newFilename);
    if (!fout.is_open()) {
        cout << "Ошибка создания файла!\n";
        return;
    }

    for (int i = 0; i < count; ++i) {
        fout << arr[i].id << " " << arr[i].name << " " << arr[i].type << " " 
             << arr[i].url << " " << arr[i].size << " " << arr[i].year << "\n";
    }
    fout.close();
    cout << "Данные успешно сохранены в файл " << newFilename << "\n";
}

// Дополнительная функция для добавления (требуется по заданию)
void addRecord(Resource*& arr, int& count) {
    Resource* newArr = new Resource[count + 1];
    for (int i = 0; i < count; ++i) {
        newArr[i] = arr[i];
    }
    cout << "Введите ID, Название, Тип, URL, Размер, Год (через пробел): ";
    cin >> newArr[count].id >> newArr[count].name >> newArr[count].type 
        >> newArr[count].url >> newArr[count].size >> newArr[count].year;

    delete[] arr; // Удаляем старый массив
    arr = newArr; // Теперь arr указывает на новый
    count++;
    cout << "Запись добавлена.\n";
}

// === ГЛАВНАЯ ФУНКЦИЯ ===
int main() {
    setlocale(LC_ALL, "Russian"); // Для русского языка в консоли

    // Создадим исходный файл программно, чтобы у тебя сразу были данные
    ofstream testFile("data.txt");
    testFile << "1 Wikipedia Website https://wikipedia.org 1500 2001\n";
    testFile << "2 GTA_V Game https://rockstar.com 80000 2013\n";
    testFile << "3 Stepik Course https://stepik.org 50 2022\n";
    testFile << "4 GitHub Platform https://github.com 2000 2008\n";
    testFile.close();

    int count = 0;
    Resource* db = nullptr;
    int choice;

    do {
        cout << "\n=== МЕНЮ УПРАВЛЕНИЯ БАЗОЙ ДАННЫХ ===\n";
        cout << "1. Загрузить данные из файла (data.txt)\n";
        cout << "2. Просмотр данных (таблица)\n";
        cout << "3. Корректировка записи\n";
        cout << "4. Добавить новую запись\n";
        cout << "5. Вычисления\n";
        cout << "6. Сохранить в новый файл\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";

        cin >> choice;
        if (cin.fail()) { clearInput(); continue; }

        switch (choice) {
            case 1:
                if (db != nullptr) delete[] db;
                db = loadDataFromFile("data.txt", count);
                break;
            case 2:
                printTable(db, count);
                break;
            case 3:
                editRecord(db, count);
                break;
            case 4:
                addRecord(db, count);
                break;
            case 5:
                calculateStats(db, count);
                break;
            case 6:
                saveToFile(db, count);
                break;
            case 0:
                cout << "Выход из программы.\n";
                break;
            default:
                cout << "Неверный выбор.\n";
        }
    } while (choice != 0);

    if (db != nullptr) {
        delete[] db; // Очистка памяти перед выходом
    }
    return 0;
}








