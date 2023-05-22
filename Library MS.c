//Omar Khaled 221027817
//Nada Ayman 221007645
//Roula Ashraf 221007542
//Rola Khaled 221017871
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

//calling prototypes
void load();
node* add_to_list(node* head, node* new_node);
void add_book();
borrowedbooks *borrow_book();
borrowed *add_borrower();
void overdue_book(time_t due_date);
void return_book(char *name);
unsigned int hash(char *department);
node *search_for_book (char *name, char*department);
void traverse ();
void save_file();
time_t current_time();
time_t due_date();
void list_borrowed_books (borrowedbooks *firstB);
void delete_book(char *name, char *department );

int main()
{
    int choice; //initializing a valriable for the user's choice
    char name[999]; //initializing variables for user input of the book name and department for function parameters
    char dep[999];
    borrowedbooks *bookborrowed; //initializing a pointer of type 'borrowedbooks'
    load(); //calling the function to open/create the file that will be saved to
    do 
    {   
        Beep(1000, 500); //alarm sound 
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
        scanf("%d", &choice); //scans the user's input 
        fgetc(stdin); //reads new line
        switch (choice) //switch cases for the choices avaliable in the system
        {
            case 1:
                add_book(); //call the add book function
                break;
            case 2:
                printf("\033[1;35mEnter the name of the book1:\033[0m\n"); //ask for the name of name of the book to delete
                fgets(name, sizeof(name), stdin); //scans the users input
                name[strlen(name)-1]=0; //remove the newline character
                printf("\033[1;35mEnter the name of the department1:\033[0m\n"); //asks for the department of the wanted book
                fgets(dep,sizeof(dep), stdin);
                dep[strlen(dep)-1]=0; 
                delete_book(name, dep); //calls the delete book function with the user input as wanted parameters
                break;
            case 3:
                borrow_book(); //calls the borrow book function
                break;
            case 4:
                printf("\033[1;35mEnter the name of the book1:\033[0m\n"); //asks for the of the wanted book
                fgets(name, sizeof(name), stdin); //reads the user's input
                name[strlen(name)-1]=0; //remove the newline character
                return_book(name); //calls the return book function with user input as wanted parametes
                break;
            case 5:
                overdue_book(due_date()); //calls the overdue function with the due date function as it's parameter
                break;
            case 6:
                printf("\033[1;35mEnter the name of the book\033[0m\n");
                fgets(name, sizeof(name), stdin);
                name[strlen(name)-1]=0;
                printf("\033[1;35mEnter the name of the department\033[0m\n");
                fgets(dep,sizeof(dep), stdin);
                dep[strlen(dep)-1]=0;
                node* p= search_for_book(name, dep); //initializes a pointer of type node to point at the result of the search for book function with the user input as the wanted paramerers
                if (p==NULL) //if the pointer is NULL
                {
                    printf("\033[1;31mBook not found\033[0m\n"); //then output that the book is not found
                }
                else
                {
                    printf("\033[1;32mBook found\033[0m\n"); //else print that the book is found
                }
                break;
            case 7:
                list_borrowed_books(first); //call the list of borrowed books function with the pointer 'first' as it's parameter
                break;
            case 0:
                printf("\033[1;35mExiting program...\033[0m\n"); //output that user is exiting the program
                break;
        }
    }
    while (choice!=0); //loops through the entire code until the user completes all their tasks and wants to save file
    traverse(); //calls traverse function
    save_file(); //calls save file funtion
    return 0;
}

