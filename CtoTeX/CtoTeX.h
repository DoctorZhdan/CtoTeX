#pragma once
using namespace std;
#include <string>  
#include <map>
#include <set>
#include <vector>



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

/// @brief Типы токенов
enum TokenType {
    NUMBER, VARIABLE, CONSTANT,
    PLUS, MINUS, MUL, DIV, UMINUS,      ///< арифметика
    POW, SQRT, CBRT,                    ///< степень и корни
    SIN, COS, TAN, ASIN, ACOS, ATAN,    ///< тригонометрия
    LOG, LOG10, EXP,                    ///< логарифмы и экспонента
    ABS, FABS,                          ///< модуль
    EQ, NEQ, LT, GT, LE, GE,            ///< сравнения
    LAND, LOR, LNOT,                    ///< логические
    ARRAY_INDEX,                        ///< индексация
    UNKNOWN                             ///< неопределён (используется для корневого узла)
};

/// @brief Типы операндов
enum OperandType {
    ARITHMETIC, ///< арифметический операнд
    LOGICAL     ///< логический операнд
};

/// @brief Структура токена (лексемы)
struct Token {
    TokenType type;          ///< тип токена
    string value;            ///< текстовое представление токена
    OperandType operandType; ///< тип операндов (арифметический или логический)
};

/// @brief Класс узла дерева разбора
class Node {
public:
    Token token;             ///< токен узла
    Node* left = nullptr;    ///< указатель на левый потомок (единственный для унарных операций) 
    Node* right = nullptr;   ///< указатель на правый потомок (nullptr для унарных операций)

    /// @brief Конструктор для листа (число, переменная, константа)
    /// @param t токен
    Node(const Token& t) : token(t) {}

    /// @brief Конструктор для бинарной операции
    /// @param t токен операции
    /// @param l левый операнд
    /// @param r правый операнд
    Node(const Token& t, Node* l, Node* r) : token(t), left(l), right(r) {}

    /// @brief Конструктор для унарной операции
    /// @param t токен операции
    /// @param l единственный операнд
    Node(const Token& t, Node* l) : token(t), left(l) {}

    /// @brief Деструктор, рекурсивно удаляющий дочерние узлы
    ~Node() {
        delete left;
        delete right;
    }
};

/// @brief Структура параметров отображения
struct Config {
    map<string, string> paramMap; ///< словарь параметров и их значений

    /// @brief Конструктор, инициализирующий параметры значениями по умолчанию
    Config() {
        paramMap["mulIdenVar"] = "withoutChanges";
        paramMap["trigFunNoNegPow"] = "powAfterVar";
        paramMap["trigFunNegPow"] = "negPow";
        paramMap["trigFunMinusOnePow"] = "negPow";
        paramMap["zeroPointFivePow"] = "withoutChanges";
        paramMap["oneDivNPow"] = "withoutChanges";
        paramMap["abPow"] = "withoutChanges";
        paramMap["squareRoot"] = "withoutChanges";
        paramMap["logDiv"] = "logConverting";
        paramMap["arrSum"] = "withoutChanges";
        paramMap["arrMul"] = "withoutChanges";
    }
};

/// @brief Множество допустимых операций
const set<string> allowedOperations = {
    "+", "-", "*", "/", "-_",      // арифметика
    "#pow", "#sqrt", "#cbrt",      // степень и корни
    "a[i]",                         // индексы
    "#sin", "#cos", "#tan", "#asin", "#acos", "#atan", // тригонометрия
    "#log", "#log10", "#exp",      // логарифмы и экспонента
    "#abs", "#fabs",                // модуль
    "==", "!=", "<", ">", "<=", ">=", // сравнения
    "&&", "||", "!"                 // логические
};

/// @brief Множество допустимых констант
const set<string> allowedConstants = {
    "pi", "e", "phi", "i", "true", "false"
};

/// @brief Структура для хранения информации о допустимых типах операндов для операции
struct OperationTypeInfo {
    vector<vector<OperandType>> allowedSignatures; ///< список допустимых сигнатур
};

