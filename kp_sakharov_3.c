#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#define MAX 100
 
// Структура для хранения данных о проекте на GitHub
typedef struct {
    char name[50];          // Название проекта
    char description[150];  // Описание проекта (кириллица занимает 2 байта на символ)
    char language[30];      // Язык программирования
    int stars;               // Количество звезд
    int forks;                // Количество форков
    int issues;                // Количество проблем (issues)
} github_project;
 
// Прототипы функций
github_project input_data();                                   // Ввод данных с клавиатуры
void output_data(github_project project);                      // Вывод данных на экран
int show_menu();                                                // Отображение меню
int load_from_file(const char* filename, github_project* projects); // Загрузка из файла
void view_all_records(github_project* projects, int count);    // Просмотр всех записей
int search_by_name(const github_project* projects, int count, const char* query, int* found_indexes); // Поиск по названию проекта
int search_by_issues_range(const github_project* projects, int count, int min, int max, int* found_indexes); // Поиск по диапазону количества проблем
void sort_data(github_project* projects, int count);            // Сортировка данных
int add_new_record(github_project* projects, int count);        // Добавление новой записи
int save_to_file(const char* filename, const github_project* projects, int count);          // Сохранение в файл
int compare(const void* a, const void* b);                      // Функция сравнения для qsort
int getRandomNumber(int min, int max);                          // Генерация случайного числа
int generate_random_data(github_project* projects, int count);  // Генерация случайных данных
 
// Главная функция
int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    github_project projects[MAX]; // Массив структур
    int count = 0;                 // Количество записей
    int choice = 0;                // Выбор пользователя в меню
    srand((unsigned int)time(NULL)); // Инициализация генератора случайных чисел
 
    // Главный цикл программы
    while (choice != 9) {
        choice = show_menu();
        switch (choice) {
        case 1: // Загрузка данных из файла
        {
            char filename[100];
            printf("Введите имя файла: ");
            scanf("%99s", filename);
            while (getchar() != '\n');
            int loaded = load_from_file(filename, projects);
            if (loaded < 0) {
                printf("\nНе удалось открыть файл '%s'.\n", filename);
            }
            else if (loaded == 0) {
                printf("\nФайл пуст или содержит некорректные данные.\n");
            }
            else {
                count = loaded;
                printf("\nУспешно загружено %d записей из файла '%s'.\n", count, filename);
            }
        }
        break;
        case 2: // Просмотр всех записей
            view_all_records(projects, count);
            break;
        case 3: // Поиск по названию проекта
        {
            char query[50];
            int found_indexes[MAX];
            printf("Введите название проекта: ");
            scanf("%49s", query);
            while (getchar() != '\n');
            int found = search_by_name(projects, count, query, found_indexes);
            if (found < 0) {
                printf("Ошибка при выполнении поиска.\n");
            }
            else if (found == 0) {
                printf("Проект с таким названием не найден.\n");
            }
            else {
                for (int i = 0; i < found; i++) {
                    printf("\n--- Запись %d ---\n", i + 1);
                    output_data(projects[found_indexes[i]]);
                }
                printf("Найдено записей: %d\n", found);
            }
        }
        break;
        case 4: // Поиск по диапазону количества проблем
        {
            int min, max;
            int found_indexes[MAX];
            printf("Введите нижнюю границу количества проблем: ");
            scanf("%d", &min);
            printf("Введите верхнюю границу количества проблем: ");
            scanf("%d", &max);
            while (getchar() != '\n');
            int found = search_by_issues_range(projects, count, min, max, found_indexes);
            if (found < 0) {
                printf("Ошибка при выполнении поиска.\n");
            }
            else if (found == 0) {
                printf("Не найдено записей в указанном диапазоне.\n");
            }
            else {
                for (int i = 0; i < found; i++) {
                    printf("\n--- Запись %d ---\n", i + 1);
                    output_data(projects[found_indexes[i]]);
                }
                printf("Найдено записей: %d\n", found);
            }
        }
        break;
        case 5: // Сортировка данных
            sort_data(projects, count);
            break;
        case 6: // Добавление новой записи
            count = add_new_record(projects, count);
            break;
        case 7: // Сохранение в файл
        {
            char filename[100];
            printf("Введите имя файла: ");
            scanf("%99s", filename);
            while (getchar() != '\n');
            int saved = save_to_file(filename, projects, count);
            if (saved == count) {
                printf("\nУспешно сохранено %d записей в файл '%s'.\n", count, filename);
            }
            else {
                printf("\nОшибка при сохранении данных! Сохранено только %d из %d записей.\n", saved < 0 ? 0 : saved, count);
            }
        }
        break;
        case 8: // Генерация случайных данных
            count = generate_random_data(projects, count);
            break;
        case 9: // Выход
            printf("\nВыход из программы\n");
            break;
        default:
            printf("\nНеверный выбор\n");
        }
        if (choice != 9) {
            printf("\nНажмите Enter для продолжения");
            getchar();
        }
    }
    return 0;
}
 