void add_book() //function of type void to add books to list
 {
    //allocating memory for new book
    node* new_book = malloc(sizeof(node)); //initializing pointer called 'new_book' to point at structure 'book'
    if (new_book == NULL)
    {
        printf("\033[1;31mError: Failed to allocate memory\033[0m\n");
        return;
    }

    //asking user to enter book details
    printf("\033[1;35mEnter book details:\033[0m\n");
    printf("Name: ");
    fgets(new_book->book.name, sizeof(new_book->book.name), stdin); //reading the users input and storing it in the book name in the 'book' struct
    new_book->book.name[strlen(new_book->book.name)-1]=0; //remove the newline character
    printf("Author: ");
    fgets(new_book ->book.author,sizeof(new_book->book.author),stdin); //reading the users input and storing it in the book author in the 'book' struct
    new_book->book.author[strlen(new_book->book.author)-1]=0; //remove the newline character
    printf("Category: ");
    fgets(new_book->book.category, sizeof(new_book->book.category), stdin); //reading the users input and storing it in the book category in the 'book' struct
    new_book->book.category[strlen(new_book->book.category)-1]=0; //remove the newline character
    printf("department: ");
    fgets(new_book->book.department, sizeof(new_book->book.department), stdin); //reading the users input and storing it in the book department in the 'book' struct
    new_book->book.department[strlen(new_book->book.department)-1]=0; //remove the newline character 
    printf("Language: ");
    fgets(new_book->book.language, sizeof(new_book->book.language), stdin); //reading the users input and storing it in the book language in the 'book' struct
    new_book->book.language[strlen(new_book->book.language)-1]=0; //remove the newline character
    printf("Published Date: ");
    fgets(new_book->book.publisheddate, sizeof(new_book->book.publisheddate), stdin); //reading the users input and storing it in the book publish date in the 'book' struct
    new_book->book.publisheddate[strlen(new_book->book.publisheddate)-1]=0; //remove the newline character

    new_book->book.quantity = 0; //set the quantity of the new book to zero

    //if the new book is already in the list, increment the quantity of that book
    int bucket = hash(new_book->book.department); //initializing variable 'bucket' of type integer and giving it the value of the functionhash with the user input for department as the parameter
    node *p = malloc(sizeof(node)); //initializing pointer 'p' of type node and allocating it's memory
    if(p == NULL) //if pointer 'p' is pointing at NULL, then the code was unable to allocate memory
    {
        printf("\033[1;31mError, unable to allocate memory\033[0m\n");
        return;
    }

    p = search_for_book(new_book->book.name, new_book->book.department); //have pointer look at the result of search book function using the user input as parameters
    if (p != NULL) //if 'p' is ot NULL
    {
        p->book.quantity += 1; //then ahve 'p' look at 'quantity' in book strcutre and increase the quantity
        new_book->book.quantity=p->book.quantity; //have the book quantity of 'new_book' equal the book quantity of 'p'
        printf("\033[1;32mBook quantity increased.\033[0m\n"); //output that the quantity has increased
    }
    else
    {     
        free(p); //empy the memory allocated for 'p'
        if (ddc[bucket] == NULL) //if the ddc of bucked is empty
        {
            printf("\033[1;31mbuckets are empty\033[0m\n");
            ddc[bucket] = new_book; //set the new book as the first node in the bucket's linked list
            new_book->next = NULL; //have the pointer look at the next book and have it equal to NULL
            new_book->previous = NULL; //have the pointer look at the previous book and have it equal to NULL
            new_book->book.quantity = 1; //make the book quantity pointed at by 'new_book', equal to 1
        }
        else //if the bucket is not empty
        {  
            if(ddc[bucket]->previous==NULL) //if the bucket's linked list only has one node
            {
                ddc[bucket]->previous = new_book; //set the new book as the second node in the bucket's linked list
                ddc[bucket]->next = new_book; //have ddc of bucket looking at next equal to the new_book
                new_book->book.quantity = 1; //make the book quantity pointed at by 'new_book', equal to 1
            }
            else //if the bucket's linked list has more than one node
            {
                //insert the new book as the second-to-last node in the bucket's linked list
                node *temp = ddc[bucket]->previous; 
                ddc[bucket]->previous = new_book; //have the new book point to the previous node
                new_book->previous = temp; 
                temp->next = new_book; //have the previous node point to the new book
                new_book->book.quantity = 1; //increment quantity, pointed at by 'new_book', by 1
            }
        }
    }

    //output that the book has been added and output the book detailed entered
    printf("\n\033[1;32m Book Added\033[0m\n");
    printf("%s\n", new_book->book.name);
    printf("%s\n", new_book->book.author);
    printf("%s\n", new_book->book.category);
    printf("%s\n", new_book->book.department);
    printf("%s\n", new_book->book.language);
    printf("%s\n", new_book->book.publisheddate);
    printf("%d\n", new_book->book.quantity);
    bookcount++; //increase the total book count
}

unsigned int hash (char *department) //hash function that hashes a code to ASCII value
{
    unsigned int hash = 0; //initializing variable 'hash' of type unsigned initger and give it the value 0
    for (int i = 0; department[i] != '\0'; i++) //loop for the duration of 'i' of department not equal to 0 
    {
        hash = hash * 31 + tolower(department[i]); //calculate the value of hash, and have the department converted to lowercase
    }
    return (hash % 9); //return the calculated hash value divided by 9
}

