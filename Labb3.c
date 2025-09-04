#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_FILENAME_LENGHT 100
#define MAX_FILE_LENGTH 5000
#define MAX_ACCOUNT_NUMBER 1000
#define NAME_LENGTH 30
#define ACCOUNT_NUMBER_LENGTH 7

struct BankAccount
{
    char accountNumber[ACCOUNT_NUMBER_LENGTH];
    char firstName[NAME_LENGTH];
    char lastName[NAME_LENGTH];
    int balance;
};
typedef struct BankAccount BankAccount;

int isFileExist(char *fileName);
void readFromFile(BankAccount reg[], int *pNrOfAccounts, char fileName[]);
void createNewFile(char *fileName);
BankAccount createAccount(char accountNumber[], char firstName[], char lastName[], int balance);
void registerNewAccount(BankAccount bankAccounts[], int *nrOfAccounts, char fileName[]);
int isAccountExist(char accountNumber[], BankAccount bankAccounts[], int nrOfAccounts);
void convertBankAccountToString(char fileRow[], BankAccount bankkonto);
void exitProgram(BankAccount bankAccounts[], int nrOfAccounts, char *fileName);
void printAllBankAccounts(BankAccount reg[], int NrOfAccounts);
void printBankAccount(BankAccount bankAccount);
void printHeader();
void swap(BankAccount *bankAccount1, BankAccount *bankAccount2);
void sortAccountsByOwnerName(BankAccount bankAccounts[], int nrOfAccounts);
void sortAccountsByAccountNumber(BankAccount bankAccounts[], int nrOfAccounts);
void sortAccountsByBalance(BankAccount bankAccounts[], int nrOfAccounts);
void sortAccounts(BankAccount bankAccounts[], int nrOfAccounts);
void searchAccountsByOwnerName(char searchString[], BankAccount searchResults[], int *pResultsCount, BankAccount bankAccounts[], int nrOfAccounts);
BankAccount *getAccountByNumber(char accountNumber[], BankAccount bankAccounts[], int nrOfAccounts);
void searchAccountsByBalance(int minBalance, int maxBalance, BankAccount *results, int *pNoOfResults, BankAccount bankAccounts[], int nrOfAccounts);
void searchForAccount(BankAccount bankAccounts[], int nrOfAccounts);
BankAccount *askUserForAccount(BankAccount bankAccounts[], int nrOfAccounts);
void depositOrWithdrawAccount(BankAccount *bankAccount);
void changeAccountBalance(BankAccount bankAccounts[], int nrOfAccounts);
void unregisterAccount(BankAccount bankAccounts[], int *pNrOfAccounts);
void accountManagement(int choice, BankAccount bankAccounts[], int *pNrOfAccounts, char fileName[]);

int main()
{
    char fileName[MAX_FILENAME_LENGHT];
    printf("Ange filnamn: ");
    scanf("%s%*c", fileName);

    BankAccount bankAccounts[1000];
    int nrOfAccounts = 0;

    if (!isFileExist(fileName))
    {
        createNewFile(fileName);
    }

    readFromFile(bankAccounts, &nrOfAccounts, fileName);
    int choice = -1;

    while (choice != 7)
    {
        printf("(1) Registrera nya konton\n");
        printf("(2) Skriva ut alla konton\n");
        printf("(3) Sortera konton\n");
        printf("(4) Soka efter konto\n");
        printf("(5) Andra saldot pa ett konto\n");
        printf("(6) Avregistrera ett konto\n");
        printf("(7) Avsluta\n");

        scanf("%d", &choice);
        accountManagement(choice, bankAccounts, &nrOfAccounts, fileName);
    }
    return 0;
}

int isFileExist(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");
    int isExist = fp != NULL;

    if (isExist)
    {
        fclose(fp);
    }

    return isExist;
}

void readFromFile(BankAccount reg[], int *pNrOfAccounts, char fileName[])
{
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp != NULL)
    {
        char kontonummer[ACCOUNT_NUMBER_LENGTH];
        char fornamn[NAME_LENGTH];
        char efternamn[NAME_LENGTH];
        float saldo;

        while (fscanf(fp, "%s %s %s %f", kontonummer, fornamn, efternamn, &saldo) == 4)
        {
            reg[*pNrOfAccounts] = createAccount(kontonummer, fornamn, efternamn, saldo);
            (*pNrOfAccounts)++;
        }
    }
    fclose(fp);
}

void createNewFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "w");
    fclose(fp);
}

BankAccount createAccount(char accountNumber[], char firstName[], char lastName[], int balance)
{
    BankAccount account;
    strcpy(account.accountNumber, accountNumber);
    strcpy(account.firstName, firstName);
    strcpy(account.lastName, lastName);
    account.balance = balance;

    return account;
}

