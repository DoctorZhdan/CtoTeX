@echo off
setlocal enabledelayedexpansion

:: --- НАСТРОЙКА ПУТЕЙ ---
set "SOURCES_DIR=C:\Users\Asus\Desktop\CtoTeX\CtoTeX"
set "EXE=CtoTeX.exe"
set "TESTS_DIR=tests"
set "CFG_FILE=%TESTS_DIR%\test01_cfg.txt"

:: --- СОЗДАЁМ ПАПКУ ДЛЯ ОТЧЁТОВ ---
if not exist coverage_reports mkdir coverage_reports
del coverage_reports\*.cov 2>nul

:: --- ПРОГОНЯЕМ ТЕСТЫ ---
echo Running test 01...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test01.cov -- %EXE% tests\test01_expr.txt tests\test01_cfg.txt temp.txt

echo Running test 02...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test02.cov -- %EXE% tests\test02_expr.txt tests\test02_cfg.txt temp.txt

echo Running test 03...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test03.cov -- %EXE% tests\test03_expr.txt tests\test03_cfg.txt temp.txt

echo Running test 04...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test04.cov -- %EXE% tests\test04_expr.txt tests\test04_cfg.txt temp.txt

echo Running test 05...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test05.cov -- %EXE% tests\test05_expr.txt tests\test05_cfg.txt temp.txt

echo Running test 06...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test06.cov -- %EXE% tests\test06_expr.txt tests\test06_cfg.txt temp.txt

echo Running test 07...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test07.cov -- %EXE% tests\test07_expr.txt tests\test07_cfg.txt temp.txt

echo Running test 08...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test08.cov -- %EXE% tests\test08_expr.txt tests\test08_cfg.txt temp.txt

echo Running test 09...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test09.cov -- %EXE% tests\test09_expr.txt tests\test09_cfg.txt temp.txt

echo Running test 10...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test10.cov -- %EXE% tests\test10_expr.txt tests\test10_cfg.txt temp.txt

echo Running test 11...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test11.cov -- %EXE% tests\test11_expr.txt tests\test11_cfg.txt temp.txt

echo Running test 12...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test12.cov -- %EXE% tests\test12_expr.txt tests\test12_cfg.txt temp.txt

echo Running test 13...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test13.cov -- %EXE% tests\test13_expr.txt tests\test13_cfg.txt temp.txt

echo Running test 14...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test14.cov -- %EXE% tests\test14_expr.txt tests\test14_cfg.txt temp.txt

echo Running test 15...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test15.cov -- %EXE% tests\test15_expr.txt tests\test15_cfg.txt temp.txt

echo Running test 16...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test16.cov -- %EXE% tests\test16_expr.txt tests\test16_cfg.txt temp.txt

echo Running test 17...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test17.cov -- %EXE% tests\test17_expr.txt tests\test17_cfg.txt temp.txt

echo Running test 18...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test18.cov -- %EXE% tests\test18_expr.txt tests\test18_cfg.txt temp.txt

echo Running test 19...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test19.cov -- %EXE% tests\test19_expr.txt tests\test19_cfg.txt temp.txt

echo Running test 20...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test20.cov -- %EXE% tests\test20_expr.txt tests\test20_cfg.txt temp.txt

echo Running test 21...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test21.cov -- %EXE% tests\test21_expr.txt tests\test21_cfg.txt temp.txt

echo Running test 22...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test22.cov -- %EXE% tests\test22_expr.txt tests\test22_cfg.txt temp.txt

echo Running test 23...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test23.cov -- %EXE% tests\test23_expr.txt tests\test23_cfg.txt temp.txt

echo Running test 24...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test24.cov -- %EXE% tests\test24_expr.txt tests\test24_cfg.txt temp.txt

echo Running test 25...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test25.cov -- %EXE% tests\test25_expr.txt tests\test25_cfg.txt temp.txt

echo Running test 26...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test26.cov -- %EXE% tests\test26_expr.txt tests\test26_cfg.txt temp.txt

echo Running test 27...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test27.cov -- %EXE% tests\test27_expr.txt tests\test27_cfg.txt temp.txt

