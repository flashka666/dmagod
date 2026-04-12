#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;

const int SIZE = 20;
enum traintype { freight, passenger, repair };

struct ttime {
    int hours;
    int minutes;
};

struct schedule {
    char route[30];
    char departure[30];
    char arrival[30];
    ttime departuretime;
    ttime arrivetime;
    int carcount;
    traintype type;
    int price;
};

// расписание
schedule* init() {
    schedule* schedules = new schedule[SIZE];

    schedules[0] = { "Грузовой-1", "Москва", "Владивосток", {8, 0}, {22, 30}, 45, freight, 5000 };
    schedules[1] = { "Грузовой-2", "Новосибирск", "Сочи", {5, 0}, {18, 0}, 50, freight, 4500 };
    schedules[2] = { "Товарный-3", "Екатеринбург", "Калининград", {4, 15}, {16, 30}, 55, freight, 4800 };
    schedules[3] = { "Угольный", "Кемерово", "Новороссийск", {3, 0}, {15, 0}, 60, freight, 5200 };
    schedules[4] = { "Нефтяной", "Тюмень", "Севастополь", {1, 0}, {13, 0}, 48, freight, 4700 };
    schedules[5] = { "Экспресс", "Москва", "Санкт-Петербург", {9, 0}, {13, 30}, 12, passenger, 1500 };
    schedules[6] = { "Ласточка", "Казань", "Нижний Новгород", {10, 0}, {14, 0}, 8, passenger, 1200 };
    schedules[7] = { "Сапсан", "Москва", "Санкт-Петербург", {15, 0}, {19, 0}, 10, passenger, 2500 };
    schedules[8] = { "Южный", "Ростов", "Краснодар", {8, 30}, {12, 0}, 14, passenger, 1000 };
    schedules[9] = { "Пригородный", "СПб", "Выборг", {7, 0}, {9, 30}, 6, passenger, 800 };
    schedules[10] = { "Стрела", "Москва", "Тверь", {18, 0}, {20, 0}, 9, passenger, 900 };
    schedules[11] = { "Быстрый", "Самара", "Уфа", {6, 0}, {10, 30}, 11, passenger, 1100 };
    schedules[12] = { "Дальний", "Челябинск", "Екатеринбург", {14, 0}, {17, 0}, 7, passenger, 950 };
    schedules[13] = { "Ночной", "Москва", "Санкт-Петербург", {23, 0}, {6, 0}, 15, passenger, 1800 };
    schedules[14] = { "Скорый", "Новосибирск", "Томск", {12, 0}, {15, 30}, 13, passenger, 1300 };
    schedules[15] = { "Ремонтный-1", "Москва", "Тверь", {23, 0}, {5, 0}, 5, repair, 0 };
    schedules[16] = { "Ремпуть", "СПб", "Новгород", {22, 0}, {4, 0}, 4, repair, 0 };
    schedules[17] = { "Ремонтный-2", "Казань", "Йошкар-Ола", {1, 0}, {6, 0}, 6, repair, 0 };
    schedules[18] = { "Путеец", "Екатеринбург", "Пермь", {0, 0}, {5, 0}, 5, repair, 0 };
    schedules[19] = { "Ремонтник", "Новосибирск", "Барнаул", {2, 0}, {7, 0}, 4, repair, 0 };

    return schedules;
}

// тип поезда
const char* getTypeName(traintype type) {
    switch (type) {
    case freight: return "Товарный";
    case passenger: return "Пассажирский";
    case repair: return "Ремонтный";
    default: return "Неизвестно";
    }
}

// вывод маршрута
void printRoute(schedule r) {
    printf("%-15s | %-12s | %-15s | %02d:%02d    | %02d:%02d    | %3d | %-12s | %6d\n",
        r.route, r.departure, r.arrival,
        r.departuretime.hours, r.departuretime.minutes,
        r.arrivetime.hours, r.arrivetime.minutes,
        r.carcount, getTypeName(r.type), r.price);
}

// обертка
void wrapperPrint(schedule* arr, int size, const char* title) {
    printf("\n========== %s ==========\n", title);
    if (size == 0) {
        printf("Нет данных для отображения.\n");
        return;
    }
    printf("%-15s | %-12s | %-15s | %-8s | %-8s | %3s | %-12s | %6s\n",
        "Маршрут", "Отправление", "Прибытие", "Отпр", "Приб", "Ваг", "Тип", "Цена");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < size; i++) {
        printRoute(arr[i]);
    }
}

// груз в приморские города
schedule* filterFreightToSeaside(schedule* src, int& newSize) {
    schedule* filtered = new schedule[SIZE];
    newSize = 0;
    const char* seasideCities[] = { "Владивосток", "Сочи", "Калининград", "Севастополь", "Новороссийск" };

    for (int i = 0; i < SIZE; i++) {
        if (src[i].type == freight) {
            for (int j = 0; j < 5; j++) {
                if (strstr(src[i].arrival, seasideCities[j]) != nullptr) {
                    filtered[newSize++] = src[i];
                    break;
                }
            }
        }
    }
    return filtered;
}