void registerNewAccount(BankAccount bankAccounts[], int *nrOfAccounts, char fileName[])
{
    char newAccountNumber[ACCOUNT_NUMBER_LENGTH];
    char firstName[NAME_LENGTH];
    char lastName[NAME_LENGTH];

    printf("Registrera nya konton\n");

    if ((*nrOfAccounts) == MAX_ACCOUNT_NUMBER)
    {
        printf("Du har nått max begransning av bankkonto antal");
        return;
    }

    int _isAccountExist;
    do
    {
        printf("Ange kontonummer (q for att avsluta): ");
        scanf("%s%*c", newAccountNumber);

        if (newAccountNumber[0] == 'q')
        {
            exitProgram(bankAccounts, *nrOfAccounts, fileName);
            return;
        }

        _isAccountExist = isAccountExist(newAccountNumber, bankAccounts, *nrOfAccounts);
        if (_isAccountExist)
        {
            printf("Kontonummret finns redan!\n");
        }
    } while (_isAccountExist);

    printf("Ange agare: ");
    scanf("%s %s", firstName, lastName);

    if (strlen(firstName) + strlen(lastName) > NAME_LENGTH - 1)
    {
        printf("Namnet ar langre an antal tillatna tecken\n");
        return;
    }
    else
    {
        BankAccount newBankAccount = createAccount(newAccountNumber, firstName, lastName, 0);
        bankAccounts[*nrOfAccounts] = newBankAccount;
        (*nrOfAccounts)++;
    }

    registerNewAccount(bankAccounts, nrOfAccounts, fileName);
}

int isAccountExist(char accountNumber[], BankAccount bankAccounts[], int nrOfAccounts)
{
    for (int i = 0; i < nrOfAccounts; i++)
    {
        if (strcmp(bankAccounts[i].accountNumber, accountNumber) == 0)
            return 1;
    }

    return 0;
}

void convertBankAccountToString(char fileRow[], BankAccount bankkonto)
{
    strcat(fileRow, bankkonto.accountNumber);
    strcat(fileRow, " ");
    strcat(fileRow, bankkonto.firstName);
    strcat(fileRow, " ");
    strcat(fileRow, bankkonto.lastName);
    strcat(fileRow, " ");
    char balanceStr[10];
    _itoa(bankkonto.balance, balanceStr, 10); //Ändra från int to string
    strcat(fileRow, balanceStr);
}

void exitProgram(BankAccount bankAccounts[], int nrOfAccounts, char *fileName)
{
    char fileContent[MAX_FILE_LENGTH] = "";
    for (int i = 0; i < nrOfAccounts; i++)
    {
        char bankAccountTextRow[50] = "\0";
        convertBankAccountToString(bankAccountTextRow, bankAccounts[i]);
        strcat(bankAccountTextRow, "\n");
        strcat(fileContent, bankAccountTextRow);
    }

    FILE *fp;
    fp = fopen(fileName, "w");
    if (fp)
    {
        fprintf(fp, fileContent);
    }

    fclose(fp);
}

void printAllBankAccounts(BankAccount reg[], int nrOfAccounts)
{
    printf("Alla konton\n");
    printHeader();
    for (int i = 0; i < nrOfAccounts; i++)
    {
        printBankAccount(reg[i]);
    }
}

void printBankAccount(BankAccount bankAccount)
{
    printf("%s\t %s %s\t %d\n", bankAccount.accountNumber, bankAccount.firstName, bankAccount.lastName, bankAccount.balance);
}

void printHeader()
{
    printf("Konto-nr\tAgare\t  Saldo\n");
}

void swap(BankAccount *bankAccount1, BankAccount *bankAccount2)
{
    BankAccount tmp;
    strcpy(tmp.accountNumber, (*bankAccount1).accountNumber);
    strcpy(tmp.firstName, (*bankAccount1).firstName);
    strcpy(tmp.lastName, (*bankAccount1).lastName);
    tmp.balance = (*bankAccount1).balance;

    strcpy((*bankAccount1).accountNumber, (*bankAccount2).accountNumber);
    strcpy((*bankAccount1).firstName, (*bankAccount2).firstName);
    strcpy((*bankAccount1).lastName, (*bankAccount2).lastName);
    (*bankAccount1).balance = (*bankAccount2).balance;

    strcpy((*bankAccount2).accountNumber, tmp.accountNumber);
    strcpy((*bankAccount2).firstName, tmp.firstName);
    strcpy((*bankAccount2).lastName, tmp.lastName);
    (*bankAccount2).balance = tmp.balance;
}

void sortAccountsByOwnerName(BankAccount bankAccounts[], int nrOfAccounts)
{
    int i, j, tmp;
    for (i = 0; i < nrOfAccounts - 1; i++)
    {
        for (j = 0; j < nrOfAccounts - 1 - i; j++)
        {
            if (strcmp(bankAccounts[j].lastName, bankAccounts[j + 1].lastName) > 0)
            {
                swap(&bankAccounts[j], &bankAccounts[j + 1]);
            }
        }
    }
}