/// @brief Словарь допустимых типов операндов для каждой операции
const map<TokenType, OperationTypeInfo> operationTypes = {
    // Арифметика
    {PLUS,  {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {MINUS, {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {MUL,   {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {DIV,   {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {UMINUS, {{{OperandType::ARITHMETIC}}}},

    // Степень и корни
    {POW,  {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {SQRT, {{{OperandType::ARITHMETIC}}}},
    {CBRT, {{{OperandType::ARITHMETIC}}}},

    // Тригонометрия
    {SIN,  {{{OperandType::ARITHMETIC}}}},
    {COS,  {{{OperandType::ARITHMETIC}}}},
    {TAN,  {{{OperandType::ARITHMETIC}}}},
    {ASIN, {{{OperandType::ARITHMETIC}}}},
    {ACOS, {{{OperandType::ARITHMETIC}}}},
    {ATAN, {{{OperandType::ARITHMETIC}}}},

    // Логарифмы и экспонента
    {LOG,   {{{OperandType::ARITHMETIC}}}},
    {LOG10, {{{OperandType::ARITHMETIC}}}},
    {EXP,   {{{OperandType::ARITHMETIC}}}},

    // Модуль
    {ABS,  {{{OperandType::ARITHMETIC}}}},
    {FABS, {{{OperandType::ARITHMETIC}}}},

    // Сравнения
    {EQ,  {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC},
            {OperandType::LOGICAL, OperandType::LOGICAL}}}},

    {NEQ, {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC},
            {OperandType::LOGICAL, OperandType::LOGICAL}}}},

    {LT,  {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {GT,  {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {LE,  {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},
    {GE,  {{{OperandType::ARITHMETIC, OperandType::ARITHMETIC}}}},

    // Логические
    {LAND, {{{OperandType::LOGICAL, OperandType::LOGICAL}}}},
    {LOR,  {{{OperandType::LOGICAL, OperandType::LOGICAL}}}},
    {LNOT, {{{OperandType::LOGICAL}}}}
};

/// @brief Структура с информацией об операторе
struct OperatorInfo {
    int precedence;   ///< уровень приоритета (чем меньше число, тем выше приоритет)
    bool leftAssoc;   ///< ассоциативность (true — левая, false — правая)
    int arity;        ///< количество операндов (1 или 2)
    string tex;       ///< TeX-представление оператора
};

/// @brief Словарь операторов с информацией о них (приоритет, ассоциативность, арность, TeX-строка)
const map<TokenType, OperatorInfo> operatorInfo = {
    // индексация 
    {ARRAY_INDEX, {1, true, 2, ""}},

    // функции 
    {SIN,   {2, false, 1, "\\sin"}},
    {COS,   {2, false, 1, "\\cos"}},
    {TAN,   {2, false, 1, "\\tan"}},
    {ASIN,  {2, false, 1, "\\arcsin"}},
    {ACOS,  {2, false, 1, "\\arccos"}},
    {ATAN,  {2, false, 1, "\\arctan"}},
    {SQRT,  {2, false, 1, "\\sqrt"}},
    {CBRT,  {2, false, 1, "\\sqrt[3]"}},
    {LOG,   {2, false, 1, "\\log"}},
    {LOG10, {2, false, 1, "\\log_{10}"}},
    {EXP,   {2, false, 1, "\\exp"}},
    {ABS,   {2, false, 1, "\\left|"}},
    {FABS,  {2, false, 1, "\\left|"}},

    // унарные
    {UMINUS, {3, false, 1, "-"}},
    {LNOT,   {3, false, 1, "\\lnot"}},

    // степень 
    {POW, {4, false, 2, "^"}},

    // умножение / деление 
    {MUL, {5, true, 2, "\\cdot"}},
    {DIV, {5, true, 2, "/"}},

    // сложение / вычитание 
    {PLUS,  {6, true, 2, "+"}},
    {MINUS, {6, true, 2, "-"}},

    // сравнения 
    {LT,  {7, true, 2, "<"}},
    {GT,  {7, true, 2, ">"}},
    {LE,  {7, true, 2, "\\le"}},
    {GE,  {7, true, 2, "\\ge"}},
    {EQ,  {7, true, 2, "="}},
    {NEQ, {7, true, 2, "\\neq"}},

    // логические
    {LAND, {8, true, 2, "\\land"}},
    {LOR,  {9, true, 2, "\\lor"}}
};

/// @brief Функция валидации файла конфигурации с параметрами отображения
/// @param filename путь к файлу с параметрами отображения
/// @param config структура для сохранения параметров и их значений
/// @param errors вектор для сбора ошибок
/// @return true при успешной валидации, false при наличии ошибок
bool validateConfigFile(const string& filename, Config& config, vector<Error>& errors);


