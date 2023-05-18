#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

const int length = 999;
int bookcount = 0;

typedef struct books
{
    char * name[1000];
    char * author[30] ;
    char * category[20];
    char * department[20];
    char * language[20];
    char * publisheddate[1000];
    int quantity;
} books;

typedef struct node
{
    books book ;
    struct node * next;
    struct node * previous;
} node;

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
void addbook();
void add_borrower();

int main()
{
    // if (load() == 1)
    // {
    //     return 0;
    // }
    // addbook();
    add_borrower();
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
        node * p = malloc(sizeof(node));
        if (p == NULL)
        {
            return 1;
        }
       if (fread(p, sizeof(node),1, f) != 1)
       {
        free (p);
        break;
       }
       free (p);
    bookcount++;
    }
    fclose(f);
    return 0;
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
    scanf("%s", &new_borrower.dateborrowed);
    printf("Borrower Added");
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