// Функция отображения меню
int show_menu() {
    int choice;
    printf("\nМеню управления проектами GitHub\n");
    printf("1. Загрузка данных из файла\n");
    printf("2. Просмотр всех записей\n");
    printf("3. Поиск по названию проекта\n");
    printf("4. Поиск по диапазону количества проблем\n");
    printf("5. Сортировка данных\n");
    printf("6. Добавление новой записи\n");
    printf("7. Сохранение данных в файл\n");
    printf("8. Заполнение случайными данными\n");
    printf("9. Выход\n");
    printf("Выберите пункт меню: ");
    scanf("%d", &choice);
    while (getchar() != '\n');
    return choice;
}
 
// Функция ввода данных с клавиатуры
github_project input_data() {
    github_project project;
    printf("Название проекта: ");
    scanf("%49s", project.name);
    printf("Описание проекта: ");
    scanf(" %149[^\n]", project.description);
    printf("Язык программирования: ");
    scanf("%29s", project.language);
    printf("Количество звезд: ");
    scanf("%d", &project.stars);
    printf("Количество форков: ");
    scanf("%d", &project.forks);
    printf("Количество проблем (issues): ");
    scanf("%d", &project.issues);
    return project;
}
 
// Функция вывода данных на экран
void output_data(github_project project) {
    printf("Название: %s\n", project.name);
    printf("Описание: %s\n", project.description);
    printf("Язык программирования: %s\n", project.language);
    printf("Количество звезд: %d\n", project.stars);
    printf("Количество форков: %d\n", project.forks);
    printf("Количество проблем: %d\n", project.issues);
}
 
/**
 * Загрузка данных из бинарного файла
 * @param filename - имя файла
 * @param projects - массив для записи считанных структур
 * @return количество загруженных записей или -1 в случае ошибки
 */
int load_from_file(const char* filename, github_project* projects) {
    if (filename == NULL || projects == NULL) {
        return -1;
    }
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }
    // Чтение данных из файла в массив структур
    int count = (int)fread(projects, sizeof(github_project), MAX, file);
    fclose(file);
    return count;
}

// Просмотр всех записей
void view_all_records(github_project* projects, int count) {
    printf("\nПросмотр всех записей\n");
    if (count == 0) {
        printf("Записей нет. Сначала добавьте запись.\n");
        return;
    }
    printf("\nВсе записи (%d)\n", count);
    for (int i = 0; i < count; i++) {
        printf("\n--- Запись %d ---\n", i + 1);
        output_data(projects[i]);
    }
}
 
/**
 * Поиск записей по точному названию проекта
 * @param projects - массив структур
 * @param count - количество записей в массиве
 * @param query - искомое название проекта
 * @param found_indexes - массив для индексов найденных записей
 * @return количество найденных записей или -1 в случае ошибки
 */
int search_by_name(const github_project* projects, int count, const char* query, int* found_indexes) {
    if (projects == NULL || query == NULL || found_indexes == NULL || count < 0) {
        return -1;
    }
    int found = 0;
    // Поиск точного совпадения по названию
    for (int i = 0; i < count; i++) {
        if (strcmp(projects[i].name, query) == 0) {
            found_indexes[found] = i;
            found++;
        }
    }
    return found;
}

