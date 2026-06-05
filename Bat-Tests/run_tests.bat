@echo off
chcp 65001 > nul
setlocal enabledelayedexpansion

set passed=0
set failed=0

for %%f in (tests\*_in.txt) do (

    set "filename=%%~nf"
    set "basename=!filename:_in=!"

    set "expected=tests\!basename!_out.txt"
    set "config=tests\!basename!_cfg.txt"

    echo Testing %%~nxf ...

    CtoTeX.exe "%%f" "!config!" temp.txt

    echo --- EXPECTED ---
    type "!expected!"

    echo --- GOT ---
    type temp.txt

    fc temp.txt "!expected!"

    if errorlevel 1 (
        echo  FAILED: %%~nxf
        set /a failed+=1
    ) else (
        echo  PASSED: %%~nxf
        set /a passed+=1
    )

    echo -------------------------
)

echo Passed: %passed%
echo Failed: %failed%

pause