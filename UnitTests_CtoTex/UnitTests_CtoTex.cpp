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
    };
}