/**
 * Поиск записей по диапазону количества проблем
 * @param projects - массив структур
 * @param count - количество записей в массиве
 * @param min - нижняя граница диапазона
 * @param max - верхняя граница диапазона
 * @param found_indexes - массив для индексов найденных записей
 * @return количество найденных записей или -1 в случае ошибки
 */
int search_by_issues_range(const github_project* projects, int count, int min, int max, int* found_indexes) {
    if (projects == NULL || found_indexes == NULL || count < 0 || min > max) {
        return -1;
    }
    int found = 0;
    // Поиск записей, количество проблем которых входит в диапазон
    for (int i = 0; i < count; i++) {
        if (projects[i].issues >= min && projects[i].issues <= max) {
            found_indexes[found] = i;
            found++;
        }
    }
    return found;
}

// Функция сравнения для сортировки (по убыванию количества звезд)
int compare(const void* a, const void* b) {
    const github_project* p1 = (const github_project*)a;
    const github_project* p2 = (const github_project*)b;
    if (p1->stars < p2->stars) return 1;
    if (p1->stars > p2->stars) return -1;
    return 0;
}
 
// Сортировка данных с помощью qsort
void sort_data(github_project* projects, int count) {
    if (count == 0) {
        printf("Нет данных для сортировки.\n");
        return;
    }
    qsort(projects, count, sizeof(github_project), compare);
    printf("Данные отсортированы по убыванию количества звезд!\n");
}
 
// Добавление новой записи
int add_new_record(github_project* projects, int count) {
    printf("\nДобавление новой записи\n");
    if (count < MAX) {
        projects[count] = input_data();
        count++;
        printf("Запись успешно добавлена! Всего записей: %d\n", count);
    }
    else {
        printf("Достигнут максимальный лимит записей (%d)!\n", MAX);
    }
    return count;
}
 
/**
 * Сохранение данных в бинарный файл
 * @param filename - имя файла
 * @param projects - массив структур
 * @param count - количество записей для сохранения
 * @return количество сохранённых записей или -1 в случае ошибки
 */
int save_to_file(const char* filename, const github_project* projects, int count) {
    if (filename == NULL || projects == NULL || count < 0) {
        return -1;
    }
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return -1;
    }
    // Запись массива структур в файл
    int result = (int)fwrite(projects, sizeof(github_project), count, file);
    fclose(file);
    return result;
}

// Генерация случайного числа в заданном диапазоне
int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}
 
// Генерация заданного количества случайных записей
int generate_random_data(github_project* projects, int count) {
    int num_r;
    char* names[] = { "react", "linux", "tensorflow", "vscode" };
    char* descriptions[] = {
        "Библиотека для построения пользовательских интерфейсов",
        "Ядро операционной системы Linux",
        "Библиотека машинного обучения с открытым кодом",
        "Кроссплатформенный редактор кода"
    };
    char* languages[] = { "JavaScript", "C", "C++", "TypeScript" };
    printf("Сколько записей сгенерировать? ");
    scanf("%d", &num_r);
    if (count + num_r > MAX) {
        printf("Нельзя добавить %d записей. Максимум можно добавить %d записей.\n", num_r, MAX - count);
        return count;
    }
    for (int i = 0; i < num_r; i++) {
        int index = count + i;
        int name_index = rand() % 4;
        int desc_index = rand() % 4;
        int lang_index = rand() % 4;
        strcpy(projects[index].name, names[name_index]);
        strcpy(projects[index].description, descriptions[desc_index]);
        strcpy(projects[index].language, languages[lang_index]);
        projects[index].stars = getRandomNumber(100, 250000);
        projects[index].forks = getRandomNumber(10, 80000);
        projects[index].issues = getRandomNumber(0, 3000);
    }
    count += num_r;
    printf("Добавлено %d случайных записей. Всего записей: %d\n", num_r, count);
    return count;
}