void sortAccountsByAccountNumber(BankAccount bankAccounts[], int nrOfAccounts)
{
    BankAccount tmp;
    for (int i = 0; i < nrOfAccounts - 1; i++)
    {
        for (int j = 0; j < nrOfAccounts - 1 - i; j++)
        {
            int ba1 = atoi(bankAccounts[j].accountNumber); //ändra från string to int
            int ba2 = atoi(bankAccounts[j + 1].accountNumber);
            if (ba1 > ba2)
            {
                swap(&bankAccounts[j], &bankAccounts[j + 1]);
            }
        }
    }
}

void sortAccountsByBalance(BankAccount bankAccounts[], int nrOfAccounts)
{
    BankAccount tmp;
    for (int i = 0; i < nrOfAccounts - 1; i++)
    {
        for (int j = 0; j < nrOfAccounts - 1 - i; j++)
        {
            if (bankAccounts[j].balance > bankAccounts[j + 1].balance)
            {
                swap(&bankAccounts[j], &bankAccounts[j + 1]);
            }
        }
    }
}

void sortAccounts(BankAccount bankAccounts[], int nrOfAccounts)
{
    printf("Vill du sortera efter konto-nr(1), agare(2) eller saldo(3)");
    int choice = -1;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        sortAccountsByAccountNumber(bankAccounts, nrOfAccounts);
        break;
    case 2:
        sortAccountsByOwnerName(bankAccounts, nrOfAccounts);
        break;
    case 3:
        sortAccountsByBalance(bankAccounts, nrOfAccounts);
        break;
    case 4:
        break;
    default:
        break;
    }

    printHeader();
    printAllBankAccounts(bankAccounts, nrOfAccounts);
}

void searchAccountsByOwnerName(char searchString[], BankAccount searchResults[], int *pResultsCount, BankAccount bankAccounts[], int nrOfAccounts)
{
    for (int i = 0; i < nrOfAccounts; i++)
    {
        char name[NAME_LENGTH] = " ";
        strcpy(name, bankAccounts[i].firstName);
        strcat(name, bankAccounts[i].lastName);

        if (strstr(name, searchString) != NULL)
        {
            searchResults[(*pResultsCount)++] = bankAccounts[i];
        }
    }
}

BankAccount *getAccountByNumber(char accountNumber[], BankAccount bankAccounts[], int nrOfAccounts)
{
    for (int i = 0; i < nrOfAccounts; i++)
    {
        if (strcmp(bankAccounts[i].accountNumber, accountNumber) == 0)
        {
            return &(bankAccounts[i]);
        }
    }

    return NULL;
}

void searchAccountsByBalance(int minBalance, int maxBalance, BankAccount *results, int *pNoOfResults, BankAccount bankAccounts[], int nrOfAccounts)
{
    for (int i = 0; i < nrOfAccounts; i++)
    {
        if (bankAccounts[i].balance >= minBalance && bankAccounts[i].balance <= maxBalance)
        {
            results[*pNoOfResults] = bankAccounts[i];
            (*pNoOfResults)++;
        }
    }
}

void searchForAccount(BankAccount bankAccounts[], int nrOfAccounts)
{
    printf("Sokning\n");
    printf("(1) Konto-nr\n");
    printf("(2) Agare\n");
    printf("(3) saldo\n");
    printf("(4) Huvudmeny\n");
    printf("Ange val\n");

    int choice = -1;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        char accountNumber[ACCOUNT_NUMBER_LENGTH];
        printf("Ange konto-nr: ");
        scanf("%s%*c", accountNumber);
        BankAccount *resultBankAccount = getAccountByNumber(accountNumber, bankAccounts, nrOfAccounts);

        if (resultBankAccount == NULL)
            printf("Ingen traff");
        else
            printBankAccount(*resultBankAccount);
    }
    break;
    case 2:
    {
        char searchString[NAME_LENGTH];
        BankAccount searchResults[MAX_ACCOUNT_NUMBER];
        int resultsCount = 0;
        printf("Ange sokstrang: ");
        scanf("%s%*c", searchString);
        searchAccountsByOwnerName(searchString, searchResults, &resultsCount, bankAccounts, nrOfAccounts);
        if (resultsCount == 0)
        {
            printf("Ingen traff\n");
            return;
        }

        printAllBankAccounts(searchResults, resultsCount);
    }
    break;
    case 3:
    {
        int minBalance;
        int maxBalance;
        printf("Ange min: ");
        scanf("%d", &minBalance);
        printf("Ange max: ");
        scanf("%d", &maxBalance);
        BankAccount results[1000];
        int noOfResults = 0;
        searchAccountsByBalance(minBalance, maxBalance, results, &noOfResults, bankAccounts, nrOfAccounts);

        if (noOfResults == 0)
        {
            printf("Inga traff\n");
        }
        else
        {
            for (int i = 0; i < noOfResults; i++)
            {
                printBankAccount(results[i]);
            }
        }
    }
    break;
    case 4:
        break;
    default:
        break;
    }
}

