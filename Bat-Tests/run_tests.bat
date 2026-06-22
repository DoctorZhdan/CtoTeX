@echo off
chcp 65001 > nul
cd /d "%~dp0"
setlocal enabledelayedexpansion

del temp.txt 2>nul
del results.txt 2>nul

set passed=0
set failed=0

echo ======================================== >> results.txt
echo Running tests... >> results.txt
echo ======================================== >> results.txt
echo. >> results.txt

call :run_test "01. Simple addition" "tests\test01_expr.txt" "tests\test01_cfg.txt" "a + b"
call :run_test "02. Multiplication with powVarN" "tests\test02_expr.txt" "tests\test02_cfg.txt" "x^{3}"
call :run_test "03. Array sum with combineInSum" "tests\test03_expr.txt" "tests\test03_cfg.txt" "\sum_{i=1}^{3} a_{i}"
call :run_test "04. Sin power after var" "tests\test04_expr.txt" "tests\test04_cfg.txt" "\sin x^{2}"
call :run_test "05. Sin power after fun" "tests\test05_expr.txt" "tests\test05_cfg.txt" "\sin^{2}x"
call :run_test "06. Sin negative power divNoNegPow" "tests\test06_expr.txt" "tests\test06_cfg.txt" "\frac{1}{\sin(x)^{2}}"
call :run_test "07. Sin minus one power reverseFun" "tests\test07_expr.txt" "tests\test07_cfg.txt" "\arcsin x"
call :run_test "08. Log division logConverting" "tests\test08_expr.txt" "tests\test08_cfg.txt" "\log_{y}(x)"
call :run_test "09. Power 0.5 to sqrt" "tests\test09_expr.txt" "tests\test09_cfg.txt" "\sqrt{x}"
call :run_test "10. Square root to pow" "tests\test10_expr.txt" "tests\test10_cfg.txt" "x^{0.5}"
call :run_test "11. Array mul with combineInMul" "tests\test11_expr.txt" "tests\test11_cfg.txt" "\prod_{i=1}^{3} a_{i}"
call :run_test "12. Power 1/3 to sqrt" "tests\test12_expr.txt" "tests\test12_cfg.txt" "\sqrt[3]{x}"
call :run_test "13. Power 2/3 to sqrt" "tests\test13_expr.txt" "tests\test13_cfg.txt" "\sqrt[3]{x^{2}}"
call :run_test "14. Sin negative power negPow" "tests\test14_expr.txt" "tests\test14_cfg.txt" "\sin(x)^{-2}"
call :run_test "15. Sin minus one power divNoNegPow" "tests\test15_expr.txt" "tests\test15_cfg.txt" "\frac{1}{\sin x}"
call :run_test "16. Sin minus one power negPow" "tests\test16_expr.txt" "tests\test16_cfg.txt" "\sin x^{-1}"
call :run_test "17. Log division withoutChanges" "tests\test17_expr.txt" "tests\test17_cfg.txt" "\frac{\ln(x)}{\ln(y)}"
call :run_test "18. Square root sqrtToPow" "tests\test18_expr.txt" "tests\test18_cfg.txt" "x^{0.5}"
call :run_test "19. Mixed: array sum + power" "tests\test19_expr.txt" "tests\test19_cfg.txt" "\sum_{i=1}^{3} a_{i} + x^{2}"
call :run_test "20. Mixed: multiplication powVarN + array mul" "tests\test20_expr.txt" "tests\test20_cfg.txt" "x^{3} \cdot (\prod_{i=1}^{2} a_{i})"
call :run_test "21. Cos power after var" "tests\test21_expr.txt" "tests\test21_cfg.txt" "\cos x^{2}"
call :run_test "22. Cos power after fun" "tests\test22_expr.txt" "tests\test22_cfg.txt" "\cos^{2}x"
call :run_test "23. Tan negative power divNoNegPow" "tests\test23_expr.txt" "tests\test23_cfg.txt" "\frac{1}{\tan(x)^{3}}"
call :run_test "24. Tan minus one power reverseFun" "tests\test24_expr.txt" "tests\test24_cfg.txt" "\arctan x"
call :run_test "25. Sin of sum" "tests\test25_expr.txt" "tests\test25_cfg.txt" "\sin (a + b)"
call :run_test "26. Cos with power" "tests\test26_expr.txt" "tests\test26_cfg.txt" "\cos^{3}x"
call :run_test "27. Log base 10" "tests\test27_expr.txt" "tests\test27_cfg.txt" "\log_{10}(x)"
call :run_test "28. Exp function" "tests\test28_expr.txt" "tests\test28_cfg.txt" "e^{x}"
call :run_test "29. Double function" "tests\test29_expr.txt" "tests\test29_cfg.txt" "\sin (\cos x)"
call :run_test "30. Negative number" "tests\test30_expr.txt" "tests\test30_cfg.txt" "-5"
call :run_test "31. Fraction with sum" "tests\test31_expr.txt" "tests\test31_cfg.txt" "\frac{a + b}{c}"
call :run_test "32. Fraction with product" "tests\test32_expr.txt" "tests\test32_cfg.txt" "\frac{a \cdot b}{c}"
call :run_test "33. Power inside sin" "tests\test33_expr.txt" "tests\test33_cfg.txt" "\sin (x^{2})"
call :run_test "34. Sin squared of sum" "tests\test34_expr.txt" "tests\test34_cfg.txt" "\sin (a + b)^{2}"
call :run_test "35. Multiple operations" "tests\test35_expr.txt" "tests\test35_cfg.txt" "a + b \cdot c"
call :run_test "36. Power over fraction" "tests\test36_expr.txt" "tests\test36_cfg.txt" "\frac{a}{b}^{2}"
call :run_test "37. Fraction in power" "tests\test37_expr.txt" "tests\test37_cfg.txt" "x^{\frac{a}{b}}"
call :run_test "38. Log with power" "tests\test38_expr.txt" "tests\test38_cfg.txt" "\ln(x^{2})"
call :run_test "39. Log10 function" "tests\test39_expr.txt" "tests\test39_cfg.txt" "\log_{10}(x)"
call :run_test "40. Complex mixed" "tests\test40_expr.txt" "tests\test40_cfg.txt" "\frac{\sin x^{2} + \cos x^{2}}{y}"
call :run_test "41. Minus_Standard" "tests\test41_expr.txt" "tests\test41_cfg.txt" "a - b"
call :run_test "42. Mul_Standard" "tests\test42_expr.txt" "tests\test42_cfg.txt" "a \cdot b"
call :run_test "43. Div_Standard" "tests\test43_expr.txt" "tests\test43_cfg.txt" "\frac{a}{b}"
call :run_test "44. UnaryMinus_Standard" "tests\test44_expr.txt" "tests\test44_cfg.txt" "-a"
call :run_test "45. Mul_PowVarN" "tests\test45_expr.txt" "tests\test45_cfg.txt" "x^{3}"
call :run_test "46. Mul_CombineInMul" "tests\test46_expr.txt" "tests\test46_cfg.txt" "\prod_{i=1}^{3} a_{i}"
call :run_test "47. Pow_Standard" "tests\test47_expr.txt" "tests\test47_cfg.txt" "x^{2}"
call :run_test "48. Pow_ZeroPointFive_ToSqrt" "tests\test48_expr.txt" "tests\test48_cfg.txt" "\sqrt{x}"
call :run_test "49. Pow_OneDivN_ToSqrt" "tests\test49_expr.txt" "tests\test49_cfg.txt" "\sqrt[3]{x}"
call :run_test "50. Pow_OneDivN_ToSqrt" "tests\test50_expr.txt" "tests\test50_cfg.txt" "\sqrt[3]{x^{2}}"
call :run_test "51. Sqrt_Standard" "tests\test51_expr.txt" "tests\test51_cfg.txt" "\sqrt{x}"
call :run_test "52. Sqrt_ToPow" "tests\test52_expr.txt" "tests\test52_cfg.txt" "x^{0.5}"
call :run_test "53. Cbrt_Standard" "tests\test53_expr.txt" "tests\test53_cfg.txt" "\sqrt[3]{x}"
call :run_test "54. Sin_Standard" "tests\test54_expr.txt" "tests\test54_cfg.txt" "\sin x"
call :run_test "55. Cos_Standard" "tests\test55_expr.txt" "tests\test55_cfg.txt" "\cos x"
call :run_test "56. Tan_Standard" "tests\test56_expr.txt" "tests\test56_cfg.txt" "\tan x"
call :run_test "57. Asin_Standard" "tests\test57_expr.txt" "tests\test57_cfg.txt" "\arcsin x"
call :run_test "58. Acos_Standard" "tests\test58_expr.txt" "tests\test58_cfg.txt" "\arccos x"
call :run_test "59. Atan_Standard" "tests\test59_expr.txt" "tests\test59_cfg.txt" "\arctan x"
call :run_test "60. Trig_PowAfterFun" "tests\test60_expr.txt" "tests\test60_cfg.txt" "\sin^{2}x"
call :run_test "61. Trig_PowAfterVar" "tests\test61_expr.txt" "tests\test61_cfg.txt" "\sin x^{2}"
call :run_test "62. Trig_MinusOne_ReverseFun" "tests\test62_expr.txt" "tests\test62_cfg.txt" "\arcsin x"
call :run_test "63. Trig_NegPow_DivNoNegPow" "tests\test63_expr.txt" "tests\test63_cfg.txt" "\frac{1}{\sin(x)^{2}}"
call :run_test "64. Log_Standard" "tests\test64_expr.txt" "tests\test64_cfg.txt" "\ln(x)"
call :run_test "65. Log10_Standard" "tests\test65_expr.txt" "tests\test65_cfg.txt" "\log_{10}(x)"
call :run_test "66. Exp_Standard" "tests\test66_expr.txt" "tests\test66_cfg.txt" "e^{x}"
call :run_test "67. LogDiv_Converting" "tests\test67_expr.txt" "tests\test67_cfg.txt" "\log_{b}(a)"
call :run_test_from_file "68. Abs_Standard" "tests\test68_expr.txt" "tests\test68_cfg.txt" "tests\test68_expected.txt"
call :run_test "69. Eq_Standard" "tests\test69_expr.txt" "tests\test69_cfg.txt" "a = b"
call :run_test "70. Neq_Standard" "tests\test70_expr.txt" "tests\test70_cfg.txt" "a \neq b"
call :run_test_from_file "71. Lt_Standard" "tests\test71_expr.txt" "tests\test71_cfg.txt" "tests\test71_expected.txt"
call :run_test_from_file "72. Gt_Standard" "tests\test72_expr.txt" "tests\test72_cfg.txt" "tests\test72_expected.txt"
call :run_test "73. Le_Standard" "tests\test73_expr.txt" "tests\test73_cfg.txt" "a \leq b"
call :run_test "74. Ge_Standard" "tests\test74_expr.txt" "tests\test74_cfg.txt" "a \geq b"
call :run_test "75. Land_Standard" "tests\test75_expr.txt" "tests\test75_cfg.txt" "true \land false"
call :run_test "76. Lor_Standard" "tests\test76_expr.txt" "tests\test76_cfg.txt" "true \lor false"
call :run_test "77. Lnot_Standard" "tests\test77_expr.txt" "tests\test77_cfg.txt" "\lnot true"
call :run_test "78. ArrayIndex_Standard" "tests\test78_expr.txt" "tests\test78_cfg.txt" "a_{5}"
call :run_test "79. ArrSum_CombineInSum" "tests\test79_expr.txt" "tests\test79_cfg.txt" "\sum_{i=1}^{3} a_{i}"
call :run_test "80. Number" "tests\test80_expr.txt" "tests\test80_cfg.txt" "10"
call :run_test "81. Variable" "tests\test81_expr.txt" "tests\test81_cfg.txt" "x"
call :run_test "82. ConstantPi" "tests\test82_expr.txt" "tests\test82_cfg.txt" "\pi"
call :run_test "83. Complex_Test" "tests\test83_expr.txt" "tests\test83_cfg.txt" "(a + b) \cdot (c - d) + \frac{e}{f} - \sin x^{2} + \cos y \cdot (\sum_{i=1}^{2} z_{i})"

