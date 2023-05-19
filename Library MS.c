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
void add_book(node ** new_head);
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
    FILE * f; //initializing file pointer 'f'
    f = fopen("books.bin", "rb"); //opening binary file 'books.bin' to read
    if (f == NULL)
    {
        printf("No memory found. Creating new file to store the memory...\n"); //if the file is not found then create new binary file
        f = fopen("books.bin","wb"); //named 'books.bin' to write
        if(f == NULL) //check again if file exists in memory
        {
            printf("Unable to create file\n"); //if not, then the file was unable to be created
            return 1;
        }
        fclose(f); //close the file
        f = fopen("books.bin","rb"); //if the file was created, then have pointer 'f' point to file
        if (f == NULL) //if pointer is pointing at NULL
        {
            printf("Unable to open file\n"); //then return 1 and print that file cannot be opened
            return 1;
        }

    }

    while(true) //while file is open
    {
        books *p = malloc(sizeof(books)); //have pointer 'p' point at book structure and allocate memory based on size of structure
        if(p == NULL) //if pointer 'p' value is NULL, then return 1
        {
            return 1; //memory was unable to be allocated
        }
       if(fread(p,(sizeof(books)),1, f) != 1) //if memory was allocated to structure 'books' and read to file
       {
            free (p); //then free data from memory
            break; //exit if
       }
       books *new_book = malloc(sizeof(books)); //have pointer'new_books" point at structure 'books' and allocate memory the size of 'books'

       if(new_book == NULL) //if pointer is NULL
       {
            printf("unable to allocate memory"); //print that memory was not allocated
            free(p); //free pointer 'p' from memory
            return 1;
       }

       //memory was allocated
       strcpy(new_book->name, p->name); //copy the name of the book from 'p' to the name of the book from 'new_book'
       strcpy(new_book->author, p->author); //copy the name of the author from 'p' to the name of the author from 'new_book'
       strcpy(new_book->department, p->department); //copy the name of the department from 'p' to the name of the department from 'new_book'
       strcpy(new_book->category, p->category); //copy the name of the category from 'p' to the name of the category from 'new_book'
       strcpy(new_book->language,p->language); //copy the language from 'p' to the language from 'new_book'
       strcpy(new_book->publisheddate,p->publisheddate); //copy the publish date from 'p' to the publish date from 'new_book'

       new_book -> quantity = p -> quantity; //have pointer 'new_book' point to quantity which is equal to quantity from structure 'books'
       
       node *b = malloc(sizeof(node)); //have pointer 'b' point to memory allocation of 'node'
       
       if(b == NULL)
       {
            printf("Unable to access memory\n"); //if 'b' is NULL, the memory was unable to be accessed
            return 1;
       }
       b -> book = *new_book; //have current book in pointer 'b' assigned the value of 'new_book' to it
       b -> next = NULL; //have the next new book in pointer 'b' be NULL
       b -> previous = NULL; //have the previous new book in pointer 'b' be NULL
       bookcount++; //increment the book counter by 1

       int bucket = hash(new_book -> department); //initialize variable 'bucket' and give it the value of function 'hash' with the department in 'new_book' as the parameter

       if(ddc[bucket] == NULL) //checks if the book is the first in the list
       {
            ddc[bucket]=b; //if yes, then it immediately assigns the book to the list
       }
       else
       {
            node *temp = ddc[bucket]; //initializes a pointer to temporarily hold the book
            while (temp->next!=NULL) //while the next book in the list does does not equal NULL
            {
                temp = temp -> next; //make temp equal to the next book in the list
            }
            temp -> next = b; //make the next book in the list equal to the value of 'b'
            b -> previous = temp; //have the previous book in pointer 'b' equal to the value of 'temp' 
       }
    }
    fclose(f); //close file 'f'
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

void add_book(node ** new_head)
//, books  new_data
{
    //allocating memoery for new book
    node* new_book = (node*) malloc(sizeof(node));

     //new_book->book =new_data;

    // Set the next and previous pointers for the new node to NULL
    new_book->next = NULL;
    new_book->previous = NULL;

    // If the list is empty, set the new node as the head
    if (*new_head == NULL) 
    {
        *new_head = new_book;
        return;
    }

    // Traverse the list to find the last node
    Node* last = *new_head;
    while (last->next != NULL) 
    {
        last = last->next;
    }

    // Insert the new node after the last node
    last->next = new_book;
    new_book->previous = last;

     //books newb;
    printf("Enter book details:\n");
    // printf("Name: ");
    // gets(newb.name);
    printf("\nAuthor: ");
    gets(*new_book ->book.author);
    printf("\nCategory: ");
    gets(*new_book ->book.category);
    printf("\ndepartment: ");
    gets(*new_book ->book.department);
    printf("\nPublished date: ");
    gets(*new_book ->book.language);
    printf("\nLanguage: ");
    gets(*new_book ->book.publisheddate);
    printf("\nQuantity: ");
    scanf("%d", new_book ->book.quantity);
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