echo Running test 28...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test28.cov -- %EXE% tests\test28_expr.txt tests\test28_cfg.txt temp.txt

echo Running test 29...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test29.cov -- %EXE% tests\test29_expr.txt tests\test29_cfg.txt temp.txt

echo Running test 30...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test30.cov -- %EXE% tests\test30_expr.txt tests\test30_cfg.txt temp.txt

echo Running test 31...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test31.cov -- %EXE% tests\test31_expr.txt tests\test31_cfg.txt temp.txt

echo Running test 32...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test32.cov -- %EXE% tests\test32_expr.txt tests\test32_cfg.txt temp.txt

echo Running test 33...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test33.cov -- %EXE% tests\test33_expr.txt tests\test33_cfg.txt temp.txt

echo Running test 34...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test34.cov -- %EXE% tests\test34_expr.txt tests\test34_cfg.txt temp.txt

echo Running test 35...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test35.cov -- %EXE% tests\test35_expr.txt tests\test35_cfg.txt temp.txt

echo Running test 36...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test36.cov -- %EXE% tests\test36_expr.txt tests\test36_cfg.txt temp.txt

echo Running test 37...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test37.cov -- %EXE% tests\test37_expr.txt tests\test37_cfg.txt temp.txt

echo Running test 38...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test38.cov -- %EXE% tests\test38_expr.txt tests\test38_cfg.txt temp.txt

echo Running test 39...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test39.cov -- %EXE% tests\test39_expr.txt tests\test39_cfg.txt temp.txt

echo Running test 40...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test40.cov -- %EXE% tests\test40_expr.txt tests\test40_cfg.txt temp.txt

echo Running test 41...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test41.cov -- %EXE% tests\test41_expr.txt tests\test41_cfg.txt temp.txt

echo Running test 42...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test42.cov -- %EXE% tests\test42_expr.txt tests\test42_cfg.txt temp.txt

echo Running test 43...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test43.cov -- %EXE% tests\test43_expr.txt tests\test43_cfg.txt temp.txt

echo Running test 44...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test44.cov -- %EXE% tests\test44_expr.txt tests\test44_cfg.txt temp.txt

echo Running test 45...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test45.cov -- %EXE% tests\test45_expr.txt tests\test45_cfg.txt temp.txt

echo Running test 46...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test46.cov -- %EXE% tests\test46_expr.txt tests\test46_cfg.txt temp.txt

echo Running test 47...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test47.cov -- %EXE% tests\test47_expr.txt tests\test47_cfg.txt temp.txt

echo Running test 48...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test48.cov -- %EXE% tests\test48_expr.txt tests\test48_cfg.txt temp.txt

echo Running test 49...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test49.cov -- %EXE% tests\test49_expr.txt tests\test49_cfg.txt temp.txt

echo Running test 50...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test50.cov -- %EXE% tests\test50_expr.txt tests\test50_cfg.txt temp.txt

echo Running test 51...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test51.cov -- %EXE% tests\test51_expr.txt tests\test51_cfg.txt temp.txt

echo Running test 52...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test52.cov -- %EXE% tests\test52_expr.txt tests\test52_cfg.txt temp.txt

echo Running test 53...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test53.cov -- %EXE% tests\test53_expr.txt tests\test53_cfg.txt temp.txt

echo Running test 54...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test54.cov -- %EXE% tests\test54_expr.txt tests\test54_cfg.txt temp.txt

echo Running test 55...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test55.cov -- %EXE% tests\test55_expr.txt tests\test55_cfg.txt temp.txt

echo Running test 56...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test56.cov -- %EXE% tests\test56_expr.txt tests\test56_cfg.txt temp.txt

echo Running test 57...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test57.cov -- %EXE% tests\test57_expr.txt tests\test57_cfg.txt temp.txt

echo Running test 58...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test58.cov -- %EXE% tests\test58_expr.txt tests\test58_cfg.txt temp.txt

echo Running test 59...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test59.cov -- %EXE% tests\test59_expr.txt tests\test59_cfg.txt temp.txt

echo Running test 60...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test60.cov -- %EXE% tests\test60_expr.txt tests\test60_cfg.txt temp.txt

