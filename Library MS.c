#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

const int length = 999;
int bookcount = 0;

typedef struct books
{
    char name[1000];
    char author[30] ;
    char category[20];
    char department[20];
    char language[20];
    char publisheddate[1000];
    int quantity;
} books;
typedef struct node
{
    books book;
    struct node * next;
    struct node * previous;
} node;
typedef struct borrowed {
    char fname[100];
    char lname[100];
    char number[100];
    char address[100];
    char email[100];
    char dateborrowed[100];
} borrowed;
typedef struct borrowedbooks
{
    books * book;
    borrowed * borrower;
    struct borrowedbooks * next;
} borrowedbooks;

node *ddc[9];
borrowedbooks * first = NULL;

int load();
char current_time();
time_t due_date();
void addbook();
void add_borrower();
void overdue_book(time_t due_date);
unsigned int hash(char*department);
node * search_for_book (char *name, char*department);

int main()
{
    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add book\n");
        printf("2. Delete book\n");
        printf("3. Add borrower\n");
        printf("4. Borrowed book\n");
        printf("5. Return book\n");
        printf("6. passed due date\n");
        printf("7. Search for a book\n");
        printf("8. List borrowed books\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            addbook();
            break;
        case 2:
            deletebooks();
            break;
        case 3:
            addborrower();
            break;
        case 4:
            borrowedbook();
            break;
        case 5:
            returnbook();
            break;
        case 6:
            Passedduedate();
            break;
        case 7:
            searchforbook();
            break;
        case 8:
            listborrowedbooks();
            break;
        case 0:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (choice != 0);
    return 0;
}

int load()
{
    FILE * f;
    f = fopen("filename", "br");
    if (f == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }

    while(true)
    {
        books * p = malloc(sizeof(books));
        if (p == NULL)
        {
            return 1;
        }
       if (fread(p,(sizeof(books)),1, f) != 1)
       {
        free (p);
        break;
       }
       books *new_book = malloc(sizeof(books));
       if(new_book == NULL)
       {
        printf("unable to allocate memory");
        free(p);
        return 1;
       }
       strcpy(new_book -> name, p -> name);
       strcpy(new_book -> author, p -> author);
       strcpy(new_book -> department, p -> department);
       strcpy(new_book -> category, p -> category);
       strcpy(new_book -> language, p -> language);
       strcpy(new_book -> publisheddate, p -> publisheddate);
       new_book -> quantity = p -> quantity;
        node *b = malloc(sizeof(node));
        if (b == NULL)
        {
            printf("Unable to access memory\n");
            return 1;
        }
        b -> book = *new_book;
        b -> next = NULL;
        b -> previous = NULL;
        bookcount++;

        int bucket = hash(new_book -> department);
        if(ddc[bucket] == NULL)
        {
            ddc[bucket] = b;
        }
        else
        {
            node *temp = ddc[bucket];
            while (temp -> next != NULL)
            {
                temp = temp -> next;
            }
            temp -> next = b;
            b -> previous = temp;

        }
    }
    fclose(f);
    return 0;
}

 unsigned int hash (char * department)
{
    unsigned int hash = 0;
    for (int i = 0; department[i] != '\0'; i++)
    {
        hash = hash * 31 + department[i];
    }
    return (hash % 9);
}

void add_borrower()
{
    borrowed new_borrower;
    printf( "Enter the borrower's details\n");
    printf("First Name: ");
    gets(new_borrower.fname);
    printf("Last Name: ");
    gets(new_borrower.lname);
    printf("Cellphone Number: ");
    scanf("%d", &new_borrower.cellnumber);
    printf("Home Number: ");
    scanf("%d", &new_borrower.homenumber);
    printf("Address: ");
    scanf("%s", &new_borrower.address);
    printf("Email: ");
    scanf("%s", &new_borrower.email);
    printf("Date Borrowed: ");
    current_time();
    printf("\nBorrower Added");
}

void addbook() 
{
    books newb;
    printf("Enter book details:\n");
    printf("Name: ");
    gets(newb.name);
    printf("\nAuthor: ");
    gets(newb.author);
    printf("\nCategory: ");
    gets(newb.category);
    printf("\ndepartment: ");
    gets(newb.department);
    printf("\nPublished date: ");
    gets(newb.language);
    printf("\nLanguage: ");
    gets(newb.publisheddate);
    printf("\nQuantity: ");
    scanf("%d", &newb.quantity);
    printf("Book Added");
}

void borrow_book(char *name, char*department) 
{
    char  borrower_name[50];
    printf("Enter book name to borrow: ");
    scanf("%s", name);
    printf("Enter department: ");
    scanf("%s", department);
    printf("Enter borrower name: ");
    scanf("%s", borrower_name);
    node* p = search_for_book( name,  department );
    if ((p-> book.quantity) > 0)
    {
        p->book.quantity--;
        printf("%s borrowed successfully.\n", name);
        printf("Due date: ");
        printf(due_date());
        return;
    }
    else 
    {
        printf("Sorry, the book is not available right now.\n");
        return;
    }
}

node * search_for_book (char *name, char*department)
{
    int bucket =hash(department);
    node *d = ddc[bucket];
    while (d != NULL)
    {
        if(strcmp(name,d -> book.name) == 0)
        {
            return d;
        }
        d = d -> next;
    }
    return NULL;

}

char current_time()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    size_t ret = strftime(s, sizeof(s), "%c", tm);
    assert(ret);
    printf("%s\n", s);
    return 0;
}

time_t due_date()
{
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    // Calculate due date (two weeks from now)
    struct tm due_date = *now;
    due_date.tm_mday += 14; // Add 14 days to the current day
    mktime(&due_date); // Normalize the struct tm
    // Store the due date as a constant value for future use
    const time_t DUE_DATE = mktime(&due_date);
    printf("%02d/%02d/%02d %02d:%02d:%02d\n", due_date.tm_mon +1, due_date.tm_mday + 1, due_date.tm_year % 100, now->tm_hour, now->tm_min, now->tm_sec);
    return DUE_DATE;
}

void overdue_book(time_t due_date)
{
    time_t current_time = time(NULL);
    double days_overdue = difftime(current_time, due_date) / (24 * 60 * 60);
    if (days_overdue > 0) 
    {
        printf("The book is %.0f days overdue\n", days_overdue);
    } 
    else 
    {
        printf("There are %.0f days left until the due date\n", -days_overdue);
    }
}

void saveFile(char* filename,char* data)
{
    FILE* fp;

    fp = fopen(filename, "wb"); 

    if (fp == NULL)
    {
        printf("404. cannot open file\n");
        exit(1);
    }

    fwrite(data,sizeof(books),num_books,fp);

    fclose(fp); 
}