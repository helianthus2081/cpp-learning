@echo off
g++ -std=c++11 -Wall -Wextra binary_tree_exp3.cpp -o binary_tree_exp3.exe
if errorlevel 1 exit /b %errorlevel%
binary_tree_exp3.exe
