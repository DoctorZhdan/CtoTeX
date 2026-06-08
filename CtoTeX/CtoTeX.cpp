
#include "../CtoTeX/CtoTeX.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


bool validateConfigFile(const string& filename, Config& config, vector<Error>& errors) {
    // 1. Открыть файл
    ifstream file(filename);

    // 2. Если не удалось открыть файл
    if (!file.is_open())
    {
        // 2.1. Добавить ошибку InputFileNotExist в вектор ошибок и вернуть false
        Error err;
        err.code = InputFileNotExist;
        err.line = filename;
        errors.push_back(err);
        return false;
    }

    // 3. Создать пустое множество для хранения имён параметров, которые уже встречались
    set<string> processedParams;

    string line;
    int lineNum = 0;

    // 4. Для каждой строки файла
    while (getline(file, line))
    {
        lineNum++;

        // 4.1. Если строка пустая
        if (line.empty())
        {
            // 4.1.1. Добавить ошибку EmptyLine в вектор ошибок
            Error err;
            err.code = EmptyLine;
            err.line = line;
            err.position = 0;
            errors.push_back(err);
        }
        else {
            // 4.2. Разбить строку на два слова (по пробелу)
            stringstream lineStream(line);
            string param, value, extra;
            lineStream >> param >> value;

            // 4.3. Если в строке меньше 2 слов
            if (param.empty() || value.empty())
            {
                // 4.3.1. Добавить ошибку MissingParamValue в вектор ошибок
                Error err;
                err.code = MissingParamValue;
                err.line = line;
                errors.push_back(err);
            }
            // 4.4. Если в строке больше 2 слов
            else if (lineStream >> extra)
            {
                // 4.4.1. Добавить ошибку TooManyWords в вектор ошибок
                Error err;
                err.code = TooManyWords;
                err.line = line;
                errors.push_back(err);
            }
            // 4.5. Если первое слово (параметр) не найдено в словаре допустимых значений параметров allowedConfigParams
            else if (allowedConfigParams.find(param) == allowedConfigParams.end())
            {
                // 4.5.1. Добавить ошибку NonexistentParam в вектор ошибок
                Error err;
                err.code = NonexistentParam;
                err.line = line;
                errors.push_back(err);
            }
            // 4.6. Если второе слово (значение) не входит в набор допустимых значений для этого параметра
            else {
                const set<string>& allowedValues = allowedConfigParams.at(param);
                if (allowedValues.find(value) == allowedValues.end())
                {
                    // 4.6.1. Добавить ошибку InvalidParamValue в вектор ошибок
                    Error err;
                    err.code = InvalidParamValue;
                    err.line = line;
                    errors.push_back(err);
                }
                // 4.7. Если данный параметр уже содержит значение
                else if (processedParams.find(param) != processedParams.end())
                {
                    // 4.7.1. Добавить ошибку DuplicateParamValue в вектор ошибок
                    Error err;
                    err.code = DuplicateParamValue;
                    err.line = line;
                    errors.push_back(err);
                }
                else {
                    // 4.8. Добавить параметр в множество обработанных параметров
                    processedParams.insert(param);
                    // 4.9. Сохранить в словаре параметров отображения значение, присвоенное данному параметру
                    config.paramMap[param] = value;
                }
            }
        }
    }

    // 5. Закрыть файл 
    file.close();

    // 5. Если вектор ошибок пуст, вернуть true
    if (errors.empty()) {
        return true;
    }

    // 5.1. Иначе вернуть false и заполненный вектор с ошибками (errors)
    return false;
}

bool readInputFiles(const string& exprFilename, const string& configFilename, string& expression, Config& config, vector<Error>& errors) {
    // 1. Открыть файл с выражением
    ifstream exprFile(exprFilename);

    // 2. Добавить  ошибку InputFileNotExist в вектор ошибок, если не удалось открыть файл с выражением 
    if (!exprFile.is_open())
    {
        Error err;
        err.code = InputFileNotExist;
        err.line = exprFilename;
        errors.push_back(err);
        return false;
    }

    // Проверка, что в файле больше одной строки
    vector<string> lines;
    string line;
    while (getline(exprFile, line))
    {
        lines.push_back(line);
    }
    exprFile.close();

    //Добавить ошибку MultipleLinesInTreeFile в вектор ошибок, если в файле больше одной строки 
    if (lines.size() != 1) {
        Error err;
        err.code = MultipleLinesInTreeFile;
        errors.push_back(err);
        return false;
    }

    // 3. Сохранить выражение
    expression = lines[0];

    // 4. Открыть файл конфигурации и сохранить настройки конфигурации 
    if (!validateConfigFile(configFilename, config, errors)) {
        return false;
    }

    // 5. Вернуть успех, если ошибок нет
    return errors.empty();
}

void printErrors(const vector<Error>& errors) {
    // 1. Если вектор ошибок пуст
    if (errors.empty()) {
        // 1.1. Завершить выполнение
        return;
    }

    // 2. Для каждой ошибки из вектора ошибок
    for (const auto& err : errors) {
        // 2.1. Сгенерировать сообщение об ошибке (generateErrorMessage)
        string msg = err.generateErrorMessage();

        // 2.2. Вывести сообщение в консоль
        cout << msg;

        // 2.3. Вывести символ перевода строки
        cout << endl;
    }
}

