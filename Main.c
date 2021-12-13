//Headers
#include "SuperMarket.h"

// Strings (Done)
void inputString(char** _Str) 
{
	char string[100] = { NULL };

	scanf_s("%s", string, 100);
	*_Str = malloc(strlen(string) * sizeof(char) + sizeof(char));
	strcpy(*_Str, string);
}
void appendString(char** _Str1, char* _Str2) 
{
	char* pString = NULL;
	
	pString = malloc((strlen(*_Str1) + strlen(_Str2)) * sizeof(char) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, *_Str1);
	strcat(pString, _Str2);
	free(*_Str1);
	*_Str1 = pString;
}
void appendStringComma(char** _Str1, char* _Str2) 
{
	appendString(_Str1, _Str2);
	appendString(_Str1, ",");
}
void appendStringNewLine(char** _Str1, char* _Str2) 
{
	appendString(_Str1, _Str2);
	appendString(_Str1, "\n");
}
char* copyString(char* _Str) 
{
	char* pString = NULL;

	pString = malloc(sizeof(char) * strlen(_Str) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, _Str);
	return pString;
}
char* strToLower(char* _Str) 
{
	char* pString = NULL;

	pString = copyString(_Str);
	if (!pString) exit(1);

	for (int i = 0; i < strlen(_Str); i++)
		pString[i] = tolower(pString[i]);

	return pString;
}
int convertString()
{
	char string[500] = { NULL };

	printf("Input --> ");
	scanf_s("%s", string, 500);

	return atoi(string);
}

//Manager + Customer (Status)
void registerUserType(UserType type) 
{
	Details d;

	printf("\nRegister Stage\n");

	while (!verifyName(&d))
		printf("Invalid, Try again\n\n");

	while (!verifyPassword(&d))
		printf("Invalid, Try again\n\n");

	while (!verifyId(&d))
		printf("Invalid, Try again!\n\n");

	if (type == customer)
		if (!verifyAge())
			return;

	while (!verifyPhone(&d))
		printf("Invalid, Try again\n\n");

	if (!termsAndConditions())
		return;

	writeUserType(&d, type);
}
void welcomeScreen()
{
	int selection = 0;
	bool loop = true;

	printf("Welcome To HomeScreen\n");

	while(loop)
	{
		printf("%-15s%-15s%-15s\n", "'1' Register", "'2' Login", "'3' Exit");
		selection = convertString();
		
		switch (selection) 
		{
		case 1:
			registerStage();
			break;

		case 2:
			loginUser();
			break;

		case 3:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 3]\n\n");
			break;
		}
	}
}
void registerStage() 
{
	int selection = 0, managerCode = 0;
	bool loop = true;

	printf("\nRegister as\n");

	while (loop)
	{
		printf("'1' Customer\t'2' Manager\t'3' Exit\n");
		selection = convertString();

		switch (selection) 
		{
		case 1:
			registerUserType(customer);
			loop = false;
			break;

		case 2:
			printf("Manager Code ");
			managerCode = convertString();

			if (MANAGER_CODE == managerCode)
				registerUserType(manager);

			else
				printf("Wrong Code, GoodBye");

			loop = false;
			break;

		case 3:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 3]\n\n");
			break;
		}
	}
}
void loginUser() 
{
	char* userId, * userPassword;

	printf("\nLogin Stage\nID --> ");
	inputString(&userId);

	Identity = userId;

	UserType type = findUserType();

	if (!type) 
	{
		printf("ID Does Not Exist\n\n");
		return none;
	}

	printf("Password --> ");
	inputString(&userPassword);

	Details* user = readUser(type == customer ? FILE_CUSTOMERS : FILE_MANAGERS, type);

	if (strcmp(readUser(type == customer ? FILE_CUSTOMERS : FILE_MANAGERS, readPassword)->password, userPassword) == 0) 
	{
		printf("You've Logged As A ");

		if (type == customer) 
		{
			printf("Customer\n");
			customerMenu();
		}

		if (type == manager) 
		{
			printf("Manager.\n");
			managerMenu();
		}
	}

	else
		printf("Invalid Password\n\n");
}
void profile() 
{
	int selection = 0;
	bool loop = true;

	printf("\nProfile Actions\n");

	while (loop)
	{
		printf("'1' Print Profile\t'2' Update Profile\t'3' Exit\n");
		selection = convertString();

		switch (selection) 
		{
		case 1:
			printProfile();
			loop = false;
			break;

		case 2:
			updateProfile();
			loop = false;
			break;

		case 3:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 3]\n\n");
			break;
		}
	}
}
void printProfile() 
{
	Details* details = readUser(findUserType(Identity) == customer ? FILE_CUSTOMERS : FILE_MANAGERS, findUserType(Identity));
	char name[100] = { NULL }, id[100] = { NULL }, password[100] = { NULL }, phone[100] = { NULL };
	float po = details->points;

	strcpy(name, details->name);
	strcpy(id, details->ID);
	strcpy(password, details->password);
	strcpy(phone, details->phone);

	printf("\nUser Information\n");
	printf("Name --> %s\n", name);
	printf("ID --> %s\n", id);
	printf("Password --> %s\n", password);
	printf("Phone --> %s\n", phone);

	if (findUserType(Identity) == customer)
		printf("Supermarket Points --> %.2f\n", po);
}

int main()
{
  return 0;
}
