@echo off
:: 快速编译脚本 - 直接使用Debug模式编译，不清理旧文件

echo [快速编译] 使用Debug模式编译...
call build.bat debug
exit /b %errorlevel%