bool tokenizeExpression(const string& expression, vector<Token>& tokens, vector<Error>& errors)
{
    // 1. Разбить строку expression на слова по пробелам, сохранить в список слов (wordList)
    vector<string> wordList;
    string currentWord = "";
    for (int i = 0; i < expression.size(); i++)
    {
        if (expression[i] == ' ')
        {
            if (!currentWord.empty())
            {
                wordList.push_back(currentWord);
                currentWord = "";
            }
        }
        else {
            currentWord += expression[i];
        }
    }
    if (!currentWord.empty())
    {
        wordList.push_back(currentWord);
    }

    // 2. Установить нулевое значение счётчика узлов (nodeCount = 0)
    int nodeCount = 0;

    // 3. Для каждого слова из списка слов (wordList):
    for (int i = 0; i < wordList.size(); i++)
    {
        string word = wordList[i];
        bool processed = false; // processed - флаг того, что слово уже распознано как что-то допустимое

        // Посчитать количество оператороов в слове
        int opCounter = 0;
        for (const string& op : allowedOperations) {
            size_t pos = 0;
            while ((pos = word.find(op, pos)) != string::npos) {
                opCounter++;
                pos += op.size();
            }
        }

        // Специальная проверка для унарного минуса
        if (word == "-_") {
            if (errors.empty()) {
                Token t;
                t.type = UMINUS;
                t.value = "-_";
                t.operandType = ARITHMETIC;
                tokens.push_back(t);
                nodeCount++;
            }
            processed = true;
        }

        // Специальная проверка для оператора неравенства
        if (word == "!=") {
            if (errors.empty()) {
                Token t;
                t.type = NEQ;
                t.value = "!=";
                t.operandType = LOGICAL;
                tokens.push_back(t);
                nodeCount++;
            }
            processed = true;
        }

        // Специальная проверка для оператора меньше или равно
        if (word == "<=") {
            if (errors.empty()) {
                Token t;
                t.type = LE;
                t.value = "<=";
                t.operandType = ARITHMETIC;
                tokens.push_back(t);
                nodeCount++;
            }
            processed = true;
        }

        // Специальная проверка для оператора больше или равно
        if (word == ">=") {
            if (errors.empty()) {
                Token t;
                t.type = GE;
                t.value = ">=";
                t.operandType = ARITHMETIC;
                tokens.push_back(t);
                nodeCount++;
            }
            processed = true;
        }

        // Специальная проверка для десятичного логарифма
        if (word == "#log10") {
            if (errors.empty()) {
                Token t;
                t.type = LOG10;
                t.value = "#log10";
                t.operandType = ARITHMETIC;
                tokens.push_back(t);
                nodeCount++;
            }
            processed = true;
        }

        // 3.1 Если слово содержит содержит более одного оператора или слово содержит 1 оператор из списка допустимых операторов, но всё слово не является оператором 
        if (!processed && (opCounter > 1 || (opCounter == 1 && allowedOperations.find(word) == allowedOperations.end())))
        {
            // Проверить, не является ли слово отрицательным числом
            bool isNegativeNumber = false;

            if (word.size() > 1 && word[0] == '-')
            {
                isNegativeNumber = true;
                bool dotFound = false;
                for (int j = 1; j < word.size(); j++)
                {
                    if (isdigit(word[j])) {}
                    else if (word[j] == '.' && !dotFound)
                    {
                        dotFound = true;
                    }
                    else
                    {
                        isNegativeNumber = false;
                    }
                }
            }

            if (!isNegativeNumber)
            {

                // 3.1.1 Занести соответствующую ошибку в вектор ошибок
                Error err;
                err.code = (opCounter > 1) ? InvalidSymbolSequence : OperatorNotSeparatedBySpaces;
                err.position = 0;
                err.length = word.size();
                err.line = word;
                errors.push_back(err);

                // 3.1.2 Перейти к обработке следующего слова
                processed = true;
            }
        }

        // 3.2. Если слово входит в список допустимых констант (allowedConstants): 
        if (!processed && allowedConstants.find(word) != allowedConstants.end())
        {
            // 3.2.1 Если вектор ошибок пуст
            if (errors.empty())
            {
                Token t;
                t.type = CONSTANT;
                t.value = word;

                // 3.2.1.1 Если слово совпадает с "true" или "false", создать токен типа CONSTANT и установить логическое значение типа операнда (operandType = LOGICAL)
                if (word == "true" || word == "false")
                {
                    t.operandType = LOGICAL;
                }
                // 3.2.1.2 Иначе создать токен типа CONSTANT и установить арифметическое значение типа операнда (operandType = ARITHMETIC)
                else
                {
                    t.operandType = ARITHMETIC;
                }

                // 3.2.1.3 Добавить созданный токен в вектор токенов (tokens)
                tokens.push_back(t);

                // 3.2.1.4 Инкрементировать значение счётчика узлов
                nodeCount++;
            }
            // 3.2.2 Перейти к обработке следующего слова
            processed = true;
        }

        // 3.3. Если слово совпадает с любым оператором из списка допустимых операторов (allowedOperations)
        if (!processed && allowedOperations.find(word) != allowedOperations.end())
        {
            // 3.3.1 Если вектор ошибок пуст
            if (errors.empty())
            {
                Token t;
                t.value = word;
                t.operandType = ARITHMETIC;

                // 3.3.1.1 Создать токен с соответствующим типом оператора
                if (word == "+") t.type = PLUS;
                else if (word == "-") t.type = MINUS;
                else if (word == "*") t.type = MUL;
                else if (word == "/") t.type = DIV;
                else if (word == "-_") t.type = UMINUS;
                else if (word == "#pow") t.type = POW;
                else if (word == "#sqrt") t.type = SQRT;
                else if (word == "#cbrt") t.type = CBRT;
                else if (word == "#sin") t.type = SIN;
                else if (word == "#cos") t.type = COS;
                else if (word == "#tan") t.type = TAN;
                else if (word == "#asin") t.type = ASIN;
                else if (word == "#acos") t.type = ACOS;
                else if (word == "#atan") t.type = ATAN;
                else if (word == "#log") t.type = LOG;
                else if (word == "#log10") t.type = LOG10;
                else if (word == "#exp") t.type = EXP;
                else if (word == "#abs") t.type = ABS;
                else if (word == "#fabs") t.type = FABS;
                else if (word == "==") t.type = EQ;
                else if (word == "!=") t.type = NEQ;
                else if (word == "<") t.type = LT;
                else if (word == ">") t.type = GT;
                else if (word == "<=") t.type = LE;
                else if (word == ">=") t.type = GE;
                else if (word == "&&") { t.type = LAND; t.operandType = LOGICAL; }
                else if (word == "||") { t.type = LOR; t.operandType = LOGICAL; }
                else if (word == "!") { t.type = LNOT; t.operandType = LOGICAL; }
                else t.type = UNKNOWN;

                // 3.3.1.2 Добавить созданный токен в вектор токенов (tokens)
                tokens.push_back(t);

                // 3.3.1.3 Инкрементировать значение счётчика узлов
                nodeCount++;
            }
            // 3.3.2 Перейти к обработке следующего слова
            processed = true;
        }

        // 3.4 Если слово содержит '[' и ']'
        if (!processed && word.find('[') != string::npos && word.find(']') != string::npos)
        {
            // 3.4.1 Найти позиции '[' и ']'
            size_t l = word.find('[');
            size_t r = word.find(']');

            // 3.4.2 Проверка корректности
            if (r == string::npos || l > r || r != word.size() - 1)
            {
                // 3.4.2.1 ошибка
                errors.push_back({ InvalidSymbolSequence, 0, (int)word.size(), word });
                // 3.4.2.2 перейти дальше
                processed = true;

            }
            else 
            {

                // 3.4.3 Имя массива
                string name = word.substr(0, l);

                // 3.4.4 Индекс
                string indexStr = word.substr(l + 1, r - l - 1);

                // 3.4.5 Проверка имени массива
                if (name.empty() || !isalpha(name[0]))
                {
                    errors.push_back({ InvalidSymbol, 0, (int)name.size(), name });
                }

                for (int j = 1; j < name.size(); j++)
                {
                    if (!isalnum(name[j]))
                    {
                        errors.push_back({ InvalidSymbol, j, 1, name });
                    }
                }

                // 3.4.6 Проверка индекса
                for (char c : indexStr)
                {
                    if (!isdigit(c))
                    {
                        errors.push_back({ InvalidSymbolSequence, 0, (int)indexStr.size(), indexStr });
                    }
                }

                // 3.4.7 Если ошибок нет
                if (errors.empty())
                {
                    // Токен для имени массива
                    Token arrayToken;
                    arrayToken.type = VARIABLE;
                    arrayToken.value = name;
                    arrayToken.operandType = ARITHMETIC;
                    tokens.push_back(arrayToken);

                    // Токен для индекса
                    Token indexToken;
                    indexToken.type = NUMBER;
                    indexToken.value = indexStr;
                    indexToken.operandType = ARITHMETIC;
                    tokens.push_back(indexToken);

                    // Токен для оператора индексации
                    Token indexOpToken;
                    indexOpToken.type = ARRAY_INDEX;
                    indexOpToken.value = "a[i]";
                    indexOpToken.operandType = ARITHMETIC;
                    tokens.push_back(indexOpToken);

                    // 3.4.7.5 увеличить счётчик
                    nodeCount += 3;
                }

                // 3.4.8 перейти дальше
                processed = true;
            }
        }

        // 3.5. Если первый символ слова – буква:
        if (!processed && isalpha(word[0]))
        {
            // 3.5.1 Для каждого символа слова, начиная с позиции 1:
            for (size_t j = 1; j < word.size(); j++)
            {
                // 3.5.1.1 Если символ является буквой или цифрой, перейти к следующему символу
                if (!(isalnum(word[j])))
                {
                    // 3.5.1.2 В противном случае считать символ недопустимым и занести соответствующую ошибку в вектор ошибок
                    errors.push_back({ InvalidSymbol, (int)j, 1, word });
                }
                // 3.5.1.3 Перейти к следующему символу
            }

            // 3.5.2 Если длина слова превышает 255 символов, занести соответствующую ошибку в вектор ошибок
            if (word.size() > 255)
            {
                errors.push_back({ VariableNameTooLong, -1, (int)word.size(), word });
            }

            // 3.5.3 Если вектор ошибок пуст
            if (errors.empty())
            {
                Token t{ VARIABLE, word, ARITHMETIC };

                // 3.5.3.2 Добавить созданный токен в вектор токенов (tokens)
                tokens.push_back(t);

                // 3.5.3.3 Инкрементировать значение счётчика узлов
                nodeCount++;
            }

            // 3.5.4 Перейти к обработке следующего слова
            processed = true;
        }

        // 3.6. Если первый символ слова – цифра или если длина слова больше 1 и первый символ – минус, а второй – цифра  
        bool isNumberStart = isdigit(word[0]) || (word.size() > 1 && word[0] == '-' && isdigit(word[1]));
        if (!processed && isNumberStart)
        {
            // 3.6.1 Считать, что точка ещё не встречалась в слове
            bool dotFound = false;
            int dotPos = -1;

            // 3.6.2 Для каждого символа слова, начиная с позиции 1:
            for (size_t k = 1; k < word.size(); k++)
            {
                // 3.6.2.1 Если символ является цифрой, перейти к следующему символу
                if (isdigit(word[k]))
                {
                }
                // 3.6.2.2 Если символ является точкой, и точка ещё не встречалась в слове, запомнить местоположение точки и перейти к следующему символу
                else if (word[k] == '.' && !dotFound)
                {
                    dotFound = true;
                    dotPos = k;
                }
                // 3.6.2.3 В противном случае считать символ недопустимым и занести соответствующую ошибку в вектор ошибок
                else {
                    errors.push_back({ InvalidSymbolSequence, (int)k, 1, word });
                }
                // 3.6.2.4 Перейти к следующему символу
            }

            // 3.6.3 Если в слове есть точка
            if (dotFound)
            {
                // 3.6.3.1 Если число символов после точки превышает 8, занести соответствующую ошибку в вектор ошибок
                int afterSym = word.size() - dotPos - 1;
                if (afterSym > 8)
                {
                    errors.push_back({ TooManyDecimalDigits, dotPos + 1, afterSym, word });
                }
            }

            // 3.6.4 Если число не входит в диапазон [-2*10^9, 2*10^9], занести соответствующую ошибку в вектор ошибок
            try {
                double val = stod(word);
                if (val < -2e9 || val > 2e9)
                {
                    errors.push_back({ IntegerOverflow, -1, (int)word.size(), word });
                }
            }
            catch (...) {
                errors.push_back({ InvalidSymbolSequence, -1, (int)word.size(), word });
            }

            // 3.6.5 Если вектор ошибок пуст
            if (errors.empty())
            {
                Token t{ NUMBER, word, ARITHMETIC };

                // 3.6.5.2 Добавить созданный токен в вектор токенов (tokens)
                tokens.push_back(t);

                // 3.6.5.3 Инкрементировать значение счётчика узлов
                nodeCount++;
            }

            // 3.6.6 Перейти к обработке следующего слова
            processed = true;
        }

        // 3.7 В противном случае считать, что слово начинается с недопустимого символа
        if (!processed)
        {
            // 3.8 Занести соответствующую ошибку в вектор ошибок
            errors.push_back({ InvalidSymbol, 0, (int)word.size(), word });

            // 3.9 Перейти к обработке следующего слова
        }
    }

    // 4. Если вектор ошибок пуст, вернуть true и заполненный вектор с токенами (tokens)
    if (errors.empty())
    {
        return true;
    }
    // 4.1. Иначе вернуть false и заполненный вектор с ошибками (errors)
    else {
        return false;
    }
}