BankAccount *askUserForAccount(BankAccount bankAccounts[], int nrOfAccounts)
{
    printf("Vill du ange konto-nr (1) eller soka pa agare (2): ");

    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 2:
    {
        char name[NAME_LENGTH];
        printf("Ange agare: ");
        scanf("%s%*c", name);

        BankAccount searchResults[MAX_ACCOUNT_NUMBER];
        int nrOfResults = 0;
        searchAccountsByOwnerName(name, searchResults, &nrOfResults, bankAccounts, nrOfAccounts);
        if (nrOfResults == 0)
        {
            printf("Ingen traff\n");
            return askUserForAccount(bankAccounts, nrOfAccounts);
        }
        printAllBankAccounts(searchResults, nrOfResults);
    }
    case 1:
    {
        char accountNumber[ACCOUNT_NUMBER_LENGTH];
        printf("Ange konto-nr: ");
        scanf("%s%*c", accountNumber);
        BankAccount *theAccount = getAccountByNumber(accountNumber, bankAccounts, nrOfAccounts);
        if (theAccount == NULL)
        {
            printf("Account not found\n");
            return askUserForAccount(bankAccounts, nrOfAccounts);
        }
        else
        {
            return theAccount;
        }
    }
    break;
    default:
        break;
    }
}

void depositOrWithdrawAccount(BankAccount *bankAccount)
{
    printf("Vill du gora en insattning (1) eller ett uttag (2): ");
    int choice = -1;
    scanf("%d", &choice);

    int amount;
    printf("Ange belopp: ");
    scanf("%d", &amount);

    switch (choice)
    {
    case 1:
    {
        if (amount >= 0)
            (*bankAccount).balance += amount;
    }
    break;
    case 2:
        if ((*bankAccount).balance - amount < 0)
        {
            printf("Du kan inte uttag det har beloppet\n");
        }
        else if (amount >= 0)
        {
            (*bankAccount).balance -= amount;
        }
        break;
    default:
        break;
    }

    printf("Nytt saldo: %d\n", (*bankAccount).balance);
}

void changeAccountBalance(BankAccount bankAccounts[], int nrOfAccounts)
{
    printf("Andra saldo\n");
    BankAccount *theAccount = askUserForAccount(bankAccounts, nrOfAccounts);
    depositOrWithdrawAccount(theAccount);
}

void unregisterAccount(BankAccount bankAccounts[], int *pNrOfAccounts)
{
    printf("Avregistrera\n");
    BankAccount *theAccount = askUserForAccount(bankAccounts, *pNrOfAccounts);

    if ((*theAccount).balance != 0)
    {
        printf("Kontot kan inte tas bort eftersom saldot inte ar 0\n");
        return;
    }

    int foundAtIndex = -1; //default värde som ska aldrig finnas

    for (int i = 0; i < *pNrOfAccounts; i++)
    {
        if (strcmp((*theAccount).accountNumber, bankAccounts[i].accountNumber) == 0)
        {
            foundAtIndex = i;
            break;
        }
    }

    if (foundAtIndex > -1) //hittades
    {
        int i = foundAtIndex;
        while (i < *pNrOfAccounts - 1)
        {
            strcpy(bankAccounts[i].accountNumber, bankAccounts[i + 1].accountNumber);
            strcpy(bankAccounts[i].firstName, bankAccounts[i + 1].firstName);
            strcpy(bankAccounts[i].lastName, bankAccounts[i + 1].lastName);
            bankAccounts[i].balance = bankAccounts[i + 1].balance;
            i++;
        }
        (*pNrOfAccounts)--;
    }
}

void accountManagement(int choice, BankAccount bankAccounts[], int *pNrOfAccounts, char fileName[])
{
    switch (choice)
    {
    case 1:
        registerNewAccount(bankAccounts, pNrOfAccounts, fileName);
        break;
    case 2:
        printAllBankAccounts(bankAccounts, *pNrOfAccounts);
        break;
    case 3:
        sortAccounts(bankAccounts, *pNrOfAccounts);
        break;
    case 4:
        searchForAccount(bankAccounts, *pNrOfAccounts);
        break;
    case 5:
        changeAccountBalance(bankAccounts, *pNrOfAccounts);
        break;
    case 6:
        unregisterAccount(bankAccounts, pNrOfAccounts);
        break;
    case 7:
        exitProgram(bankAccounts, *pNrOfAccounts, fileName);
    default:
        break;
    }
}