@echo off

@echo:
@echo [SoftRenderer MSVC project generation]
@echo ============================================================================
PUSHD %~dp0

@echo:
@echo Removing previous project folder.
IF EXIST "Project" rd "Project" /s /q
@echo:

IF NOT EXIST "Project" @echo Creating a new project folder.
IF NOT EXIST "Project" md Project
IF NOT EXIST "Project" @echo:

cd Project
@echo ============================================================================
@echo:
@echo Running CMAKE script.
@echo:

cmake -G "Visual Studio 15 2017" ..\

@echo:
@echo ============================================================================
@echo:
@echo A new solution file is generated in %~dp0Project
@echo:
POPD
PAUSE
