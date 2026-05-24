
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

        // 3.1 Если слово содержит  содержит более одного оператора или слово содержит 1 оператор из списка допустимых операторов, но всё слово не является оператором 
        if (opCounter > 1 || (opCounter == 1 && allowedOperations.find(word) == allowedOperations.end()))
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
                else if (word == "a[i]") t.type = ARRAY_INDEX;
                else t.type = UNKNOWN;

                // 3.3.1.2 Добавить созданный токен в вектор токенов (tokens)
                tokens.push_back(t);

                // 3.3.1.3 Инкрементировать значение счётчика узлов
                nodeCount++;
            }
            // 3.3.2 Перейти к обработке следующего слова
            processed = true;
        }

        // 3.4. Если первый символ слова – буква:
        if (!processed && isalpha(word[0]))
        {
            // 3.4.1 Для каждого символа слова, начиная с позиции 1:
            for (size_t i = 1; i < word.size(); i++)
            {
                // 3.4.1.1 Если символ является буквой или цифрой, перейти к следующему символу
                if (!(isalnum(word[i])))
                {
                    // 3.4.1.2 В противном случае считать символ недопустимым и занести соответствующую ошибку в вектор ошибок
                    errors.push_back({ InvalidSymbol, (int)i, 1, word });
                }
                // 3.4.1.3 Перейти к следующему символу
            }

            // 3.4.2 Если длина слова превышает 255 символов, занести соответствующую ошибку в вектор ошибок
            if (word.size() > 255)
            {
                errors.push_back({ VariableNameTooLong, -1, (int)word.size(), word });
            }

            // 3.4.3 Если вектор ошибок пуст
            if (errors.empty())
            {
                Token t{ VARIABLE, word, ARITHMETIC };

                // 3.4.3.2 Добавить созданный токен в вектор токенов (tokens)
                tokens.push_back(t);

                // 3.4.3.3 Инкрементировать значение счётчика узлов
                nodeCount++;
            }

            // 3.4.4 Перейти к обработке следующего слова
            processed = true;
        }

        // 3.5. Если первый символ слова – цифра или если длина слова больше 1 и первый символ – минус, а второй – цифра  
        bool isNumberStart = isdigit(word[0]) || (word.size() > 1 && word[0] == '-' && isdigit(word[1]));
        if (!processed && isNumberStart)
        {
            // 3.5.1 Считать, что точка ещё не встречалась в слове
            bool dotFound = false;
            int dotPos = -1;

            // 3.5.2 Для каждого символа слова, начиная с позиции 1:
            for (size_t i = 1; i < word.size(); i++)
            {
                // 3.5.2.1 Если символ является цифрой, перейти к следующему символу
                if (isdigit(word[i]))
                {
                }
                // 3.5.2.2 Если символ является точкой, и точка ещё не встречалась в слове, запомнить местоположение точки и перейти к следующему символу
                else if (word[i] == '.' && !dotFound)
                {
                    dotFound = true;
                    dotPos = i;
                }
                // 3.5.2.3 В противном случае считать символ недопустимым и занести соответствующую ошибку в вектор ошибок
                else {
                    errors.push_back({ InvalidSymbolSequence, (int)i, 1, word });
                }
                // 3.5.2.4 Перейти к следующему символу
            }

            // 3.5.3 Если в слове есть точка
            if (dotFound)
            {
                // 3.5.3.1 Если число символов после точки превышает 8, занести соответствующую ошибку в вектор ошибок
                int afterSym = word.size() - dotPos - 1;
                if (afterSym > 8)
                {
                    errors.push_back({ TooManyDecimalDigits, dotPos + 1, afterSym, word });
                }
            }

            // 3.5.4 Если число не входит в диапазон [-2*10^9, 2*10^9], занести соответствующую ошибку в вектор ошибок
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

            // 3.5.5 Если вектор ошибок пуст
            if (errors.empty())
            {
                Token t{ NUMBER, word, ARITHMETIC };

                // 3.5.5.2 Добавить созданный токен в вектор токенов (tokens)
                tokens.push_back(t);

                // 3.5.5.3 Инкрементировать значение счётчика узлов
                nodeCount++;
            }

            // 3.5.6 Перейти к обработке следующего слова
            processed = true;
        }

        // 3.6 В противном случае считать, что слово начинается с недопустимого символа
        if (!processed)
        {
            // 3.7 Занести соответствующую ошибку в вектор ошибок
            errors.push_back({ InvalidSymbol, 0, (int)word.size(), word });

            // 3.8 Перейти к обработке следующего слова
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
    if (childPrec == parentPrec)
    {
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





string cToTex(Node* node, TokenType parentType, bool isRightChild, const Config& config) {
    return "texxt";
}


int main()
{
    
}


