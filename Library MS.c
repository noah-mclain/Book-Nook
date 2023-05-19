#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

const int length = 999;
int bookcount = 0; //initializing the number of books as an integer and starting from 0

typedef struct books //initializing the structure books
{
    char name[1000]; 
    char author[30] ;
    char category[20];
    char department[20];
    char language[20];
    char publisheddate[1000];
    int quantity;
} books; //setting 'books' as the structure's variable name to be used throughout the code
typedef struct node //initializing the structure for the pointer for books
{
    books book;
    struct node * next; //having a pointer point to the next book in the list
    struct node * previous; //having a pointer point to the previous book in the list
} node; //setting 'nodes' as the structure's variable name to be used throughout the code
typedef struct borrowed //initializing the borrowers structure
{
    char fname[100];
    char lname[100];
    char number[100];
    char address[100];
    char email[100];
    char dateborrowed[100];
} borrowed; //setting 'borrowed' as the structure's varible to be used throughout the code
typedef struct borrowedbooks //initializing the borrowed books structure
{
    books * book; //having a pointer point to the book borrowed
    borrowed * borrower; //having a pointer point to the borrower of the book
    struct borrowedbooks * next; //having a pointer point to the next borrowed book in the list
} borrowedbooks; //setting 'borrowedbooks' as the structure's variable to be used throughout the code
typedef struct Node  //list for borrwed books
{
  borrowedbooks * bookborrowed ;
  struct Node *next;
  struct Node *prev;
}Node;

node *ddc[9]; //having a pointer point to an array of the 9 departments in the filing system
borrowedbooks *first = NULL; //having a pointer point to the first borrowed book of the library and initializing it as NULL

//calling prototypes
int load();
char current_time();
time_t due_date();
void add_book();
void delete_book(char *name, char *department );
node *borrow_book();
borrowed *add_borrower();
void overdue_book(time_t due_date);
void return_book(char *name, char *department);
unsigned int hash(char*department);
node *search_for_book (char *name, char*department);
void list_borrowed_books (borrowedbooks *firstB);

int main()
{
    char name[length], dep[length];
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
        switch (choice) 
        {
            case 1:
                add_book();
                break;
            case 2:
                printf("Enter the name of the book and the department\n");
                gets(name);
                gets(dep);
                delete_book(name, dep);
                break;
            case 3:
                add_borrower();
                break;
            case 4:
                borrow_book();
                break;
            case 5:
                printf("Enter the name of the book and the department\n");
                gets(name);
                gets(dep);
                return_book(name, dep);
                break;
            case 6:
                overdue_book(due_date());
                break;
            case 7:
                printf("Enter the name of the book and the department\n");
                gets(name);
                gets(dep);
                search_for_book(name, dep);
                break;
            case 8:
                list_borrowed_books(first);
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
    f = fopen("books.bin", "rb");
    if (f == NULL)
    {
        printf("No memory found. Creating new file to store the memory...\n");
        f = fopen("books.bin","wb");
        if(f == NULL)
        {
            printf("Unable to create file\n");
            return 1;
        }
        fclose(f);
        f = fopen("books.bin","rb");
        if (f == NULL)
        {
            printf("Unable to open file\n");
            return 1;
        }

    }

    while(true)
    {
        books *p = malloc(sizeof(books));
        if(p == NULL)
        {
            return 1;
        }
       if(fread(p,(sizeof(books)),1, f) != 1)
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

       strcpy(new_book->name, p->name);
       strcpy(new_book->author, p->author);
       strcpy(new_book->department, p->department);
       strcpy(new_book->category, p->category);
       strcpy(new_book->language,p->language);
       strcpy(new_book->publisheddate,p->publisheddate);

       new_book -> quantity = p -> quantity;
       
       node *b = malloc(sizeof(node));
       
       if(b == NULL)
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
            ddc[bucket]=b;
       }
       else
       {
            node *temp = ddc[bucket];
            while (temp->next!=NULL)
            {
            temp=temp->next;
            }
            temp->next=b;
            b->previous=temp;
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

borrowed *add_borrower()
{
    borrowed *p = malloc(sizeof(borrowed));
    printf( "Enter the borrower's details\n");
    printf("First Name: ");
    gets(p -> fname);
    printf("Last Name: ");
    gets(p -> lname);
    printf("Cellphone Number: ");
    scanf("%d", p -> number);
    printf("Address: ");
    gets(p -> address);
    printf("Email: ");
    gets(p -> email);
    printf("Date Borrowed: ");
    current_time();
    printf("\nBorrower Added");
    return p;
}

void add_book() 
{
    books newb;
    printf("Enter book details:\n");
    printf("Name: ");
    gets(newb.name);
    printf("Author: ");
    gets(newb.author);
    printf("Category: ");
    gets(newb.category);
    printf("department: ");
    gets(newb.department);
    printf("Published date: ");
    gets(newb.publisheddate);
    printf("Language: ");
    gets(newb.language);
    printf("Quantity: ");
    scanf("%d", &newb.quantity);
    printf("Book Added");
}

void delete_book(char *name, char *department )
{
    node *p= search_for_book (name, department);
    node *temp= p -> next;
    temp -> previous = p -> previous;
    free(p);
}

node *borrow_book()
{
    char name[1000];
    char department[20];
    printf("Enter book name to borrow: ");
    gets(name);
    printf("Enter department: ");
    gets(department);
    node* p = search_for_book( name,  department );
    if ((p -> book.quantity) > 0)
    {
        p -> book.quantity--;
        printf("%s Book is available!.\n", name);
        borrowedbooks * request = malloc(sizeof(borrowedbooks));
        request -> book = &p->book;
        request -> borrower = add_borrower();
        if(first == NULL)
        {
            first = request;
        }
        else
        {
            borrowedbooks *temp = first;
            first = request;
            first -> next = temp;
        }
        printf("Due date: ");
        printf(due_date());
        return p;
    }
    else
    {
        printf("Sorry, the book is not available right now.\n");
        return NULL;
    }
}

void return_book(char *name, char *department)
{

    node *borrowed_book= borrow_book(name, department);

    if(borrowed_book== NULL)
    {
        printf("Error: book not found");
        return;
    }

    Node *returned_book= borrowed_book;
    Node *temp= returned_book  -> next;
    temp -> prev = returned_book -> prev;
    free(returned_book);
}

node *search_for_book (char *name, char*department)
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

void list_borrowed_books (borrowedbooks *firstB)
{
    if (firstB != NULL)
    {
        if(first -> next != NULL)
        {
            list_borrowed_books(firstB -> next);
            printf("Name of book: %s ", firstB -> book -> name);
            printf("Name of borrower: %s", firstB -> borrower -> fname);
        }
        else
        printf("Name of book: %s", firstB -> book -> name);
        printf("Name of borrower: %s", firstB -> borrower -> fname);
    }
    else
    printf("No borrowed books\n");
}

void save_file(char* filename,books* data, int num_books)
{
    FILE* fp;

    fp = fopen("book_nook.bin", "wb"); 

    if (fp == NULL)
    {
        printf("404. cannot open file\n");
        exit(1);
    }

    fwrite(data,sizeof(books),num_books,fp);

    fclose(fp); 
}