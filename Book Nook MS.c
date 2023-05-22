#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

int bookcount = 0; //initializing the number of books as an integer and starting from 0

typedef struct books //initializing the structure books
{
    char name[1000];
    char author[30];
    char category[20];
    char department[20];
    char language[20];
    char publisheddate[1000];
    int quantity;
} books; //setting 'books' as the structure's variable name to be used throughout the code

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
    books *book; //having a pointer point to the book borrowed
    borrowed *borrower; //having a pointer point to the borrower of the book
    struct borrowedbooks *next; //having a pointer point to the next borrowed book in the list
} borrowedbooks; //setting 'borrowedbooks' as the structure's variable to be used throughout the code

typedef struct node //initializing the structure for the pointer for books
{
    books book;
    struct node *next; //having a pointer point to the next book in the list
    struct node *previous; //having a pointer point to the previous book in the list
} node; //setting 'nodes' as the structure's variable name to be used throughout the code


node *ddc[9]; //having a pointer point to an array of the 9 departments in the filing system
borrowedbooks *first = NULL; //having a pointer point to the first borrowed book of the library and initializing it as NULL

void load();
node* add_to_list(node* head, node* new_node);
void add_book();
borrowedbooks *borrow_book();
borrowed *add_borrower();
void overdue_book(time_t due_date);
void return_book(char * name);
unsigned int hash(char*department);
node *search_for_book (char *name, char*department);
void traverse ();
void save_file();
time_t current_time();
time_t due_date();
void list_borrowed_books (borrowedbooks *firstB);
void delete_book(char *name, char *department );

int main()
{
    int choice;
    char name[999];
    char dep[999];
    borrowedbooks *bookborrowed;
    load();
    do
    {   
        Beep(1000, 500);
        printf("\n                       \033[1;36m      \a    \2\2\2\2Library Management System\2\2\2\2\033[0m     \a  \n");
        printf("                        \033[1;34m\4\4\4 1. Add book \4\4\4\033[0m\n");
        printf("                        \033[1;34m\4\4\4 2. Delete book \4\4\4 \033[0m\n");
        printf("                        \033[1;34m\4\4\4 3. Borrow a book \4\4\4 \033[0m\n");
        printf("                        \033[1;34m\4\4\4 4. Return book \4\4\4 \033[0m\n");
        printf("                        \033[1;34m\4\4\4 5. Check due date \4\4\4 \033[0m\n");
        printf("                        \033[1;34m\4\4\4 6. Search for a book \4\4\4 \033[0m\n");
        printf("                        \033[1;34m\4\4\4 7. List borrowed books \4\4\4 \033[0m\n");
        printf("                        \033[1;34m\4\4\4 0. Exit \4\4\4 \033[0m\n");
        printf("\033[1;35m\1Enter your choice\1:\033[0m ");
        scanf("%d", &choice);
        fgetc(stdin);
        switch (choice)
        {
            case 1:
                add_book();
                break;
            case 2:
                printf("\033[1;35mEnter the name of the book1:\033[0m\n");
                fgets(name, sizeof(name), stdin);
                name[strlen(name)-1]=0;
                printf("\033[1;35mEnter the name of the department1:\033[0m\n");
                fgets(dep,sizeof(dep), stdin);
                dep[strlen(dep)-1]=0;
                delete_book(name, dep);
                break;
            case 3:
                borrow_book();
                break;
            case 4:
                printf("\033[1;35mEnter the name of the book1:\033[0m\n");
                fgets(name, sizeof(name), stdin);
                name[strlen(name)-1]=0;
                return_book(name);
                break;
            case 5:
                overdue_book(due_date());
                break;
            case 6:
                printf("\033[1;35mEnter the name of the book\033[0m\n");
                fgets(name, sizeof(name), stdin);
                name[strlen(name)-1]=0;
                printf("\033[1;35mEnter the name of the department\033[0m\n");
                fgets(dep,sizeof(dep), stdin);
                dep[strlen(dep)-1]=0;
                node* p= search_for_book(name, dep);
                if (p==NULL)
                {
                    printf("\033[1;31mBook not found\033[0m\n");
                }
                else
                {
                    printf("\033[1;32mBook found\033[0m\n");
                }
                break;
            case 7:
                list_borrowed_books(first);
                break;
            case 0:
                printf("\033[1;35mExiting program...\033[0m\n");
                break;
        }
    }
    while (choice!=0);
    traverse();
    save_file();
    return 0;

}