echo Running test 61...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test61.cov -- %EXE% tests\test61_expr.txt tests\test61_cfg.txt temp.txt

echo Running test 62...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test62.cov -- %EXE% tests\test62_expr.txt tests\test62_cfg.txt temp.txt

echo Running test 63...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test63.cov -- %EXE% tests\test63_expr.txt tests\test63_cfg.txt temp.txt

echo Running test 64...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test64.cov -- %EXE% tests\test64_expr.txt tests\test64_cfg.txt temp.txt

echo Running test 65...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test65.cov -- %EXE% tests\test65_expr.txt tests\test65_cfg.txt temp.txt

echo Running test 66...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test66.cov -- %EXE% tests\test66_expr.txt tests\test66_cfg.txt temp.txt

echo Running test 67...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test67.cov -- %EXE% tests\test67_expr.txt tests\test67_cfg.txt temp.txt

echo Running test 68...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test68.cov -- %EXE% tests\test68_expr.txt tests\test68_cfg.txt temp.txt

echo Running test 69...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test69.cov -- %EXE% tests\test69_expr.txt tests\test69_cfg.txt temp.txt

echo Running test 70...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test70.cov -- %EXE% tests\test70_expr.txt tests\test70_cfg.txt temp.txt

echo Running test 71...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test71.cov -- %EXE% tests\test71_expr.txt tests\test71_cfg.txt temp.txt

echo Running test 72...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test72.cov -- %EXE% tests\test72_expr.txt tests\test72_cfg.txt temp.txt

echo Running test 73...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test73.cov -- %EXE% tests\test73_expr.txt tests\test73_cfg.txt temp.txt

echo Running test 74...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test74.cov -- %EXE% tests\test74_expr.txt tests\test74_cfg.txt temp.txt

echo Running test 75...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test75.cov -- %EXE% tests\test75_expr.txt tests\test75_cfg.txt temp.txt

echo Running test 76...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test76.cov -- %EXE% tests\test76_expr.txt tests\test76_cfg.txt temp.txt

echo Running test 77...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test77.cov -- %EXE% tests\test77_expr.txt tests\test77_cfg.txt temp.txt

echo Running test 78...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test78.cov -- %EXE% tests\test78_expr.txt tests\test78_cfg.txt temp.txt

echo Running test 79...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test79.cov -- %EXE% tests\test79_expr.txt tests\test79_cfg.txt temp.txt

echo Running test 80...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test80.cov -- %EXE% tests\test80_expr.txt tests\test80_cfg.txt temp.txt

echo Running test 81...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test81.cov -- %EXE% tests\test81_expr.txt tests\test81_cfg.txt temp.txt

echo Running test 82...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test82.cov -- %EXE% tests\test82_expr.txt tests\test82_cfg.txt temp.txt

echo Running test 83...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test83.cov -- %EXE% tests\test83_expr.txt tests\test83_cfg.txt temp.txt

echo Running test 84...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test84.cov -- %EXE% tests\test84_expr.txt tests\test84_cfg.txt temp.txt

echo Running test 85...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test85.cov -- %EXE% tests\test85_expr.txt tests\test85_cfg.txt temp.txt

echo Running test 86...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test86.cov -- %EXE% tests\test86_expr.txt tests\test86_cfg.txt temp.txt

echo Running test 87...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test87.cov -- %EXE% tests\test87_expr.txt tests\test87_cfg.txt temp.txt

echo Running test 88...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test88.cov -- %EXE% tests\test88_expr.txt tests\test88_cfg.txt temp.txt

echo Running test 89...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test89.cov -- %EXE% tests\test89_expr.txt tests\test89_cfg.txt temp.txt

echo Running test 90...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test90.cov -- %EXE% tests\test90_expr.txt tests\test90_cfg.txt temp.txt

echo Running test 91...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test91.cov -- %EXE% tests\test91_expr.txt tests\test91_cfg.txt temp.txt

echo Running test 92...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test92.cov -- %EXE% tests\test92_expr.txt tests\test92_cfg.txt temp.txt

echo Running test 93...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test93.cov -- %EXE% tests\test93_expr.txt tests\test93_cfg.txt temp.txt