// количество вагонов
void SortByCarCount(schedule* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].carcount > arr[j + 1].carcount) {
                schedule temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// вывод конкретного электропоезда
void printRouteByName(schedule* routes, const char* name) {
    for (int i = 0; i < SIZE; i++) {
        if (strcmp(routes[i].route, name) == 0) {
            printf("\n=== Данные по маршруту \"%s\" ===\n", name);
            printRoute(routes[i]);
            return;
        }
    }
    printf("\nМаршрут \"%s\" не найден!\n", name);
}

// 3 самых длинных пассажирских
void printThreeLongestPassenger(schedule* routes) {
    schedule* passengers = new schedule[SIZE];
    int passCount = 0;

    for (int i = 0; i < SIZE; i++) {
        if (routes[i].type == passenger) {
            passengers[passCount++] = routes[i];
        }
    }

    for (int i = 0; i < passCount - 1; i++) {
        for (int j = 0; j < passCount - i - 1; j++) {
            if (passengers[j].carcount < passengers[j + 1].carcount) {
                schedule temp = passengers[j];
                passengers[j] = passengers[j + 1];
                passengers[j + 1] = temp;
            }
        }
    }

    printf("\n=== 3 пассажирских поезда с наибольшим количеством вагонов ===\n");
    for (int i = 0; i < (passCount < 3 ? passCount : 3); i++) {
        printRoute(passengers[i]);
    }
    delete[] passengers;
}

// изменение данных маршрута
void changeRouteData(schedule* routes, const char* name) {
    for (int i = 0; i < SIZE; i++) {
        if (strcmp(routes[i].route, name) == 0) {
            printf("\n=== Редактирование маршрута \"%s\" ===\n", name);
            printf("Введите новое название: "); cin >> routes[i].route;
            printf("Введите пункт отправления: "); cin >> routes[i].departure;
            printf("Введите пункт прибытия: "); cin >> routes[i].arrival;

            do {
                printf("Время отправления (часы минуты, 0-23 0-59): ");
                cin >> routes[i].departuretime.hours >> routes[i].departuretime.minutes;
            } while (routes[i].departuretime.hours < 0 || routes[i].departuretime.hours > 23 ||
                routes[i].departuretime.minutes < 0 || routes[i].departuretime.minutes > 59);

            do {
                printf("Время прибытия (часы минуты, 0-23 0-59): ");
                cin >> routes[i].arrivetime.hours >> routes[i].arrivetime.minutes;
            } while (routes[i].arrivetime.hours < 0 || routes[i].arrivetime.hours > 23 ||
                routes[i].arrivetime.minutes < 0 || routes[i].arrivetime.minutes > 59);

            do {
                printf("Количество вагонов (>= 0): ");
                cin >> routes[i].carcount;
            } while (routes[i].carcount < 0);

            int type;
            do {
                printf("Тип (0-товарный, 1-пассажирский, 2-ремонтный): ");
                cin >> type;
            } while (type < 0 || type > 2);
            routes[i].type = (traintype)type;

            do {
                printf("Стоимость билета (>= 0): ");
                cin >> routes[i].price;
            } while (routes[i].price < 0);

            printf("\nДанные обновлены!\n");
            return;
        }
    }
    printf("Маршрут не найден!\n");
}

// ФАЙЛЫ
// чтение из текстового файла
void readPricesFromFile(const char* filename, schedule* routes, int size) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "\n!!! Файл " << filename << " не найден. Цены остаются из инициализации. !!!\n";
        return;
    }

    for (int i = 0; i < size; i++) {
        fin >> routes[i].price;
    }
    fin.close();
    cout << "\nЦены успешно загружены из " << filename << endl;
}

// запись в текстовый файл
void writeToTextFile(const char* filename, schedule* routes, int size) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Ошибка: не удалось создать текстовый файл" << filename << endl;
        return;
    }

    fout << "Маршрут          | Отправление   | Прибытие      | Отпр  | Приб  | Ваг | Тип          | Цена" << endl;
    fout << "-------------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        fout << routes[i].route << " | "
            << routes[i].departure << " | "
            << routes[i].arrival << " | "
            << routes[i].departuretime.hours << ":" << routes[i].departuretime.minutes << " | "
            << routes[i].arrivetime.hours << ":" << routes[i].arrivetime.minutes << " | "
            << routes[i].carcount << " | "
            << getTypeName(routes[i].type) << " | "
            << routes[i].price << endl;
    }

    fout.close();
    cout << "Текстовый файл " << filename << " создан." << endl;
}

