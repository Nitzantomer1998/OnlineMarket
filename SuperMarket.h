#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

// Strings (Done)
void inputString(char** _str);
void appendString(char** _str1, char* _str2);
void appendStringComma(char** _str1, char* _str2);
void appendStringNewLine(char** _str1, char* _str2);
char* copyString(char* _str);
char* strToLower(char* _str);
int convertString();

//Manager + Customer (Done)
void registerUserType(UserType type);
void welcomeScreen();
void registerStage();
void loginUser();
void profile();
void printProfile();