node *search_for_book (char *name, char*department) //function of type node to search for a book in the list using its name and department as parameters
{
    printf("%s\n", name);
    printf("%s\n", department);
    int bucket = hash(department); //initialize a variable bucket and have it equal to the hash of the deparment 
    node *d = ddc[bucket]; //initialize a pointer of type node and have it point to the first node in the list at the bucket
    while (d != NULL) //while pointer 'd' does not equal to NULL
    {
        if(strcasecmp(name, d->book.name) == 0)
        {
            return d; //return the value of d if the book is found in the list
        }
        d = d->next; //move to the next node in the list
    }
    return NULL; // Return NULL if the book is not found in the linked list
}

void traverse() //function of type void to output all the information of the books in the library
{
    int count; //initializes variable 'count' of type integer to keep track of all the books in each bucket
    int total = 0; //initializes variable of type integer to keep track of all the books in all the buckets combined
    for (int i = 0; i < 9; i++) //loops through each bucket
    {
        printf("bucket: %d\n", i);
        node *p = ddc[i]; // initializes pointer of type node to the fist node in the current bucket
        count = 0; //resets the count for each bucket
        while (p != NULL) //while 'p' is not NULL, print the details of each book
        {
            printf("Book #%d  Bookname: %s  Department: %s\n", count+1, p->book.name, p->book.department);
            p = p->next; //move to the next book in the list
            count++; //increment the count by 1
            total++; //increment the total by 1
        }
    }
    //output the total books traversed and the true total number of books
    printf("Total Traversed: %d\n", total); 
    printf("True Total: %d\n", bookcount); 
    return;
}

void load() //function to open file or create file to write data to it
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
            return; //exit function
        }
        fclose(f); //close the file
        f = fopen("books.bin","rb"); //if the file was created, then have pointer 'f' point to file
        if (f == NULL) //if pointer is pointing at NULL
        {
            printf("Unable to open file\n"); //then return 1 and print that file cannot be opened
            return;
        }

    }
    while(!feof(f)) //while it is not the end of the file
    {
        node* new_book = malloc(sizeof(node)); //initialize pointer of type node and allocate its memory
        if (new_book == NULL) //if 'new_book' is NULL the memory was not allocated
        {
            printf("Error: Failed to allocate memory\n");
            return; //exit function
        }

        if (fread(&new_book->book, sizeof(books), 1, f) != 1) //if the details of the fle cannot be read
        {
            free(new_book); //free the allocated memory for pointer 'new_book'
            break;
        }

        new_book->next = NULL; //initialize the next pointer of node 'new_book'
        new_book->previous = NULL; //initialize the next pointer of node 'new_book'

        int bucket = hash(new_book->book.department); //calculate the bucket for the new list based on its department
        ddc[bucket] = add_to_list(ddc[bucket], new_book); //add the book to the list
        bookcount++; //increment book list by 1
    }

    fclose(f); //close the file
    printf("\033[0;31mData loaded successfully from binary file.\033[0m\n"); //output that the data was successfully loaded
}

void save_file() //function of type void to save library data to it
{
    FILE* fp; //declare file pointer 'fp'

    fp = fopen("books.bin", "wb"); //open the binary file to write

    if (fp == NULL) //if the file is unable to open, output an error message
    {
        printf("\033[1;31m404. cannot open file\033[0m\n");
        return; //exit the function
    }
    for(int i=0;i<9;i++) //iterate through each bucket
    {
        node *temp= ddc[i]; //initialize a temp pointer of type node and have it point to the first book of the list in the current bucket
        while (temp!=NULL) //loop through the current bucket
        {
            fwrite(&(temp->book),sizeof(books),1,fp); //write the book details to the binary file
            temp=temp->next; //move to the next book in the list
        }
    }

    fclose(fp); //close the file
    return;
}

node* add_to_list(node* head, node* new_node) //function of type node to sort nodes for load() function
{
    if (head == NULL) //if the list is empty
    {
        return new_node; //make 'new_node' the head of the list
    }

    node* current = head; //have the link list find the last node
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = new_node; //add the new node to the end of the list
    new_node->previous = current; //update the previous pointer of the new node

    return head; //return the head of the link list
}

void delete_book(char *name, char *department) //function of type void to delete a book from the list
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