echo 84. Not enough operands >> results.txt
CtoTeX.exe tests\test84_expr.txt tests\test84_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 85. Too many operands >> results.txt
CtoTeX.exe tests\test85_expr.txt tests\test85_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 86. Invalid symbol >> results.txt
CtoTeX.exe tests\test86_expr.txt tests\test86_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 87. Operator not separated by spaces >> results.txt
CtoTeX.exe tests\test87_expr.txt tests\test87_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 88. Too many decimals >> results.txt
CtoTeX.exe tests\test88_expr.txt tests\test88_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 89. The expression parsing tree file contains more than one line >> results.txt
CtoTeX.exe tests\test89_expr.txt tests\test89_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 90. Sequence of invalid characters >> results.txt
CtoTeX.exe tests\test90_expr.txt tests\test90_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 91. Going beyond the range of integers >> results.txt
CtoTeX.exe tests\test91_expr.txt tests\test91_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 92. The size of the variable name is too large >> results.txt
CtoTeX.exe tests\test92_expr.txt tests\test92_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 93. Assigning an invalid value to a parameter >> results.txt
CtoTeX.exe tests\test93_expr.txt tests\test93_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 94. The parameter value is missing >> results.txt
CtoTeX.exe tests\test94_expr.txt tests\test94_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 95. The record contains a non-existent parameter >> results.txt
CtoTeX.exe tests\test95_expr.txt tests\test95_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 96. You cannot assign a value to a parameter more than once >> results.txt
CtoTeX.exe tests\test96_expr.txt tests\test96_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 97. The number of spaces exceeds the allowed value >> results.txt
CtoTeX.exe tests\test97_expr.txt tests\test97_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 98. The number of words in the string exceeds the allowed value (2) >> results.txt
CtoTeX.exe tests\test98_expr.txt tests\test98_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 99. Incompatible operand and operation types >> results.txt
CtoTeX.exe tests\test99_expr.txt tests\test99_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 100. Incompatible operand types >> results.txt
CtoTeX.exe tests\test100_expr.txt tests\test100_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 101. The record contains an empty string >> results.txt
CtoTeX.exe tests\test101_expr.txt tests\test101_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

