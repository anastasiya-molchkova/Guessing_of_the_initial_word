/* Возьмите любое слово, например, «корова». Используя генерацию случайных чисел, 
переставьте буквы этого слова в рандомном порядке. 
Делайте это до тех пор, пока полученное слово не совпадёт с начальным словом. 
Используя массив, укажите при перестановке букв их индексы. Программа должна корректно работать с любым словом. */

#include <iostream>
#include <Windows.h> // для кириллицы
#include <string>    // для работы со строками
#include <vector>    // использую вектор вместо массива, т.к. длина слова заранее не известна
#include <cstdlib>   // для функций выбора случайного числа srand() и rand()
#include <ctime>     // чтобы в randomize опираться на время запуска программы
#include <algorithm> // для std::swap

/* везде в программе будем использовать тип uint16_t для индексов букв,
потому что:
1) в слове букв немного, это положительное число уж точно меньше 128
2) тип uint8_t не подходит, потому что через cout выводится какая-то ерунда вместо его символов */

// получаем слово от пользователя
std::string get_word()
{
    std::cout << "Программа случайным образом меняет буквы в слове, пока не получит начальное слово.\n";
    std::cout << "Введите слово (лучше не очень длинное): ";
    std::string word;
    std::getline(std::cin, word);
    return word;
}

// функция выбора случайного числа между двумя заданными значениями
static uint16_t getRandomNumber(uint16_t min, uint16_t max)
{
    static const double fraction = 1.0 / static_cast<double>(RAND_MAX + 1.0);
    return static_cast<uint16_t>(rand() * fraction * (max - min + 1) + min);
}

// функция создаёт новое слово по данному слову и индексам букв из него
std::string form_new_word_by_indexes(const std::string& initial_word, const std::vector<uint16_t>& indexes)
{
    std::string new_word{};
    for (uint16_t i = 0; i < initial_word.length(); ++i)
        new_word += initial_word[indexes[i]];
    return new_word;
}

void mix_cycle(const std::string& initial_word);

int main()
{
    // подключаем кириллицу:
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    //аналог randomize с привязкой ко времени запуска:
    srand(static_cast<unsigned int>(time(0)));
    rand();
   
    mix_cycle(get_word());

    return 0;
}

// основной цикл программы: случайно переставляем буквы в данном слове, пока не получим его же
void mix_cycle(const std::string& initial_word)
{
    // количество букв в слове:
    const uint16_t letters_number{ static_cast<uint16_t>(initial_word.length()) };

    // массив доступных для перестановки номеров букв, сначала это все номера по порядку 0, 1, 2, ...
    std::vector <uint16_t> available_indexes{};
    available_indexes.resize(letters_number);
    for (uint16_t i = 0; i < letters_number; ++i)
        available_indexes[i] = i;
    // количество доступных индексов, сначала это все:
    uint16_t available_indexes_number{ letters_number };

    // массив номеров букв для нового слова, сначала там пусто
    std::vector <uint16_t> new_indexes{};
    new_indexes.resize(letters_number);

    unsigned iteration{ 1 };  // счётчик итераций для печати на экране

    while (true)
    {
        std::cout << "[" << iteration++ << "] "; // печатаем номер итерации, после чего его увеличиваем

        // цикл по номерам букв нового слова:
        for (uint16_t i = 0; i < letters_number; ++i)
        {
            // случайно выбираем номер буквы из доступных индексов
            uint16_t new_index = getRandomNumber(0, available_indexes_number - 1);
            new_indexes[i] = available_indexes[new_index];
            std::cout << new_indexes[i];

            /* выбранный индекс меняем местами с последним доступным и уменьшаем количество доступных индексов на 1,
               чтобы сделать его недоступным: */
            std::swap(available_indexes[new_index], available_indexes[available_indexes_number - 1]);
            --available_indexes_number;
        }

        // теперь по начальному слову и индексам букв из него формируем новое слово, печатаем результат:
        std::string new_word = form_new_word_by_indexes(initial_word, new_indexes);
        std::cout << " " << new_word << std::endl;

        // если получилось начальное слово, выходим из цикла while, иначе будем составлять другое слово, перемешивая полученное:
        if (initial_word == new_word)
            break;
        else // готовимся к следующей итерации
        {
            available_indexes_number = letters_number;    // снова все индексы букв доступны
            for (uint16_t i = 0; i < letters_number; ++i)
                available_indexes[i] = new_indexes[i];    // теперь будем перемешивать на основе получившихся индексов
        }
    }
}