void add_book()
 {
    //allocating memory for new book
    node* new_book = malloc(sizeof(node));
    if (new_book == NULL)
    {
        printf("\033[1;31mError: Failed to allocate memory\033[0m\n");
        return;
    }

    printf("\033[1;35mEnter book details:\033[0m\n");
    printf("Name: ");
    fgets(new_book->book.name, sizeof(new_book->book.name), stdin);
    new_book->book.name[strlen(new_book->book.name)-1]=0;
    printf("Author: ");
    fgets(new_book ->book.author,sizeof(new_book->book.author),stdin);
    new_book->book.author[strlen(new_book->book.author)-1]=0;
    printf("Category: ");
    fgets(new_book->book.category, sizeof(new_book->book.category), stdin);
    new_book->book.category[strlen(new_book->book.category)-1]=0;
    printf("department: ");
    fgets(new_book->book.department, sizeof(new_book->book.department), stdin);
    new_book->book.department[strlen(new_book->book.department)-1]=0;
    printf("Language: ");
    fgets(new_book->book.language, sizeof(new_book->book.language), stdin);
    new_book->book.language[strlen(new_book->book.language)-1]=0;
    printf("Published Date: ");
    fgets(new_book->book.publisheddate, sizeof(new_book->book.publisheddate), stdin);
    new_book->book.publisheddate[strlen(new_book->book.publisheddate)-1]=0;

    new_book->book.quantity = 0;

    int bucket = hash(new_book->book.department);
    node *p = malloc(sizeof(node));
    if(p == NULL)
    {
        printf("\033[1;31mError, unable to allocate memory\033[0m\n");
        return;
    }

    p = search_for_book(new_book->book.name, new_book->book.department);
    if (p != NULL)
    {
        p->book.quantity += 1;
        new_book->book.quantity=p->book.quantity;
        printf("\033[1;32mBook quantity increased.\033[0m\n");
    }
    else
    {
        
        free(p);
        if (ddc[bucket] == NULL)
        {
            printf("\033[1;31mbuckets are empty\033[0m\n");
            ddc[bucket] = new_book;
            new_book->next = NULL;
            new_book->previous = NULL;
            new_book->book.quantity=1;
        }
        else
        {
            
            if(ddc[bucket]->previous==NULL)
            {
                ddc[bucket]->previous=new_book;
                ddc[bucket]->next=new_book;
                new_book->book.quantity=1;
            }
            else
            {
                node *temp=ddc[bucket]->previous;
                ddc[bucket]->previous= new_book;
                new_book->previous=temp;
                temp->next=new_book;
                new_book->book.quantity=1;
            }
        }
    }

    printf("\033[0;31mBook Added\033[0m\n");
    printf("\n\032[0;31mBook Added\033[0m\n");
    printf("%s\n", new_book->book.name);
    printf("%s\n", new_book->book.author);
    printf("%s\n", new_book->book.category);
    printf("%s\n", new_book->book.department);
    printf("%s\n", new_book->book.language);
    printf("%s\n", new_book->book.publisheddate);
    printf("%d\n", new_book->book.quantity);
    bookcount++;
}

unsigned int hash (char * department) //
{
    printf("department in hash: %s \n", department);
    unsigned int hash = 0;
    for (int i = 0; department[i] != '\0'; i++)
    {
        hash = hash * 31 + tolower(department[i]);
    }
    printf("hash value: %d\n", hash%9);
    return (hash % 9);
}

node *search_for_book (char *name, char*department)
{
    printf("%s\n", name);
    printf("%s\n", department);
    int bucket = hash(department);
    node *d = ddc[bucket];
    while (d != NULL)
    {
        if(strcasecmp(name, d->book.name) == 0)
        {
            return d;
        }
        d = d->next;
    }
    return NULL;

}

void traverse()
{
    int count;
    int total = 0;
    for (int i = 0; i < 9; i++)
    {
        printf("bucket: %d\n", i);
        node *p = ddc[i];
        count = 0;
        while (p != NULL)
        {
            printf("Book #%d  Bookname: %s  Department: %s\n", count+1, p->book.name, p->book.department);
            p = p->next;
            count++;
            total++;
        }
    }
    printf("Total Traversed: %d\n", total);
    printf("True Total: %d\n", bookcount);
    return;
}

void load()
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
            return;
        }
        fclose(f); //close the file
        f = fopen("books.bin","rb"); //if the file was created, then have pointer 'f' point to file
        if (f == NULL) //if pointer is pointing at NULL
        {
            printf("Unable to open file\n"); //then return 1 and print that file cannot be opened
            return;
        }

    }
    while(!feof(f))
    {
        node* new_book = malloc(sizeof(node));
        if (new_book == NULL)
        {
            printf("Error: Failed to allocate memory\n");
            return;
        }

        if (fread(&new_book->book, sizeof(books), 1, f) != 1)
        {
            free(new_book);
            break;
        }

        new_book->next = NULL;
        new_book->previous = NULL;

        int bucket = hash(new_book->book.department);
        ddc[bucket] = add_to_list(ddc[bucket], new_book);
        bookcount++;
    }

    fclose(f);
    printf("\033[0;31mData loaded successfully from binary file.\033[0m\n");
}

void save_file()
{
    FILE* fp;

    fp = fopen("books.bin", "wb");

    if (fp == NULL)
    {
        printf("\033[1;31m404. cannot open file\033[0m\n");
        return;
    }
    for(int i=0;i<9;i++)
    {
        node *temp= ddc[i];
        while (temp!=NULL)
        {
            fwrite(&(temp->book),sizeof(books),1,fp);
            temp=temp->next;
        }
    }

    fclose(fp);
    return;
}

node* add_to_list(node* head, node* new_node)
{
    if (head == NULL)
    {
        return new_node;
    }

    node* current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = new_node;
    new_node->previous = current;

    return head;
}

