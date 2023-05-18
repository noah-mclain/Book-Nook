#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
}
books;

typedef struct node
{
    books book ;
    struct node * next;
    struct node * previous;
}
node;

int load();


int main()
{
    if (load() == 1)
    {
        return 0;
    }
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

void addbook(books)
{
    if (load() == 1)
    {
        return 0;
    }
    else
    {
        do
        {
            printf( "Enter the book details\n" );
            
            
            printf ("Add another entity? y,n \n");
            fflush(stdin);
            scanf("%c",&exit);
        }  while ( exit=='y');
    }
}