bool buildTree(const string& expression, Node*& root, const map<TokenType, OperatorInfo>& operatorInfo, vector<Error>& errors) {
    // 1. Разбить строку на токены (tokenizeExpression)
    vector<Token> tokens;
    if (!tokenizeExpression(expression, tokens, errors)) {
        return false;
    }

    // 2. Создать пустой стек узлов nodeStack
    stack<Node*> nodeStack;

    // 3. Установить nodeCount = 0
    int nodeCount = 0;

    // 4. Для каждого токена token из вектора токенов tokens:
    for (const Token& token : tokens) {

        // 4.1. Если токен имеет тип NUMBER или VARIABLE или CONSTANT (токен является операндом):
        if (token.type == NUMBER || token.type == VARIABLE || token.type == CONSTANT) {

            // 4.1.1. Создать новый узел Node с этим токеном
            Node* newNode = new Node(token);

            // 4.1.2. Положить узел в nodeStack
            nodeStack.push(newNode);

            // 4.1.3. Увеличить nodeCount на 1
            nodeCount++;

            // 4.1.4. Если nodeCount > 1000
            if (nodeCount > 1000) {
                // 4.1.4.1. Добавить ошибку TooManyNodes в вектор ошибок
                Error err;
                err.code = TooManyNodes;
                errors.push_back(err);
            }
        }

        // 4.2. Если токен имеет тип какого-либо оператора (токен является оператором):
        else {

            // 4.2.1. Получить арность оператора из operatorInfo
            auto it = operatorInfo.find(token.type);

            int arity = 0;
            bool operatorFound = (it != operatorInfo.end());

            if (operatorFound) {
                arity = it->second.arity;
            }
            else {
                Error err;
                err.code = InvalidSymbol;
                err.line = expression;
                errors.push_back(err);
            }

            Node* newNode = nullptr;

            // 4.2.2. Если арность равна 2:
            if (operatorFound && arity == 2) {

                // 4.2.2.1. Если в nodeStack меньше 2 узлов
                if (nodeStack.size() < 2) {
                    // 4.2.2.1.1. Добавить ошибку NotEnoughOperands в вектор ошибок
                    Error err;
                    err.code = NotEnoughOperands;
                    errors.push_back(err);
                }
                else {
                    // 4.2.2.2. Извлечь из стека right узел
                    Node* right = nodeStack.top();
                    nodeStack.pop();

                    // 4.2.2.3. Извлечь из стека left узел
                    Node* left = nodeStack.top();
                    nodeStack.pop();

                    // 4.2.2.4. Если типы операндов и операции не совместимы между собой
                    auto opIt = operationTypes.find(token.type);

                    if (opIt != operationTypes.end()) {
                        bool compatible = false;

                        for (const auto& sig : opIt->second.allowedSignatures) {
                            if (sig.size() == 2) {
                                if (sig[0] == left->token.operandType &&
                                    sig[1] == right->token.operandType) {
                                    compatible = true;
                                }
                            }
                        }

                        if (!compatible) {
                            // 4.2.2.4.1. Добавить ошибку MismatchedOperandTypes в вектор ошибок
                            Error err;
                            err.code = MismatchedOperandTypes;
                            errors.push_back(err);
                        }
                    }

                    // 4.2.2.5. Создать новый узел с токеном, установить left и right
                    newNode = new Node(token, left, right);
                }
            }

            // 4.2.3. Если арность равна 1:
            else if (operatorFound && arity == 1) {

                // 4.2.3.1. Если в nodeStack меньше 1 узла
                if (nodeStack.size() < 1) {
                    // 4.2.3.1.1. Добавить ошибку NotEnoughOperands в вектор ошибок
                    Error err;
                    err.code = NotEnoughOperands;
                    errors.push_back(err);
                }
                else {
                    // 4.2.3.2. Извлечь из стека child узел
                    Node* child = nodeStack.top();
                    nodeStack.pop();

                    // 4.2.3.3. Если типы операнда и операции несовместимы
                    auto opIt = operationTypes.find(token.type);

                    if (opIt != operationTypes.end()) {
                        bool compatible = false;

                        for (const auto& sig : opIt->second.allowedSignatures) {
                            if (sig.size() == 1) {
                                if (sig[0] == child->token.operandType) {
                                    compatible = true;
                                }
                            }
                        }

                        if (!compatible) {
                            // 4.2.3.3.1. Добавить ошибку MismatchedOperandTypes в вектор ошибок
                            Error err;
                            err.code = MismatchedOperandTypes;
                            errors.push_back(err);
                        }
                    }

                    // 4.2.3.4. Создать новый узел с токеном, установить left = child, right = nullptr
                    newNode = new Node(token, child);
                }
            }

            // 4.2.4. Положить новый узел в nodeStack
            if (newNode != nullptr) {
                nodeStack.push(newNode);

                // 4.2.5. Увеличить nodeCount на 1
                nodeCount++;

                // 4.2.6. Если nodeCount больше 1000
                if (nodeCount > 1000) {
                    // 4.2.6.1. Добавить ошибку TooManyNodes в вектор ошибок
                    Error err;
                    err.code = TooManyNodes;
                    errors.push_back(err);
                }
            }
        }
    }

    // 5. Если nodeStack пуст 
    if (nodeStack.empty()) {
        // 5.1. Добавить ошибку NotEnoughOperands в вектор ошибок
        Error err;
        err.code = NotEnoughOperands;
        errors.push_back(err);
    }

    // 6. Если в nodeStack больше одного узла
    if (nodeStack.size() > 1) {
        // 6.1. Добавить ошибку TooManyOperands в вектор ошибок
        Error err;
        err.code = TooManyOperands;
        errors.push_back(err);
    }

    // 7. Извлечь единственный узел из стека и сохранить в root
    if (!nodeStack.empty()) {
        root = nodeStack.top();
        nodeStack.pop();
    }

    // 8. Если вектор ошибок (errors) не пуст 
    if (!errors.empty()) {
        // 8.1. Вернуть false
        return false;
    }
    // 8.2. Иначе вернуть true
    return true;
}