echo Running test 94...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test94.cov -- %EXE% tests\test94_expr.txt tests\test94_cfg.txt temp.txt

echo Running test 95...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test95.cov -- %EXE% tests\test95_expr.txt tests\test95_cfg.txt temp.txt

echo Running test 96...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test96.cov -- %EXE% tests\test96_expr.txt tests\test96_cfg.txt temp.txt

echo Running test 97...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test97.cov -- %EXE% tests\test97_expr.txt tests\test97_cfg.txt temp.txt

echo Running test 98...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test98.cov -- %EXE% tests\test98_expr.txt tests\test98_cfg.txt temp.txt

echo Running test 99...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test99.cov -- %EXE% tests\test99_expr.txt tests\test99_cfg.txt temp.txt

echo Running test 100...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test100.cov -- %EXE% tests\test100_expr.txt tests\test100_cfg.txt temp.txt

echo Running test 101...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test101.cov -- %EXE% tests\test101_expr.txt tests\test101_cfg.txt temp.txt

echo Running test 102...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test102.cov -- %EXE% tests\test102_expr.txt tests\test102_cfg.txt temp.txt

echo Running test 103...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test103.cov -- %EXE% tests\test103_expr.txt tests\test103_cfg.txt temp.txt

echo Running test 104...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test104.cov -- %EXE% tests\test104_expr.txt tests\test104_cfg.txt temp.txt

echo Running test 105...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test105.cov -- %EXE% tests\test105_expr.txt tests\test105_cfg.txt temp.txt

echo Running test 106...
OpenCppCoverage.exe --sources "%SOURCES_DIR%" --export_type=binary:coverage_reports\test106.cov -- %EXE% tests\test1000_expr.txt tests\test1000_cfg.txt temp.txt


