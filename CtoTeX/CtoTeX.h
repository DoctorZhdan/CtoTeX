#pragma once
using namespace std;
#include <string>  
#include <map>
#include <set>
#include <vector>
#include <stack>



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
    ErrorCode code = InputFileNotExist; ///< Код ошибки из перечисления
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
    TokenType type = UNKNOWN; ///< тип токена
    string value;            ///< текстовое представление токена
    OperandType operandType = ARITHMETIC; ///< тип операндов (арифметический или логический)
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

    /// @brief Запрет конструктора копирования
    Node(const Node&) = delete;

    /// @brief Запрет оператора присваивания
    Node& operator=(const Node&) = delete;

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
    int precedence = 0;   ///< уровень приоритета (чем меньше число, тем выше приоритет)
    bool leftAssoc = true;   ///< ассоциативность (true — левая, false — правая)
    int arity = 0;        ///< количество операндов (1 или 2)
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

/// @brief Функция чтения входных файлов (выражения и конфигурации)
/// @param exprFilename путь к файлу с выражением
/// @param configFilename путь к файлу конфигурации
/// @param expression строка для сохранения прочитанного выражения
/// @param config объект для сохранения параметров отображения
/// @param errors вектор для сбора ошибок
/// @return true при успешном чтении, false при наличии ошибок
bool readInputFiles(const string& exprFilename, const string& configFilename, string& expression, Config& config, vector<Error>& errors);

/// @brief Функция вывода ошибок из вектора в консоль
/// @param errors вектор с ошибками
void printErrors(const vector<Error>& errors);

/// @brief Функция разбиения строки выражения на токены
/// @param expression строка с выражением в обратной польской записи
/// @param tokens вектор для заполнения токенами
/// @param errors вектор для сбора ошибок
/// @return true при успешной токенизации, false при наличии ошибок
bool tokenizeExpression(const string& expression, vector<Token>& tokens, vector<Error>& errors);

/// @brief Функция построения дерева разбора из строки выражения
/// @param expression строка с выражением в обратной польской записи
/// @param root указатель на корень построенного дерева
/// @param operatorInfo словарь операторов с информацией о них 
/// @param errors вектор для сбора ошибок
/// @return true при успешном построении дерева, false при наличии ошибок
bool buildTree(const string& expression, Node*& root, const map<TokenType, OperatorInfo>& operatorInfo, vector<Error>& errors);

/// @brief Функция сохранения TeX-строки в выходной файл
/// @param filename путь к выходному файлу
/// @param texString сгенерированная TeX-строка
/// @param errors вектор для сбора ошибок
/// @return true при успешной записи, false при наличии ошибок
bool saveToOutFile(const string& filename, const string& texString, vector<Error>& errors);

/// @brief Функция определения необходимости добавления скобок вокруг дочернего узла
/// @param parent указатель на текущий узел (текущая операция)
/// @param child указатель на дочерний узел (потомок, для которого проверяем)
/// @param isRightChild true, если потомок является правым, false — если левым
/// @return true, если вокруг строки дочернего узла необходимо добавить круглые скобки, false — если скобки не требуются
bool needsParentheses(Node* parent, Node* child, bool isRightChild);

/// @brief Функция получения TeX-строки дочернего узла с добавлением скобок при необходимости
/// @param parent указатель на текущий узел (текущая операция)
/// @param child указатель на дочерний узел (потомок, для которого получаем строку)
/// @param isRightChild true, если потомок является правым, false — если левым
/// @param config объект структуры Config, содержащий параметры отображения
/// @return TeX-строка дочернего узла, дополненная круглыми скобками при необходимости
string getChildTexWithParens(Node* parent, Node* child, bool isRightChild, const Config& config);

/// @brief Функция проверки, является ли поддерево цепочкой умножений одинаковых операндов
/// @param node указатель на текущий узел дерева (проверяемое поддерево)
/// @param operandStr строка для сохранения строкового представления операнда
/// @param varCount целое число для сохранения количества одинаковых операндов в цепочке умножений
/// @param config объект структуры Config, содержащий параметры отображения
/// @return true, если поддерево является цепочкой умножений одинаковых операндов, иначе false
bool isMulIdenVar(Node* node, string& operandStr, int& varCount, const Config& config);