// запись в бинарный файл
void writeToBinaryFile(const char* filename, schedule* routes, int size) {
    ofstream out(filename, ios::binary);
    if (!out.is_open()) {
        cout << "Ошибка: не удалось создать бинарный файл " << filename << endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        out.write(routes[i].route, sizeof(routes[i].route));
        out.write(routes[i].departure, sizeof(routes[i].departure));
        out.write(routes[i].arrival, sizeof(routes[i].arrival));
        out.write((char*)&routes[i].departuretime, sizeof(ttime));
        out.write((char*)&routes[i].arrivetime, sizeof(ttime));
        out.write((char*)&routes[i].carcount, sizeof(int));
        out.write((char*)&routes[i].type, sizeof(traintype));
        out.write((char*)&routes[i].price, sizeof(int));
    }
    out.close();
    cout << "Бинарный файл " << filename << " записан." << endl;
}

// чтение бинарного файла
void readFromBinaryFile(const char* filename, schedule* routes, int size) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        cout << "Ошибка: не удается бинарный файл " << filename << endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        in.read(routes[i].route, sizeof(routes[i].route));
        in.read(routes[i].departure, sizeof(routes[i].departure));
        in.read(routes[i].arrival, sizeof(routes[i].arrival));
        in.read((char*)&routes[i].departuretime, sizeof(ttime));
        in.read((char*)&routes[i].arrivetime, sizeof(ttime));
        in.read((char*)&routes[i].carcount, sizeof(int));
        in.read((char*)&routes[i].type, sizeof(traintype));
        in.read((char*)&routes[i].price, sizeof(int));
    }
    in.close();
    cout << "Бинарный файл " << filename << " прочитан." << endl;
}

// сохранение данных в файлы (обновление информации)
void saveAllToFiles(schedule* routes, int size) {
    writeToTextFile("routes_text.txt", routes, size);
    writeToBinaryFile("routes_binary.bin", routes, size);
    cout << "Данные сохранены в текстовый и бинарный файлы." << endl;
}

// ВАРИАНТ 6
int main() {
    setlocale(LC_ALL, "Russian");

    cout << "==========================================" << endl;
    cout << "    РАСПИСАНИЕ ЭЛЕКТРОПОЕЗДОВ" << endl;
    cout << "==========================================" << endl;

    // массив маршрутов
    schedule* routes = init();

    // исходные данные
    wrapperPrint(routes, SIZE, "ИСХОДНЫЕ ДАННЫЕ");

    // ПРАКТИКА 10 (Работа с файлами)
    // считывание цен из текстового файла
    readPricesFromFile("prices.txt", routes, SIZE);

    // данные после загрузки цен
    wrapperPrint(routes, SIZE, "ПОСЛЕ ЗАГРУЗКИ ЦЕН ИЗ ФАЙЛА");

    // сохранение в текстовый файл
    writeToTextFile("routes_text.txt", routes, SIZE);

    // сохранение в бинарный файл
    writeToBinaryFile("routes_binary.bin", routes, SIZE);

    // прочтение из бинарного файла в новый массив
    schedule* routesFromBin = new schedule[SIZE];
    readFromBinaryFile("routes_binary.bin", routesFromBin, SIZE);

    // прочитанное из бинарного файла
    wrapperPrint(routesFromBin, SIZE, "ПРОЧИТАНО ИЗ БИНАРНОГО ФАЙЛА");

    // ПРАКТИКА 9 (Структуры)
    cout << "\n========== ВЫПОЛНЕНИЕ ЗАДАНИЯ ВАРИАНТА 6 ==========" << endl;

    // грузовые в приморские города
    int filteredSize;
    schedule* filtered = filterFreightToSeaside(routes, filteredSize);
    wrapperPrint(filtered, filteredSize, "ГРУЗОВЫЕ МАРШРУТЫ В ПРИМОРСКИЕ ГОРОДА");

    // сортировка по количеству вагонов
    SortByCarCount(filtered, filteredSize);
    wrapperPrint(filtered, filteredSize, "ОТСОРТИРОВАНО ПО КОЛИЧЕСТВУ ВАГОНОВ");

    // данные по конкретному поезду
    printRouteByName(routes, "Грузовой-1");

    // 3 самых длинных пассажирских поезда
    printThreeLongestPassenger(routes);

    // изменение данных маршрута
    changeRouteData(routes, "Грузовой-1");
    saveAllToFiles(routes, SIZE); // сохранение изменений в файлы

    // очистка памяти
    delete[] routes;
    delete[] routesFromBin;
    delete[] filtered;

    cout << "\n==========================================" << endl;
    cout << "ПРОГРАММА ЗАВЕРШЕНА!" << endl;
    cout << "Файлы созданы по пути C:\Users\user\source\repos\dmagod\dmagod:" << endl;
    cout << "  - routes_text.txt  (текстовый файл с новыми ценами)" << endl;
    cout << "  - routes_binary.bin (бинарный файл)" << endl;
    cout << "==========================================" << endl;

    system("pause");
    return 0;
}
