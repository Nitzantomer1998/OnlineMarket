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
void inputString(char** _Str);
void appendString(char** _Str1, char* _Str2);
void appendStringComma(char** _Str1, char* _Str2);
void appendStringNewLine(char** _Str1, char* _Str2);
char* copyString(char* _Str);
char* strToLower(char* _Str);
int convertString();

// Authentication (Done)
bool verifyName(Details* d);
bool verifyPassword(Details* d);
bool verifyId(Details* d);
bool verifyAge();
bool verifyPhone(Details* d);
bool verifyPayment(char* creditCard, char* cvv, int expirationMonth, int expirationYear);
bool termsAndConditions();

//Manager + Customer (Done)
void registerUserType(UserType type);
void welcomeScreen();
void registerStage();
void loginUser();
void profile();
void printProfile();