bool saveToOutFile(const string& filename, const string& texString, vector<Error>& errors) {
    // 1. Открыть файл по пути filename для записи
    ofstream outFile(filename);

    // 2. Если файл не удалось открыть
    if (!outFile.is_open()) {
        // 2.1. Добавить ошибку OutputFileNotCreate в вектор errors
        Error err;
        err.code = OutputFileNotCreate;
        err.line = filename;
        errors.push_back(err);
        // 2.2. Вернуть false
        return false;
    }

    // 3. Записать строку texString в файл
    outFile << texString;

    // 4. Если запись не удалась
    if (outFile.fail())
    {
        // 4.1. Добавить ошибку OutputFileNotCreate в вектор errors
        Error err;
        err.code = OutputFileNotCreate;
        err.line = filename;
        errors.push_back(err);
        // 4.2. Вернуть false
        outFile.close();
        return false;
    }

    // 5. Закрыть файл
    outFile.close();

    // 6. Вернуть true
    return true;
}

bool needsParentheses(Node* parent, Node* child, bool isRightChild) {
    // 1. Если child является листом (NUMBER, VARIABLE, CONSTANT)
    TokenType childType = child->token.type;
    if (childType == NUMBER || childType == VARIABLE || childType == CONSTANT) {
        // 1.1. Вернуть false
        return false;
    }

    // 2. Если child является унарным минусом (UMINUS)
    if (childType == UMINUS) {
        // 2.1. Вернуть true
        return true;
    }

    TokenType parentType = parent->token.type;

    // 3. Если parent является тригонометрической функцией (SIN, COS, TAN, ASIN, ACOS, ATAN)
    if (parentType == SIN || parentType == COS || parentType == TAN ||
        parentType == ASIN || parentType == ACOS || parentType == ATAN) {
        // 3.1. Вернуть true
        return true;
    }

    // 4. Если parent является логарифмом (LOG, LOG10)
    if (parentType == LOG || parentType == LOG10) {
        // 4.1. Вернуть true
        return true;
    }

    // Получить информацию о приоритетах из operatorInfo
    auto parentIt = operatorInfo.find(parentType);
    auto childIt = operatorInfo.find(childType);

    // Если информация не найдена, скобки не ставим
    if (parentIt == operatorInfo.end() || childIt == operatorInfo.end()) {
        return false;
    }

    int parentPrec = parentIt->second.precedence;
    int childPrec = childIt->second.precedence;
    bool parentLeftAssoc = parentIt->second.leftAssoc;

    // 5. Если приоритет потомка выше
    if (childPrec < parentPrec) {
        // 5.1. Вернуть false
        return false;
    }

    // 6. Если приоритет потомка ниже
    if (childPrec > parentPrec) {
        // 6.1. Вернуть true
        return true;
    }

    // 7. Если приоритеты равны
    // 7.1. Если текущая операция левоассоциативная (parentLeftAssoc == true)
    if (parentLeftAssoc)
    {
        // 7.1.1. Вернуть isRightChild
        return isRightChild;
    }
    // 7.2. Если текущая операция правоассоциативная (parentLeftAssoc == false)
    else {
        // 7.2.1. Вернуть !isRightChild
        return !isRightChild;
    }

    // 8. Вернуть false
    return false;
}

string getChildTexWithParens(Node* parent, Node* child, bool isRightChild, const Config& config) {
    // 1. Получить TeX-строку дочернего узла (cToTex)
    string childStr = cToTex(child, parent->token.type, isRightChild, config);

    // 2. Если требуются скобки (needsParentheses)
    if (needsParentheses(parent, child, isRightChild)) {
        // 2.1. Вернуть "(" + childStr + ")"
        return string("(") + childStr + ")";
    }

    // 2.2. Иначе вернуть childStr
    return childStr;
}