borrowedbooks *borrow_book() //function of type borrowedbooks to borrow a book
{
    char name[100]; //initialize string array for user to enter name of book and department
    char department[100];
    printf("Enter book name to borrow: ");
    fgets(name, sizeof(name), stdin); //read the user input
    name[strcspn(name, "\n")] = '\0'; // remove the newline character
    printf("Enter department: ");
    fgets(department, sizeof(department), stdin);
    department[strcspn(department, "\n")] = '\0'; // remove the newline character

    node *p = search_for_book(name, department); //initialize pointer of type node to search for the wanted book
    if (p == NULL) //
    {
        printf("Sorry, the book is not available right now.\n");
        return NULL;
    }
    else if (p->book.quantity > 0) // check if book is available
    {
        p->book.quantity--; //decrement the book quantity
        printf("%s book is available!\n", name); //print that the book is available
        borrowedbooks *request = malloc(sizeof(borrowedbooks)); //create a bew borrowed books node and add it to the borrowed books list
        request->book = &(p->book);
        request->borrower = add_borrower();
        request->next = NULL; // set the next pointer to NULL

        if (first == NULL) //ass the borrowed book to the list of borrowed books
        {
            first = request;
        }
        else
        {
            borrowedbooks *temp = first;
            first = request;
            first->next = temp;
        }

        //print the due date
        printf("Due date: ");
        printf("%ld", due_date());
        printf("\n");
        return request; //return the borrowed book
    }
    else //print a message indicating that the book is not available
    {
        printf("Sorry, the book is not available right now.\n");
        return NULL;
    }
}

borrowed *add_borrower() //function of type borrowed to add borrower
{
    
    borrowed* p = malloc(sizeof(borrowed)); //initialize pointer of type borrowed and allocate memory
    if (p == NULL) //if memiry was unable to be allocated, output and error message
    {
        printf("Error: Failed to allocate memory\n");
        return NULL;
    }
    //promt user to enter the borrower's details
    printf("Enter the borrower's details\n");
    printf("First Name: ");
    scanf("%49s", p->fname); //use scanf() to read input without newline character
    printf("Last Name: ");
    scanf("%49s", p->lname);
    printf("Cellphone Number: ");
    scanf("%19s", p->number);
    printf("Address: ");
    scanf("%99s", p->address);
    printf("Email: ");
    scanf("%99s", p->email);
    time_t now = time(NULL); //get the current time and format it as a string
    char* current_time = ctime(&now);
    current_time[strlen(current_time) - 1] = '\0';
    printf("Date Borrowed: %s\n", current_time);

    //print the entered borrower's details and the time they borrowed the book
    printf("\nBorrower Added\n");
    printf("%s %s\n", p->fname, p->lname);
    printf("%s\n", p->number);
    printf("%s\n", p->address);
    printf("%s\n", p->email);
    printf("%s\n", current_time);
    return p; //return the pointer to the new borrower
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

time_t current_time() //function of type time to calculate current time
{
    time_t t = time(NULL); //get the current time
    struct tm *tm = localtime(&t); //convert the current time to the local time (Egypt)
    char s[64]; //create a character array to store the formatted time
    size_t ret = strftime(s, sizeof(s), "%c", tm); //format the local time to string and save it in the array
    assert(ret); //check if the formatted time string was created successfully
    printf("%s\n", s); //print the formatted time string
    return t;
}

time_t due_date() //function of type time to calculate due date of book
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

void overdue_book(time_t due_date) //function of type void to check if book is overdue
{
    time_t current_time = time(NULL); //get current time
    //calculate the number of days left tille due date
    double days_overdue = difftime(current_time, due_date) / (24 * 60 * 60);
    if (days_overdue > 0) //if the book is overdue, print by how much and sound an alarm
    {
        Beep(1000, 500); //alarm sound 
        printf("The book is %.0f days overdue\a\n", days_overdue);
    } 
    else //else print how long left till due date
    {
        printf("There are %.0f days left until the due date\n", -days_overdue);
    }
}

void list_borrowed_books(borrowedbooks *firstB) //function of type void to print list of borrowed books and who borrowed them
{
    if (firstB != NULL) //iterate while the borrowed booklist is not empty
    {
        if (firstB->next != NULL)
        {
            list_borrowed_books(firstB->next);
        }
        printf("Name of book: %s\n", firstB->book->name); //output the name of the book
        printf("Name of borrower: %s %s\n", firstB->borrower->fname, firstB->borrower->lname); //output the name of the borrower
    }
    else //if the borrowed booklist is empty, output a message indicating that there aren't any books borrowed
    {
        printf("\033[1;34mNo borrowed books\033[0m\n");
    }
}
