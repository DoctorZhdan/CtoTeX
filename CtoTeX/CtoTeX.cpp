
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
    splitIntoWords(expression, wordList);

    // 2. Установить нулевое значение счётчика узлов (nodeCount = 0)
    int nodeCount = 0;

    // 3. Для каждого слова из списка слов (wordList):
    for (int i = 0; i < wordList.size(); i++)
    {
        string word = wordList[i];
        bool processed = false; // processed - флаг того, что слово уже распознано как что-то допустимое


        // Обработка операторов 
        if (parseOperator(word, tokens, nodeCount))
        {
            processed = true;
        }

        // Проверка корректности расположения операторов в слове
        if (!processed)
        {
            if (checkOperatorSpacing(word, errors, allowedOperations))
            {
                processed = true;
            }
        }
        
        // 3.2. Обработка константы
        if (!processed && parseConstant(word, tokens, nodeCount))
        {
            processed = true;
        }

        // 3.4. Обработка элемента массива
        if (!processed && parseArray(word, tokens, errors, nodeCount))
        {
            processed = true;
        }

        // 3.5. Обработка переменной
        if (!processed)
        {
            if (parseVariable(word, tokens, errors, nodeCount))
            {
                processed = true;
            }
        }

        // 3.6. Обработка числа
        if (!processed)
        {
            if (parseNumber(word, tokens, errors, nodeCount))
            {
                processed = true;
            }
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

            processOperand(token, nodeStack, nodeCount, errors);
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
            if (operatorFound && arity == 2)
            {
                newNode = processBinaryOperator(token, nodeStack, errors);
            }
            // 4.2.3. Если арность равна 1:
            else if (operatorFound && arity == 1)
            {
                newNode = processUnaryOperator(token, nodeStack, errors);
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

    // 5. Проверить состояние стека
    checkStackState(nodeStack, errors);

    // 6. Извлечь единственный узел из стека и сохранить в root
    if (!nodeStack.empty()) {
        root = nodeStack.top();
        nodeStack.pop();
    }

    // 7. Если вектор ошибок (errors) не пуст 
    if (!errors.empty()) {
        // 7.1. Вернуть false
        return false;
    }
    // 7.2. Иначе вернуть true
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

    // 5. Сравнение приоритетов 
    return comparePrecedence(parentPrec, childPrec, parentLeftAssoc, isRightChild);
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
        return getConstantTeX(node->token.value);

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
        return getMulOperationTeX(node, config);
    }

            // 4. Если узел является оператором сложения (PLUS)
    case PLUS: {
        return getPlusOperationTeX(node, config);
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
        return getDivOperationTeX(node, config, type);
    }

            // 8. Если узел является оператором возведения в степень (POW)
    case POW: {
        // 1. Проверка на тригонометрическую функцию
        TokenType leftType = node->left->token.type;
        bool isTrig = (leftType == SIN || leftType == COS || leftType == TAN ||
            leftType == ASIN || leftType == ACOS || leftType == ATAN);
        if (isTrig)
        {
            return getTrigPowerTeX(node, config);
        }

        // 2. Проверка на дробную степень
        string* fractionResult = getFractionPowerTeX(node, config, type);
        if (fractionResult != nullptr)
        {
            string result = *fractionResult;
            delete fractionResult;
            return result;
        }

        // 3. Обычная степень
        return getRegularPowerTeX(node, config, type);
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

            // 12. Если узел является тригонометрической функцией
    case SIN:
    case COS:
    case TAN:
    case ASIN:
    case ACOS:
    case ATAN: {
        return getTrigFunctionTeX(node, config);
    }

             // 13. Если узел является оператором логарифма 
    case LOG:
    case LOG10: {
        return getLogFunctionTeX(node, config);
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



bool parseOperator(const string& word, vector<Token>& tokens, int& nodeCount)
{
    // 1. Обработка специального случая унарного минуса 
   
    if (word == "-_") {
        tokens.push_back({ UMINUS, "-_", ARITHMETIC });
        nodeCount++;
        return true;
    }

    // Таблица операторов и функций
    static const map<string, TokenType> opMap = {
        {"+", PLUS}, {"-", MINUS}, {"*", MUL}, {"/", DIV},
        {"#pow", POW}, {"#sqrt", SQRT}, {"#cbrt", CBRT},
        {"#sin", SIN}, {"#cos", COS}, {"#tan", TAN},
        {"#asin", ASIN}, {"#acos", ACOS}, {"#atan", ATAN},
        {"#log", LOG}, {"#log10", LOG10}, {"#exp", EXP},
        {"#abs", ABS}, {"#fabs", FABS},
        {"==", EQ}, {"!=", NEQ}, {"<", LT}, {">", GT},
        {"<=", LE}, {">=", GE},
        {"&&", LAND}, {"||", LOR}, {"!", LNOT}
    };

    //  Поиск слова в таблице операторов 
    auto it = opMap.find(word);

    if (it != opMap.end()) {
        Token t;

        // Устанавливаем тип токена
        t.type = it->second;

        // Сохраняем исходное строковое значение
        t.value = word;

        // Определяем тип операнда
        if (word == "&&" || word == "||" || word == "!") {
            t.operandType = LOGICAL;
        }
        else {
            t.operandType = ARITHMETIC;
        }

        // Добавляем токен в результат
        tokens.push_back(t);

        // Увеличиваем счётчик узлов
        nodeCount++;

        return true;
    }

    // 4. Если слово не является оператором
    return false;
}

bool checkOperatorSpacing(const string& word, vector<Error>& errors, const set<string>& allowedOperations)
{
    // 1. Подсчёт количества операторов в слове 
    int opCounter = 0;

    for (const string& op : allowedOperations) {
        size_t pos = 0;
        while ((pos = word.find(op, pos)) != string::npos) {
            opCounter++;
            pos += op.size();
        }
    }

    // 2. Проверка на наличие оператора внутри слова 
    if (opCounter > 1 || (opCounter == 1 && allowedOperations.find(word) == allowedOperations.end()))
    {
        //  3. Проверка, не является ли слово отрицательным числом 
        bool isNegativeNumber = false;

        if (word.size() > 1 && word[0] == '-') {
            isNegativeNumber = true;
            bool dotFound = false;

            for (size_t i = 1; i < word.size(); i++) {
                if (isdigit(word[i])) {}
                else if (word[i] == '.' && !dotFound) {
                    dotFound = true;
                }
                else {
                    isNegativeNumber = false;
                }
            }
        }

        //  4. Если это не число, ошибка
        if (!isNegativeNumber)
        {
            Error err;
            err.code = (opCounter > 1) ? InvalidSymbolSequence : OperatorNotSeparatedBySpaces;
            err.position = 0;
            err.length = word.size();
            err.line = word;

            errors.push_back(err);
            return true;
        }
    }

    return false;
}

bool parseNumber(const string& word, vector<Token>& tokens, vector<Error>& errors, int& nodeCount)
{
    // 1. Проверка, может ли слово быть числом
    bool isNumberStart = isdigit(word[0]) ||
        (word.size() > 1 && word[0] == '-' && isdigit(word[1]));

    if (!isNumberStart)
        return false;

    // 2. Проверка структуры числа
    bool dotFound = false;
    int dotPos = -1;

    for (size_t i = 1; i < word.size(); i++)
    {
        if (isdigit(word[i])) {}

        else if (word[i] == '.' && !dotFound)
        {
            dotFound = true;
            dotPos = i;
        }

        else
        {
            errors.push_back({ InvalidSymbolSequence, (int)i, 1, word });
        }
    }

    // 3. Проверка количества знаков после запятой
    if (dotFound)
    {
        int after = word.size() - dotPos - 1;
        if (after > 8)
        {
            errors.push_back({ TooManyDecimalDigits, dotPos + 1, after, word });
        }
    }

    // 4. Проверка диапазона
    try {
        double val = stod(word);
        if (val < -2e9 || val > 2e9)
        {
            errors.push_back({ IntegerOverflow, -1, (int)word.size(), word });
        }
    }
    catch (...)
    {
        errors.push_back({ InvalidSymbolSequence, -1, (int)word.size(), word });
    }

    // 5. Если ошибок нет, добавляем токен
    if (errors.empty())
    {
        tokens.push_back({ NUMBER, word, ARITHMETIC });
        nodeCount++;
    }

    return true;
}

bool parseVariable(const string& word, vector<Token>& tokens, vector<Error>& errors, int& nodeCount)
{
    // 1. Проверка: переменная должна начинаться с буквы
    if (!isalpha(word[0]))
        return false;

    size_t errBefore = errors.size();

    // 2. Проверка допустимых символов
    for (size_t i = 1; i < word.size(); i++) {
        if (!isalnum(word[i])) {
            errors.push_back({ InvalidSymbol, (int)i, 1, word });
        }
    }

    // 3. Ограничение длины имени
    if (word.size() > 255) {
        errors.push_back({ VariableNameTooLong, -1, (int)word.size(), word });
    }

    // 4. Если ошибок нет, добавляем токен
    if (errors.size() == errBefore) {
        tokens.push_back({ VARIABLE, word, ARITHMETIC });
        nodeCount++;
    }

    return true;
}

bool parseConstant(const string& word, vector<Token>& tokens, int& nodeCount)
{
    // Проверка, входит ли слово в список допустимых констант
    if (allowedConstants.find(word) == allowedConstants.end())
        return false;

    Token t;
    t.type = CONSTANT;
    t.value = word;

    // Определение типа операнда
    if (word == "true" || word == "false")
    {
        t.operandType = LOGICAL;
    }
    else
    {
        t.operandType = ARITHMETIC;
    }

    tokens.push_back(t);
    nodeCount++;

    return true;
}

bool parseArray(const string& word, vector<Token>& tokens, vector<Error>& errors, int& nodeCount)
{
    // 1. Проверка наличия символов '[' и ']'
    size_t l = word.find('[');
    size_t r = word.find(']');

    if (l == string::npos || r == string::npos || l > r || r != word.size() - 1)
    {
        return false;
    }

    // 2. Извлечение имени массива и индекса
    string name = word.substr(0, l);
    string indexStr = word.substr(l + 1, r - l - 1);

    // 3. Проверка имени массива
    if (name.empty() || !isalpha(name[0]))
    {
        errors.push_back({ InvalidSymbol, 0, (int)name.size(), name });
    }

    for (size_t j = 1; j < name.size(); j++)
    {
        if (!isalnum(name[j]))
        {
            errors.push_back({ InvalidSymbol, (int)j, 1, name });
        }
    }

    // 4. Проверка индекса 
    bool hasError = false;
    for (char c : indexStr)
    {
        if (!hasError && !isdigit(c))
        {
            errors.push_back({ InvalidSymbolSequence, 0, (int)indexStr.size(), indexStr });
            hasError = true;
        }
    }

    // 5. Если есть ошибки, не создаём токены
    if (!errors.empty())
    {
        return true;
    }

    // 6. Создание трёх токенов: имя массива, индекс, оператор индексации
    Token arrayToken;
    arrayToken.type = VARIABLE;
    arrayToken.value = name;
    arrayToken.operandType = ARITHMETIC;
    tokens.push_back(arrayToken);

    Token indexToken;
    indexToken.type = NUMBER;
    indexToken.value = indexStr;
    indexToken.operandType = ARITHMETIC;
    tokens.push_back(indexToken);

    Token indexOpToken;
    indexOpToken.type = ARRAY_INDEX;
    indexOpToken.value = "a[i]";
    indexOpToken.operandType = ARITHMETIC;
    tokens.push_back(indexOpToken);

    nodeCount += 3;

    return true;
}

void splitIntoWords(const string& expression, vector<string>& wordList)
{
    string currentWord = ""; 

    // Для каждого символа строки
    for (int i = 0; i < expression.size(); i++)
    {
        // Если встретили пробел
        if (expression[i] == ' ')
        {
            // Если текущее слово пустое, добавляем слово в список
            if (!currentWord.empty())
            {
                wordList.push_back(currentWord);
                currentWord = "";  
            }
        }
        else
        {
            // Иначе добавляем символ к текущему слову
            currentWord += expression[i];
        }
    }

    // Добавляем последнее слово, если оно есть
    if (!currentWord.empty())
    {
        wordList.push_back(currentWord);
    }
}



Node* processBinaryOperator(const Token& token, stack<Node*>& nodeStack, vector<Error>& errors)
{
    // 1. Проверка наличия двух операндов в стеке
    if (nodeStack.size() < 2)
    {
        Error err;
        err.code = NotEnoughOperands;
        errors.push_back(err);
        return nullptr;
    }

    // 2. Извлечение операндов
    Node* right = nodeStack.top();
    nodeStack.pop();
    Node* left = nodeStack.top();
    nodeStack.pop();

    // 3. Проверка совместимости типов
    auto opIt = operationTypes.find(token.type);
    if (opIt != operationTypes.end())
    {
        bool compatible = false;
        for (const auto& sig : opIt->second.allowedSignatures)
        {
            if (!compatible && sig.size() == 2 &&
                sig[0] == left->token.operandType &&
                sig[1] == right->token.operandType)
            {
                compatible = true;
            }
        }

        if (!compatible)
        {
            Error err;
            err.code = MismatchedOperandTypes;
            errors.push_back(err);
            delete left;
            delete right;
            return nullptr;
        }
    }

    // 4. Создание нового узла
    return new Node(token, left, right);
}

Node* processUnaryOperator(const Token& token, stack<Node*>& nodeStack, vector<Error>& errors)
{
    // 1. Проверка наличия одного операнда в стеке
    if (nodeStack.size() < 1)
    {
        Error err;
        err.code = NotEnoughOperands;
        errors.push_back(err);
        return nullptr;
    }

    // 2. Извлечение операнда
    Node* child = nodeStack.top();
    nodeStack.pop();

    // 3. Проверка совместимости типа
    auto opIt = operationTypes.find(token.type);
    if (opIt != operationTypes.end())
    {
        bool compatible = false;
        for (const auto& sig : opIt->second.allowedSignatures)
        {
            if (!compatible && sig.size() == 1 && sig[0] == child->token.operandType)
            {
                compatible = true;
            }
        }

        if (!compatible)
        {
            Error err;
            err.code = MismatchedOperandTypes;
            errors.push_back(err);
            delete child;
            return nullptr;
        }
    }

    // 4. Создание нового узла
    return new Node(token, child);
}

void processOperand(const Token& token, stack<Node*>& nodeStack, int& nodeCount, vector<Error>& errors)
{
    // 1. Создать новый узел Node с этим токеном
    Node* newNode = new Node(token);

    // 2. Положить узел в nodeStack
    nodeStack.push(newNode);

    // 3. Увеличить nodeCount на 1
    nodeCount++;

    // 4. Если nodeCount > 1000
    if (nodeCount > 1000)
    {
        Error err;
        err.code = TooManyNodes;
        errors.push_back(err);
    }
}

void checkStackState(stack<Node*>& nodeStack, vector<Error>& errors)
{
    // 1. Если стек пуст, в выражении не было операндов
    if (nodeStack.empty())
    {
        Error err;
        err.code = NotEnoughOperands;
        errors.push_back(err);
    }

    // 2. Если в стеке больше одного узла, в выражении остались лишние операнды
    if (nodeStack.size() > 1)
    {
        Error err;
        err.code = TooManyOperands;
        errors.push_back(err);
    }
}




bool comparePrecedence(int parentPrec, int childPrec, bool parentLeftAssoc, bool isRightChild)
{
    // 1. Если приоритет потомка выше
    if (childPrec < parentPrec)
    {
        return false;
    }

    // 2. Если приоритет потомка ниже
    if (childPrec > parentPrec)
    {
        return true;
    }

    // 3. Если приоритеты равны
    // 3.1. Левоассоциативная операция
    if (parentLeftAssoc)
    {
        return isRightChild;
    }
    // 3.2. Правоассоциативная операция
    else
    {
        return !isRightChild;
    }
}




string getConstantTeX(const string& value)
{
    if (value == "pi") return "\\pi";
    if (value == "phi") return "\\varphi";
    if (value == "e") return "e";
    if (value == "i") return "i";
    if (value == "true") return "true";
    if (value == "false") return "false";
    return value;
}

string getTrigFunctionTeX(Node* node, const Config& config)
{
    // 1. Определение типа тригонометрической функции
    TokenType type = node->token.type;
    string funcName;

    // 2. Выбор соответствующей TeX-команды
    switch (type)
    {
    case SIN:  funcName = "\\sin";   break;
    case COS:  funcName = "\\cos";   break;
    case TAN:  funcName = "\\tan";   break;
    case ASIN: funcName = "\\arcsin"; break;
    case ACOS: funcName = "\\arccos"; break;
    case ATAN: funcName = "\\arctan"; break;
    default:   return "";            
    }

    // 3. Получение TeX-строки аргумента с учётом скобок
    string argStr = getChildTexWithParens(node, node->left, false, config);

    // 4. Формирование итоговой строки: \sin(x)
    return funcName + " " + argStr;
}

string getLogFunctionTeX(Node* node, const Config& config)
{
    // 1. Определение типа логарифма
    TokenType type = node->token.type;

    // 2. Получение TeX-строки аргумента
    string argStr = cToTex(node->left, type, false, config);

    // 3. Формирование итоговой строки в зависимости от основания
    if (type == LOG)
    {
        // Натуральный логарифм
        return "\\ln(" + argStr + ")";
    }
    else 
    {
        // Десятичный логарифм
        return "\\log_{10}(" + argStr + ")";
    }
}

string getTrigPowerTeX(Node* node, const Config& config)
{
    // 1. Получение параметров
    TokenType leftType = node->left->token.type;
    string exponent = node->right->token.value;
    double expVal = stod(exponent);

    // 2. Определение TeX-имени тригонометрической функции 
    string trigFunc = getTrigFunctionName(leftType);
    if (trigFunc.empty()) return "";  // неизвестный тип

    // 3. Получение настроек конфигурации для тригонометрических функций
    string trigFunNoNegPowVal = config.paramMap.at("trigFunNoNegPow");       // неотрицательная степень
    string trigFunNegPowVal = config.paramMap.at("trigFunNegPow");           // отрицательная степень (кроме -1)
    string trigFunMinusOnePowVal = config.paramMap.at("trigFunMinusOnePow"); // степень -1

    string temp;  

    // 4. Обработка в зависимости от значения степени

    // 4.1. Неотрицательная степень 
    if (expVal >= 0)
    {
        // Получение строки аргумента 
        temp = getChildTexWithParens(node->left, node->left->left, false, config);

        if (trigFunNoNegPowVal == "powAfterFun")
        {
            // Степень перед аргументом
            return trigFunc + "^{" + exponent + "}" + temp;
        }
        else
        {
            // Степень после аргумента 
            return trigFunc + " " + temp + "^{" + exponent + "}";
        }
    }
    // 4.2. Отрицательная степень (кроме -1)
    else if (expVal < 0 && expVal != -1)
    {
        // Удаляем знак минуса для вывода положительной степени в знаменателе
        string absExp = (exponent[0] == '-') ? exponent.substr(1) : exponent;
        temp = cToTex(node->left->left, leftType, false, config);

        if (trigFunNegPowVal == "divNoNegPow")
        {
            // Дробь
            return "\\frac{1}{" + trigFunc + "(" + temp + ")^{" + absExp + "}}";
        }
        else
        {
            // Отрицательная степень 
            return trigFunc + "(" + temp + ")^{" + exponent + "}";
        }
    }
    // 4.3. Степень -1 
    else if (expVal == -1)
    {
        temp = getChildTexWithParens(node->left, node->left->left, false, config);

        if (trigFunMinusOnePowVal == "divNoNegPow")
        {
            // Дробь
            return "\\frac{1}{" + trigFunc + " " + temp + "}";
        }
        else if (trigFunMinusOnePowVal == "reverseFun")
        {
            // Обратная функция
            if (leftType == SIN) return "\\arcsin " + temp;
            if (leftType == COS) return "\\arccos " + temp;
            if (leftType == TAN) return "\\arctan " + temp;
        }
        // Степень -1 
        return trigFunc + " " + temp + "^{-1}";
    }

    // 5. Стандартный случай (если ни одно условие не подошло)
    temp = getChildTexWithParens(node, node->left, false, config);
    return trigFunc + temp + "^{" + exponent + "}";
}

string* getFractionPowerTeX(Node* node, const Config& config, TokenType type)
{
    string exponent = cToTex(node->right, type, true, config);
    string temp = cToTex(node->left, type, false, config);

    // 1. Степень 0.5 (или 1/2)
    if (exponent == "0.5" || exponent == "1/2")
    {
        string zeroPointFivePowVal = config.paramMap.at("zeroPointFivePow");
        if (zeroPointFivePowVal == "powToSqrt")
        {
            return new string("\\sqrt{" + temp + "}");
        }
        else
        {
            return new string(cToTex(node->left, type, false, config) + "^{" + exponent + "}");
        }
    }

    // 2. Извлечение числителя и знаменателя
    string numerator, denominator;
    if (!getExponentFraction(node, numerator, denominator))
    {
        return nullptr;
    }

    // 3. Степень 1/n (кроме n = 2)
    if (numerator == "1" && denominator != "2")
    {
        string oneDivNPowVal = config.paramMap.at("oneDivNPow");
        if (oneDivNPowVal == "powToSqrt")
        {
            return new string("\\sqrt[" + denominator + "]{" + temp + "}");
        }
    }

    // 4. Степень a/b (кроме a=1 и b=2)
    if (numerator != "1" || denominator != "2")
    {
        string abPowVal = config.paramMap.at("abPow");
        if (abPowVal == "powToSqrt")
        {
            return new string("\\sqrt[" + denominator + "]{" + temp + "^{" + numerator + "}}");
        }
    }

    return nullptr;
}

string getRegularPowerTeX(Node* node, const Config& config, TokenType type)
{
    string exponent = cToTex(node->right, type, true, config);
    return cToTex(node->left, type, false, config) + "^{" + exponent + "}";
}

string getTrigFunctionName(TokenType type)
{
    switch (type)
    {
    case SIN:  return "\\sin";
    case COS:  return "\\cos";
    case TAN:  return "\\tan";
    case ASIN: return "\\arcsin";
    case ACOS: return "\\arccos";
    case ATAN: return "\\arctan";
    default:   return "";
    }
}

bool getExponentFraction(Node* node, string& numerator, string& denominator)
{
    // 1. Проверка, что правый потомок является оператором деления
    if (node->right->token.type != DIV)
    {
        return false;
    }

    Node* numeratorNode = node->right->left;
    Node* denominatorNode = node->right->right;

    // 2. Проверка, что оба потомка существуют и являются числами
    if (!numeratorNode || !denominatorNode ||
        numeratorNode->token.type != NUMBER ||
        denominatorNode->token.type != NUMBER)
    {
        return false;
    }

    // 3. Извлечение значений
    numerator = numeratorNode->token.value;
    denominator = denominatorNode->token.value;

    return true;
}

bool isValidIndexRange(const vector<int>& indexes, int startIndex, int endIndex)
{
    // 1. Проверка размера диапазона
    if ((endIndex - startIndex + 1) != (int)indexes.size())
    {
        return false;
    }

    // 2. Проверка непрерывности (все индексы от startIndex до endIndex присутствуют)
    for (int i = startIndex; i <= endIndex; i++)
    {
        bool found = false;
        for (int val : indexes)
        {
            if (val == i)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        }
    }

    // 3. Проверка на дубликаты
    for (size_t i = 0; i < indexes.size(); i++)
    {
        for (size_t j = i + 1; j < indexes.size(); j++)
        {
            if (indexes[i] == indexes[j])
            {
                return false;
            }
        }
    }

    return true;
}

string getMulOperationTeX(Node* node, const Config& config)
{
    // 1. Получение настроек конфигурации
    string mulIdenVarVal = config.paramMap.at("mulIdenVar");
    string arrMulVal = config.paramMap.at("arrMul");

    // 2. Проверка на mulIdenVar (цепочка одинаковых умножений)
    if (mulIdenVarVal == "powVarN")
    {
        string operandStr;
        int varCount = 0;
        if (isMulIdenVar(node, operandStr, varCount, config))
        {
            return operandStr + "^{" + to_string(varCount) + "}";
        }
    }

    // 3. Проверка на arrMul (произведение элементов массива)
    if (arrMulVal == "combineInMul")
    {
        string arrayName;
        vector<int> indexes;
        bool arrayNameFound = false;

        if (isArrOperation(node, MUL, arrayName, indexes, arrayNameFound))
        {
            int startIndex = *min_element(indexes.begin(), indexes.end());
            int endIndex = *max_element(indexes.begin(), indexes.end());

            if (isValidIndexRange(indexes, startIndex, endIndex))
            {
                return "\\prod_{i=" + to_string(startIndex) + "}^{" + to_string(endIndex) + "} " + arrayName + "_{i}";
            }
        }
    }

    // 4. Обычное умножение
    string leftStr = getChildTexWithParens(node, node->left, false, config);
    string rightStr = getChildTexWithParens(node, node->right, true, config);
    return leftStr + " \\cdot " + rightStr;
}

string getPlusOperationTeX(Node* node, const Config& config)
{
    // 1. Получение настройки для суммы массива
    string arrSumVal = config.paramMap.at("arrSum");

    // 2. Проверка на arrSum (сумма элементов массива)
    if (arrSumVal == "combineInSum")
    {
        string arrayName;
        vector<int> indexes;
        bool arrayNameFound = false;

        if (isArrOperation(node, PLUS, arrayName, indexes, arrayNameFound))
        {
            // 2.1. Найти минимальный и максимальный индексы
            int startIndex = *min_element(indexes.begin(), indexes.end());
            int endIndex = *max_element(indexes.begin(), indexes.end());

            // 2.2. Проверить непрерывность индексов
            if (isValidIndexRange(indexes, startIndex, endIndex))
            {
                return "\\sum_{i=" + to_string(startIndex) + "}^{" + to_string(endIndex) + "} " + arrayName + "_{i}";
            }
        }
    }

    // 3. Обычное сложение 
    string leftStr = getChildTexWithParens(node, node->left, false, config);
    string rightStr = getChildTexWithParens(node, node->right, true, config);
    return leftStr + " + " + rightStr;
}

string getDivOperationTeX(Node* node, const Config& config, TokenType type)
{
    // 1. Получение настройки для преобразования логарифмов
    string logDivVal = config.paramMap.at("logDiv");

    // 2. Проверка на logDiv (деление логарифмов с одинаковым основанием)
    if (logDivVal == "logConverting")
    {
        string base, argument;
        if (isLogDiv(node, base, argument, config))
        {
            return "\\log_{" + base + "}(" + argument + ")";
        }
    }

    // 3. Обычное деление
    string leftStr = cToTex(node->left, type, false, config);
    string rightStr = cToTex(node->right, type, true, config);
    return "\\frac{" + leftStr + "}{" + rightStr + "}";
}