bool isMulIdenVar(Node* node, string& operandStr, int& varCount, const Config& config) {
    // 1. Если текущий узел не существует
    if (node == nullptr) {
        // 1.1. Вернуть false
        return false;
    }

    // 2. Если узел является оператором умножения (MUL)
    if (node->token.type == MUL)
    {
        // 2.1. Проверить левого потомка на соответствие шаблону параметра (isMulIdenVar)
        bool leftResult = isMulIdenVar(node->left, operandStr, varCount, config);
        // 2.2. Проверить правого потомка на соответствие шаблону параметра (isMulIdenVar)
        bool rightResult = isMulIdenVar(node->right, operandStr, varCount, config);
        // 2.3. Вернуть true, если оба потомка подходят под шаблон
        return leftResult && rightResult;
    }

    // 3. Если узел является листом (NUMBER, VARIABLE, CONSTANT)
    TokenType type = node->token.type;
    if (type == NUMBER || type == VARIABLE || type == CONSTANT)
    {
        // 3.1. Получить строковое представление узла (cToTex)
        string currentStr = cToTex(node, UNKNOWN, false, config);

        // 3.2. Если строка со строковым отображением узла пустая
        if (operandStr.empty()) {
            // 3.2.1. Считать отображением узла полученное отображение листа
            operandStr = currentStr;
            // 3.2.2. Инкрементировать количество найденных множителей
            varCount++;
        }
        // 3.3. Иначе если текущее отображение равно полученному отображению листа
        else if (operandStr == currentStr) {
            // 3.3.1. Инкрементировать количество найденных множителей
            varCount++;
        }
        // 3.4. Иначе
        else {
            // 3.4.1. Вернуть false
            return false;
        }
        // 3.5. Вернуть true
        return true;
    }

    // 4. Если узел является другим типом (не MUL и не лист)
    // 4.1. Вернуть false
    return false;
}

bool isLogDiv(Node* node, string& base, string& argument, const Config& config) {
    // 1. Если текущий узел не существует
    if (node == nullptr) {
        // 1.1. Вернуть false
        return false;
    }

    // 2. Если узел не является оператором деления (DIV)
    if (node->token.type != DIV) {
        // 2.1. Вернуть false
        return false;
    }

    Node* leftNode = node->left;   // левый потомок (числитель)
    Node* rightNode = node->right; // правый потомок (знаменатель)

    string leftBase;   // основание левого логарифма
    string leftArg;    // аргумент левого логарифма
    string rightBase;  // основание правого логарифма
    string rightArg;   // аргумент правого логарифма

    // 3. Проверить левого потомка: является ли он логарифмом (LOG или LOG10)
    // 3.1. Если левый узел является натуральным логарифмом (LOG)
    if (leftNode->token.type == LOG) {
        // 3.1.1. Получить аргумент логарифма (левый потомок узла с логарифмом)
        leftArg = cToTex(leftNode->left, UNKNOWN, false, config);
        // 3.1.2. Считать основанием левого логарифма "e" (натуральный логарифм)
        leftBase = "e";
    }
    // 3.2. Если левый узел является десятичным логарифмом (LOG10)
    else if (leftNode->token.type == LOG10) {
        // 3.2.1. Получить аргумент логарифма (левый потомок узла с логарифмом)
        leftArg = cToTex(leftNode->left, UNKNOWN, false, config);
        // 3.2.2. Считать основанием левого логарифма "10" (десятичный логарифм)
        leftBase = "10";
    }
    // 3.3. Иначе вернуть false
    else {
        return false;
    }

    // 4. Проверить правого потомка: является ли он логарифмом (LOG или LOG10)
    // 4.1. Если правый узел является натуральным логарифмом (LOG)
    if (rightNode->token.type == LOG) {
        rightArg = cToTex(rightNode->left, UNKNOWN, false, config);
        rightBase = "e";
    }
    // 4.2. Если правый узел является десятичным логарифмом (LOG10)
    else if (rightNode->token.type == LOG10) {
        rightArg = cToTex(rightNode->left, UNKNOWN, false, config);
        rightBase = "10";
    }
    // 4.3. Иначе вернуть false
    else {
        return false;
    }

    // 5. Проверить, что основания совпадают
    // 5.1. Если основание левого логарифма не равно основанию правого логарифма, вернуть false
    if (leftBase != rightBase) {
        return false;
    }

    // 6. Сохранить результаты в выходные параметры
    // основание = аргумент правого логарифма (y), аргумент = аргумент левого логарифма (x)
    base = rightArg;
    argument = leftArg;

    // 6. Вернуть true
    return true;
}

bool isArrOperation(Node* node, TokenType opType, string& arrayName, vector<int>& indexes, bool& arrayNameFound)
{
    // 1. Если текущий узел не существует
    if (!node)
    {
        // 1.1 Вернуть false
        return false;
    }

    // 2. Обойти поддерево и собрать все индексы:

    // 2.1. Если узел является оператором и совпадает с искомым оператором
    if (node->token.type == opType)
    {
        // 2.1.1. Рекурсивно обработать левого потомка
        if (!isArrOperation(node->left, opType,
            arrayName, indexes, arrayNameFound))
        {
            return false;
        }

        // 2.1.2. Рекурсивно обработать правого потомка
        if (!isArrOperation(node->right, opType,
            arrayName, indexes, arrayNameFound))
        {
            return false;
        }
    }
    // 2.2. Если узел является оператором индексации (ARRAY_INDEX):
    else if (node->token.type == ARRAY_INDEX)
    {
        // 2.2.1. Получить имя массива 
        string name = node->left->token.value;

        // 2.2.2. Если имя массива до сих пор не было найдено
        if (!arrayNameFound)
        {
            // 2.2.2.1. Сохранить найденное имя массива 
            arrayName = name;
            arrayNameFound = true;
        }
        // 2.2.3. Иначе если найденное имя массива не совпадает с существующим именем массива
        else if (arrayName != name)
        {
            // 2.2.3.1. Вернуть false (разные массивы)
            return false;
        }

        // 2.2.4. Получить значение индекса из правого потомка 
        if (!node->right || node->right->token.type != NUMBER)
        {
            return false;
        }

        int index = stoi(node->right->token.value);

        //2.2.5. Добавить индекс в вектор indexes
        indexes.push_back(index);
    }
    else // 2.3. Иначе узел является другим типом (не искомый оператор и не ARRAY_INDEX)
    {
        // 2.3.1. Вернуть false
        return false;
    }

    // 3. После сбора всех индексов:

    // 3.1.	Если indexes пуст
    if (indexes.empty())
    {
        // 3.1.1.Вернуть false
        return false;
    }

    // 4. Вернуть true
    return true;
}

