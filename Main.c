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

// Files (Done)
void checkFiles() 
{
	if (!doesFileExists(FILE_TEMP)) writeFile(FILE_TEMP, "");
	if (!doesFileExists(FILE_TICKET)) writeFile(FILE_TICKET, "");
	if (!doesFileExists(FILE_ORDERS)) writeFile(FILE_ORDERS, "");
	if (!doesFileExists(FILE_GLOBAL)) writeFile(FILE_GLOBAL, "0");
	if (!doesFileExists(FILE_CATALOG)) writeFile(FILE_CATALOG, "");
	if (!doesFileExists(FILE_MANAGERS)) writeFile(FILE_MANAGERS, "");
	if (!doesFileExists(FILE_CUSTOMERS)) writeFile(FILE_CUSTOMERS, "");
}
void checkFolder() 
{
	if (!doesFileExists(FOLDER_DATA)) createFolder(FOLDER_DATA);
	if (!doesFileExists(FOLDER_DATA_USERS)) createFolder(FOLDER_DATA_USERS);
	if (!doesFileExists(FOLDER_DATA_ORDERS)) createFolder(FOLDER_DATA_ORDERS);
	if (!doesFileExists(FOLDER_DATA_TICKETS)) createFolder(FOLDER_DATA_TICKETS);
}
void createFolder(char* dirname) 
{
	int check = 0;
	
	check = mkdir(dirname);
	if (check) exit(1);
}
void writeFile(char* filename, char* content) 
{
	FILE* file = fopen(filename, "ab+");
	if (!file) exit(1);

	fprintf(file, "%s", content);
	fclose(file);
}
void writeUserType(Details* d, UserType type) 
{
	char output[200] = { NULL };

	if (type == customer) 
	{
		sprintf(output, "%s,%s,%s,%.2f,%s\n", d->name, d->ID, d->password, 0.0, d->phone);
		writeFile(FILE_CUSTOMERS, output);
	}

	if (type == manager) 
	{
		sprintf(output, "%s,%s,%s,%s\n", d->name, d->ID, d->password, d->phone);
		writeFile(FILE_MANAGERS, output);
	}
}
bool doesFileExists(char* filename) 
{
	struct stat buffer;
	return !stat(filename, &buffer);
}
Details* readUser(char* filename, UserType type) 
{
	Details details = { NULL, NULL, NULL, NULL, 0 };
	char nameString[100] = { NULL }, IdString[100] = { NULL }, passwordString[100] = { NULL }, buffer[500] = { NULL }, phoneString[100] = { NULL };
	float supermarketPoints = 0.0;

	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	while (fscanf(file, "%s", buffer) == 1)
	{
		if (type == customer) sscanf(buffer, "%[^,],%[^,],%[^,],%f,%[^,]", nameString, IdString, passwordString, &supermarketPoints, phoneString);
		if (type == manager)  sscanf(buffer, "%[^,],%[^,],%[^,],%[^,]", nameString, IdString, passwordString, phoneString);

		if (strcmp(IdString, Identity) == 0) 
		{
			Details returnDetails = { copyString(nameString), copyString(IdString), copyString(passwordString), copyString(phoneString), supermarketPoints };
			fclose(file);
			return &returnDetails;
		}
	}

	fclose(file);
	return &details;
}
UserType findUserType() 
{
	Details* details = readUser(FILE_CUSTOMERS, customer);
	if (details->ID) return customer;

	details = readUser(FILE_MANAGERS, manager);
	if (details->ID) return manager;

	return none;
}