void delete_book(char *name, char *department)
{
   // Check if the linked list for the department is empty
    if (ddc[hash(department)] == NULL) 
    {
        printf("There are no books in the '%s'department.\n", department);
        return;
    }

    node *p = ddc[hash(department)];
    bool deleted = false;
    while (p != NULL) 
    {
        if (strcmp(p->book.name, name) == 0 && strcmp(p->book.department, department) == 0) 
        {
            if(p->book.quantity > 1)
            {
                p->book.quantity--;
                printf("Book quantity decreased\n");
                printf("Quantity remaining: %d\n", p->book.quantity);
            }
            // Update the linked list to remove the node
            else if (p->previous == NULL) 
            {
                // The book is the head of the linked list
                ddc[hash(department)] = p->next;
                if (p->next != NULL) 
                {
                    p->next->previous = NULL;
                }
            } 
            else 
            {
                // The book is not the head of the linked list
                p->previous->next = p->next;
                if(p->next != NULL) 
                {
                    p->next->previous = p->previous;
                }
            }

            // Free the node and print a message indicating success
            free(p);
            printf("A book with name '%s' and department '%s' has been deleted from the library\n", name, department);

            // Set the deleted flag to true and break out of the loop
            deleted = true;
            break;
        }
        else
        {
            // Check the next node if the deleted flag is not set
            p = p->next;
        }
    }

    // Print a message ifthe book was not found in the given department
    if (!deleted) 
    {
        printf("The book '%s' in '%s' does not exist in the library.\n", name, department);
        return;
    }

    // Update the ddc array pointer only if a book was found and deleted
    if (ddc[hash(department)] == NULL) 
    {
        printf("There are no books remaining in the '%s' department\n", department);
    }
    
}

borrowedbooks *borrow_book()
{
    char name[100];
    char department[100];
    printf("Enter book name to borrow: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // remove the newline character
    printf("Enter department: ");
    fgets(department, sizeof(department), stdin);
    department[strcspn(department, "\n")] = '\0'; // remove the newline character

    node *p = search_for_book(name, department);
    if (p == NULL)
    {
        printf("Sorry, the book is not available right now.\n");
        return NULL;
    }
    else if (p->book.quantity > 0) // check if book is available
    {
        p->book.quantity--;
        printf("%s book is available!\n", name);
        borrowedbooks *request = malloc(sizeof(borrowedbooks));
        request->book = &(p->book);
        request->borrower = add_borrower();
        request->next = NULL; // set the next pointer to NULL

        if (first == NULL)
        {
            first = request;
        }
        else
        {
            borrowedbooks *temp = first;
            first = request;
            first->next = temp;
        }
       Continuation:

        printf("Due date: ");
        printf("%ld", due_date());
        printf("\n");
        return request;
    }
    else
    {
        printf("Sorry, the book is not available right now.\n");
        return NULL;
    }
}


borrowed *add_borrower()
{
    
    borrowed* p = malloc(sizeof(borrowed));
    if (p == NULL) 
    {
        printf("Error: Failed to allocate memory\n");
        return NULL;
    }
    printf("Enter the borrower's details\n");
    printf("First Name: ");
    scanf("%49s", p->fname); // Use scanf() to read input without newline character
    printf("Last Name: ");
    scanf("%49s", p->lname);
    printf("Cellphone Number: ");
    scanf("%19s", p->number);
    printf("Address: ");
    scanf("%99s", p->address);
    printf("Email: ");
    scanf("%99s", p->email);
    time_t now = time(NULL);
    char* current_time = ctime(&now);
    current_time[strlen(current_time) - 1] = '\0';
    printf("Date Borrowed: %s\n", current_time);

    printf("\nBorrower Added\n");
    printf("%s %s\n", p->fname, p->lname);
    printf("%s\n", p->number);
    printf("%s\n", p->address);
    printf("%s\n", p->email);
    printf("%s\n", current_time);
    return p;
}

void return_book (char * name)

{
    printf("%s\n",first->book->name );
    borrowedbooks *p= first;

    while(p!=NULL)
    {
        if (strcasecmp(p->book->name, name)==0)
        {
            p->book->quantity++;
            free(p);
            printf("\033[1;32mBook returned successfully.\033[0m\n");
            return;
        }
        p=p->next;
    }
    printf("\033[1;31mBook not currently borrowed.\033[0m\n");
    return;
}

time_t current_time()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    size_t ret = strftime(s, sizeof(s), "%c", tm);
    assert(ret);
    printf("%s\n", s);
    return t;
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
        printf("The book is %.0f days overdue\a\n", days_overdue);
    } 
    else 
    {
        printf("There are %.0f days left until the due date\n", -days_overdue);
    }
}

void list_borrowed_books(borrowedbooks *firstB)
{
    if (firstB != NULL)
    {
        if (firstB->next != NULL)
        {
            list_borrowed_books(firstB->next);
        }
        printf("Name of book: %s\n", firstB->book->name);
        printf("Name of borrower: %s\n", firstB->borrower->fname);
    }
    else
    {
        printf("\033[1;34mNo borrowed books\033[0m\n");
    }
}