:: --- ОБЪЕДИНЯЕМ ВСЕ .cov ФАЙЛЫ В ИТОГОВЫЙ ОТЧЁТ ---
echo Merging reports...
OpenCppCoverage.exe ^
    --input_coverage=coverage_reports\test01.cov ^
    --input_coverage=coverage_reports\test02.cov ^
    --input_coverage=coverage_reports\test03.cov ^
    --input_coverage=coverage_reports\test04.cov ^
    --input_coverage=coverage_reports\test05.cov ^
    --input_coverage=coverage_reports\test06.cov ^
    --input_coverage=coverage_reports\test07.cov ^
    --input_coverage=coverage_reports\test08.cov ^
    --input_coverage=coverage_reports\test09.cov ^
    --input_coverage=coverage_reports\test10.cov ^
    --input_coverage=coverage_reports\test11.cov ^
    --input_coverage=coverage_reports\test12.cov ^
    --input_coverage=coverage_reports\test13.cov ^
    --input_coverage=coverage_reports\test14.cov ^
    --input_coverage=coverage_reports\test15.cov ^
    --input_coverage=coverage_reports\test16.cov ^
    --input_coverage=coverage_reports\test17.cov ^
    --input_coverage=coverage_reports\test18.cov ^
    --input_coverage=coverage_reports\test19.cov ^
    --input_coverage=coverage_reports\test20.cov ^
    --input_coverage=coverage_reports\test21.cov ^
    --input_coverage=coverage_reports\test22.cov ^
    --input_coverage=coverage_reports\test23.cov ^
    --input_coverage=coverage_reports\test24.cov ^
    --input_coverage=coverage_reports\test25.cov ^
    --input_coverage=coverage_reports\test26.cov ^
    --input_coverage=coverage_reports\test27.cov ^
    --input_coverage=coverage_reports\test28.cov ^
    --input_coverage=coverage_reports\test29.cov ^
    --input_coverage=coverage_reports\test30.cov ^
    --input_coverage=coverage_reports\test31.cov ^
    --input_coverage=coverage_reports\test32.cov ^
    --input_coverage=coverage_reports\test33.cov ^
    --input_coverage=coverage_reports\test34.cov ^
    --input_coverage=coverage_reports\test35.cov ^
    --input_coverage=coverage_reports\test36.cov ^
    --input_coverage=coverage_reports\test37.cov ^
    --input_coverage=coverage_reports\test38.cov ^
    --input_coverage=coverage_reports\test39.cov ^
    --input_coverage=coverage_reports\test40.cov ^
    --input_coverage=coverage_reports\test41.cov ^
    --input_coverage=coverage_reports\test42.cov ^
    --input_coverage=coverage_reports\test43.cov ^
    --input_coverage=coverage_reports\test44.cov ^
    --input_coverage=coverage_reports\test45.cov ^
    --input_coverage=coverage_reports\test46.cov ^
    --input_coverage=coverage_reports\test47.cov ^
    --input_coverage=coverage_reports\test48.cov ^
    --input_coverage=coverage_reports\test49.cov ^
    --input_coverage=coverage_reports\test50.cov ^
    --input_coverage=coverage_reports\test51.cov ^
    --input_coverage=coverage_reports\test52.cov ^
    --input_coverage=coverage_reports\test53.cov ^
    --input_coverage=coverage_reports\test54.cov ^
    --input_coverage=coverage_reports\test55.cov ^
    --input_coverage=coverage_reports\test56.cov ^
    --input_coverage=coverage_reports\test57.cov ^
    --input_coverage=coverage_reports\test58.cov ^
    --input_coverage=coverage_reports\test59.cov ^
    --input_coverage=coverage_reports\test60.cov ^
    --input_coverage=coverage_reports\test61.cov ^
    --input_coverage=coverage_reports\test62.cov ^
    --input_coverage=coverage_reports\test63.cov ^
    --input_coverage=coverage_reports\test64.cov ^
    --input_coverage=coverage_reports\test65.cov ^
    --input_coverage=coverage_reports\test66.cov ^
    --input_coverage=coverage_reports\test67.cov ^
    --input_coverage=coverage_reports\test68.cov ^
    --input_coverage=coverage_reports\test69.cov ^
    --input_coverage=coverage_reports\test70.cov ^
    --input_coverage=coverage_reports\test71.cov ^
    --input_coverage=coverage_reports\test72.cov ^
    --input_coverage=coverage_reports\test73.cov ^
    --input_coverage=coverage_reports\test74.cov ^
    --input_coverage=coverage_reports\test75.cov ^
    --input_coverage=coverage_reports\test76.cov ^
    --input_coverage=coverage_reports\test77.cov ^
    --input_coverage=coverage_reports\test78.cov ^
    --input_coverage=coverage_reports\test79.cov ^
    --input_coverage=coverage_reports\test80.cov ^
    --input_coverage=coverage_reports\test81.cov ^
    --input_coverage=coverage_reports\test82.cov ^
    --input_coverage=coverage_reports\test83.cov ^
    --input_coverage=coverage_reports\test84.cov ^
    --input_coverage=coverage_reports\test85.cov ^
    --input_coverage=coverage_reports\test86.cov ^
    --input_coverage=coverage_reports\test87.cov ^
    --input_coverage=coverage_reports\test88.cov ^
    --input_coverage=coverage_reports\test89.cov ^
    --input_coverage=coverage_reports\test90.cov ^
    --input_coverage=coverage_reports\test91.cov ^
    --input_coverage=coverage_reports\test92.cov ^
    --input_coverage=coverage_reports\test93.cov ^
    --input_coverage=coverage_reports\test94.cov ^
    --input_coverage=coverage_reports\test95.cov ^
    --input_coverage=coverage_reports\test96.cov ^
    --input_coverage=coverage_reports\test97.cov ^
    --input_coverage=coverage_reports\test98.cov ^
    --input_coverage=coverage_reports\test99.cov ^
    --input_coverage=coverage_reports\test100.cov ^
    --input_coverage=coverage_reports\test101.cov ^
    --input_coverage=coverage_reports\test102.cov ^
    --input_coverage=coverage_reports\test103.cov ^
    --input_coverage=coverage_reports\test104.cov ^
    --input_coverage=coverage_reports\test105.cov ^
    --input_coverage=coverage_reports\test106.cov ^
    --export_type=html:coverage_reports\MergedReport

echo Done! Open coverage_reports\MergedReport\index.html
pause