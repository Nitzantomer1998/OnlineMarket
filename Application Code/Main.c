//Headers
#include "OnlineMarket.h"


// Strings
void inputString(char** _Str)
{
	char string[100] = { NULL };

	scanf_s(" %[^\n]", string, 100);
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
float stringToFloat()
{
	char string[500] = { NULL };

	scanf_s("%s", string, 500);

	return atof(string);
}
int stringToInt()
{
	char string[500] = { NULL };

	scanf_s("%s", string, 500);

	return atoi(string);
}


// Files
void checkFiles()
{
	//Check if the files exist, Else create them// 

	if (!doesFileExists(FILE_TEMP)) writeFile(FILE_TEMP, "");
	if (!doesFileExists(FILE_ORDERS)) writeFile(FILE_ORDERS, "");
	if (!doesFileExists(FILE_TICKETS)) writeFile(FILE_TICKETS, "");
	if (!doesFileExists(FILE_CATALOGS)) writeFile(FILE_CATALOGS, "");
	if (!doesFileExists(FILE_MANAGERS)) writeFile(FILE_MANAGERS, "");
	if (!doesFileExists(FILE_CUSTOMERS)) writeFile(FILE_CUSTOMERS, "");
}
void checkFolder()
{
	//Check if the folders exist, Else create them//

	if (!doesFileExists(FOLDER_DATA)) createFolder(FOLDER_DATA);
	if (!doesFileExists(FOLDER_DATA_USERS)) createFolder(FOLDER_DATA_USERS);
	if (!doesFileExists(FOLDER_DATA_ORDERS)) createFolder(FOLDER_DATA_ORDERS);
	if (!doesFileExists(FOLDER_DATA_TICKETS)) createFolder(FOLDER_DATA_TICKETS);
	if (!doesFileExists(FOLDER_DATA_CATALOGS)) createFolder(FOLDER_DATA_CATALOGS);
	if (!doesFileExists(FOLDER_DATA_ORDERS_ORDERSUMMARY)) createFolder(FOLDER_DATA_ORDERS_ORDERSUMMARY);
	if (!doesFileExists(FOLDER_DATA_TICKETS_TICKETSUMMARY)) createFolder(FOLDER_DATA_TICKETS_TICKETSUMMARY);
}
void createFolder(char* dirname)
{
	//Create a new folder according to the parameter data//

	int check = 0;
	check = mkdir(dirname);
	if (check) exit(1);
}
void writeFile(char* filename, char* content)
{
	//Writing the data from parameter contect into the desirable file//

	FILE* file = fopen(filename, "ab+");
	if (!file) exit(1);

	fprintf(file, "%s", content);
	fclose(file);
}
void writeUserType(Details* d, UserType type)
{
	//Write the data from the parameter (Details *d) into the right file//

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
	//Returning true if the file exist, Else false//

	struct stat buffer;
	return !stat(filename, &buffer);
}
Details* readUser(char* filename, UserType type)
{
	//Returning the current user information from the right file//

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
	//Returning the current user type//

	Details* details = readUser(FILE_CUSTOMERS, customer);
	if (details->ID) return customer;

	details = readUser(FILE_MANAGERS, manager);
	if (details->ID) return manager;

	return none;
}


// Authentication
void verifyName(Details* d)
{
	//Getting Name from the user and run until the Name is validate//

	char string[100] = { NULL };
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		printf("Name --> ");
		scanf_s(" %[^\n]", string, 100);

		for (int i = 0; i < strlen(string); i++)
		{
			if ((!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z'))& flag == true))
			{
				printf(ANSI_COLOR_RED   "Name May Contain Only English Alphabet\n\n\n"   ANSI_COLOR_RESET);
				d->name = NULL;
				flag = false;
			}
		}

		if (flag == true)
		{
			char* pString = NULL;
			pString = copyString(string);
			d->name = pString;
		}
	}
}
void verifyPassword(Details* d)
{
	//Getting Password from the user and run until the Password is validate//

	char* pString = NULL;
	char* reString = NULL;
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		printf("Password --> ");
		inputString(&pString);

		if (strlen(pString) < MIN_PASSWORD_LEN)
		{
			printf(ANSI_COLOR_RED   "Password Must Contain At Least Six Characters\n"   ANSI_COLOR_RESET);
			flag = false;
		}

		int letters = 0, numbers = 0;

		for (int i = 0; i < strlen(pString); i++)
		{
			if (pString[i] >= 'a' && pString[i] <= 'z')
				letters++;

			else if (pString[i] >= 'A' && pString[i] <= 'Z')
				letters++;

			else if (pString[i] >= '0' && pString[i] <= '9')
				numbers++;

			else
			{
				printf(ANSI_COLOR_RED   "Password Contain Only English Alphabet\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}

		if (!numbers)
			printf(ANSI_COLOR_RED   "Password Must Contain At Least One Number\n"   ANSI_COLOR_RESET);

		if (!letters)
			printf(ANSI_COLOR_RED   "Password Must Contain At Least One Letter\n"   ANSI_COLOR_RESET);

		if (flag == true)
		{
			printf("ReEnter Password --> ");
			inputString(&reString);
			if (strcmp(pString, reString) == 0)
				d->password = pString;
			
			else
			{
				printf(ANSI_COLOR_RED   "Password Doesnt Match\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}

		if (flag == false)
			printf("\n\n");
	}
}
void verifyId(Details* d)
{
	//Getting ID from the user and run until the ID is validate//

	char* pString = NULL;
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		printf("ID --> ");
		inputString(&pString);

		Identity = pString;

		long idVal = strtol(pString, NULL, 10);
		int Digit1 = 0, Digit2 = 0, Digit3 = 0, Digit4 = 0, Digit5 = 0, Digit6 = 0, Digit7 = 0, Digit8 = 0, Digit9 = 0, Sum = 0, Res = 0;

		if ((idVal >= 10000000) && (idVal <= 999999999))
		{
			Digit1 = idVal / 100000000 * 1;
			Digit2 = idVal / 10000000 % 10 * 2;
			Digit3 = idVal / 1000000 % 10 * 1;
			Digit4 = idVal / 100000 % 10 * 2;
			Digit5 = idVal / 10000 % 10 * 1;
			Digit6 = idVal / 1000 % 10 * 2;
			Digit7 = idVal / 100 % 10 * 1;
			Digit8 = idVal / 10 % 10 * 2;
			Digit9 = idVal % 10;

			if (Digit1 >= 10 || Digit2 >= 10 || Digit3 >= 10 || Digit4 >= 10 || Digit5 >= 10 || Digit6 >= 10 || Digit7 >= 10 || Digit8 >= 10 || Digit9 >= 10)
			{
				Digit1 = (Digit1 / 10) + (Digit1 % 10);
				Digit2 = (Digit2 / 10) + (Digit2 % 10);
				Digit3 = (Digit3 / 10) + (Digit3 % 10);
				Digit4 = (Digit4 / 10) + (Digit4 % 10);
				Digit5 = (Digit5 / 10) + (Digit5 % 10);
				Digit6 = (Digit6 / 10) + (Digit6 % 10);
				Digit7 = (Digit7 / 10) + (Digit7 % 10);
				Digit8 = (Digit8 / 10) + (Digit8 % 10);
				Digit9 = (Digit9 / 10) + (Digit9 % 10);
			}
			Sum = Digit1 + Digit2 + Digit3 + Digit4 + Digit5 + Digit6 + Digit7 + Digit8 + Digit9;

			if (Sum % 10 == 0)
			{
				if (findUserType(pString))
				{
					printf(ANSI_COLOR_RED	"ID Already Exist In The System\n\n"	ANSI_COLOR_RESET);

					int selection = 0;
					bool loop = true;

					while (loop)
					{
						printf("\n[Available Actions]\n'1' Wrong ID     '2' Report For Identity Steal\nInput --> ");
						selection = stringToInt();

						switch (selection)
						{
						case 1:
							printf("\n\nYou've Chose To Re Enter Other ID\n\n\n");
							loop = false;
							break;

						case 2:
							openTicket();
							printf("We Are Sry To Hear, Your Report Will Be Treated\nGoodbye\n");
							exit(1);

						default:
							printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 2]\n\n"   ANSI_COLOR_RESET);
							break;
						}
					}
					flag = false;
				}
				
				else
				{
					d->ID = pString;
					flag = true;
				}
			}

			else
			{
				printf(ANSI_COLOR_RED   "Incorrect ID\n\n\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}

		else
		{
			printf(ANSI_COLOR_RED   "ID Must Contain [8 To 9] Digits\n\n\n"   ANSI_COLOR_RESET);
			flag = false;
		}
	}
}
void verifyAge()
{
	//Getting Age from the user, If he is a minor the program will end, Else fine//

	int age = 0;

	printf("Age --> ");
	age = stringToInt();

	if (age < LEGAL_AGE)
	{
		printf(ANSI_COLOR_RED   "We Are Sry, The Minimum Age Is Sixteen\n\n"   ANSI_COLOR_RESET);
		exit(true);
	}

	if (age > 120)
	{
		printf(ANSI_COLOR_RED   "Sry If You Are Trully  %d Years Old You Probably Death. Good Day\n\n\n", age);
		printf(ANSI_COLOR_RESET);
		exit(true);
	}
}
void verifyPhone(Details* d)
{
	//Getting Phone from the user and run until the Phone is validate//

	char* pString = NULL;
	bool flag = false;

	while (flag == false)
	{
		flag = true;
		
		printf("Phone --> ");
		inputString(&pString);

		if (pString == NULL) 
			exit(true);

		if (strlen(pString) != PHONE_NUM_LEN)
		{
			printf(ANSI_COLOR_RED   "Phone Number Must Contain Ten Digits\n"   ANSI_COLOR_RESET);
			flag = false;
		}

		for (int i = 0; i < strlen(pString); i++)
		{
			if ((!(pString[i] >= '0' && pString[i] <= '9')) & flag == true)
			{
				printf(ANSI_COLOR_RED   "Phone Number May Contain Only Digits\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}

		if (flag == false)
			printf("\n\n");

		d->phone = pString;
	}
}
void termsAndConditions()
{
	//Checking if the user is agree to the Market policy terms, Incase he doesnt the program end, Else fine//
	system("cls");

	printf("[Terms and Conditions]\n");

	int selection = 0;
	while (!(selection >= 1 && selection <= 2))
	{
		printf("Do You Agree To The Terms And Conditions\n'1' Yes     '2' No\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 2))
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 2]\n\n"   ANSI_COLOR_RESET);
	}

	if (selection == 2)
	{
		printf(ANSI_COLOR_RED   "We Are Sry To Hear, Have A Good Day\n\n"   ANSI_COLOR_RESET);
		exit(true);
	}
}
void paymentProcess()
{
	printf("\n\n[Payment Process]\n");
	verifyCreditCard();
	verifyCVV();
	verifyDate();
}
void verifyCreditCard()
{
	//Getting Credit Card from the user and run until the Credit Card is validate//

	char* pCreditCard = NULL;
	bool flag = false;
	
	while (flag == false)
	{
		flag = true;

		printf("Credit Card Digits --> ");
		inputString(&pCreditCard);

		if (strlen(pCreditCard) != CREDIT_CARD)
		{
			printf(ANSI_COLOR_RED   "Credit Card Must Contain Sixteen Digits\n\n\n"   ANSI_COLOR_RESET);
			flag = false;
		}

		int numbers = 0;

		for (int i = 0; i < strlen(pCreditCard); i++)
		{
			if ((!(pCreditCard[i] >= '0' && pCreditCard[i] <= '9')) & flag == true)
			{
				printf(ANSI_COLOR_RED   "Credit Card Contain Only Digits\n\n\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}
	}
}
void verifyCVV()
{
	//Getting Credit Card CVV from the user and run until the CVV is validate//

	int CVV = 0;

	while (!(CVV >= 100 && CVV <= 999))
	{
		printf("CVV --> ");
		CVV = stringToInt();

		if (!(CVV >= 100 && CVV <= 999))
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [100 To 999]\n\n\n"   ANSI_COLOR_RESET);
	}
}
void verifyDate()
{
	//Getting Credit Card Date from the user and run until the Date is validate//

	int Month = 0;
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		while (!(Month >= 1 && Month <= 12))
		{
			printf("Expiration Month  --> ");
			Month = stringToInt();

			if (!(Month >= 1 && Month <= 12))
				printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 12]\n\n\n"   ANSI_COLOR_RESET);
		}

		int Year = 0;

		while (!(getCurrentDate().year <= Year && Year <= 2035))
		{
			printf("Expiration Year --> ");
			Year = stringToInt();

			if (!(getCurrentDate().year <= Year && Year <= 2035))
			{
				printf(ANSI_COLOR_RED   "Invalid Input, Try Between [%d To 2035]\n\n\n", getCurrentDate().year);
				printf(ANSI_COLOR_RESET);
			}
		}

		Date date = getCurrentDate();

		if (date.year == Year)
		{
			if (date.month >= Month)
			{
				printf(ANSI_COLOR_RED   "The Card Is Expired\n\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}
	}
}
void deliveryProcess()
{
	printf("\n\n[Delivery Process]\n");
	verifyCountry();
	verifyCity();
	verifyStreet();
	verifyHomeNumber();
}
void verifyCountry()
{
	//Getting Country from the user and run until the Country is validate//

	char string[100] = { NULL };
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		printf("Country --> ");
		scanf_s(" %[^\n]", string, 100);

		for (int i = 0; i < strlen(string); i++)
		{
			if ((!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z') || (string[i] == ' ')) & flag == true))
			{
				printf(ANSI_COLOR_RED   "Country May Contain Only English Alphabet\n\n\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}
	}
}
void verifyCity()
{
	//Getting City from the user and run until the City is validate//

	char string[100] = { NULL };
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		printf("City --> ");
		scanf_s(" %[^\n]", string, 100);

		for (int i = 0; i < strlen(string); i++)
		{
			if ((!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z') || (string[i] == ' ')) & flag == true))
			{
				printf(ANSI_COLOR_RED   "City May Contain Only English Alphabet\n\n\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}
	}
}
void verifyStreet()
{
	//Getting Street from the user and run until the Street is validate//

	char string[100] = { NULL };
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		printf("Street --> ");
		scanf_s(" %[^\n]", string, 100);

		for (int i = 0; i < strlen(string); i++)
		{
			if ((!((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z') || (string[i] == ' ')) & flag == true))
			{
				printf(ANSI_COLOR_RED   "Street May Contain Only English Alphabet\n\n\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}
	}
}
void verifyHomeNumber()
{
	//Getting Home Number from the user and run until the Home Number is validate//

	char* pString = NULL;
	char* reString = NULL;
	bool flag = false;

	while (flag == false)
	{
		flag = true;

		printf("Home Number --> ");
		inputString(&pString);


		for (int i = 0; i < strlen(pString); i++)
		{
			if (!(pString[i] >= '0' && pString[i] <= '9') & flag == true)
			{
				printf(ANSI_COLOR_RED   "Home Number Contain Only Numbers Alphabet\n\n\n"   ANSI_COLOR_RESET);
				flag = false;
			}
		}
	}
}


//CustomerMenu
void customerMenu()
{
	//The Customer Homepage//

	Cart c = { NULL , 0 };
	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("[Customer Menu]\n");
		printf("'1' Store     '2' Profile     '3' View Cart     '4' Finish Order     '5' Submit Ticket     '6' Log Out\nInput-- > ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			system("cls");
			customerShop(&c);
			break;

		case 2:
			system("cls");
			profile();
			break;

		case 3:
			system("cls");
			viewCart(&c);
			break;

		case 4:
			system("cls");
			finishOrder(&c, Identity);
			break;

		case 5:
			system("cls");
			openTicket();
			break;

		case 6:
			system("cls");
			for (int i = 0; i < c.itemsCount; i++)
				changeQuantity(&c, i, 0);

			printf(ANSI_COLOR_GREEN   "You've Successfully Logged Out\n\n\n"   ANSI_COLOR_RESET);
			loop = false;
			break;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 6]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}
}
void customerShop(Cart* cart)
{
	//Customer in shop section Homepage//

	int selection = 0;
	bool loop = true;
	Product product = { NULL, NULL, NULL, 0, 0.0 };
	char* search = NULL;
	char* category = NULL;

	while (loop)
	{
		printf("[Search Product By]\n'1' Name/Company     '2' Category     '3' Show All Products\nInput --> ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			printf("\n\n[Search By Name/Company]\nInput --> ");
			inputString(&search);
			product = selectProduct(retrieveProducts(false, search, NULL));
			if (product.quantity == 0)
				return;

			loop = false;
			break;

		case 2:
			printf("\n\n[Available Categories]");
			product = selectByCategory();
			loop = false;
			break;

		case 3:
			product = selectProduct(retrieveProducts(false, NULL, NULL));
			loop = false;
			break;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 3]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}

	int selectedQuantity = 0;
	while (!(selectedQuantity >= 1 && selectedQuantity <= product.quantity))
	{
		printf("\n\n[Product Quantity]\nInput --> ");
		selectedQuantity = stringToInt();

		if (!(selectedQuantity >= 1 && selectedQuantity <= product.quantity))
		{
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To %d]\n", product.quantity);
			printf(ANSI_COLOR_RESET);
		}
	}

	printf(ANSI_COLOR_GREEN   "Product Added Successfully\n\n\n"   ANSI_COLOR_RESET);
	Product addedProduct = product;
	addedProduct.quantity = selectedQuantity;
	addToCart(cart, addedProduct);
	updateCatalog(&product, product.quantity - selectedQuantity);
}
void printCart(Cart* cart)
{
	//Customer Cart view//

	printf("[Customer Current Cart View]\n");
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "No.", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");

	float total = 0;

	for (int i = 0; i < cart->itemsCount; i++)
	{
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart->products[i].name, cart->products[i].company, cart->products[i].category, cart->products[i].price, cart->products[i].quantity);
		total += cart->products[i].price * cart->products[i].quantity;
	}
	printf("Total --> %.2f\n\n", total);
}
void viewCart(Cart* cart)
{
	//Customer Cart possible actions//

	int selection = 0, optionA = 0, optionB = -1;
	bool loop = true;

	while (loop)
	{
		printCart(cart);
		printf("\n[Cart Actions]\n'1' Edit Cart     '2' Back To Menu\nInput --> ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			optionA = 0;
			while (!(optionA >= 1 && optionA <= cart->itemsCount))
			{
				printf("\n\n[Select Product]\nInput --> ");
				optionA = stringToInt();

				if (!(optionA >= 1 && optionA <= cart->itemsCount))
				{
					printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To %d]\n", cart->itemsCount);
					printf(ANSI_COLOR_RESET);
				}
			}

			optionB = 0;
			while (!(optionB >= 1 && optionB <= 3))
			{
				printf("\n\n[Product Actions]\n'1' Change Quantity     '2' Remove Product     '3' Return\nInput --> ");
				optionB = stringToInt();

				if (!(optionB >= 1 && optionB <= 3))
					printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 3]\n"   ANSI_COLOR_RESET);
			}

			if (optionB == 1)
			{
				optionB = 0;
				while (optionB <= 0)
				{
					printf("\n\n[New Quantity]\nInput --> ");
					optionB = stringToInt();

					if (optionB <= 0)
						printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 Or Greater]\n"   ANSI_COLOR_RESET);
				}

				changeQuantity(cart, optionA - 1, optionB);
			}

			else if (optionB == 2)
			{
				printf(ANSI_COLOR_GREEN   "Product Removed Successfully\n"   ANSI_COLOR_RESET);
				removeFromCart(cart, optionA - 1);
			}

			else if (optionB == 3)
				system("cls");

			loop = false;
			break;

		case 2:
			system("cls");
			loop = false;
			return;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 2]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}
}
void addToCart(Cart* cart, Product product)
{
	//Customer adding item to the cart//

	if (cart->itemsCount == 0)
	{
		cart->products = malloc(sizeof(Product));
		cart->products[0] = product;
	}

	else
	{
		Product* newProducts = malloc(sizeof(Product) * (cart->itemsCount + 1));
		for (int i = 0; i < cart->itemsCount; i++)
			newProducts[i] = cart->products[i];

		newProducts[cart->itemsCount] = product;
		cart->products = newProducts;
	}
	cart->itemsCount++;
}
void removeFromCart(Cart* cart, int index)
{
	//Customer remove item from his cart//

	Product* newProduct = malloc(sizeof(Product) * (cart->itemsCount - 1));
	Product p = readProduct(FILE_CATALOGS, cart->products[index].name, cart->products[index].company);

	updateCatalog(&p, p.quantity + cart->products[index].quantity);

	for (int i = 0; i < index; i++)
	{
		Product tempProduct = { copyString(cart->products[i].name), copyString(cart->products[i].company), copyString(cart->products[i].category), cart->products[i].price, cart->products[i].quantity };
		newProduct[i] = tempProduct;
	}

	for (int i = index + 1; i < index; i++)
	{
		Product tempProduct = { copyString(cart->products[i].name), copyString(cart->products[i].company), copyString(cart->products[i].category), cart->products[i].price, cart->products[i].quantity };
		newProduct[i - 1] = tempProduct;
	}
	cart->itemsCount--;
}
void changeQuantity(Cart* cart, int index, int newQuantity)
{
	//Customer choose product from his cart and change the item quantity//

	bool flag = false;
	Product p = readProduct(FILE_CATALOGS, cart->products[index].name, cart->products[index].company);
	int availableQuantity = p.quantity;

	if (newQuantity > cart->products[index].quantity)
	{
		if (newQuantity - cart->products[index].quantity > availableQuantity)
		{
			printf(ANSI_COLOR_RED   "Invalid, The Entered Quantity Exceeds The Available Quantity\n"   ANSI_COLOR_RESET);
			printf("Available Stock --> %d\n", availableQuantity + cart->products[index].quantity);
		}

		else
		{
			updateCatalog(&p, availableQuantity - (newQuantity - cart->products[index].quantity));
			cart->products[index].quantity = newQuantity;
			printf(ANSI_COLOR_GREEN   "Product Quantity Successfully Changed\n\n\n"   ANSI_COLOR_RESET);
		}
	}

	else
	{
		updateCatalog(&p, availableQuantity + (cart->products[index].quantity - newQuantity));
		cart->products[index].quantity = newQuantity;
		if (newQuantity != 0)
			printf(ANSI_COLOR_GREEN   "Product Quantity Successfully Changed\n\n\n"   ANSI_COLOR_RESET);
	}
}
void finishOrder(Cart* cart)
{
	//The customer finish the shopping and end his purchase//

	printCart(cart);

	int selection = 0;

	while (!(selection >= 1 && selection <= 2))
	{
		printf("\nAre You Sure You Would Like To Finish The Order?\n'1' Yes     '2' No\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 2))
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 2]\n\n"   ANSI_COLOR_RESET);
	}

	if (selection == 2)
		return;

	deliveryProcess();
	paymentProcess();
	writeOrder(cart);
	cart->itemsCount = 0;
	free(cart->products);
}
void writeOrder(Cart* cart)
{
	//The customer order is saving in the data of the market//

	char source[300] = { NULL }, orderNum[100] = { NULL };
	strcpy(orderNum, getNextOrderIdStr());

	sprintf(source, "%s%s.csv", FOLDER_DATA_ORDERS_ORDERSUMMARY, orderNum);

	FILE* file = fopen(source, "w");
	if (!file) exit(1);

	Date date = getCurrentDate();
	fprintf(file, "%s,%d,%d/%d/%d\n", orderNum, cart->itemsCount, date.day, date.month, date.year);

	float total = 0;

	for (int i = 0; i < cart->itemsCount; i++)
	{
		fprintf(file, "%s,%s,%s,%f,%d\n", cart->products[i].name, cart->products[i].company, cart->products[i].category, cart->products[i].price, cart->products[i].quantity);
		total += cart->products[i].price * cart->products[i].quantity;
	}

	fprintf(file, "%.2f\n", total);
	fclose(file);

	file = fopen(FILE_ORDERS, "ab+");
	if (!file) exit(1);

	fprintf(file, "%s,%s,%.2f,%d/%d/%d,WAITING\n", orderNum, Identity, total, date.day, date.month, date.year);
	fclose(file);

	Details* user = readUser(FILE_CUSTOMERS, customer);
	float _Points = user->points;
	int selection = 0;
	if (_Points > 0)
	{
		while (!(selection > 0 && selection <= 2))
		{
			printf("\n\n[Available Market Points --> %.2f]\nWould You Like To Reedem Them?\n'1' Yes     '2' No\nInput --> ", _Points);
			selection = stringToInt();

			if (!(selection > 0 && selection <= 2))
				printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 2]\n"   ANSI_COLOR_RESET);

			if (selection == 1)
			{
				updatePoints(_Points >= total ? _Points - total : 0);
				_Points = _Points >= total ? _Points - total : 0;
			}
		}
	}

	printf(ANSI_COLOR_GREEN   "In This Purchase You've Earned %.2f Market Points\n", total * 0.03);
	printf(ANSI_COLOR_RESET);
	updatePoints(_Points + total * 0.03);
	printf(ANSI_COLOR_GREEN   "Your Purchase Was Successful\n\n\n"   ANSI_COLOR_RESET);
}
void updatePoints(float newPoints)
{
	//The customer marketPoints updating according to his purchase//

	FILE* file = fopen(FILE_CUSTOMERS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL }, temp[500] = { NULL }, name[30] = { NULL }, customerId[40] = { NULL }, pass[30] = { NULL }, phone[30] = { NULL }, * output = copyString("");
	float points = -1;

	while (fscanf(file, "%s", buffer) == 1)
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f,%[^,]", name, customerId, pass, &points, phone);
		sprintf(temp, "%s,%s,%s,%f,%s\n", name, customerId, pass, strcmp(customerId, Identity) == 0 ? newPoints : points, phone);
		appendString(&output, temp);
	}

	fclose(file);
	file = fopen(FILE_CUSTOMERS, "w");
	if (!file) exit(1);

	fputs(output, file);
	fclose(file);
	free(output);
}
void openTicket()
{
	//Customer Submit a feedback (Good/Bad)//

	char source[300] = { NULL }, ticketNum[100] = { NULL }, _Report[500] = { NULL };
	strcpy(ticketNum, getNextTicketIdStr());

	sprintf(source, "%s%s.csv", FOLDER_DATA_TICKETS_TICKETSUMMARY, ticketNum);

	FILE* file = fopen(source, "w");
	if (!file) exit(1);

	Date date = getCurrentDate();
	fprintf(file, "%s,%d/%d/%d\n", ticketNum, date.day, date.month, date.year);

	printf("[Feedback System]\nInput Report --> ");
	scanf_s(" %[^\n]", _Report, 500);

	fprintf(file, "%s\n", _Report);
	fclose(file);

	file = fopen(FILE_TICKETS, "ab+");
	if (!file) exit(1);

	fprintf(file, "%s,%s,%d/%d/%d,WAITING\n", ticketNum, Identity, date.day, date.month, date.year);
	fclose(file);

	printf(ANSI_COLOR_GREEN   "Ticket Been Sent, Have A Good Day\n\n\n"   ANSI_COLOR_RESET);
}
Product selectByCategory()
{
	//The customer pick a product according the category he would like//

	Cart tempCart = retrieveProducts(false, NULL, NULL);
	int count = 0;
	char** categories = malloc(sizeof(char*) * tempCart.itemsCount);
	printf("\nNo.     CATEGORY\n");
	for (int i = 0; i < tempCart.itemsCount; i++)
	{
		bool flag = false;
		for (int j = 0; j < count; j++)
		{
			if (strcmp(tempCart.products[i].category, categories[j]) == 0)
			{
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			categories[count] = malloc(sizeof(char) * strlen(tempCart.products[i].category) + sizeof(char));
			strcpy(categories[count], tempCart.products[i].category);
			printf("%d       %s\n", count + 1, categories[count]);
			count++;
		}
	}

	int selection = 0;
	while (!(selection >= 1 && selection <= count))
	{
		printf("Input No. --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= count))
		{
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To %d]\n\n\n", count);
			printf(ANSI_COLOR_RESET);
		}
	}

	tempCart = retrieveProducts(false, NULL, categories[selection - 1]);
	return selectProduct(tempCart);
}


//ManagerMenu
void managerMenu()
{
	//The Manager Homepage//

	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("[Manager Menu]\n");
		printf("'1' Store     '2' Profile     '3' View Orders     '4' Store Profits     '5' View Tickets     '6' Log Out\nInput-- > ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			system("cls");
			managerStoreActions();
			break;

		case 2:
			system("cls");
			profile();
			break;

		case 3:
			system("cls");
			showOrders();
			break;

		case 4:
			system("cls");
			printRevenue();
			break;

		case 5:
			system("cls");
			seeTickets();
			break;

		case 6:
			system("cls");
			printf(ANSI_COLOR_GREEN   "You've Successfully Logged Out\n\n\n"   ANSI_COLOR_RESET);
			loop = false;
			break;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 6]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}
}
void managerStoreActions()
{
	//Manager in shop section Homepage//

	int selection = 0;
	bool loop = true;
	Product p = { NULL, NULL, NULL, 0, 0.0 };

	while (loop)
	{
		printf("[Manager Store Actions]\n");
		printf("'1' Add Product     '2' Delete Product     '3' Update Product     '4' Return\nInput-- > ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			system("cls");
			addToCatalog();
			break;

		case 2:
			system("cls");
			printf("[Manager Delete Product]\n");
			p = selectProduct(retrieveProducts(true, NULL, NULL));
			deleteFromCatalog(&p);
			break;

		case 3:
			system("cls");
			printf("[Manager Update Catalog]\n");
			p = selectProduct(retrieveProducts(true, NULL, NULL));
			updateCatalog(&p, -1);
			break;

		case 4:
			system("cls");
			loop = false;
			break;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 4]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}
}
void addToCatalog()
{
	//The manager adding item to the catalog//

	printf("[Manager Add Product]\n");

	char* rProductName = NULL;
	while (!rProductName)
	{
		printf("Name --> ");
		inputString(&rProductName);

		for (int i = 0; i < strlen(rProductName); i++)
		{
			if (!(rProductName[i] >= 'a' && rProductName[i] <= 'z' || rProductName[i] >= 'A' && rProductName[i] <= 'Z'))
			{
				rProductName = NULL;
				printf(ANSI_COLOR_RED   "Name Contain Only English Alphabet\n\n\n"   ANSI_COLOR_RESET);
				break;
			}
		}
	}

	char* rProductCompany = NULL;
	while (!rProductCompany)
	{
		printf("Company --> ");
		inputString(&rProductCompany);

		if (doesProductExist(FILE_CATALOGS, rProductName, rProductCompany))
		{
			printf(ANSI_COLOR_RED   "Company Product Already In Stock\n"   ANSI_COLOR_RESET);
			return;
		}

		for (int i = 0; i < strlen(rProductCompany); i++)
		{
			if (!(rProductCompany[i] >= 'a' && rProductCompany[i] <= 'z' || rProductCompany[i] >= 'A' && rProductCompany[i] <= 'Z'))
			{
				rProductCompany = NULL;
				printf(ANSI_COLOR_RED   "Company Contain Only English Alphabet\n\n\n"   ANSI_COLOR_RESET);
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
				printf(ANSI_COLOR_RED   "Category Contain Only English Alphabet\n\n\n"   ANSI_COLOR_RESET);
				break;
			}
		}
	}

	float rProductPrice = 0;
	while (rProductPrice <= 0)
	{
		printf("Price --> ");
		rProductPrice = stringToFloat();

		if (rProductPrice <= 0)
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 Or Greater]\n\n\n"   ANSI_COLOR_RESET);
	}

	int rProductQuantity = 0;
	while (rProductQuantity <= 0)
	{
		printf("Quantity --> ");
		rProductQuantity = stringToInt();

		if (rProductQuantity <= 0)
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 Or Greater]\n\n\n"   ANSI_COLOR_RESET);
	}

	system("cls");
	printf("[Added Product]\nName --> %s\nCompany --> %s\nCategory --> %s\nPrice --> %.2f\nQuantity --> %d\n", rProductName, rProductCompany, rProductCategory, rProductPrice, rProductQuantity);
	printf(ANSI_COLOR_GREEN   "You've Successfully Added The Product\n\n\n"   ANSI_COLOR_RESET);
	
	char output[200] = { NULL };
	sprintf(output, "%s,%s,%s,%.2f,%d\n", rProductName, rProductCompany, rProductCategory, rProductPrice, rProductQuantity);
	writeFile(FILE_CATALOGS, output);
}
void deleteFromCatalog(Product* p)
{
	//The manager delete item from the catalog//

	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* CataLog = fopen(FILE_CATALOGS, "r");
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

	CataLog = fopen(FILE_CATALOGS, "w");
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

	system("cls");
	printf("[Deleted Product]\nName --> %s\nCompany --> %s\nCategory --> %s\nPrice --> %.2f\nQuantity --> %d\n", p->name, p->company, p->category, p->price, p->quantity);
	printf(ANSI_COLOR_GREEN   "You've Successfully Deleted The Product\n\n\n"   ANSI_COLOR_RESET);
}
void updateCatalog(Product* p, int userQuantity)
{
	//The manager updating an item from the catalog (price/quantity)//

	FILE* Temp = fopen(FILE_TEMP, "w");
	if (!Temp) exit(1);

	FILE* CataLog = fopen(FILE_CATALOGS, "r");
	if (!CataLog) exit(1);

	char _Name[100] = { NULL }, _Company[100] = { NULL }, _Category[100] = { NULL }, _Price[100] = { NULL };
	char buffer[500] = { NULL }, newPrice[100] = { NULL }, newQuantity[100] = { NULL }, _Quantity[100] = { NULL };
	int selection = 0;

	if (userQuantity == -1)
	{
		while (!(selection >= 1 && selection <= 2))
		{
			printf("\n\n[Product Actions]\n");
			printf("'1' Update Price     '2' Update Quantity\nInput --> ");
			selection = stringToInt();

			if (!(selection >= 1 && selection <= 2))
				printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 2]\n"   ANSI_COLOR_RESET);
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
				while (updatedPrice <= 0)
				{
					printf("\n\n[Update Price]\nInput --> ");
					updatedPrice = stringToFloat();

					if (updatedPrice <= 0)
						printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 Or Greater]\n"   ANSI_COLOR_RESET);

					else
					{
						system("cls");
						printf("[Updated Product]\nName --> %s\nCompany --> %s\nCategory --> %s\nPrice --> %.2f\nQuantity --> %d\n", p->name, p->company, p->category, updatedPrice, p->quantity);
						printf(ANSI_COLOR_GREEN   "You've Successfully Update The Product\n\n\n"   ANSI_COLOR_RESET);
					}
				}

				sprintf(newPrice, "%.2f", updatedPrice);
				fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Company, _Category, newPrice, _Quantity);
			}

			if (selection == 2)
			{
				if (userQuantity == -1)
				{
					while (updatedQuantity <= 0)
					{
						printf("\n\nUpdate Quantity\nInput --> ");
						updatedQuantity = stringToInt();

						if (updatedQuantity <= 0)
							printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 Or Greater]\n"   ANSI_COLOR_RESET);

						else
						{
							system("cls");
							printf("[Updated Product]\nName --> %s\nCompany --> %s\nCategory --> %s\nPrice --> %.2f\nQuantity --> %d\n", p->name, p->company, p->category, p->price, updatedQuantity);
							printf(ANSI_COLOR_GREEN   "You've Successfully Update The Product\n\n\n"   ANSI_COLOR_RESET);
						}
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

	CataLog = fopen(FILE_CATALOGS, "w");
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
	//The manager Homepage to seeing all the tickets who's been submited//

	int selection = 0;
	while (!(selection >= 1 && selection <= 3))
	{
		printf("[Ticket Actions]\n'1' Print All Tickets     '2' Confirm/Unconfirmed Tickets     '3' Return\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 3))
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 3]\n\n\n"   ANSI_COLOR_RESET);
	}

	FILE* file = fopen(FILE_TICKETS, "r");
	if (!file) exit(1);

	int count = 0, waitingCount = 0;
	char buffer[500] = { NULL }, status[30] = { NULL };

	while (fscanf(file, "%s", buffer) == 1)
	{
		sscanf(buffer, "%*[^,],%*[^,],%*[^,],%[^,]", status);

		if (strcmp(status, "WAITING") == 0)
			waitingCount++;
		count++;
	}

	fclose(file);

	int* tickets = NULL, * waitingTickets = NULL, ticketId = 0;
	char customerId[30] = { NULL }, date[40] = { NULL };

	if (selection == 1)
	{
		file = fopen(FILE_TICKETS, "r");
		if (!file) exit(1);

		tickets = malloc(sizeof(int) * count);
		
		printf("\n\n[All Tickets View]\n%-15s%-15s%-15s%-15s\n", "Ticket No.", "Customer ID", "Date", "Status");

		int i = 0;
		while (fscanf(file, "%s", buffer) == 1)
		{
			sscanf(buffer, "%d,%[^,],%[^,],%[^,]", &ticketId, customerId, date, status);
			printf("%-15d%-15s%-15s%-15s\n", ticketId, customerId, date, status);
			tickets[i] = ticketId;
			i++;
		}

		bool flag = false;
		while (!flag)
		{
			printf("Select Ticket (Ticket No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < count; i++)
			{
				if (tickets[i] == selection)
				{
					printTicket(selection);
					printf("\n\n");
					free(tickets);
					fclose(file);
					return;
				}
			}
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [0 To %d]\n\n", i - 1);
			printf(ANSI_COLOR_RESET);
		}
	}

	else if (selection == 2)
	{
		file = fopen(FILE_TICKETS, "r");
		if (!file) exit(1);

		waitingTickets = malloc(sizeof(int) * waitingCount);

		printf("\n[Confirm / Unconfirmed Tickets Stage]\n%-15s%-15s%-15s%-15s\n", "Ticket No.", "Customer ID", "Date", "Status");

		int i = 0;

		while (fscanf(file, "%s", buffer) == 1)
		{
			sscanf(buffer, "%d,%[^,],%[^,],%[^,]", &ticketId, customerId, date, status);

			if (strcmp(status, "WAITING") == 0)
			{
				printf("%-15d%-15s%-15s%-15s\n", ticketId, customerId, date, status);
				waitingTickets[i] = ticketId;
				i++;
			}
		}
		bool flag = false;

		while (!flag)
		{
			printf("Select Ticket (Ticket No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < waitingCount; i++)
			{
				if (waitingTickets[i] == selection)
				{
					printTicket(selection);
					changeTicketStatus(selection);
					free(waitingTickets);
					fclose(file);
					return;
				}
			}
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [The Ticket No. Column]\n\n"   ANSI_COLOR_RESET);
		}
	}

	else if (selection == 3)
		return;
}
void printTicket(int ticketId)
{
	//The desireable ticket will be printed/viewed//

	char source[300] = { NULL };
	sprintf(source, "%s%d.csv", FOLDER_DATA_TICKETS_TICKETSUMMARY, ticketId);

	FILE* file = fopen(source, "r");
	if (!file) exit(1);

	char str[100] = { NULL };
	fscanf(file, "%*d,%s", str);

	printf("\n\n[Ticket No. --> %d From %s]", ticketId, str);

	char buffer[500] = { NULL }, _Index[50] = { NULL }, _Report[400] = { NULL };
	while (fgets(buffer, 500, file))
		printf("%s", buffer);

	fclose(file);
}
void changeTicketStatus(int id)
{
	//The manager choose which ticket to confirm//

	int count = 0;
	char* output = copyString("");

	FILE* file = fopen(FILE_TICKETS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL }, temp[500] = { NULL }, customerId[30] = { NULL }, date[40] = { NULL }, status[30] = { NULL };
	int ticketId = 0;

	while (fscanf(file, "%s", buffer) == 1)
	{
		sscanf(buffer, "%d,%[^,],%[^,],%[^,]", &ticketId, customerId, date, status);
		sprintf(temp, "%d,%s,%s,%s\n", ticketId, customerId, date, id == ticketId ? "CONFIRMED" : status);
		appendString(&output, temp);
	}

	fclose(file);

	file = fopen(FILE_TICKETS, "w");
	if (!file) exit(1);

	fputs(output, file);
	fclose(file);
	free(output);
	printf(ANSI_COLOR_GREEN   "Ticket Has Been Approved\n\n\n" ANSI_COLOR_RESET);
}
void printRevenue()
{
	//The manager Revenue system, The manager choose from this menu to see his revenue in that period of time//

	int selection = 0;

	while (!(selection >= 1 && selection <= 4))
	{
		printf("[Revenue Actions]\n'1' Last 30 Days     '2' Last 7 Days     '3' Current Day     '4' Return\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 4))
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 4]\n\n\n"   ANSI_COLOR_RESET);
	}

	if (selection == 4)
	{
		system("cls");
		return;
	}

	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	Date tempDate = { 0,0,0 };
	char buffer[500] = { NULL };
	float revenue = 0.0, price = 0.0;
	int ordersCount = 0;

	while (fscanf(file, "%s", buffer) == 1)
	{
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
	printf(ANSI_COLOR_GREEN   "[Asked Revenue --> %.2f In %d Orders]\n\n\n", revenue, ordersCount);
	printf(ANSI_COLOR_RESET);
}
void showOrders()
{
	//The manager Homepage to seeing all the orders who's been purchase//

	int selection = 0;
	while (!(selection >= 1 && selection <= 3))
	{
		printf("[Order Actions]\n'1' Print All Orders     '2' Confirm/Unconfirmed Orders     '3' Return\nInput --> ");
		selection = stringToInt();

		if (!(selection >= 1 && selection <= 3))
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 3]\n\n\n"   ANSI_COLOR_RESET);
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

	float price = 0.0;
	int* orders = NULL, * waitingOrders = NULL, orderId = 0;
	char customerId[30] = { NULL }, date[40] = { NULL };

	if (selection == 1)
	{
		file = fopen(FILE_ORDERS, "r");
		if (!file) exit(1);

		orders = malloc(sizeof(int) * count);

		printf("\n\n[All Orders View]\n%-15s%-15s%-15s%-15s%-15s\n", "Order No. ", "Customer ID", "Total", "Date", "Status");

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
			printf("Select Order (Order No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < count; i++)
			{
				if (orders[i] == selection)
				{
					printOrder(selection);
					printf("\n\n");
					free(orders);
					fclose(file);
					return;
				}
			}
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [0 To %d]\n\n\n", i - 1);
			printf(ANSI_COLOR_RESET);
		}
	}

	else if (selection == 2)
	{
		file = fopen(FILE_ORDERS, "r");
		if (!file) exit(1);

		waitingOrders = malloc(sizeof(int) * waitingCount);
		
		printf("\n\n[Confirm/Unconfirmed Orders Stage]\n%-15s%-15s%-15s%-15s%-15s\n", "Order No. ", "Customer ID", "Total", "Date", "Status");

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
			int selection = 0;

			printf("Select Order (Order No. ) --> ");
			selection = stringToInt();

			for (int i = 0; i < waitingCount; i++)
			{
				if (waitingOrders[i] == selection)
				{
					printOrder(selection);
					changeOrderStatus(selection);
					free(waitingOrders);
					fclose(file);
					return;
				}
			}
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [The Order No. Column]\n\n\n"   ANSI_COLOR_RESET);
		}
	}

	else if (selection == 3)
	{
		system("cls");
		return;
	}
}
void printOrder(int orderId)
{
	//The desireable order will be printed/viewed//

	char source[300] = { NULL };
	sprintf(source, "%s%d.csv", FOLDER_DATA_ORDERS_ORDERSUMMARY, orderId);

	FILE* file = fopen(source, "r");
	if (!file) exit(1);

	int count = 0;
	char str[100] = { NULL };
	fscanf(file, "%*d,%d,%s", &count, str);

	printf("\n\n[Order No. --> %d From %s]\n", orderId, str);
	printf("%-14s%-15s%-15s%-15s%s", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");

	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL };
	int quantity = -1;
	float price = -1;

	for (int i = 0; i < count; i++)
	{
		fscanf(file, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);
		printf("%-15s%-15s%-15s%-15.2f%-15d", name, company, category, price, quantity);
	}

	fscanf(file, "%f", &price);
	printf("\nTotal --> %.2f\n", price);
	fclose(file);
}
void changeOrderStatus(int id)
{
	//The manager choose which order to confirm//

	int count = 0;
	char* output = copyString("");

	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL }, temp[500] = { NULL }, customerId[30] = { NULL }, date[40] = { NULL }, status[30] = { NULL };
	int orderId = 0;
	float price = 0;

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
	printf(ANSI_COLOR_GREEN   "Order Has Been Approved\n\n\n"   ANSI_COLOR_RESET);
}
int calcDateDiff(Date d2)
{
	//Return the amount of days between the current date and the date from the function parameter//

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
	//Getting the current day of today//

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


//General
void welcomeScreen()
{
	//Main Homepage//

	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("[OnlineMarket HomePage]\n");
		printf("'1' Register	'2' Login	'3' Exit\nInput --> ");

		selection = stringToInt();

		switch (selection)
		{
		case 1:
			system("cls");
			registerStage();
			break;

		case 2:
			system("cls");
			loginUser();
			break;

		case 3:
			system("cls");
			printf(ANSI_COLOR_GREEN   "GoodBye\n\n"  ANSI_COLOR_RESET);
			loop = false;
			break;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 3]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}
}
void registerStage()
{
	//Register HomePage//

	int selection = 0, managerCode = 0;
	bool loop = true;

	while (loop)
	{
		printf("[Register As]\n");
		printf("'1' Customer	'2' Manager	'3' Return\nInput --> ");
		selection = stringToInt();

		switch (selection)
		{
		case 1:
			system("cls");
			registerUserType(customer);
			loop = false;
			break;

		case 2:
			printf("Input Manager Code --> ");
			managerCode = stringToInt();

			if (MANAGER_CODE == managerCode)
			{
				system("cls");
				registerUserType(manager);
			}

			else
				printf(ANSI_COLOR_RED   "Wrong Code, GoodBye\n\n\n"   ANSI_COLOR_RESET);

			loop = false;
			break;

		case 3:
			loop = false;
			printf("\n");
			break;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 3]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}
}
void registerUserType(UserType type)
{
	//Register to the system/App//
	
	printf("[Register Stage]\n");

	Details d;

	verifyName(&d);
	verifyPassword(&d);
	verifyId(&d);

	if (type == customer)
		verifyAge();
			
	verifyPhone(&d);
	termsAndConditions();

	system("cls");
	printf("[User Information]\nName --> %s\nPassword --> %s\nID --> %s\nPhone --> %s\n", d.name, d.password, d.ID, d.phone);
	printf(ANSI_COLOR_GREEN   "You've Successfully Registered\n\n\n"   ANSI_COLOR_RESET);
	writeUserType(&d, type);
}
void loginUser()
{
	//Login Homepage//

	printf("[Login Stage]\n");

	char* userId = NULL, * userPassword = NULL;

	printf("ID --> ");
	inputString(&userId);

	Identity = userId;

	UserType type = findUserType();

	if (type == none)
	{
		printf(ANSI_COLOR_RED   "ID Does Not Exist\n\n\n"   ANSI_COLOR_RESET);
		return;
	}

	printf("Password --> ");
	inputString(&userPassword);

	Details* user = readUser(type == customer ? FILE_CUSTOMERS : FILE_MANAGERS, type);

	if (strcmp(readUser(type == customer ? FILE_CUSTOMERS : FILE_MANAGERS, readPassword)->password, userPassword) == 0)
	{
		if (type == customer)
		{
			system("cls");
			customerMenu();
		}

		if (type == manager)
		{
			system("cls");
			managerMenu();
		}
	}

	else
		printf(ANSI_COLOR_RED   "Invalid Password\n\n\n"   ANSI_COLOR_RESET);
}
void profile()
{
	//Profile Homepage//

	int selection = 0;
	bool loop = true;

	while (loop)
	{
		printf("[Profile Menu]\n");
		printf("'1' Print Profile     '2' Update Profile     '3' Return\nInput --> ");
		selection = stringToInt();

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
			system("cls");
			loop = false;
			break;

		default:
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 3]\n\n\n"   ANSI_COLOR_RESET);
			break;
		}
	}
}
void printProfile()
{
	//Print the current user details//

	printf("\n\n[User Information]\n");

	Details* details = readUser(findUserType(Identity) == customer ? FILE_CUSTOMERS : FILE_MANAGERS, findUserType(Identity));
	float _Points = details->points;
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
		printf("Supermarket Points --> %.2f\n", _Points);
	}
	printf("\n\n");
}
void updateProfile()
{
	//Profile Update (Name/Paswword ext..)//

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
				while (loop)
				{
					printf("\n\n[Profile Updatable Actions]\n");
					printf("'1' Name     '2' Password     '3' ID     '4' Phone     '5' Return\nInput --> ");
					selection = stringToInt();

					switch (selection)
					{
					case 1:
						printf("\n\n[Name Update]\n");
						verifyName(&d);
							
						fprintf(Temp, "%s,%s,%s,%s,%s\n", d.name, _Id, _Password, _Points, _Phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your Name\n"   ANSI_COLOR_RESET);
						break;

					case 2:
						printf("\n\n[Password Update]\n");
						verifyPassword(&d);

						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, d.password, _Points, _Phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your Password\n"   ANSI_COLOR_RESET);
						break;

					case 3:
						printf("\n\n[ID Update]\n");
						verifyId(&d);

						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, d.ID, _Password, _Points, _Phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your ID\n"   ANSI_COLOR_RESET);
						break;

					case 4:
						printf("\n\n[Phone Update]\n");
						verifyPhone(&d);

						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, d.phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your Phone\n"   ANSI_COLOR_RESET);
						break;

					case 5:
						fprintf(Temp, "%s,%s,%s,%s,%s\n", _Name, _Id, _Password, _Points, _Phone);
						loop = false;
						break;

					default:
						printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 4]\n"   ANSI_COLOR_RESET);
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
				while (loop)
				{
					printf("\n\n[Profile Updatable Actions]\n");
					printf("'1' Name     '2' Password     '3' ID     '4' Phone     '5' Return\nInput --> ");
					selection = stringToInt();

					switch (selection)
					{
					case 1:
						printf("\n\n[Name Update]\n");
						verifyName(&d);
					
						fprintf(Temp, "%s,%s,%s,%s\n", d.name, _Id, _Password, _Phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your Name\n"   ANSI_COLOR_RESET);
						break;

					case 2:
						printf("\n\n[Password Update]\n");
						verifyPassword(&d);

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, d.password, _Phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your Password\n"   ANSI_COLOR_RESET);
						break;

					case 3:
						printf("\n\n[ID Update]\n");
						verifyId(&d);

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, d.ID, _Password, _Phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your ID\n"   ANSI_COLOR_RESET);
						break;

					case 4:
						printf("\n\n[Phone Update]\n");
						verifyPhone(&d);

						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, _Password, d.phone);
						loop = false;
						printf(ANSI_COLOR_GREEN   "You've Successfully Updated Your Phone\n"   ANSI_COLOR_RESET);
						break;

					case 5:
						fprintf(Temp, "%s,%s,%s,%s\n", _Name, _Id, _Password, _Phone);
						loop = false;
						break;

					default:
						printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To 4]\n"   ANSI_COLOR_RESET);
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
	printf("\n\n");
}
char* getNextOrderIdStr()
{
	//Return the next order in the folder//

	char _Index[50] = { NULL };
	int num = 0;

	FILE* file = fopen(FILE_ORDERS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL };
	while (fgets(buffer, 500, file))
		num++;

	sprintf(_Index, "%d", num);
	fclose(file);

	char* pString = NULL;
	pString = (char*)malloc(strlen(_Index) * sizeof(char) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, _Index);
	return pString;
}
char* getNextTicketIdStr()
{
	//Return the next ticket in the folder//

	char _Index[50] = { NULL };
	int num = 0;

	FILE* file = fopen(FILE_TICKETS, "r");
	if (!file) exit(1);

	char buffer[500] = { NULL };
	while (fgets(buffer, 500, file))
		num++;

	sprintf(_Index, "%d", num);
	fclose(file);

	char* pString = NULL;
	pString = (char*)malloc(strlen(_Index) * sizeof(char) + sizeof(char));
	if (!pString) exit(1);

	strcpy(pString, _Index);
	return pString;
}
bool doesProductExist(char* filename, char* _ProductName, char* _Company)
{
	//Return true if the product exist, Else false//

	FILE* file = fopen(filename, "r");
	if (!file) exit(1);

	Product tempProduct = { NULL, NULL, NULL, 0, 0.0 };
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
	//Select product from the catalog//

	Product product = { NULL, NULL, NULL, 0, 0.0 };

	if (cart.itemsCount == 0)
		return product;

	printf("\n\n[Store Catalog]\n%-15s%-15s%-15s%-15s%-15s%-15s\n", "No.", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");

	int i = 1, res = -1;
	char buffer[500] = { NULL };

	for (i = 0; i < cart.itemsCount; i++)
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart.products[i].name, cart.products[i].company, cart.products[i].category, cart.products[i].price, cart.products[i].quantity);

	while (!(res >= 0 && res <= cart.itemsCount))
	{
		printf("\n\n[Available Actions]\n");
		printf("'0' Sort By Ascending Price     '1 To %d' Select Product\nInput --> ", cart.itemsCount);
		res = stringToInt();

		if (!(res >= 0 && res <= cart.itemsCount))
		{
			printf(ANSI_COLOR_RED   "Invalid Input, Try Between [0 To %d]\n", cart.itemsCount);
			printf(ANSI_COLOR_RESET);
		}
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

		printf("\n%-15s%-15s%-15s%-15s%-15s%-15s\n", "No. ", "NAME", "COMPANY", "CATEGORY", "PRICE", "QUANTITY");

		for (int i = 0; i < cart.itemsCount; i++)
			printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", i + 1, cart.products[i].name, cart.products[i].company, cart.products[i].category, cart.products[i].price, cart.products[i].quantity);

		while (!(res - 1 >= 0 && res - 1 < i))
		{
			printf("\nSelect Product\nInput --> ");
			res = stringToInt();

			if (!(res - 1 >= 0 && res - 1 < i))
			{
				printf(ANSI_COLOR_RED   "Invalid Input, Try Between [1 To %d]\n\n", i);
				printf(ANSI_COLOR_RESET);
			}

		}
		return cart.products[res - 1];
	}

	else
		return cart.products[res - 1];

	return product;
}
Product readProduct(char* filename, char* _ProductName, char* _Company)
{
	//Return the choosen product from the catalog//
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
	//Retrieve all the products from the cart//
	FILE* file = fopen(FILE_CATALOGS, "r");
	if (!file) exit(1);

	int count = 0, quantity = -1;
	char name[100] = { NULL }, company[100] = { NULL }, category[100] = { NULL }, buffer[500] = { NULL };

	while (fscanf(file, "%s", buffer) == 1)
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%*f,%d", name, company, category, &quantity);
		if (search)
		{
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0)
			{
				if (quantity > 0 || returnAll)
					count++;
			}
		}

		else if (searchCategory)
		{
			if (strcmp(category, searchCategory) == 0)
			{
				if (quantity > 0 || returnAll)
					count++;
			}
		}

		else if (quantity > 0 || returnAll)
			count++;
	}

	Cart cart = { NULL, 0 };

	if (count == 0)
	{
		printf(ANSI_COLOR_RED   "Name/Company Does Not Exist In Our Catalog\n\n\n"   ANSI_COLOR_RESET);
		return cart;
	}

	float price = -1;
	int ite = 0;

	cart.products = malloc(sizeof(Product) * count);
	cart.itemsCount = count;
	fclose(file);

	file = fopen(FILE_CATALOGS, "r");
	if (!file) exit(1);

	while (fscanf(file, "%s", buffer) == 1)
	{
		sscanf(buffer, "%[^,],%[^,],%[^,],%f,%d", name, company, category, &price, &quantity);

		if (search)
		{
			if (strcmp(strToLower(name), strToLower(search)) == 0 || strcmp(strToLower(company), strToLower(search)) == 0)
			{
				if (quantity > 0 || returnAll)
				{
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}
			}
		}

		else if (searchCategory)
		{
			if (strcmp(category, searchCategory) == 0)
			{
				if (quantity > 0 || returnAll)
				{
					Product tempProduct = { copyString(name), copyString(company), copyString(category), quantity, price };
					cart.products[ite] = tempProduct;
					ite++;
				}
			}
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
	checkFolder();
	checkFiles();
	welcomeScreen();

	return 0;
}
