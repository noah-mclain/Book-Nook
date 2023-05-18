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
}books;

typedef struct node
{
    books book ;
    struct node * next;
    struct node * previous;
} node;

node * ddc[9];

typedef struct borrowed 
{
    char * fname[100];
    char * lname[100];
    int cellnumber;
    int homenumber;
    char * address[100];
    char * email[100];
    char * dateborrowed[100];
} borrowed;

int load();
char current_time();
void addbook();
void add_borrower();
char due_date();
unsigned int hash(char*department);
node * search_for_book (char *name, char*department);

int main()
{
    // if (load() == 1)
    // {
    //     return 0;
    // }
    // addbook();
    current_time();
    due_date();
}

// int load()
// {
//     FILE * f;
//     f = fopen("filename", "br");
//     if (f == NULL)
//     {
//         printf("Unable to open file\n");
//         return 1;
//     }

//     while(true)
//     {
//         node * p = malloc(sizeof(node));
//         if (p == NULL)
//         {
//             return 1;
//         }
//        if (fread(p, sizeof(node),1, f) != 1)
//        {
//         free (p);
//         break;
//        }
//        free (p);
//     bookcount++;
//     }
//     fclose(f);
//     return 0;
// }

// unsigned int hash (char* department)
// {
//     unsigned int hash = 0;
//     for (int i = 0; department[i] != '\0' ; i++)
//     {
//         hash = hash * 31 + department[i];
//     }
//     return (hash % 9);
// }

// void add_borrower()
// {
//     borrowed new_borrower;
//     printf( "Enter the borrower's details\n");
//     printf("First Name: ");
//     gets(new_borrower.fname);
//     printf("Last Name: ");
//     gets(new_borrower.lname);
//     printf("Cellphone Number: ");
//     scanf("%d", &new_borrower.cellnumber);
//     printf("Home Number: ");
//     scanf("%d", &new_borrower.homenumber);
//     printf("Address: ");
//     scanf("%s", &new_borrower.address);
//     printf("Email: ");
//     scanf("%s", &new_borrower.email);
//     printf("Date Borrowed: ");
//     current_time();
//     printf("\nBorrower Added");
// }

// void addbook() 
// {
//     books newb;
//     printf("Enter book details:\n");
//     printf("Name: ");
//     gets(newb.name);
//     printf("\nAuthor: ");
//     gets(newb.author);
//     printf("\nCategory: ");
//     gets(newb.category);
//     printf("\ndepartment: ");
//     gets(newb.department);
//     printf("\nPublished date: ");
//     gets(newb.language);
//     printf("\nLanguage: ");
//     gets(newb.publisheddate);
//     printf("\nQuantity: ");
//     scanf("%d", &newb.quantity);
//     printf("Book Added");
// }

// void borrow_book(char *name, char*department) 
// {
//     char  borrower_name[50];
//     printf("Enter book name to borrow: ");
//     scanf("%s", name);
//     printf("Enter department: ");
//     scanf("%s", department);
//     printf("Enter borrower name: ");
//     scanf("%s", borrower_name);
//     node* p = search_for_book( name,  department );
//             if ((p-> book.quantity) > 0)
//             {
//                 p->book.quantity--;
//                 printf("%s borrowed successfully.\n", name);
//                 printf("Due date: %d/%d/%d\n", rand() % 30 + 1, rand() % 12 + 1, 2022);
//                 return;
//             }
//             else 
//             {
//                 printf("Sorry, the book is not available right now.\n");
//                 return;
//             }
// }

// node * search_for_book (char *name, char*department)
// {
//     int bucket =hash(department);
//     node *d = ddc[bucket];
//     while (d != NULL)
//     {
//         if(strcmp(name,d -> book.name) == 0)
//         {
//             return d;
//         }
//         d = d -> next;
//     }
//     return NULL;

// }

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

char due_date()
{
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    now->tm_mday += 14; // Add 14 days to the current day
    mktime(now); // Normalize the struct tm
    printf("%02d/%02d/%02d %02d:%02d:%02d\n", now->tm_mday, now->tm_mon + 1, now->tm_year % 100, now->tm_hour, now->tm_min, now->tm_sec);
}

// void append_file(char *filename, void *action, size_t data_size)
// {
//     FILE *file = fopen(filename, "ab");
//     if(!file)
//     {
//         printf("File could not be opened for appending\n");
//         return 0;
//     }
//     fwrite(data, data_size, 1, file);
//     fclose(file);
// }

