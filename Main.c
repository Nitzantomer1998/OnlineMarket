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


//ManagerMenu(Done)
void managerMenu() 
{
	printf("\nManager Actions\n");

	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("'1' Store     '2' Profile     '3' View Orders     '4' Store Profits     '5' Reports     '6' Exit\n");
		selection = convertString();

		switch (selection) 
		{
		case 1:
			managerStoreActions();
			break;

		case 2:
			profile();
			break;

		case 3:
			showOrders();
			break;

		case 4:
			printRevenue();
			break;

		case 5:
			seeTickets();
			break;

		case 6:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 6]\n\n");
			break;
		}
	}
}
void managerStoreActions() 
{
	printf("\nStore Actions\n");

	int selection = 0;
	bool loop = true;
	Product p;
	
	while (loop)
	{
		printf("'1' Add Product     '2' Delete Product     '3' Update Product     '4' Exit\n");
		selection = convertString();

		switch (selection)
		{
		case 1:
			addToCatalog();
			break;

		case 2:
			p = selectProduct(retrieveProducts(true, NULL, NULL));
			deleteFromCatalog(&p);
			break;

		case 3:
			p = selectProduct(retrieveProducts(true, NULL, NULL));
			updateCatalog(&p, -1);
			break;

		case 4:
			loop = false;
			break;

		default:
			printf("Invalid Input, Try Between [1 To 4]\n\n");
			break;
		}
	}
}
void addToCatalog() 
{
	char* rProductName = NULL;
	while (!rProductName) 
	{
		printf("\nName --> ");
		inputString(&rProductName);

		for (int i = 0; i < strlen(rProductName); i++)
		{
			if (!(rProductName[i] >= 'a' && rProductName[i] <= 'z' || rProductName[i] >= 'A' && rProductName[i] <= 'Z'))
			{
				rProductName = NULL;
				printf("Invalid Input, Try Again\n\n");
				break;
			}
		}
	}

	char* rProductCompany = NULL;
	while (!rProductCompany) 
	{
		printf("Company --> ");
		inputString(&rProductCompany);

		if (doesProductExist(FILE_CATALOG, rProductName, rProductCompany)) 
		{
			printf("Company Product Already In Stock\n");
			return;
		}

		for (int i = 0; i < strlen(rProductCompany); i++) 
		{
			if (!(rProductCompany[i] >= 'a' && rProductCompany[i] <= 'z' || rProductCompany[i] >= 'A' && rProductCompany[i] <= 'Z')) 
			{
				rProductCompany = NULL;
				printf("Invlaid Input, Try Again\n\n");
				break;
			}
		}
	}
	
	char* rProductCategory = NULL;
	while (!rProductCategory) 
	{
		printf("Category --> ");
		inputString(&rProductCategory);

		for (int i = 0; i < strlen(rProductCategory); i++) 
		{
			if (!(rProductCategory[i] >= 'a' && rProductCategory[i] <= 'z' || rProductCategory[i] >= 'A' && rProductCategory[i] <= 'Z')) 
			{
				rProductCategory = NULL;
				printf("Invalid Input, Try Again\n\n");
				break;
			}
		}
	}

	float rProductPrice = -1;
	while (rProductPrice < 0) 
	{
		printf("Price --> ");
		scanf_s("%f", &rProductPrice);

		if (rProductPrice < 0)
			printf("Invalid Input, Try Again\n\n");
	}

	int rProductQuantity = -1;
	while (rProductQuantity < 0)
	{
		printf("Quantity --> ");
		scanf_s("%d", &rProductQuantity);

		if (rProductQuantity < 0)
			printf("Invalid Input, Try Again\n\n");
	}

	char output[200] = { NULL };
	sprintf(output, "%s,%s,%s,%.2f,%d\n", rProductName, rProductCompany, rProductCategory, rProductPrice, rProductQuantity);
	writeFile(FILE_CATALOG, output);
}
void deleteFromCatalog(Product* p) 
{
	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* CataLog = fopen(FILE_CATALOG, "r");
	if (!CataLog) exit(1);

	char _Name[100] = { NULL }, _Company[100] = { NULL }, _Category[100] = { NULL }, _Price[100] = { NULL }, _Quantity[100] = { NULL }, buffer[500] = { NULL };
	while (fscanf(CataLog, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);

		if (!(strcmp(p->name, _Name) == 0 && strcmp(p->company, _Company) == 0))
			fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);

	CataLog = fopen(FILE_CATALOG, "w");
	if (!CataLog) exit(1);

	Temp = fopen(FILE_TEMP, "r");
	if (!Temp) exit(1);

	while (fscanf(Temp, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);
		fprintf(CataLog, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);
}
void updateCatalog(Product* p, int userQuantity) 
{
	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* CataLog = fopen(FILE_CATALOG, "r");
	if (!CataLog) exit(1);

	char _Name[100] = { NULL }, _Company[100] = { NULL }, _Category[100] = { NULL }, _Price[100] = { NULL };
	char buffer[500] = { NULL }, newPrice[100] = { NULL }, newQuantity[100] = { NULL }, _Quantity[100] = { NULL };
	

	int selection = 0;

	if (userQuantity == -1) 
	{
		printf("\nProduct Actions\n");
		
		while (!(selection > 0 && selection <= 2)) 
		{
			printf("'1' Update Price     '2' Update Quantity\n");
			selection = convertString();

			if (!(selection > 0 && selection <= 2))
				printf("Invalid Input, Try Again\n\n");
		}
	}

	else
		selection = 2;
	
	int updatedQuantity = -1;
	float updatedPrice = -1;

	while (fscanf(CataLog, "%s", buffer) == 1)
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);

		if (strcmp(p->name, _Name) == 0 && strcmp(p->company, _Company) == 0) 
		{
			if (selection == 1) 
			{
				while (updatedPrice < 0) 
				{
					printf("Updated Price --> ");
					scanf_s("%f", &updatedPrice);

					if (updatedPrice < 0)
						printf("Invalid input, Try again\n\n");
				}

				sprintf(newPrice, "%.2f", updatedPrice);
				fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, newPrice, _Quantity);
			}

			if (selection == 2) 
			{
				if (userQuantity == -1) 
				{
					while (updatedQuantity < 0) 
					{
						printf("Updated Quantity --> ");
						scanf_s("%d", &updatedQuantity);

						if (updatedQuantity < 0)
							printf("Invalid Input, Try Again\n\n");
					}
				}

				sprintf(newQuantity, "%d", userQuantity == -1 ? updatedQuantity : userQuantity);
				fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, newQuantity);
			}
		}

		else
			fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);

	CataLog = fopen(FILE_CATALOG, "w");
	if (!CataLog) exit(1);

	Temp = fopen(FILE_TEMP, "r");
	if (!Temp) exit(1);

	while (fscanf(Temp, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,]", _Name, _Company, _Category, _Price, _Quantity);
		fprintf(CataLog, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, _Price, _Quantity);
	}

	fclose(CataLog);
	fclose(Temp);
}
void seeTickets() 
{
	FILE* User = fopen(FILE_TICKET, "r");
	if (!User) exit(1);

	char buffer[500] = { NULL }, _Index[50] = { NULL }, _Report[400] = { NULL };
	while (fgets(buffer, 500, User))
		printf("%s", buffer);

	fclose(User);
}
void printRevenue() 
{
	int selection = -1;

	printf("\nRevenue Action\n");

	while (!(selection > 0 && selection <= 3)) 
	{
		printf("'1' Last 30 Days     '2' Last 7 Days     '3' Current Day\n");
		selection = convertString();

		if (!(selection > 0 && selection <= 3)) 
			printf("Invalid Input, Try Again\n\n");
	}
	
	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	Date tempDate = { 0,0,0 };
	char buffer[500] = { NULL };
	float revenue = 0;
	int ordersCount = 0;
	
	while (fscanf(file, "%s", buffer) == 1) 
	{	
		float price = 0;
		
		sscanf(buffer, "%*[^,],%*[^,],%f,%d/%d/%d,%*[^,]", &price, &tempDate.day, &tempDate.month, &tempDate.year);
		
		int dayDiff = calcDateDiff(tempDate);
		
		if (selection == 1) 
		{
			if (dayDiff <= 30) 
			{
				revenue += price;
				ordersCount++;
			}
		}
		
		else if (selection == 2) 
		{
			if (dayDiff <= 7) 
			{
				revenue += price;
				ordersCount++;
			}
		}
		
		else if (selection == 3) 
		{
			if (dayDiff == 0) 
			{
				revenue += price;
				ordersCount++;
			}
		}
	}

	fclose(file);
	printf("Asked Revenue --> %.2f In %d Orders\n", revenue, ordersCount);
}
void showOrders() 
{	
	printf("\nOrders Actions\n");

	int selection = -1;
	while (!(selection > 0 && selection <= 2)) 
	{
		printf("'1' Print All Orders     '2' Confirm/Unconfirmed Orders\n");
		selection = convertString();

		if (!(selection > 0 && selection <= 2))
			printf("Invalid Input, Try Again\n\n");
	}
	
	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	int count = 0, waitingCount = 0;
	char buffer[500] = { NULL }, status[30] = { NULL };
	
	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%*[^,],%*[^,],%*[^,],%*[^,],%[^,]", status);
		
		if (strcmp(status, "WAITING") == 0) 
			waitingCount++;
		count++;
	}
	
	fclose(file);
	
	float price = -1;
	int* orders = NULL, * waitingOrders = NULL, orderId = -1;
	char customerId[30] = { NULL }, date[40] = { NULL }, address[40] = { NULL };
	
	if (selection == 1) 
	{
		file = fopen(FILE_ORDERS, "r");
		if (!file) exit(1);

		orders = malloc(sizeof(int) * count);

		printf("\n%-15s%-15s%-15s%-15s%-15s\n", "Order ID", "Customer ID", "Total", "Date", "Status");
		
		int i = 0;
		while (fscanf(file, "%s", buffer) == 1) 
		{	
			sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
			printf("%-15d%-15s%-15.2f%-15s%-15s\n", orderId, customerId, price, date, status);
			orders[i] = orderId;
			i++;
		}
		
		bool flag = false;
		while (!flag) 
		{
			int ans = 0;
			printf("Select order (ID) -- > ");
			scanf_s("%d", &ans);

			for (int i = 0; i < count; i++) 
			{
				if (orders[i] == ans) 
				{
					printOrder(ans);
					free(orders);
					fclose(file);
					return;
				}
			}
			printf("Invalid Input, Try Again\n\n");
		}
	}

	else if (selection == 2) 
	{	
		file = fopen(FILE_ORDERS, "r");
		if (!file) exit(1);

		waitingOrders = malloc(sizeof(int) * waitingCount);

		printf("\n%-15s%-15s%-15s%-15s%-15s\n", "Order ID", "Customer ID", "Total", "Date", "Status");

		int i = 0;

		while (fscanf(file, "%s", buffer) == 1) 
		{
			sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
		
			if (strcmp(status, "WAITING") == 0) 
			{
				printf("%-15d%-15s%-15.2f%-15s%-15s\n", orderId, customerId, price, date, status);
				waitingOrders[i] = orderId;
				i++;
			}
		}
		bool flag = false;

		while (!flag) 
		{
			int ans = 0;

			printf("Select order (ID) -- > ");
			scanf_s("%d", &ans);
			
			for (int i = 0; i < waitingCount; i++) 
			{
				if (waitingOrders[i] == ans) 
				{
					printOrder(ans);
					changeOrderStatus(ans);
					free(waitingOrders);
					fclose(file);
					return;
				}
			}
			printf("Invalid Input, Try Again\n\n");
		}
	}
}
void printOrder(int orderId) 
{
	char source[300] = { NULL };
	sprintf(source, "%s%d.csv", FOLDER_DATA_ORDERS, orderId);

	if (!doesFileExists(source)) exit(1);

	FILE* file = fopen(source, "r");
	if (!file) exit(1);

	int count = 0;
	char str[20] = { NULL };
	fscanf(file, "%*d,%d,%s", &count, str);

	printf("\nOrder ID --> %d from %s\n", orderId, str);
	printf("%-14s%-15s%-15s%-15s%s", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");

	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL };
	int quantity = -1;
	float price = -1;

	for (int i = 0; i < count; i++) 
	{
		fscanf(file, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);
		printf("%-15s%-15s%-15s%-15.2f%-15d",
			name,
			company,
			category,
			price,
			quantity);
	}
	fscanf(file, "%f", &price);
	printf("\nTotal --> %.2f\n", price);
	fclose(file);
}
void changeOrderStatus(int id) 
{
	int count = 0;
	char* output = copyString("");

	if (!doesFileExists(FILE_ORDERS)) exit(1);

	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL }, temp[500] = { NULL }, customerId[30] = { NULL }, date[40] = { NULL }, status[30] = { NULL };
	int orderId = -1;
	float price = -1;
	
	while (fscanf(file, "%s", buffer) == 1) 
	{	
		sscanf(buffer, "%d,%[^,],%f,%[^,],%[^,]", &orderId, customerId, &price, date, status);
		sprintf(temp, "%d,%s,%f,%s,%s\n", orderId, customerId, price, date, id == orderId ? "CONFIRMED" : status);
		appendString(&output, temp);
	}

	fclose(file);
	
	file = fopen(FILE_ORDERS, "w");
	if (!file) exit(1);

	fputs(output, file);
	fclose(file);
	free(output);
	printf("Order Has Been Approved\n");
}
int calcDateDiff(Date d2) 
{
	Date d1 = getCurrentDate();
	int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int count1 = d1.year * 365 + d1.day;
	int count2 = d2.year * 365 + d2.day;
	
	for (int i = 0; i < d1.month; i++) 
		count1 += daysInMonth[i];
	
	for (int i = 0; i < d2.month; i++) 
		count2 += daysInMonth[i];
	
	return count1 - count2;
}
Date getCurrentDate() 
{
	Date date = { 0,0,0 };
	struct tm* tm;
	time_t t;
	char str_date[100] = { NULL };
	t = time(NULL);
	tm = localtime(&t);
	strftime(str_date, sizeof(str_date), "%d %m %Y", tm);
	sscanf(str_date, "%d %d %d", &date.day, &date.month, &date.year);
	return date;
}


