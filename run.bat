@echo off
:: 运行脚本 - 自动编译并运行游戏

setlocal

set "BUILD_TYPE=Debug"
set "BUILD_DIR=build"

:: 检查是否指定了release参数
if /i "%1"=="release" (
    set "BUILD_TYPE=Release"
)

echo [信息] 检查是否需要编译...

:: 检查可执行文件是否存在
if "%BUILD_TYPE%"=="Debug" (
    set "EXE_PATH=%BUILD_DIR%\Debug\FDS_2D.exe"
    if not exist "%EXE_PATH%" (
        set "EXE_PATH=%BUILD_DIR%\FDS_2D.exe"
    )
) else (
    set "EXE_PATH=%BUILD_DIR%\Release\FDS_2D.exe"
    if not exist "%EXE_PATH%" (
        set "EXE_PATH=%BUILD_DIR%\FDS_2D.exe"
    )
)

:: 如果可执行文件不存在，先编译
if not exist "%EXE_PATH%" (
    echo [信息] 可执行文件不存在，开始编译...
    call build.bat %BUILD_TYPE%
    if %errorlevel% neq 0 (
        echo [错误] 编译失败，无法运行
        exit /b 1
    )
    echo.
)

:: 运行游戏
echo [信息] 启动游戏...
echo.
if exist "%EXE_PATH%" (
    "%EXE_PATH%"
) else (
    echo [错误] 找不到可执行文件: %EXE_PATH%
    exit /b 1
)

exit /b 0

