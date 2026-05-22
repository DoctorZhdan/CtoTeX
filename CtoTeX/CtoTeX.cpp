
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


int main()
{
    
}