// Authentication (Done)
bool verifyName(Details* d) 
{
	char string[100] = { NULL };

	printf("Name --> ");
	scanf_s(" %[^\n]", string, 100);

	for (int i = 0; i < strlen(string); i++) 
	{
		if (!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z'))) 
		{
			printf("Name Can Contain Only English Alphabet\n");
			d->name = NULL;
			return false;
		}
	}

	char* pString = NULL;
	pString = copyString(string);

	d->name = pString;

	return true;
}
bool verifyPassword(Details* d) 
{
	char* pString = NULL;

	printf("Password --> ");
	inputString(&pString);

	if (strlen(pString) < MIN_PASSWORD_LEN) 
	{
		printf("Password Must Contain At Least 6 Characters\n");
		return false;
	}

	int letters = 0, numbers = 0;

	for (int i = 0; i < strlen(pString); i++)
	{
		if (!(pString[i] >= 'a' && pString[i] <= 'z')) 
		{
			if (!(pString[i] >= 'A' && pString[i] <= 'Z')) 
			{
				if (!(pString[i] >= '0' && pString[i] <= '9'))
					return false;
				else numbers++;
			}
			else letters++;
		}
		else letters++;
	}

	if (!numbers)
		printf("Password Must Contain At Least One Number\n");

	if (!letters)
		printf("Password Must Contain At Least One letter\n");

	if (letters && numbers) 
	{
		d->password = pString;
		return true;
	}

	return false;
}
bool verifyId(Details* d) 
{
	char* pString = NULL;

	printf("ID --> ");
	inputString(&pString);
	
	Identity = pString;
	
	long idVal = strtol(pString, NULL, 10);
	int Digit1 = 0, Digit2 = 0, Digit3 = 0, Digit4 = 0, Digit5 = 0, Digit6 = 0, Digit7 = 0, Digit8 = 0, Digit9 = 0, Sum = 0, Res = 0;

	if ((idVal <= 999999999) && (idVal >= 100000000)) 
	{
		Digit1 = (idVal % 1000000000) / 100000000;
		Digit2 = (idVal % 100000000) / 10000000;
		Digit3 = (idVal % 10000000) / 1000000;
		Digit4 = (idVal % 1000000) / 100000;
		Digit5 = (idVal % 100000) / 10000;
		Digit6 = (idVal % 10000) / 1000;
		Digit7 = (idVal % 1000) / 100;
		Digit8 = (idVal % 100) / 10;
		Digit9 = (idVal % 10);

		Digit1 = Digit1 * 1;
		Digit2 = Digit2 * 2;
		Digit3 = Digit3 * 1;
		Digit4 = Digit4 * 2;
		Digit5 = Digit5 * 1;
		Digit6 = Digit6 * 2;
		Digit7 = Digit7 * 1;
		Digit8 = Digit8 * 2;

		Digit2 = ((Digit2 / 10) + (Digit2 % 10));
		Digit4 = ((Digit4 / 10) + (Digit4 % 10));
		Digit6 = ((Digit6 / 10) + (Digit6 % 10));
		Digit8 = ((Digit8 / 10) + (Digit8 % 10));

		Sum = Digit1 + Digit2 + Digit3 + Digit4 + Digit5 + Digit6 + Digit7 + Digit8;

		Res = 10 - (Sum % 10);

		if (Res == Digit9) 
		{
			if (findUserType(pString)) 
			{
				printf("ID Already Exist In The System\n");

				int selection = 0;
				bool loop = true;

				printf("\nAvailable Actions\n");

				while (loop)
				{
					printf("'1' Wrong ID\t'2' Report For Identity Steal\n");
					selection = convertString();

					switch (selection) 
					{
					case 1:
						loop = false;
						break;

					case 2:
						printf("We Are Sry To Hear, Your Report Will Be Treated\nGoodbye\n");
						exit(1);

					default:
						printf("Invalid Input, Try Between [1 To 2]\n\n");
						break;
					}
				}
				return false;
			}
			d->ID = pString;
			return true;
		}

		else if (Res != Digit9)
			return false;
	}

	else
		return false;
}
bool verifyAge() 
{
	int age = 0;

	printf("Age --> ");
	scanf_s("%d", &age);

	if (age < LEGAL_AGE) 
	{
		printf("We Are Sry, The Minimum Age Is 16\n");
		return false;
	}

	return true;
}
bool verifyPhone(Details* d) 
{
	char* pString = NULL;

	printf("Phone --> ");
	inputString(&pString);

	if (!pString) exit(1);

	if (strlen(pString) != PHONE_NUM_LEN) 
	{
		printf("Phone Number Must Contain Ten Digits\n");
		return false;
	}

	for (int i = 0; i < strlen(pString); i++) 
	{
		if (!(pString[i] >= '0' && pString[i] <= '9')) 
		{
			printf("Phone Number Can Contain Only Digits\n");
			return false;
		}
	}

	d->phone = pString;
	return true;
}
bool verifyPayment(char* creditCard, char* cvv, int expirationMonth, int expirationYear) 
{
	if (expirationMonth <= 0 || expirationMonth > 12)
		return false;

	for (int i = 0; i < strlen(creditCard); i++)
		if (!(creditCard[i] >= '0' && creditCard[i] <= '9'))
			return false;

	for (int i = 0; i < strlen(cvv); i++)
		if (!(cvv[i] >= '0' && cvv[i] <= '9'))
			return false;

	Date date = getCurrentDate();
	if (date.year > expirationYear)
		return false;

	else if (date.year == expirationYear)
		return date.month <= expirationMonth;

	return true;
}
bool termsAndConditions() 
{

	char terms = { NULL };

	printf("Terms and Conditions\n\n1. The Site, including any content and / or service available through it, is provided to you 'As It Is'. Although the Company takes all efforts to present the Site or through it as accurate and reliable information as possible, the Company is not and will not be responsible, directly or indirectly, for the availability, veracity, reliability and/or accuracy of the content appearing on the Site, and reliance on any content displayed on or through the Site is at your full responsibility.\n\n2. You may use the Site and the content available through it for private and personal purposes only. The content of the Site does not grant you any rights other than those set forth in these Terms, which constitutes an agreement for all intents and purposes between you and the Company.\n\n3. The content of the Website may not be used as a basis for the purpose of making financial, legal, personal and/or other decisions on your part. Any action taken by you based on this Content is done at your sole discretion and at your own responsibility. Products, descriptions, design, colors or the appearance of products and services described or displayed on the site are for illustration purposes only, in order to simulate the user's desired product in the closest and best way. If you have identified a significant gap between the visibility of the actual requested product and the product appearing on the site, please contact us to correct it, at the company's sole discretion. In any case, the Company shall not be liable in connection with any product and/or recipe and/or recommendations detailed or displayed on the website and through it.\n\n");

	while (terms != 'Y' && terms != 'N') 
	{
		printf("Do you agree to the terms and conditions (Y/N)\nInput --> ");
		scanf_s(" %c", &terms, 1);

		if (terms != 'Y' && terms != 'N')
			printf("Invalid Input, Try Again\n\n");
	}
	
	if (terms == 'Y') return true;

	else if (terms == 'N') return false;
}

//Manager + Customer (Status)
void registerUserType(UserType type) 
{
	printf("\nRegister Stage\n");

	Details d;

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
	printf("Welcome To HomeScreen\n");

	int selection = 0;
	bool loop = true;

	while(loop)
	{
		printf("'1' Register     '2' Login     '3' Exit\n");
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
	printf("\nRegister as\n");

	int selection = 0, managerCode = 0;
	bool loop = true;

	while (loop)
	{
		printf("'1' Customer     '2' Manager     '3' Exit\n");
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
		printf("'1' Print Profile     '2' Update Profile     '3' Exit\n");
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
void updateProfile() 
{
	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* User = fopen(findUserType(Identity) == customer ? FILE_CUSTOMERS : FILE_MANAGERS, "r");
	if (!User) exit(1);

	bool loop = true;
	int selection = 0;
	char _Name[100] = { NULL }, _Id[100] = { NULL }, _Password[100] = { NULL }, _Phone[100] = { NULL }, _Points[100] = { NULL }, buffer[500] = { NULL };
	Details d;

	if (findUserType(Identity) == customer) 
	{
		while (fscanf(User, "%s", buffer) == 1) 
		{
			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Points, _Phone);

			if ((strcmp(Identity, _Id)) == 0) 
			{
				printf("\nChange Options\n");

				while (loop)
				{
					printf("'1' Name     '2' ID     '3' Password     '4' Phone\n");
					selection = convertString();

					switch (selection)
					{
					case 1:
						while (true)
							if (verifyName(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s,%s\n", d.name, _Id, _Password, _Points, _Phone);
						loop = false;
						break;

					case 2:
						while (true)
							if (verifyId(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, d.ID, _Password, _Points, _Phone);
						loop = false;
						break;

					case 3:
						while (true)
							if (verifyPassword(&d))
								break;
						
						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, d.password, _Points, _Phone);
						loop = false;
						break;

					case 4:
						while (true)
							if (verifyPhone(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, d.phone);
						loop = false;
						break;

					default:
						printf("Invalid Input, Try Between [1 To 4]\n\n");
						break;
					}
				}
			}

			else
				fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, _Phone);
		}

		fclose(Temp);
		fclose(User);

		User = fopen(FILE_CUSTOMERS, "w");
		if (!User) exit(1);

		Temp = fopen(FILE_TEMP, "r");
		if (!Temp) exit(1);

		while (fscanf(Temp, "%s", buffer) == 1) 
		{

			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Points, _Phone);
			fprintf(User, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, _Phone);
		}
		fclose(Temp);
		fclose(User);
	}


	else if (findUserType(Identity) == manager) 
	{
		while (fscanf(User, "%s", buffer) == 1) 
		{
			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Phone);

			if ((strcmp(Identity, _Id)) == 0) 
			{
				printf("\nChange Options\n");

				while (loop)
				{
					printf("'1' Name     '2' ID     '3' Password     '4' Phone\n");
					selection = convertString();

					switch (selection)
					{
					case 1:
						while (true)
							if (verifyName(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", d.name, _Id, _Password, _Phone);
						loop = false;
						break;

					case 2:
						while (true)
							if (verifyId(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, d.ID, _Password, _Phone);
						loop = false;
						break;

					case 3:
						while (true)
							if (verifyPassword(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, d.password, _Phone);
						loop = false;
						break;

					case 4:
						while (true)
							if (verifyPhone(&d))
								break;

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, _Password, d.phone);
						loop = false;
						break;

					default:
						printf("Invalid Input, Try Between [1 To 4]\n\n");
						break;
					}
				}
			}

			else
				fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, _Password, _Phone);
		}

		fclose(Temp);
		fclose(User);

		User = fopen(FILE_MANAGERS, "w");
		if (!User) exit(1);

		Temp = fopen(FILE_TEMP, "r");
		if (!Temp) exit(1);

		while (fscanf(Temp, "%s", buffer) == 1) 
		{

			sscanf(buffer, "%[^,],%[^,],%[^,],%[^,]", _Name, _Id, _Password, _Phone);
			fprintf(User, "%s,%s,%s,%s\n", _Name, _Id, _Password, _Phone);
		}
		fclose(Temp);
		fclose(User);
	}
}
void appendOrderId() 
{
	FILE* file = fopen(FILE_GLOBAL, "r");
	if (!file) exit(1);

	int next = 0;

	fscanf(file, "%d", &next);
	fclose(file);

	file = fopen(FILE_GLOBAL, "w");
	fprintf(file, "%d", ++next);
	fclose(file);
}
int getNextOrderId() 
{
	FILE* file = fopen(FILE_GLOBAL, "r");
	if (!file) exit(1);

	int next = 0;

	fscanf(file, "%d", &next);
	fclose(file);
	return next;
}
char* getNextOrderIdStr() 
{
	FILE* file = fopen(FILE_GLOBAL, "r");
	if (!file) exit(1);

	char* str = NULL;
	str = (char*)malloc(sizeof(char) * 100);
	if (!str) exit(1);

	fscanf(file, "%s", str);
	fclose(file);
	return str;
}
bool doesProductExist(char* filename, char* _productName, char* _company) 
{
	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	Product tempProduct = { NULL, NULL, NULL, 0, 0 };
	char name[100] = { NULL }, company[100] = { NULL }, buffer[500] = { NULL };

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%*[^,],%*d,%*f", name, company);

		if (strcmp(_productName, name) == 0 && strcmp(_company, company) == 0) 
		{
			fclose(file);
			return true;
		}
	}

	fclose(file);
	return false;
}
Product selectProduct(Cart cart) 
{
	printf("\n%-15s%-15s%-15s%-15s%-15s%-15s\n", "ID", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
	
	Product product = { NULL, NULL, NULL, 0, 0 };
	int i = 1, res = 0;
	char buffer[500] = { NULL };

	for (i = 0; i < cart.itemsCount; i++) 
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart.products[i].name, cart.products[i].company, cart.products[i].category, cart.products[i].price, cart.products[i].quantity);

	printf("\nAvailable Action\n'0' Sort By Ascending Price\tElse Select Product\nInput --> ");
	scanf_s("%d", &res);

sort:
	if (res == 0) 
	{
		for (int p = 0; p < cart.itemsCount; p++) 
		{
			for (int j = p + 1; j < cart.itemsCount; j++) 
			{
				if (cart.products[p].price > cart.products[j].price) 
				{
					Product tempPrd = cart.products[p];
					cart.products[p] = cart.products[j];
					cart.products[j] = tempPrd;
				}
			}
		}

		printf("\n%-15s%-15s%-15s%-15s%-15s%-15s\n", "ID", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");
		
		for (int i = 0; i < cart.itemsCount; i++) 
			printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart.products[i].name, cart.products[i].company, cart.products[i].category, cart.products[i].price, cart.products[i].quantity);

		printf("Select Product --> ");
		scanf_s("%d", &res);
		
		while (!(res - 1 >= 0 && res - 1 < i)) 
		{
			printf("Invalid Selection, Please Try Again\nSelect Product --> ");
			scanf_s("%d", &res);
		}
		return cart.products[res - 1];
	}

	else 
	{
		while (!(res - 1 >= 0 && res - 1 < i)) 
		{
			printf("Invalid Selection, Please Try Again\nSelect Product --> ");
			scanf_s("%d", &res);
			if (res == 0) 
				goto sort;
		}
		return cart.products[res - 1];
	}
	return product;
}
Product readProduct(char* filename, char* _productName, char* _company) 
{
	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	Product tempProduct = { NULL, NULL, NULL, 0, 0 };
	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL }, buffer[500] = { NULL };
	int quantity = 0;
	float price = 0;

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f, %d", name, company, category, &price, &quantity);

		if (strcmp(_productName, name) == 0 && strcmp(_company, company) == 0) 
		{
			Product anotherTempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
			return anotherTempProduct;
		}
	}
	return tempProduct;
}
Cart retrieveProducts(bool returnAll, char* search, char* searchCategory) 
{
	FILE* file = fopen(FILE_CATALOG, "r");
	if (!file) exit(1);

	int count = 0, quantity = -1;
	char name[40] = { NULL }, company[40] = { NULL }, category[40] = { NULL }, buffer[500] = { NULL };

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%*f,%d", name, company, category, &quantity);

		if (search) 
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0) 
				if (quantity > 0 || returnAll)
					count++;

		else if (searchCategory)
			if (strcmp(category, searchCategory) == 0)
				if (quantity > 0 || returnAll)
					count++;

		else if (quantity > 0 || returnAll)
			count++;
	}

	float price = -1;
	int ite = 0;
	Cart cart = { 0, NULL };

	cart.products = malloc(sizeof(Product) * count);
	cart.itemsCount = count;

	fclose(file);

	file = fopen(FILE_CATALOG, "r");
	if (!file) exit(1);

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);
		
		if (search)
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0)
				if (quantity > 0 || returnAll) 
				{
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}

		else if (searchCategory)
			if (strcmp(category, searchCategory) == 0)
				if (quantity > 0 || returnAll) 
				{
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}
			
		else if (quantity > 0 || returnAll) 
		{
			Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
			cart.products[ite] = tempProduct;
			ite++;
		}
	}

	fclose(file);
	return cart;
}


int main()
{
  return 0;
}