/// @brief Функция проверки, является ли узел делением двух логарифмов с одинаковым основанием
/// @param node указатель на текущий узел дерева (оператор деления)
/// @param base строка для сохранения основания логарифма (аргумент логарифма из знаменателя)
/// @param argument строка для сохранения аргумента логарифма (аргумент логарифма из числителя)
/// @param config объект структуры Config, содержащий параметры отображения
/// @return true, если узел является делением двух логарифмов с одинаковым основанием, иначе false
bool isLogDiv(Node* node, string& base, string& argument, const Config& config);

/// @brief Функция проверки, является ли поддерево цепочкой операций с элементами массива
/// @param node указатель на текущий узел дерева (проверяемое поддерево)
/// @param opType ожидаемый тип оператора (MUL для умножения, PLUS для сложения)
/// @param arrayName строка для сохранения имени массива (выходной параметр)
/// @param indexes вектор для сбора индексов элементов массива 
/// @param arrayNameFound флаг, указывающий, было ли уже найдено имя массива (для проверки совпадения)
/// @return true, если поддерево является цепочкой операций opType с элементами массива, иначе false
bool isArrOperation(Node* node, TokenType opType, string& arrayName, vector<int>& indexes, bool& arrayNameFound);

/// @brief Рекурсивная функция генерации TeX-строки из дерева выражения
/// @param node указатель на текущий узел дерева, для которого генерируется TeX-строка
/// @param parentType тип операции родительского узла (значение UNKNOWN, если узел – корень)
/// @param isRightChild флаг, указывающий, является ли текущий узел правым потомком (true) или левым/корнем (false)
/// @param config объект структуры Config, содержащий параметры отображения 
/// @return сгенерированная TeX-строка
string cToTex(Node* node, TokenType parentType, bool isRightChild, const Config& config);




/// @brief Проверяет, является ли слово оператором или функцией, и при необходимости добавляет соответствующий токен
/// @param word текущее слово (лексема), выделенное из входного выражения
/// @param tokens вектор токенов, в который добавляется результат разбора
/// @param nodeCount счётчик узлов (увеличивается при добавлении токена)
/// @return true, если слово распознано как оператор/функция; false в противном случае
bool parseOperator(const string& word, vector<Token>& tokens, int& nodeCount);

/// @brief Проверяет корректность расположения операторов в слове 
/// @param word текущее слово 
/// @param errors вектор ошибок, в который добавляются найденные ошибки
/// @param allowedOperations множество допустимых операторов
/// @return true, если слово содержит ошибку, связанную с операторами; false в противном случае
bool checkOperatorSpacing(const string& word, vector<Error>& errors, const set<string>& allowedOperations);

/// @brief Проверяет, является ли слово числом и добавляет токен
/// @param word текущее слово 
/// @param tokens вектор токенов 
/// @param errors вектор ошибок
/// @param nodeCount счётчик узлов 
/// @return true, если слово обработано как число, иначе false
bool parseNumber(const string& word, vector<Token>& tokens, vector<Error>& errors, int& nodeCount);

/// @brief Проверяет, является ли слово переменной и добавляет токен
/// @param word текущее слово 
/// @param tokens вектор токенов 
/// @param errors вектор ошибок 
/// @param nodeCount счётчик узлов 
/// @return true, если слово обработано как переменная, иначе false
bool parseVariable(const string& word, vector<Token>& tokens, vector<Error>& errors, int& nodeCount);

/// @brief Проверяет, является ли слово константой и добавляет токен
/// @param word текущее слово
/// @param tokens вектор токенов
/// @param nodeCount счётчик узлов
/// @return true, если слово обработано как константа, иначе false
bool parseConstant(const string& word, vector<Token>& tokens, int& nodeCount);