string cToTex(Node* node, TokenType parentType, bool isRightChild, const Config& config) {
    // 1. Если текущий узел не существует 
    if (node == nullptr) {
        // 1.1. Завершить выполнение
        return "";
    }

    // 2. Если узел является листом (NUMBER, VARIABLE, CONSTANT)
    TokenType type = node->token.type;
    if (type == NUMBER || type == VARIABLE || type == CONSTANT) {
        // 2.1. Вернуть ТеХ-отображение узла
        if (type == CONSTANT) {
            if (node->token.value == "pi") return "\\pi";
            if (node->token.value == "phi") return "\\varphi";
            if (node->token.value == "e") return "e";
            if (node->token.value == "i") return "i";
            if (node->token.value == "true") return "true";
            if (node->token.value == "false") return "false";
        }

        return node->token.value;
    }

    // Получение настроек конфигурации
    string mulIdenVarVal = config.paramMap.at("mulIdenVar");
    string arrMulVal = config.paramMap.at("arrMul");
    string arrSumVal = config.paramMap.at("arrSum");
    string logDivVal = config.paramMap.at("logDiv");
    string trigFunNoNegPowVal = config.paramMap.at("trigFunNoNegPow");
    string trigFunNegPowVal = config.paramMap.at("trigFunNegPow");
    string trigFunMinusOnePowVal = config.paramMap.at("trigFunMinusOnePow");
    string zeroPointFivePowVal = config.paramMap.at("zeroPointFivePow");
    string oneDivNPowVal = config.paramMap.at("oneDivNPow");
    string abPowVal = config.paramMap.at("abPow");
    string squareRootVal = config.paramMap.at("squareRoot");

    string result;
    string temp;  // временная переменная для хранения результата вызова функции

    switch (type) {

        // 3. Если узел является оператором умножения (MUL)
    case MUL: {
        // 3.1. Если параметр отображения MulIdenVar имеет значение powVarN 
        // и функция-детектор параметра отображения MulIdenVar вернула истинное значение (isMulIdenVar)
        if (mulIdenVarVal == "powVarN") {
            string operandStr;
            int varCount = 0;
            if (isMulIdenVar(node, operandStr, varCount, config)) {
                // 3.1.1. Сгенерировать ТеХ-отображение узла как операнда в соответствующей степени
                result = operandStr + "^{" + to_string(varCount) + "}";
                return result;
            }
        }

        // 3.2. Если параметр отображения arrMul имеет значение combineInMul 
        // и функция-детектор параметра отображения arrMul вернула истинное значение (isArrOperation)
        if (arrMulVal == "combineInMul") {
            string arrayName;
            int startIndex, endIndex;
            vector<int> indexes;
            bool arrayNameFound = false;

            if (isArrOperation(node, MUL, arrayName, indexes, arrayNameFound)) {

                // 3.2.1. Найти минимальный и максимальный индексы элементов массива  
                startIndex = *min_element(indexes.begin(), indexes.end());
                endIndex = *max_element(indexes.begin(), indexes.end());

                // 3.2.2. Проверить, что все индексы образуют непрерывную последовательность
                bool isCorrect = true;

                // Размер диапазона
                if ((endIndex - startIndex + 1) != indexes.size())
                {
                    isCorrect = false;
                }

                // Непрерывность
                for (int i = startIndex; i <= endIndex; i++)
                {
                    bool continuity = false;

                    for (int val : indexes)
                    {
                        if (val == i)
                        {
                            continuity = true;
                        }
                    }

                    if (!continuity)
                    {
                        isCorrect = false;
                    }
                }

                // Дубликаты
                for (int i = 0; i < indexes.size(); i++)
                {
                    for (int j = i + 1; j < indexes.size(); j++)
                    {
                        if (indexes[i] == indexes[j])
                        {
                            isCorrect = false;
                        }
                    }
                }

                // 3.2.3. Если индексы образуют непрерывную последовательность
                if (isCorrect) {
                    // 3.2.3.1. Сгенерировать ТеХ-отображение узла как произведение элементов массива
                    result = "\\prod_{i=" + to_string(startIndex) + "}^{" + to_string(endIndex) + "} " + arrayName + "_{i}";
                    return result;
                }
            }
        }
        // 3.3. Иначе сгенерировать ТеХ-отображение узла: 
        // строка левого потомка с учётом скобок + «\cdot» + строка правого потомка с учётом скобок (getChildTexWithParens)
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " \\cdot " + rightStr;
        return result;
    }

            // 4. Если узел является оператором сложения (PLUS), параметр отображения arrSum имеет значение combineInSum 
            // и функция-детектор параметра отображения arrSum вернула истинное значение (isArrOperation)
    case PLUS: {
        if (arrSumVal == "combineInSum") {
            string arrayName;
            int startIndex = 0;
            int endIndex = 0;
            vector<int> indexes;
            bool arrayNameFound = false;
            if (isArrOperation(node, PLUS, arrayName, indexes, arrayNameFound))
            {
                // 4.1. Найти минимальный и максимальный индексы элементов массива
                startIndex = *min_element(indexes.begin(), indexes.end());
                endIndex = *max_element(indexes.begin(), indexes.end());

                // 4.2. Проверить, что все индексы образуют непрерывную последовательность
                bool isCorrect = true;

                // Размер диапазона
                if ((endIndex - startIndex + 1) != indexes.size())
                {
                    isCorrect = false;
                }

                // Непрерывность
                for (int i = startIndex; i <= endIndex; i++)
                {
                    bool continuity = false;

                    for (int val : indexes)
                    {
                        if (val == i)
                        {
                            continuity = true;
                        }
                    }

                    if (!continuity)
                    {
                        isCorrect = false;
                    }
                }

                // Дубликаты
                for (int i = 0; i < indexes.size(); i++)
                {
                    for (int j = i + 1; j < indexes.size(); j++)
                    {
                        if (indexes[i] == indexes[j])
                        {
                            isCorrect = false;
                        }
                    }
                }

                // 4.3. Если индексы образуют непрерывную последовательность
                if (isCorrect) {
                    // 4.3.1. Сгенерировать ТеХ-отображение узла как сумму элементов массива
                    result = "\\sum_{i=" + to_string(startIndex) + "}^{" + to_string(endIndex) + "} " + arrayName + "_{i}";
                    return result;
                }


            }
        }

        // 4.4. Иначе сгенерировать ТеХ-отображение узла: 
        // строка левого потомка с учётом скобок + «+» + строка правого потомка с учётом скобок (getChildTexWithParens)
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " + " + rightStr;
        return result;
    }

             // 5. Если узел является оператором вычитания (MINUS)
    case MINUS: {
        // 5.1. Сгенерировать ТеХ-отображение узла: 
        // строка левого потомка с учётом скобок + «-» + строка правого потомка с учётом скобок (getChildTexWithParens)
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " - " + rightStr;
        return result;
    }

              // 6. Если узел является унарным минусом (UMINUS)
    case UMINUS: {
        // 6.1. Сгенерировать ТеХ-отображение узла: «-» + строка левого потомка с учётом скобок (getChildTexWithParens)
        string childStr = getChildTexWithParens(node, node->left, false, config);
        result = "-" + childStr;
        return result;
    }

               // 7. Если узел является оператором деления (DIV)
    case DIV: {
        // 7.1. Если параметр отображения logDiv имеет значение logConverting 
        // и функция-детектор параметра отображения logDiv вернула истинное значение (isLogDiv)
        if (logDivVal == "logConverting") {
            string base, argument;
            if (isLogDiv(node, base, argument, config))
            {
                // 7.1.1. Сгенерировать ТеХ-отображение узла как логарифм левого потомка с основанием правого потомка
                result = "\\log_{" + base + "}(" + argument + ")";
                return result;
            }
        }

        // 7.2. Иначе сгенерировать ТеХ-отображение узла: \frac{строка левого потомка}{строка правого потомка}
        string leftStr = cToTex(node->left, type, false, config);
        string rightStr = cToTex(node->right, type, true, config);
        result = "\\frac{" + leftStr + "}{" + rightStr + "}";
        return result;
    }

            // 8. Если узел является оператором возведения в степень (POW)
    case POW: {
        // 8.1. Если левый потомок является тригонометрической функцией
        TokenType leftType = node->left->token.type;
        bool isTrig = (leftType == SIN || leftType == COS || leftType == TAN ||
            leftType == ASIN || leftType == ACOS || leftType == ATAN);

        if (isTrig) {
            string trigFunc;
            switch (leftType) {
            case SIN: trigFunc = "\\sin"; break;
            case COS: trigFunc = "\\cos"; break;
            case TAN: trigFunc = "\\tan"; break;
            case ASIN: trigFunc = "\\arcsin"; break;
            case ACOS: trigFunc = "\\arccos"; break;
            case ATAN: trigFunc = "\\arctan"; break;
            default: break;
            }

            string exponent = node->right->token.value;
            double expVal = stod(exponent);

            // 8.1.1. Если параметр отображения trigFunNoNegPow имеет значение powAfterFun 
            // и правый потомок является неотрицательным числом
            if (expVal >= 0) {
                if (trigFunNoNegPowVal == "powAfterFun") {
                    // 8.1.1.1. Сгенерировать ТеХ-отображение узла:
                    // тригонометрическая функция + «^{»  + правый потомок + «}» + левый потомок тригонометрической функции (getChildTexWithParens)
                    temp = getChildTexWithParens(node->left, node->left->left, false, config);
                    result = trigFunc + "^{" + exponent + "}" + temp;
                    return result;
                }
                else {
                    // 8.1.1.2. Иначе сгенерировать ТеХ-отображение узла:
                    // тригонометрическая функция + левый потомок тригонометрической функции (getChildTexWithParens) + «^{»  + правый потомок + «}» 
                    temp = getChildTexWithParens(node->left, node->left->left, false, config);
                    result = trigFunc + " " + temp + "^{" + exponent + "}";
                    return result;
                }
            }
            // 8.1.2. Иначе если параметр отображения trigFunNegPow имеет значение divNoNegPow 
            // и правый потомок является отрицательным числом (кроме -1)
            else if (expVal < 0 && expVal != -1) {
                if (trigFunNegPowVal == "divNoNegPow") {
                    // 8.1.2.1. Сгенерировать ТеХ-отображение узла:
                    // «\frac{1}{» + тригонометрическая функция + «(» + левый потомок тригонометрической функции + «)» + «^{» + число без знака минус + «}}»
                    string absExp = exponent;
                    if (!absExp.empty() && absExp[0] == '-')
                    {
                        absExp = absExp.substr(1);
                    }

                    temp = cToTex(node->left->left, leftType, false, config);
                    result = "\\frac{1}{" + trigFunc + "(" + temp + ")^{" + absExp + "}}";
                    return result;
                }
                else {
                    // 8.1.2.2. Иначе сгенерировать ТеХ-отображение узла:
                    // тригонометрическая функция + «(» + левый потомок тригонометрической функции + «)» + «^{» + число  + «}»
                    temp = cToTex(node->left, type, false, config);
                    result = trigFunc + "(" + temp + ")^{" + exponent + "}";
                    return result;
                }
            }
            // 8.1.3. Иначе если правый потомок является числом -1 и параметр отображения trigFunMinusOnePow имеет значение divNoNegPow
            else if (expVal == -1) {
                if (trigFunMinusOnePowVal == "divNoNegPow") {
                    // 8.1.3.1. Сгенерировать ТеХ-отображение узла:
                    // «\frac{1}{» + тригонометрическая функция + левый потомок тригонометрической функции (getChildTexWithParens) + «}» 
                    temp = getChildTexWithParens(node->left, node->left->left, false, config);
                    result = "\\frac{1}{" + trigFunc + temp + "}";
                    return result;
                }
                // 8.1.3.2.	Иначе если правый потомок является числом -1 
                // и параметр отображения trigFunMinusOnePow имеет значение reverseFun
                else if (trigFunMinusOnePowVal == "reverseFun") {
                    // 8.1.3.2.1. Сгенерировать ТеХ-отображение узла как обратную тригонометрическую функцию с таким же аргументом
                    temp = getChildTexWithParens(node->left, node->left->left, false, config);
                    if (leftType == SIN) result = "\\arcsin " + temp;
                    if (leftType == COS) result = "\\arccos " + temp;
                    if (leftType == TAN) result = "\\arctan " + temp;
                    return result;
                }
                // 8.1.3.3.	Иначе сгенерировать ТеХ-отображение узла как тригонометрическую функцию в степени -1
                else {
                    temp = getChildTexWithParens(node, node->left, false, config);
                    result = trigFunc + temp + "^{-1}";
                    return result;
                }
            }
            // 8.1.4. Иначе сгенерировать ТеХ-отображение узла: 
            // тригонометрическая функция + левый потомок тригонометрической функции (getChildTexWithParens) + «^{» + правый потомок + «}»
            temp = getChildTexWithParens(node, node->left, false, config);
            result = trigFunc + temp + "^{" + exponent + "}";
            return result;
        }

        // 8.2. Если правый потомок 0.5 (или 1/2)
        string exponent = cToTex(node->right, type, true, config);
        if (exponent == "0.5" || exponent == "1/2") {
            // 8.2.1. Если значение параметра отображения zeroPointFivePow имеет значение powToSqrt
            if (zeroPointFivePowVal == "powToSqrt") {
                // 8.2.1.1. Сгенерировать ТеХ-отображение узла: «\sqrt{» + левый потомок + «}»
                temp = cToTex(node->left, type, false, config);
                result = "\\sqrt{" + temp + "}";
                return result;
            }
            else {
                // 8.2.1.2. Иначе сгенерировать ТеХ-отображение узла: 
                result = cToTex(node->left, type, false, config) + "^{" + exponent + "}";
                return result;
            }
        }

        // 8.3. Если правый потомок 1/n (кроме n = 2)
        if (node->right->token.type == DIV)
        {
            Node* numeratorNode = node->right->left;
            Node* denominatorNode = node->right->right;

            if (numeratorNode && denominatorNode &&
                numeratorNode->token.type == NUMBER &&
                denominatorNode->token.type == NUMBER)
            {
                string numerator = numeratorNode->token.value;
                string denominator = denominatorNode->token.value;

                // 8.3.1. Если параметр отображения oneDivNPow имеет значение powToSqrt
                if (oneDivNPowVal == "powToSqrt")
                {
                    if (numerator == "1" && denominator != "2")
                    {
                        // 8.3.1.1. Сгенерировать ТеХ-отображение узла: «\sqrt[» + число n + «]{» + левый потомок + «}»
                        temp = cToTex(node->left, type, false, config);
                        result = "\\sqrt[" + denominator + "]{" + temp + "}";
                        return result;
                    }
                }
            }
        }

        // 8.4. Если правый потомок a/b (кроме a=1 и b=2)
        if (node->right->token.type == DIV)
        {
            Node* numeratorNode = node->right->left;
            Node* denominatorNode = node->right->right;

            if (numeratorNode && denominatorNode &&
                numeratorNode->token.type == NUMBER &&
                denominatorNode->token.type == NUMBER)
            {
                string numerator = numeratorNode->token.value;
                string denominator = denominatorNode->token.value;

                // 8.4.1. Если параметр отображения abPow имеет значение powToSqrt
                if (abPowVal == "powToSqrt")
                {
                    if (numerator != "1" || denominator != "2")
                    {
                        // 8.4.1.1. Сгенерировать ТеХ-отображение узла:
                        // «\sqrt[» + число b + «]{» + левый потомок + «^{» + число a + «}}»
                        temp = cToTex(node->left, type, false, config);
                        result = "\\sqrt[" + denominator + "]{" + temp + "^{" + numerator + "}}";
                        return result;
                    }
                }
            }
        }

        // 8.5. Если ни одно из условий не подошло 
        // 8.5.1. Сгенерировать ТеХ-отображение узла: левый потомок + «^{» + правый потомок + «}»
        result = cToTex(node->left, type, false, config) + "^{" + exponent + "}";
        return result;
    }

            // 9. Если узел является оператором извлечения квадратного корня (SQRT) и параметр squareRoot имеет значение sqrtToPow
    case SQRT: {
        string inner = cToTex(node->left, type, false, config);
        if (squareRootVal == "sqrtToPow") {
            // 9.1. Сгенерировать ТеХ-отображение узла: левый потомок + «^{0.5}»
            result = inner + "^{0.5}";
            return result;
        }
        else {
            // 9.2. Иначе сгенерировать ТеХ-отображение узла: «\sqrt{» + левый потомок + «}»
            result = "\\sqrt{" + inner + "}";
            return result;
        }
    }

             // 10. Если узел является оператором извлечения кубического корня (CBRT)
    case CBRT: {
        // 10.1. Сгенерировать ТеХ-отображение узла
        string inner = cToTex(node->left, type, false, config);
        result = "\\sqrt[3]{" + inner + "}";
        return result;
    }

             // 11. Если узел является индексацией элемента массива (ARRAY_INDEX)
    case ARRAY_INDEX: {
        // 11.1. Сгенерировать ТеХ-отображение узла
        string arrayName = cToTex(node->left, type, false, config);
        string index = cToTex(node->right, type, true, config);
        result = arrayName + "_{" + index + "}";
        return result;
    }

            // 12. Если узел является оператором синуса (SIN)
    case SIN: {
        // 12.1. Сгенерировать ТеХ-отображение узла
        temp = getChildTexWithParens(node, node->left, false, config);
        result = "\\sin " + temp;
        return result;
    }

            // 13. Если узел является оператором косинуса (COS)
    case COS: {
        // 13.1. Сгенерировать ТеХ-отображение узла
        temp = getChildTexWithParens(node, node->left, false, config);
        result = "\\cos " + temp;
        return result;
    }

            // 14. Если узел является оператором тангенса (TAN)
    case TAN: {
        // 14.1. Сгенерировать ТеХ-отображение узла
        temp = getChildTexWithParens(node, node->left, false, config);
        result = "\\tan " + temp;
        return result;
    }

            // 15. Если узел является оператором арксинуса (ASIN)
    case ASIN: {
        // 15.1. Сгенерировать ТеХ-отображение узла
        temp = getChildTexWithParens(node, node->left, false, config);
        result = "\\arcsin " + temp;
        return result;
    }

             // 16. Если узел является оператором арккосинуса (ACOS)
    case ACOS: {
        // 16.1. Сгенерировать ТеХ-отображение узла
        temp = getChildTexWithParens(node, node->left, false, config);
        result = "\\arccos " + temp;
        return result;
    }

             // 17. Если узел является оператором арктангенса (ATAN)
    case ATAN: {
        // 17.1. Сгенерировать ТеХ-отображение узла
        temp = getChildTexWithParens(node, node->left, false, config);
        result = "\\arctan " + temp;
        return result;
    }

             // 18. Если узел является оператором логарифма (LOG)
    case LOG: {
        // 18.1. Сгенерировать ТеХ-отображение узла
        result = "\\ln(" + cToTex(node->left, type, false, config) + ")";
        return result;
    }

            // 19. Если узел является оператором десятичного логарифма (LOG10)
    case LOG10: {
        // 19.1. Сгенерировать ТеХ-отображение узла
        result = "\\log_{10}(" + cToTex(node->left, type, false, config) + ")";
        return result;
    }

              // 20. Если узел является оператором экспоненты (EXP)
    case EXP: {
        // 20.1. Сгенерировать ТеХ-отображение узла
        result = "e^{" + cToTex(node->left, type, false, config) + "}";
        return result;
    }

            // 21. Если узел является оператором модуля (ABS или FABS)
    case ABS:
    case FABS: {
        // 21.1. Сгенерировать ТеХ-отображение узла
        result = "|" + cToTex(node->left, type, false, config) + "|";
        return result;
    }

             // 22. Если узел является оператором равенства (EQ)
    case EQ: {
        // 22.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " = " + rightStr;
        return result;
    }

           // 23. Если узел является оператором неравенства (NEQ)
    case NEQ: {
        // 23.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " \\neq " + rightStr;
        return result;
    }

            // 24. Если узел является оператором меньше (LT)
    case LT: {
        // 24.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " < " + rightStr;
        return result;
    }

           // 25. Если узел является оператором больше (GT)
    case GT: {
        // 25.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " > " + rightStr;
        return result;
    }

           // 26. Если узел является оператором меньше или равно (LE)
    case LE: {
        // 26.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " \\leq " + rightStr;
        return result;
    }

           // 27. Если узел является оператором больше или равно (GE)
    case GE: {
        // 27.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " \\geq " + rightStr;
        return result;
    }

           // 28. Если узел является логическим оператором AND (LAND)
    case LAND: {
        // 28.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " \\land " + rightStr;
        return result;
    }

             // 29. Если узел является логическим оператором OR (LOR)
    case LOR: {
        // 29.1. Сгенерировать ТеХ-отображение узла
        string leftStr = getChildTexWithParens(node, node->left, false, config);
        string rightStr = getChildTexWithParens(node, node->right, true, config);
        result = leftStr + " \\lor " + rightStr;
        return result;
    }

            // 30. Если узел является оператором логического отрицания (LNOT)
    case LNOT: {
        // 30.1. Сгенерировать ТеХ-отображение узла
        string childStr = getChildTexWithParens(node, node->left, false, config);
        result = "\\lnot " + childStr;
        return result;
    }

    default:
        return "";
    }
}


