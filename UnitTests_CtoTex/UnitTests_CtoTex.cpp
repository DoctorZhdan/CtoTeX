#include "pch.h"
#include "CppUnitTest.h"
#include "../CtoTeX/CtoTeX.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestsCtoTex
{
    TEST_CLASS(getChildTexWithParens_UnitTests)
    {
    public:

        TEST_METHOD(PlusInsideMul_LeftChild)
        {
            // Плюс внутри умножения (левый потомок) 
            string expr = "a b + c *";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;

            string result = getChildTexWithParens(root, left, false, config);

            Assert::AreEqual(string("(a + b)"), result);

            delete root;
        }

        TEST_METHOD(PlusInsideMul_RightChild)
        {
            // Плюс внутри умножения (правый потомок) 
            string expr = "a b c + *";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* right = root->right;

            string result = getChildTexWithParens(root, right, true, config);

            Assert::AreEqual(string("(b + c)"), result);

            delete root;
        }

        TEST_METHOD(MulInsidePlus_LeftChild)
        {
            // Умножение внутри сложения 
            string expr = "a b * c +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;

            string result = getChildTexWithParens(root, left, false, config);

            Assert::AreEqual(string("a \\cdot b"), result);

            delete root;
        }

        TEST_METHOD(MinusInsideMinus_RightChild)
        {
            // Вычитание справа от вычитания 
            string expr = "a b c - -";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* right = root->right;

            string result = getChildTexWithParens(root, right, true, config);

            Assert::AreEqual(string("(b - c)"), result);

            delete root;
        }

        TEST_METHOD(PlusInsidePow_Base)
        {
            // Сумма в основании степени
            string expr = "a b + c #pow";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;

            string result = getChildTexWithParens(root, left, false, config);

            Assert::AreEqual(string("(a + b)"), result);

            delete root;
        }

        TEST_METHOD(PlusInsidePow_Exponent)
        {
            // Сумма в показателе степени 
            string expr = "a b c + #pow";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* right = root->right;

            string result = getChildTexWithParens(root, right, true, config);

            Assert::AreEqual(string("(b + c)"), result);

            delete root;
        }

        TEST_METHOD(PlusInsideUnaryMinus)
        {
            // Сумма под унарным минусом 
            string expr = "a b + -_";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* child = root->left;

            string result = getChildTexWithParens(root, child, false, config);

            Assert::AreEqual(string("(a + b)"), result);

            delete root;
        }

        TEST_METHOD(VariableChild)
        {
            // Сумма переменных 
            string expr = "a b +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;

            string result = getChildTexWithParens(root, left, false, config);

            Assert::AreEqual(string("a"), result);

            delete root;
        }

        TEST_METHOD(OrInsideAnd)
        {
            // Логическое ИЛИ внутри логического И 
            string expr = "true false || true &&";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;

            string result = getChildTexWithParens(root, left, false, config);

            Assert::AreEqual(string("(true \\lor false)"), result);

            delete root;
        }

        TEST_METHOD(AndInsideOr)
        {
            // Логическое И внутри логического ИЛИ  
            string expr = "true false && true ||";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;

            string result = getChildTexWithParens(root, left, false, config);

            Assert::AreEqual(string("true \\land false"), result);

            delete root;
        }

        TEST_METHOD(NotInsideAnd)
        {
            // Логическое НЕ внутри логического И 
            string expr = "true ! true &&";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;

            string result = getChildTexWithParens(root, left, false, config);

            Assert::AreEqual(string("\\lnot true"), result);

            delete root;
        }

        TEST_METHOD(ComplexLogicalExpression)
        {
            // Сложное логическое выражение   
            string expr = "true false || true false || &&";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            Node* left = root->left;
            Node* right = root->right;

            string leftResult = getChildTexWithParens(root, left, false, config);
            string rightResult = getChildTexWithParens(root, right, true, config);

            Assert::AreEqual(string("(true \\lor false)"), leftResult);
            Assert::AreEqual(string("(true \\lor false)"), rightResult);

            delete root;


        }

        TEST_METHOD(ComplexArithmeticWithoutDivision)
        {
            // Сложное арифметическое выражение (a + b) * (c - d) + e * f
            string expr = "a b + c d - * e f * +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Config config;

            Node* mulNode = root;
            Node* leftChild = mulNode->left;
            Node* rightChild = mulNode->right;

            string leftResult = getChildTexWithParens(mulNode, leftChild, false, config);
            string rightResult = getChildTexWithParens(mulNode, rightChild, true, config);

            Assert::AreEqual(string("(a + b) \\cdot (c - d)"), leftResult);
            Assert::AreEqual(string("e \\cdot f"), rightResult);

            delete root;
        }

        TEST_METHOD(InvalidExpression_NotEnoughOperands)
        {
            // Не хватает операндов
            string expr = "a +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == NotEnoughOperands) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        TEST_METHOD(InvalidExpression_TooManyOperands)
        {
            // Лишние операнды
            string expr = "a b c +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == TooManyOperands) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }


    };

    TEST_CLASS(IsArrOperation_UnitTests)
    {
    public:

        TEST_METHOD(OperatorType_PLUS)
        {
            // Тип оператора PLUS (сложение)
            string expr = "a[1] a[2] +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(OperatorType_MUL)
        {
            // Тип оператора MUL (умножение)
            string expr = "a[1] a[2] *";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, MUL, name, idx, found));

            delete root;
        }

        TEST_METHOD(Count_One)
        {
            // Один элемент массива
            string expr = "a[1]";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(Count_Two)
        {
            // Два элемента массива
            string expr = "a[1] a[2] +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(Count_ThreeOrMore)
        {
            // Более двух элементов массива
            string expr = "a[1] a[2] + a[3] +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(ArrayName_Same)
        {
            // Одинаковое имя массива
            string expr = "a[1] a[2] +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(ArrayName_Different)
        {
            // Разные имена массивов
            string expr = "a[1] b[2] +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsFalse(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(OnlyArrayIndex)
        {
            // Только обращения к массиву
            string expr = "a[1] a[2] +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(OtherOperators)
        {
            // Есть посторонний оператор
            string expr = "a[1] a[2] * a[3] +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsFalse(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(ValidTree)
        {
            // Корректное поддерево
            string expr = "a[1] a[2] *";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsTrue(isArrOperation(root, MUL, name, idx, found));

            delete root;
        }

        TEST_METHOD(Nullptr)
        {
            // Пустой указатель
            Node* root = nullptr;

            string name; vector<int> idx; bool found = false;
            Assert::IsFalse(isArrOperation(root, PLUS, name, idx, found));
        }

        TEST_METHOD(NoArrayIndex)
        {
            // Нет обращений к массиву
            string expr = "5 3 +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string name; vector<int> idx; bool found = false;
            Assert::IsFalse(isArrOperation(root, PLUS, name, idx, found));

            delete root;
        }

        TEST_METHOD(Invalid_EmptyExpression)
        {
            // Пустое выражение
            string expr = "";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == NotEnoughOperands) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        TEST_METHOD(Invalid_WrongArraySyntax)
        {
            // Неправильный синтаксис массива: a[1 без закрывающей скобки
            string expr = "a[1";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == InvalidSymbol || errors[i].code == InvalidSymbolSequence) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        TEST_METHOD(ArrayIndexRightChildInvalid)
        {
            // Правый потомок ARRAY_INDEX не является числом
            Token arrayToken;
            arrayToken.type = ARRAY_INDEX;
            arrayToken.value = "a[i]";
            arrayToken.operandType = ARITHMETIC;

            Token varToken;
            varToken.type = VARIABLE;
            varToken.value = "a";
            varToken.operandType = ARITHMETIC;

            Token invalidToken;
            invalidToken.type = VARIABLE;  
            invalidToken.value = "x";
            invalidToken.operandType = ARITHMETIC;

            Node* arrayNode = new Node(arrayToken);
            Node* leftNode = new Node(varToken);
            Node* rightNode = new Node(invalidToken);

            arrayNode->left = leftNode;
            arrayNode->right = rightNode;

            string arrayName;
            vector<int> indexes;
            bool arrayNameFound = false;

            bool result = isArrOperation(arrayNode, PLUS, arrayName, indexes, arrayNameFound);
            Assert::IsFalse(result);

            delete arrayNode;
         
        }


    };

    TEST_CLASS(IsLogDiv_UnitTests)
    {
    public:

        TEST_METHOD(Valid_LOG_DIV)
        {
            // Деление двух натуральных логарифмов (LOG / LOG), одинаковое основание (e)

            string expr = "a #log b #log /";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string base, arg;
            Config config;

            Assert::IsTrue(isLogDiv(root, base, arg, config));

            delete root;
        }

        TEST_METHOD(Valid_LOG10_DIV)
        {
            // Деление двух десятичных логарифмов (LOG10 / LOG10), одинаковое основание (10)

            string expr = "a #log10 b #log10 /";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string base, arg;
            Config config;

            Assert::IsTrue(isLogDiv(root, base, arg, config));

            delete root;
        }

        TEST_METHOD(DifferentBases)
        {
            // Логарифмы с разными основаниями (LOG / LOG10)

            string expr = "a #log b #log10 /";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string base, arg;
            Config config;

            Assert::IsFalse(isLogDiv(root, base, arg, config));

            delete root;
        }

        TEST_METHOD(NotDIVNode)
        {
            // Узел не является делением

            string expr = "a #log b #log +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string base, arg;
            Config config;

            Assert::IsFalse(isLogDiv(root, base, arg, config));

            delete root;
        }

        TEST_METHOD(LeftNotLog)
        {
            // Левый узел не является логарифмом

            string expr = "a b #log /";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string base, arg;
            Config config;

            Assert::IsFalse(isLogDiv(root, base, arg, config));

            delete root;
        }

        TEST_METHOD(RightNotLog)
        {
            // Правый узел не является логарифмом

            string expr = "a #log b /";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string base, arg;
            Config config;

            Assert::IsFalse(isLogDiv(root, base, arg, config));

            delete root;
        }

        TEST_METHOD(NullptrNode)
        {
            // Пустой указатель (nullptr)

            Node* root = nullptr;

            string base, arg;
            Config config;

            Assert::IsFalse(isLogDiv(root, base, arg, config));
        }

        TEST_METHOD(InvalidExpression_NotEnoughOperands)
        {
            // Не хватает операндов для деления
            string expr = "a #log /";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == NotEnoughOperands) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        TEST_METHOD(InvalidExpression_TooManyOperands)
        {
            // Лишние операнды после деления логарифмов
            string expr = "a #log b #log / c";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == TooManyOperands) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

    };

    TEST_CLASS(IsMulIdenVar_UnitTests)
    {
    public:

        TEST_METHOD(Basic_MulChain)
        {
            // Цепочка MUL одинаковых операндов
            string expr = "x x * x *";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string operandStr;
            int varCount = 0;
            Config config;

            Assert::IsTrue(isMulIdenVar(root, operandStr, varCount, config));
            Assert::AreEqual(3, varCount);
            Assert::AreEqual(string("x"), operandStr);

            delete root;
        }

        TEST_METHOD(SingleOperand)
        {
            // Одиночный узел, лист
            string expr = "y";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string operandStr;
            int varCount = 0;
            Config config;

            Assert::IsTrue(isMulIdenVar(root, operandStr, varCount, config));
            Assert::AreEqual(1, varCount);
            Assert::AreEqual(string("y"), operandStr);

            delete root;
        }

        TEST_METHOD(DifferentOperands)
        {
            // Цепочка MUL с разными операндами
            string expr = "x y *";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string operandStr;
            int varCount = 0;
            Config config;

            Assert::IsFalse(isMulIdenVar(root, operandStr, varCount, config));

            delete root;
        }

        TEST_METHOD(NotMulOperator)
        {
            // Узел другой операции
            string expr = "x x +";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string operandStr;
            int varCount = 0;
            Config config;

            Assert::IsFalse(isMulIdenVar(root, operandStr, varCount, config));

            delete root;
        }

        TEST_METHOD(NullptrNode)
        {
            // nullptr
            Node* root = nullptr;

            string operandStr;
            int varCount = 0;
            Config config;

            Assert::IsFalse(isMulIdenVar(root, operandStr, varCount, config));
        }

        TEST_METHOD(TwoOperands)
        {
            // Два одинаковых операнда
            string expr = "a a *";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string operandStr;
            int varCount = 0;
            Config config;

            Assert::IsTrue(isMulIdenVar(root, operandStr, varCount, config));
            Assert::AreEqual(2, varCount);
            Assert::AreEqual(string("a"), operandStr);

            delete root;
        }

        TEST_METHOD(ManyOperands)
        {
            // Четыре одинаковых операнда
            string expr = "z z * z * z *";

            Node* root = nullptr;
            vector<Error> errors;
            Assert::IsTrue(buildTree(expr, root, operatorInfo, errors));

            string operandStr;
            int varCount = 0;
            Config config;

            Assert::IsTrue(isMulIdenVar(root, operandStr, varCount, config));
            Assert::AreEqual(4, varCount);
            Assert::AreEqual(string("z"), operandStr);

            delete root;
        }

        TEST_METHOD(InvalidExpression_InvalidCharInMulChain)
        {
            // Недопустимый символ в цепочке умножений
            string expr = "x @ x *";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == InvalidSymbol) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        TEST_METHOD(InvalidExpression_OperatorNotSeparatedInMulChain)
        {
            // Оператор не отделён пробелами в цепочке умножений
            string expr = "x x*x";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == OperatorNotSeparatedBySpaces) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

    };

    TEST_CLASS(cToTex_UnitTests)
    {
    public:

        // Сложение 
        TEST_METHOD(Plus_Standard)
        {
            string expr = "a b +";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a + b"), result);
            delete root;
        }

        // Вычитание 
        TEST_METHOD(Minus_Standard)
        {
            string expr = "a b -";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a - b"), result);
            delete root;
        }

        // Умножение
        TEST_METHOD(Mul_Standard)
        {
            string expr = "a b *";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a \\cdot b"), result);
            delete root;
        }

        // Деление 
        TEST_METHOD(Div_Standard)
        {
            string expr = "a b /";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\frac{a}{b}"), result);
            delete root;
        }

        // Унарный минус 
        TEST_METHOD(UnaryMinus_Standard)
        {
            string expr = "a -_";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("-a"), result);
            delete root;
        }

        // Умножение с mulIdenVar = powVarN 
        TEST_METHOD(Mul_PowVarN)
        {
            string expr = "x x x * *";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["mulIdenVar"] = "powVarN";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("x^{3}"), result);
            delete root;
        }

        // Умножение с arrMul = combineInMul 
        TEST_METHOD(Mul_CombineInMul)
        {
            string expr = "a[1] a[2] a[3] * *";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["arrMul"] = "combineInMul";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\prod_{i=1}^{3} a_{i}"), result);
            delete root;
        }


        // Возведение в степень 
        TEST_METHOD(Pow_Standard)
        {
            string expr = "x 2 #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("x^{2}"), result);
            delete root;
        }

        // Степень 0.5 с powToSqrt 
        TEST_METHOD(Pow_ZeroPointFive_ToSqrt)
        {
            string expr = "x 0.5 #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["zeroPointFivePow"] = "powToSqrt";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sqrt{x}"), result);
            delete root;
        }

        // Степень 1/3 с powToSqrt 
        TEST_METHOD(Pow_OneDivN_ToSqrt)
        {
            string expr = "x 1 3 / #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["oneDivNPow"] = "powToSqrt";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sqrt[3]{x}"), result);
            delete root;
        }

        // Степень 2/3 с powToSqrt 
        TEST_METHOD(Pow_AB_ToSqrt)
        {
            string expr = "x 2 3 / #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["abPow"] = "powToSqrt";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sqrt[3]{x^{2}}"), result);
            delete root;
        }

        // Квадратный корень 
        TEST_METHOD(Sqrt_Standard)
        {
            string expr = "x #sqrt";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sqrt{x}"), result);
            delete root;
        }

        // Квадратный корень с sqrtToPow 
        TEST_METHOD(Sqrt_ToPow)
        {
            string expr = "x #sqrt";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["squareRoot"] = "sqrtToPow";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("x^{0.5}"), result);
            delete root;
        }

        // Кубический корень 
        TEST_METHOD(Cbrt_Standard)
        {
            string expr = "x #cbrt";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sqrt[3]{x}"), result);
            delete root;
        }

        // Синус 
        TEST_METHOD(Sin_Standard)
        {
            string expr = "x #sin";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sin x"), result);
            delete root;
        }

        // Косинус 
        TEST_METHOD(Cos_Standard)
        {
            string expr = "x #cos";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\cos x"), result);
            delete root;
        }

        // Тангенс 
        TEST_METHOD(Tan_Standard)
        {
            string expr = "x #tan";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\tan x"), result);
            delete root;
        }

        // Арксинус 
        TEST_METHOD(Asin_Standard)
        {
            string expr = "x #asin";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\arcsin x"), result);
            delete root;
        }

        // Арккосинус
        TEST_METHOD(Acos_Standard)
        {
            string expr = "x #acos";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\arccos x"), result);
            delete root;
        }

        // Арктангенс 
        TEST_METHOD(Atan_Standard)
        {
            string expr = "x #atan";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\arctan x"), result);
            delete root;
        }

        // sin^2(x) с powAfterFun 
        TEST_METHOD(Trig_PowAfterFun)
        {
            string expr = "x #sin 2 #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["trigFunNoNegPow"] = "powAfterFun";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sin^{2}x"), result);
            delete root;
        }

        //  sin(x)^2 с powAfterVar 
        TEST_METHOD(Trig_PowAfterVar)
        {
            string expr = "x #sin 2 #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["trigFunNoNegPow"] = "powAfterVar";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sin x^{2}"), result);
            delete root;
        }

        // sin^{-1}(x) с reverseFun 
        TEST_METHOD(Trig_MinusOne_ReverseFun)
        {
            string expr = "x #sin -1 #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["trigFunMinusOnePow"] = "reverseFun";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\arcsin x"), result);
            delete root;
        }

        // sin^{-2}(x) с divNoNegPow 
        TEST_METHOD(Trig_NegPow_DivNoNegPow)
        {
            string expr = "x #sin -2 #pow";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["trigFunNegPow"] = "divNoNegPow";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\frac{1}{\\sin(x)^{2}}"), result);
            delete root;
        }

        // Натуральный логарифм 
        TEST_METHOD(Log_Standard)
        {
            string expr = "x #log";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\ln(x)"), result);
            delete root;
        }

        // Десятичный логарифм 
        TEST_METHOD(Log10_Standard)
        {
            string expr = "x #log10";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\log_{10}(x)"), result);
            delete root;
        }

        // Экспонента
        TEST_METHOD(Exp_Standard)
        {
            string expr = "x #exp";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("e^{x}"), result);
            delete root;
        }

        // Деление логарифмов с logConverting 
        TEST_METHOD(LogDiv_Converting)
        {
            string expr = "a #log b #log /";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["logDiv"] = "logConverting";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\log_{b}(a)"), result);
            delete root;
        }

        // Модуль
        TEST_METHOD(Abs_Standard)
        {
            string expr = "x #abs";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("|x|"), result);
            delete root;
        }

        // Равенство 
        TEST_METHOD(Eq_Standard)
        {
            string expr = "a b ==";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a = b"), result);
            delete root;
        }

        // Неравенство
        TEST_METHOD(Neq_Standard)
        {
            string expr = "a b !=";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a \\neq b"), result);
            delete root;
        }

        // Меньше 
        TEST_METHOD(Lt_Standard)
        {
            string expr = "a b <";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a < b"), result);
            delete root;
        }

        // Больше 
        TEST_METHOD(Gt_Standard)
        {
            string expr = "a b >";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a > b"), result);
            delete root;
        }

        // Меньше или равно 
        TEST_METHOD(Le_Standard)
        {
            string expr = "a b <=";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a \\leq b"), result);
            delete root;
        }

        // Больше или равно 
        TEST_METHOD(Ge_Standard)
        {
            string expr = "a b >=";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a \\geq b"), result);
            delete root;
        }

        // Логическое И 
        TEST_METHOD(Land_Standard)
        {
            string expr = "true false &&";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("true \\land false"), result);
            delete root;
        }

        // Логическое ИЛИ 
        TEST_METHOD(Lor_Standard)
        {
            string expr = "true false ||";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("true \\lor false"), result);
            delete root;
        }

        // Логическое НЕ 
        TEST_METHOD(Lnot_Standard)
        {
            string expr = "true !";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::IsTrue(result == "\\lnot true");
            delete root;
        }

        // Индексация 
        TEST_METHOD(ArrayIndex_Standard)
        {
            string expr = "a[5]";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("a_{5}"), result);
            delete root;
        }

        // Сумма элементов массива с combineInSum 
        TEST_METHOD(ArrSum_CombineInSum)
        {
            string expr = "a[1] a[2] a[3] + +";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            config.paramMap["arrSum"] = "combineInSum";
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\sum_{i=1}^{3} a_{i}"), result);
            delete root;
        }


        // Число 
        TEST_METHOD(Number_Leaf)
        {
            string expr = "10";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("10"), result);
            delete root;
        }

        // Переменная 
        TEST_METHOD(Variable_Leaf)
        {
            string expr = "x";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("x"), result);
            delete root;
        }

        // Константа
        TEST_METHOD(ConstantPi_Leaf)
        {
            string expr = "pi";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Config config;
            string result = cToTex(root, UNKNOWN, false, config);
            Assert::AreEqual(string("\\pi"), result);
            delete root;
        }

        // Пустой узел
        TEST_METHOD(NullNode)
        {
            Config config;
            string result = cToTex(nullptr, UNKNOWN, false, config);
            Assert::AreEqual(string(""), result);
        }

        // Комплексный тест
        TEST_METHOD(Complex_Test)
        {

            string expr = "a b + c d - * x #sin 2 #pow -";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["trigFunNoNegPow"] = "powAfterVar";

            string result = cToTex(root, UNKNOWN, false, config);


            string expected = "(a + b) \\cdot (c - d) - \\sin x^{2}";

            Assert::AreEqual(expected, result);

            delete root;
        }


        // Комплексный тест 2
        TEST_METHOD(Complex_Test2)
        {
            // Выражение: (a + b) * (c - d) + e / f - sin(x)^2 + cos(y) * (z[1] + z[2])

            string expr = "a b + c d - * e f / + x #sin 2 #pow - y #cos z[1] z[2] + * +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["mulIdenVar"] = "withoutChanges";
            config.paramMap["arrSum"] = "combineInSum";
            config.paramMap["trigFunNoNegPow"] = "powAfterVar";

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "(a + b) \\cdot (c - d) + \\frac{e}{f} - \\sin x^{2} + \\cos y \\cdot (\\sum_{i=1}^{2} z_{i})";

            Assert::AreEqual(expected, result);

            delete root;
        }

        // Ошибка: несовместимые типы операндов 
        TEST_METHOD(InvalidExpression_MismatchedTypesInArithmetic)
        {
            string expr = "true 5 +";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == MismatchedOperandTypes || errors[i].code == ArithmeticWithLogical) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        // Ошибка: вещественное число с более чем 8 знаками после запятой
        TEST_METHOD(InvalidExpression_TooManyDecimalPlaces)
        {
            string expr = "3.123456789 2 +";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == TooManyDecimalDigits) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        // Несколько ошибок
        TEST_METHOD(ManyErrorsInTheExpression)
        {
            string expr = "a 1.2345678934 + c d - * e f / + x #sin 2#pow - y #cos $ z[1] z[2] + * +";
            Node* root = nullptr;
            vector<Error> errors;
            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsFalse(ok);
            Assert::IsTrue(!errors.empty());

            bool found = false;
            for (size_t i = 0; i < errors.size(); ++i) {
                if (errors[i].code == TooManyDecimalDigits || errors[i].code == OperatorNotSeparatedBySpaces || errors[i].code == InvalidSymbol) {
                    found = true;
                }
            }
            Assert::IsTrue(found);

            delete root;
        }

        // Комплексный тест 3
        TEST_METHOD(Complex_Test3)
        {
            // Выражение: (a + b) * (c - d) + e / f - sin(x)^2 + cos(y) * (z[1] + z[2])

            string expr = "a a * a * a * e #log f #log / + x #sin 2 #pow - y #cos z[1] z[2] + * +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["mulIdenVar"] = "powVarN";
            config.paramMap["arrSum"] = "combineInSum";
            config.paramMap["trigFunNoNegPow"] = "powAfterVar";
            config.paramMap["logDiv "] = "logConverting";

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "a^{4} + \\log_{f}(e) - \\sin x^{2} + \\cos y \\cdot (\\sum_{i=1}^{2} z_{i})";

            Assert::AreEqual(expected, result);

            delete root;
        }


        TEST_METHOD(ArrSum_NonContinuousIndexes)
        {
            // Сумма элементов массива с пропусками индексов
            string expr = "a[1] a[3] + a[5] +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["arrSum"] = "combineInSum";

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "a_{1} + a_{3} + a_{5}";
            Assert::AreEqual(expected, result);

            delete root;
        }


        TEST_METHOD(ArrMul_NonContinuousIndexes)
        {
            // Произведение элементов массива с пропусками индексов
            string expr = "a[1] a[3] * a[5] *";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);

            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["arrMul "] = "combineInMul";

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "a_{1} \\cdot a_{3} \\cdot a_{5}";
            Assert::AreEqual(expected, result);

            delete root;
        }

        TEST_METHOD(AllTrigFunctions)
        {
            // Сумма всех тригонометрических функций
            string expr = "x #sin x #cos + x #tan + x #asin + x #acos + x #atan +";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "\\sin x + \\cos x + \\tan x + \\arcsin x + \\arccos x + \\arctan x";
            Assert::AreEqual(expected, result);

            delete root;
        } 

        TEST_METHOD(TrigFunction_NegativePower_NegPow)
        {
            // Тригонометрическая функция в отрицательной степени 
            string expr = "x #sin -2 #pow";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["trigFunNegPow"] = "negPow";  

            string result = cToTex(root, UNKNOWN, false, config);

            
            string expected = "\\sin(x)^{-2}";
            Assert::AreEqual(expected, result);

            delete root;
        }


        TEST_METHOD(TrigFunction_MinusOnePower_DivNoNegPow)
        {
            // Тригонометрическая функция в степени -1 с настройкой trigFunMinusOnePow = divNoNegPow
            string expr = "x #sin -1 #pow";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["trigFunMinusOnePow"] = "divNoNegPow";

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "\\frac{1}{\\sin x}";
            Assert::AreEqual(expected, result);

            delete root;
        }


        TEST_METHOD(TrigFunction_MinusOnePower_NegPow)
        {
            // Тригонометрическая функция в степени -1 с настройкой trigFunMinusOnePow = negPow 
            string expr = "x #sin -1 #pow";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["trigFunMinusOnePow"] = "negPow";  

            string result = cToTex(root, UNKNOWN, false, config);

            
            string expected = "\\sin x^{-1}";
            Assert::AreEqual(expected, result);

            delete root;
        }


        TEST_METHOD(TrigFunction_PositivePower_Default)
        {
            // Тригонометрическая функция в положительной степени с настройками trigFunNoNegPow = powAfterVar
            string expr = "x #sin 2 #pow";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "\\sin x^{2}";
            Assert::AreEqual(expected, result);

            delete root;
        }

        TEST_METHOD(Power_ZeroPointFive_WithoutChanges)
        {
            // x^{0.5} с настройкой zeroPointFivePow = withoutChanges 
            string expr = "x 0.5 #pow";

            Node* root = nullptr;
            vector<Error> errors;

            bool ok = buildTree(expr, root, operatorInfo, errors);
            Assert::IsTrue(ok);
            Assert::IsTrue(errors.empty());

            Config config;
            config.paramMap["zeroPointFivePow"] = "withoutChanges";  

            string result = cToTex(root, UNKNOWN, false, config);

            string expected = "x^{0.5}";
            Assert::AreEqual(expected, result);

            delete root;
        }


    };

}
