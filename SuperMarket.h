#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

//Manager + Customer (Done)
void registerUserType(UserType type);
void welcomeScreen();
void registerStage();
void loginUser();
void profile();
void printProfile();