//Manager + Customer (Status)
void registerUserType(UserType type) 
{
	printf("\nRegister Stage\n");

	Details d;

	while (!verifyName(&d))
		printf("Invalid, Try Again\n\n");

	while (!verifyPassword(&d))
		printf("Invalid, Try Again\n\n");

	while (!verifyId(&d))
		printf("Invalid, Try Again!\n\n");

	if (type == customer)
		if (!verifyAge())
			return;

	while (!verifyPhone(&d))
		printf("Invalid, Try Again\n\n");

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
		printf("'1' Register     '2' Login     '3' Exit\nInput --> ");
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
	printf("\nRegister As\n");

	int selection = 0, managerCode = 0;
	bool loop = true;

	while (loop)
	{
		printf("'1' Customer     '2' Manager     '3' Exit\nInput --> ");
		selection = convertString();

		switch (selection) 
		{
		case 1:
			registerUserType(customer);
			loop = false;
			break;

		case 2:
			printf("Manager Code\nInput --> ");
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
	printf("\nLogin Stage\n");

	char* userId = NULL, * userPassword = NULL;

	printf("ID --> ");
	inputString(&userId);

	Identity = userId;

	UserType type = findUserType();

	if (type == none) 
	{
		printf("ID Does Not Exist\n\n");
		return;
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
			printf("Manager\n");
			managerMenu();
		}
	}

	else
		printf("Invalid Password\n\n");
}
void profile() 
{
	printf("\nProfile Actions\n");

	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("'1' Print Profile     '2' Update Profile     '3' Exit\nInput --> ");
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
	printf("\nUser Information\n");

	Details* details = readUser(findUserType(Identity) == customer ? FILE_CUSTOMERS : FILE_MANAGERS, findUserType(Identity));
	char _Name[100] = { NULL }, _ID[100] = { NULL }, _Password[100] = { NULL }, _Phone[100] = { NULL };
	
	strcpy(_Name, details->name);
	strcpy(_ID, details->ID);
	strcpy(_Password, details->password);
	strcpy(_Phone, details->phone);

	printf("Name --> %s\n", _Name);
	printf("ID --> %s\n", _ID);
	printf("Password --> %s\n", _Password);
	printf("Phone --> %s\n", _Phone);

	if (findUserType(Identity) == customer)
	{
		float _Points = details->points;
		printf("Supermarket Points --> %.2f\n", _Points);
	}
}
void updateProfile() 
{
	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* User = fopen(findUserType(Identity) == customer ? FILE_CUSTOMERS : FILE_MANAGERS, "r");
	if (!User) exit(1);

	int selection = 0;
	bool loop = true;
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
					printf("'1' Name     '2' ID     '3' Password     '4' Phone\nInput --> ");
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
					printf("'1' Name     '2' ID     '3' Password     '4' Phone\nInput --> ");
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
bool doesProductExist(char* filename, char* _ProductName, char* _Company) 
{
	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	Product tempProduct = { NULL, NULL, NULL, 0, 0 };
	char name[100] = { NULL }, company[100] = { NULL }, buffer[500] = { NULL };

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%*[^,],%*d,%*f", name, company);

		if (strcmp(_ProductName, name) == 0 && strcmp(_Company, company) == 0) 
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
	
	Product product = { NULL, NULL, NULL, 0, 0.0 };
	int i = 1, res = 0;
	char buffer[500] = { NULL };

	for (i = 0; i < cart.itemsCount; i++) 
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart.products[i].name, cart.products[i].company, cart.products[i].category, cart.products[i].price, cart.products[i].quantity);

	printf("\nAvailable Action\n");
	while (!(res >= 0 && res <= cart.itemsCount + 1))
	{
		printf("'0' Sort By Ascending Price     Else Select Product\nInput --> ");
		res = convertString();

		if (!(res >= 0 && res <= cart.itemsCount + 1))
			printf("Invalid Input, Try Between [0 To %d]\n\n", cart.itemsCount + 1);
	}

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

		while (!(res - 1 >= 0 && res - 1 < i)) 
		{
			printf("Select Product --> ");
			res = convertString();

			if (!(res - 1 >= 0 && res - 1 < i))
				printf("Invalid Input, Try Between [1 To %d]\n\n", i - 1);
		}
		return cart.products[res - 1];
	}

	else 
		return cart.products[res - 1];

	return product;
}
Product readProduct(char* filename, char* _ProductName, char* _Company) 
{
	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	Product tempProduct = { NULL, NULL, NULL, 0, 0.0 };
	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL }, buffer[500] = { NULL };
	int quantity = 0;
	float price = 0.0;

	while (fscanf(file, "%s", buffer) == 1) 
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f, %d", name, company, category, &price, &quantity);

		if (strcmp(_ProductName, name) == 0 && strcmp(_Company, company) == 0) 
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
