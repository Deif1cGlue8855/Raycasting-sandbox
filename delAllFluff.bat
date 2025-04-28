@echo off

:: Make sure build/bin exists
if not exist build\bin (
    echo No bin folder found. Skipping selective clean.
) else (
    echo Cleaning build folder except for bin...

    :: Delete everything in build except bin
    for /d %%D in (build\*) do (
        if /I not "%%~nxD"=="bin" rd /s /q "%%D"
    )
    for %%F in (build\*) do (
        if /I not "%%~nxF"=="bin" del /q "%%F"
    )
)