/// @brief Проверяет, является ли слово индексацией массива и добавляет соответствующие токены
/// @param word текущее слово 
/// @param tokens вектор токенов 
/// @param errors вектор ошибок
/// @param nodeCount счётчик узлов 
/// @return true, если слово обработано как элемент массива, иначе false
bool parseArray(const string& word, vector<Token>& tokens, vector<Error>& errors, int& nodeCount);

/// @brief Разбивает строку выражения на слова по пробелам
/// @param expression входная строка
/// @param wordList вектор для заполнения словами 
void splitIntoWords(const string& expression, vector<string>& wordList);


/// @brief Обрабатывает бинарный оператор: извлекает два операнда из стека, проверяет типы, создаёт узел
/// @param token токен оператора
/// @param nodeStack стек узлов
/// @param errors вектор ошибок
/// @return указатель на новый узел или nullptr при ошибке
Node* processBinaryOperator(const Token& token, stack<Node*>& nodeStack, vector<Error>& errors);

/// @brief Обрабатывает унарный оператор: извлекает один операнд из стека, проверяет тип, создаёт узел
/// @param token токен оператора
/// @param nodeStack стек узлов
/// @param errors вектор ошибок
/// @return указатель на новый узел или nullptr при ошибке
Node* processUnaryOperator(const Token& token, stack<Node*>& nodeStack, vector<Error>& errors);

/// @brief Обрабатывает токен-операнд (число, переменная, константа)
/// @param token токен для обработки
/// @param nodeStack стек узлов
/// @param nodeCount счётчик узлов
/// @param errors вектор ошибок
void processOperand(const Token& token, stack<Node*>& nodeStack, int& nodeCount, vector<Error>& errors);

/// @brief Проверяет состояние стека после обработки всех токенов
/// @param nodeStack стек узлов
/// @param errors вектор ошибок
void checkStackState(stack<Node*>& nodeStack, vector<Error>& errors);


/// @brief Сравнивает приоритеты parent и child, определяет необходимость скобок
/// @param parentPrec приоритет родительской операции
/// @param childPrec приоритет дочерней операции
/// @param parentLeftAssoc ассоциативность родительской операции
/// @param isRightChild флаг правого потомка
/// @return true если скобки нужны, false если нет
bool comparePrecedence(int parentPrec, int childPrec, bool parentLeftAssoc, bool isRightChild);


/// @brief Преобразует имя константы в TeX-представление
/// @param value имя константы 
/// @return TeX-представление константы
string getConstantTeX(const string& value);

/// @brief Генерирует TeX-представление тригонометрической функции
/// @param node указатель на узел тригонометрической функции
/// @param config объект с настройками отображения
/// @return TeX-строка тригонометрической функции
string getTrigFunctionTeX(Node* node, const Config& config);

/// @brief Генерирует TeX-представление логарифма
/// @param node указатель на узел логарифма
/// @param config объект с настройками отображения
/// @return TeX-строка логарифма 
string getLogFunctionTeX(Node* node, const Config& config);

/// @brief Генерирует TeX для тригонометрической функции, возведённой в степень
/// @param node указатель на узел POW (левый потомок — тригонометрическая функция)
/// @param config объект с настройками отображения
/// @return TeX-строка 
string getTrigPowerTeX(Node* node, const Config& config);

/// @brief Генерирует TeX для дробной степени 
/// @param node указатель на узел POW
/// @param config объект с настройками отображения
/// @return TeX-строка
string* getFractionPowerTeX(Node* node, const Config& config, TokenType type);

/// @brief Генерирует TeX для положительной степени
/// @param node указатель на узел POW
/// @param config объект с настройками отображения
/// @return TeX-строка 
string getRegularPowerTeX(Node* node, const Config& config, TokenType type);

/// @brief Возвращает TeX-имя тригонометрической функции
/// @param type тип узла
/// @return TeX-имя функции
string getTrigFunctionName(TokenType type);

/// @brief Извлекает числитель и знаменатель из узла-деления в показателе степени
/// @param node указатель на узел POW
/// @param numerator выходной параметр для числителя
/// @param denominator выходной параметр для знаменателя
/// @return true, если правый потомок является делением двух чисел, иначе false
bool getExponentFraction(Node* node, string& numerator, string& denominator);

