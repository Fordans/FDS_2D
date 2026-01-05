@echo off
setlocal enabledelayedexpansion

:: 默认构建类型
set "BUILD_TYPE=Debug"
set "BUILD_DIR=build"
set "CLEAN_BUILD=0"

:: 解析命令行参数
:parse_args
if "%~1"=="" goto :end_parse
if /i "%~1"=="release" (
    set "BUILD_TYPE=Release"
    shift
    goto :parse_args
)
if /i "%~1"=="debug" (
    set "BUILD_TYPE=Debug"
    shift
    goto :parse_args
)
if /i "%~1"=="clean" (
    set "CLEAN_BUILD=1"
    shift
    goto :parse_args
)
if /i "%~1"=="help" (
    goto :show_help
)
shift
goto :parse_args

:end_parse

:: 显示帮助信息
if "%1"=="help" goto :show_help

:: 检查CMake是否安装
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到CMake，请先安装CMake并添加到PATH环境变量
    exit /b 1
)

echo ========================================
echo   FDS_2D 编译脚本
echo ========================================
echo.
echo 构建类型: %BUILD_TYPE%
echo 构建目录: %BUILD_DIR%
echo.

:: 清理构建目录
if %CLEAN_BUILD%==1 (
    echo [信息] 清理旧的构建文件...
    if exist "%BUILD_DIR%" (
        rmdir /s /q "%BUILD_DIR%"
        echo [成功] 已清理构建目录
    )
    echo.
)

:: 创建构建目录
if not exist "%BUILD_DIR%" (
    echo [信息] 创建构建目录: %BUILD_DIR%
    mkdir "%BUILD_DIR%"
    echo.
)

:: 进入构建目录
cd "%BUILD_DIR%"

:: 运行CMake配置
echo [信息] 运行CMake配置...
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if %errorlevel% neq 0 (
    echo [错误] CMake配置失败!
    cd ..
    exit /b 1
)
echo [成功] CMake配置成功!
echo.

:: 编译项目
echo [信息] 开始编译项目...
cmake --build . --config %BUILD_TYPE%
if %errorlevel% neq 0 (
    echo [错误] 编译失败!
    cd ..
    exit /b 1
)
echo.
echo ========================================
echo   编译成功!
echo ========================================
echo.

:: 显示可执行文件位置
if "%BUILD_TYPE%"=="Debug" (
    if exist "Debug\FDS_2D.exe" (
        echo 可执行文件位置: %CD%\Debug\FDS_2D.exe
    ) else if exist "FDS_2D.exe" (
        echo 可执行文件位置: %CD%\FDS_2D.exe
    )
) else (
    if exist "Release\FDS_2D.exe" (
        echo 可执行文件位置: %CD%\Release\FDS_2D.exe
    ) else if exist "FDS_2D.exe" (
        echo 可执行文件位置: %CD%\FDS_2D.exe
    )
)

cd ..
exit /b 0

:show_help
echo.
echo FDS_2D 编译脚本使用说明
echo.
echo 用法: build.bat [选项]
echo.
echo 选项:
echo   debug      - 使用Debug模式编译 (默认)
echo   release    - 使用Release模式编译
echo   clean      - 清理旧的构建文件后重新编译
echo   help       - 显示此帮助信息
echo.
echo 示例:
echo   build.bat              - Debug模式编译
echo   build.bat release      - Release模式编译
echo   build.bat clean        - 清理后Debug模式编译
echo   build.bat release clean - 清理后Release模式编译
echo.
exit /b 0

