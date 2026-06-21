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


echo 41. Not enough operands >> results.txt
CtoTeX.exe tests\test41_expr.txt tests\test41_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 42. Too many operands >> results.txt
CtoTeX.exe tests\test42_expr.txt tests\test42_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 43. Invalid symbol >> results.txt
CtoTeX.exe tests\test43_expr.txt tests\test43_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 44. Operator not separated by spaces >> results.txt
CtoTeX.exe tests\test44_expr.txt tests\test44_cfg.txt temp.txt >> results.txt 2>&1

echo. >> results.txt

echo 45. Too many decimals >> results.txt
CtoTeX.exe tests\test45_expr.txt tests\test45_cfg.txt temp.txt >> results.txt 2>&1

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
REM --- создаём временный файл --- 
set "tmp_expected=expected_%RANDOM%.txt" 
echo(%expected% > %tmp_expected% 

REM --- сравнение --- 
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