call :run_test "102. Log10_Div" "tests\test102_expr.txt" "tests\test102_cfg.txt" "\log_{b}(a)"
call :run_test "103. Arcsin_pow" "tests\test103_expr.txt" "tests\test103_cfg.txt" "\arcsin x^{2}"
call :run_test "104. Arccos_pow" "tests\test104_expr.txt" "tests\test104_cfg.txt" "\arccos x^{2}"
call :run_test "105. Arctan_pow" "tests\test105_expr.txt" "tests\test105_cfg.txt" "\arctan x^{2}"


echo 106. the files do not exist >> results.txt
CtoTeX.exe tests\test1000_expr.txt tests\test1000_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo ======================================== >> results.txt
echo Results: Passed=%passed% Failed=%failed% >> results.txt
echo ======================================== >> results.txt

type results.txt
pause
exit /b

:run_test 
set "test_name=%~1" 
set "expr_file=%~2" 
set "cfg_file=%~3" 
set "expected=%~4" 

echo %test_name% >> results.txt 
CtoTeX.exe "%expr_file%" "%cfg_file%" temp.txt >> results.txt 2>&1 
REM 
set "tmp_expected=expected_%RANDOM%.txt" 
echo(%expected% > %tmp_expected% 

REM 
fc /w temp.txt %tmp_expected% > nul 
if errorlevel 1 ( 
	echo [FAIL] %test_name% >> results.txt 
	set /a failed+=1 
) else ( 
	echo [PASS] %test_name% >> results.txt 
	set /a passed+=1 
) 
del %tmp_expected% echo. >> results.txt 
exit /b


:run_test_from_file
set "test_name=%~1"
set "expr_file=%~2"
set "cfg_file=%~3"
set "expected_file=%~4"

echo %test_name% >> results.txt
CtoTeX.exe "%expr_file%" "%cfg_file%" temp.txt >> results.txt 2>&1

fc /w temp.txt "%expected_file%" > nul
if errorlevel 1 (
    echo [FAIL] %test_name% >> results.txt
    set /a failed+=1
) else (
    echo [PASS] %test_name% >> results.txt
    set /a passed+=1
)
echo. >> results.txt
exit /b


