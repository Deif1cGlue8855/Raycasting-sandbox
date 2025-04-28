@echo off

:: Check if the build folder exists, and if so, delete it
if exist build (
    echo Deleting existing build folder...
    rd /s /q build
)

:: Create the build directory
mkdir build
cd build

:: Run CMake to configure the project
cmake ..

:: Build the project
cmake --build .

:: Navigate to the Debug folder where the executable is located
cd bin/Debug

:: Check if the executable exists and run it
if exist my_project.exe (
    echo Running my_project.exe...
    my_project.exe
) else (
    echo Executable not found. Build might have failed.
)

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