/// @brief Главная функция программы
/// @param argc количество аргументов командной строки
/// @param argv массив аргументов командной строки
/// @return 0 при успешном завершении, 1 при ошибке
int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "");
    system("chcp 65001 > nul");

    // 1. Открыть входные файлы и считать данные 
    // Проверка количества аргументов командной строки
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <expression_file> <config_file> <output_file>" << endl;
        return 1;
    }

    string expression; // Строка для хранения выражения из файла
    Config config; // Объект для хранения параметров отображения
    vector<Error> errors; // Вектор для сбора ошибок

    // Чтение входных файлов
    if (!readInputFiles(argv[1], argv[2], expression, config, errors)) {
        printErrors(errors);
        return 1;
    }

    // 2. Построить дерево через стек, разбив строку с выражением на токены по пробелам
    Node* root = nullptr;       // указатель на корень дерева

    if (!buildTree(expression, root, operatorInfo, errors))
    {
        printErrors(errors);
        delete root;
        return 1;
    }

    // 3. Сгенерировать tex-строку из дерева 
    string texString = cToTex(root, UNKNOWN, false, config);

    // 4. Записать tex-строку в выходной файл 
    if (!saveToOutFile(argv[3], texString, errors))
    {
        printErrors(errors);
        delete root;
        return 1;
    }

    // 5. Завершить работу программы
    delete root;

    cout << "Success! TeX string written to " << argv[3] << endl;
    return 0;
}


