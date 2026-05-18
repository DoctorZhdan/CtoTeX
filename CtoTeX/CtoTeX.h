#pragma once
using namespace std;
#include <string>  
#include <map>
#include <set>


/// @brief Коды ошибок программы
enum ErrorCode {
    // файловые ошибки
    InputFileNotExist,          ///< Входной файл не существует
    OutputFileNotCreate,        ///< Не удалось создать выходной файл

    // ошибки дерева разбора
    MultipleLinesInTreeFile,    ///< В файле больше одной строки
    NotEnoughOperands,          ///< Не хватает операндов
    TooManyOperands,            ///< Лишние операнды

    // лексические ошибки
    InvalidSymbol,              ///< Недопустимый символ
    InvalidSymbolSequence,      ///< Последовательность недопустимых символов
    IntegerOverflow,            ///< Выход за диапазон целых чисел

    // ограничения
    TooManyNodes,               ///< Превышено количество узлов
    VariableNameTooLong,        ///< Имя переменной слишком длинное
    TooManyDecimalDigits,       ///< Слишком много знаков после запятой

    // ошибки конфигурации
    InvalidCharInParamFile,     ///< Недопустимый символ в файле параметров
    InvalidParamValue,          ///< Недопустимое значение параметра
    MissingParamValue,          ///< Отсутствует значение параметра
    NonexistentParam,           ///< Несуществующий параметр
    DuplicateParamValue,        ///< Повторное присвоение параметру
    TooManySpaces,              ///< Лишние пробелы
    TooManyWords,               ///< Слишком много слов в строке
    EmptyLine,                  ///< Пустая строка

    // ошибки типов
    ArithmeticWithLogical,      ///< Арифметическая операция с логическими операндами
    MismatchedOperandTypes,     ///< Несовместимые типы операндов

    // оператор не окружен пробелами
    OperatorNotSeparatedBySpaces ///< Оператор не отделён пробелами
};

/// @brief Словарь сообщений об ошибках
const map<ErrorCode, string> errorMessages = {
{InputFileNotExist,       "Неверно указан файл с входными данными. Возможно, файл не существует."},
{OutputFileNotCreate,     "Неверно указан файл для выходных данных. Возможно указанного расположения не существует или нет прав на запись."},

{MultipleLinesInTreeFile, "В файле с деревом разбора выражения содержится больше одной строки."},
{NotEnoughOperands,       "В выражении не хватает операндов."},
{TooManyOperands,         "В выражении содержатся лишние операнды."},

{InvalidSymbol,           "В выражении содержится недопустимый символ."},
{InvalidSymbolSequence,   "В выражении содержится последовательность недопустимых символов."},
{IntegerOverflow,         "Выход за диапазон целых чисел [-2*10^9, +2*10^9]."},

{TooManyNodes,            "Количество узлов в дереве превышает допустимое значение (1000)."},
{VariableNameTooLong,     "Размер имени переменной превышает допустимое количество символов (255)."},
{TooManyDecimalDigits,    "Количество знаков после запятой превышает допустимое значение."},

{InvalidCharInParamFile,  "В записи содержится недопустимый символ."},
{InvalidParamValue,       "Присвоение параметру недопустимого значения."},
{MissingParamValue,       "Отсутствует присваиваемое значение параметра."},
{NonexistentParam,        "В записи содержится несуществующий параметр."},
{DuplicateParamValue,     "Нельзя присваивать значение параметру более одного раза."},
{TooManySpaces,           "Количество пробелов превышает допустимое значение."},
{TooManyWords,            "Количество слов в строке превышает допустимое значение (2)."},
{EmptyLine,               "В записи содержится пустая строка."},

{ArithmeticWithLogical,   "Несовместимый тип операндов и операции."},
{MismatchedOperandTypes,  "Несовместимый тип операндов."},
{OperatorNotSeparatedBySpaces, "Оператор должен быть окружен пробелами."}
};


/// @brief Структура для хранения данных об ошибке
struct Error {
    ErrorCode code;          ///< Код ошибки из перечисления
    int position = -1;       ///< Индекс символа, содержащего ошибку, в строке 
    int length = 1;          ///< Длина последовательности некорректных символов 
    string line;             ///< Строка, содержащая ошибку

    /// @brief Метод генерации сообщения об ошибке
    /// @return сформированное сообщение об ошибке
    string generateErrorMessage() const {
        // 1. Взять базовое сообщение об ошибке, соответствующее коду ошибки
        string result = errorMessages.at(code);

        // 2. Если известна позиция ошибки в строке и сама строка не пустая
        if (position != -1 && !line.empty()) {
            // 2.1. Добавить к базовому сообщению символ перехода на новую строку и исходную строку
            result += "\n" + line;

            // 2.2. Создать пустую строку-указатель
            string pointer;

            // 2.3. Добавить в строку-указатель столько символов '-', сколько позиций нужно пропустить до места ошибки
            for (int i = 0; i < position; ++i) {
                pointer += '-';
            }

            // 2.4. Добавить в строку-указатель столько символов '^', какова длина последовательности некорректных символов
            for (int i = 0; i < length; ++i) {
                pointer += '^';
            }

            // 2.5. Добавить к сообщению символ перехода на новую строку и строку-указатель
            result += "\n" + pointer;
        }

        // 3. Вернуть полученное сообщение
        return result;
    }
};

/// @brief Словарь допустимых значений параметров отображения
const map<string, set<string>> allowedConfigParams = {
    {"mulIdenVar",       {"withoutChanges", "powVarN"}},  // произведение одинаковых переменных
    {"trigFunNoNegPow",  {"powAfterVar", "powAfterFun"}}, // триг функции в неотрицательной степени
    {"trigFunNegPow",    {"negPow", "divNoNegPow"}},      // триг функции в отрицательной степени (кроме -1)
    {"trigFunMinusOnePow", {"negPow", "divNoNegPow", "reverseFun"}}, // степень -1
    {"zeroPointFivePow", {"withoutChanges", "powToSqrt"}}, // возведение в 0.5
    {"oneDivNPow",       {"withoutChanges", "powToSqrt"}}, // возведение в 1/n
    {"abPow",            {"withoutChanges", "powToSqrt"}}, // возведение в a/b
    {"squareRoot",       {"withoutChanges", "sqrtToPow"}}, // извлечение квадратного корня
    {"logDiv",           {"logConverting", "withoutChanges"}}, // деление логарифмов
    {"arrSum",           {"withoutChanges", "combineInSum"}},  // сумма массива
    {"arrMul",           {"withoutChanges", "combineInMul"}}   // произведение массива
};
