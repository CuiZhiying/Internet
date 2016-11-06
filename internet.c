/*
 * =====================================================================================
 *
 *       Filename:  internet.c
 *
 *    Description:  距离路由算法
 *
 *        Version:  1.0
 *        Created:  06/02/2016 02:09:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  叶孤城
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define Max 1000
typedef struct Node
{   int destination;
    int hop;
    double cost;
    struct Node *next;
}Node;

typedef struct Router
{   Node *list;
    Node *old_list;
    int name;
    char *information;
}Router;

int **get_matrix( int ); //get input juzhen
void display_matrix( int **matrix, int size );
int initial( int **matrix, Router *router, int size );
void display_list( Router *router, int size);
int update_oldlist( Router *router, int size);
int change_list( int **Matrix, Router *router, int size);

int main()
{   int **input;
    int i,j;
    int n;
    char sign;
    int count;
    FILE *in;
    in = fopen("input.txt","rb");
    Router *router;
    //freopen("input.txt", "r", stdin);
    printf("Please enter the size of matrix: \n");
    fscanf( in, "%d", &n);
    input = get_matrix(n);
    router = (Router *)malloc( n * sizeof(Router) );
    for(i = 0; i < n; i++)
        for( j = 0; j < n; j++)
            fscanf(in,"%d", &input[i][j]);
    fclose(in);
    display_matrix(input, n);
    initial( input, router, n);
    printf("\ninitial successful!\n");
    //display_list( router, n);
    update_oldlist( router, n);
    display_list( router, n);
    //freopen("CON", "r", stdin);
    count = 0;
    while( (sign = getchar()) != EOF && sign == '\n' ){
        printf("交换第%d次\n", ++count);
        //printf("\noriginal:\n");
        change_list( input, router, n);
        //printf("I has changed %d!\n", ++count);
        display_list( router, n);
        printf("\n\n");
        
        if( !update_oldlist( router, n)){
            printf("网络中所有的路由器的路由表都已经稳定了\n");
            break;
        };
        //display_list( router, n);
        //printf("\n\n");
    }
    return 0;
}

int **get_matrix( int n)
{   int **result;
    int i;
    result = (int **) malloc ( n * sizeof(int *));
    for( i = 0; i < n; i++)
        result[i] = (int *)malloc( n * sizeof(int) );
    return result;
}

void display_matrix( int **matrix, int size )
{   int i;
    int j;
    for( i = 0; i < size; i++ ){
        for( j = 0; j < size; j++)
            printf("%d\t", matrix[i][j]);
        putchar('\n');
    }
}

void display_list( Router *router, int size)
{   int i;
    Node *temp;
    printf("router\t destination:next:cost\n");
    for( i = 0; i < size; i++){
        //temp = router[i].list;
        temp = router[i].old_list;
        printf("old:%c\t", router[i].name + 65);
        while( temp ){
            printf("%c:%c:%.0lf-> ", temp->destination+65, temp->hop+65, temp->cost);
            temp = temp->next;
        }
        printf("End\n");
        temp = router[i].list;
        printf("new:%c\t", router[i].name + 65);
        while( temp ){
            printf("%c:%c:%.0lf-> ", temp->destination+65, temp->hop+65, temp->cost);
            temp = temp->next;
        }
        printf("End\n");
    }
}
int initial(int **matrix, Router *router, int size)
{    int i,j;
     Node *new;
     Node *temp;
     for( i = 0; i < size; i++){
         router[i].list = NULL;
         router[i].old_list = NULL;
         router[i].name = i;
         //sprintf( router[i].information,"Router %d\\0", i);
         //router[i].information = i
     }
     for( i = 0; i < size; i++){
         for( j = 0; j < size; j++){
             if(matrix[i][j] > 0 && matrix[i][j] < Max){
                 new = (Node *)malloc(sizeof(Node));
                 new->cost = matrix[i][j];
                 new->destination = j;
                 new->hop = j;
                 new->next = NULL;
                 temp = router[i].list;
                 while( temp != NULL && temp->next != NULL)
                     temp = temp->next;
                 if( temp == NULL )
                     router[i].list = new;
                 else
                     temp->next = new;
                 }
         }
     }
     new = NULL;
     return 0;
}

int update_oldlist( Router *router, int size)
{   int i,j;
    int change;
    Node *new;
    Node *temp;
    Node *temp_old;
    Node *p;
    change = 0;
    for( i = 0; i < size; i++){
        temp = router[i].list;
        temp_old = router[i].old_list;
        if( temp_old == NULL && temp == NULL )
            continue;
        if( temp_old == NULL && temp != NULL ){
            while( temp != NULL ){
                new = (Node *)malloc( sizeof(Node) );
                new->destination = temp->destination;
                new->hop = temp->hop;
                new->cost = temp->cost;
                new->next = NULL;
                if( router[i].old_list == NULL )
                    router[i].old_list = new; 
                else
                    temp_old->next = new;
                temp = temp->next;
                temp_old = new;
            }
            continue;
        }else{
            while( temp != NULL ){
                if( temp_old != NULL && temp->destination == temp_old->destination ){
                    //printf("I just update!\n");
                    if( temp->hop != temp_old->hop || temp->cost != temp_old->cost ){
                        change = 1;
                        temp_old->hop = temp->hop;
                        temp_old->cost = temp->cost;
                    }
                }else{
                    //printf("I need new one!\n");
                    change = 1;
                    new = ( Node * )malloc( sizeof(Node) );
                    new->destination = temp->destination;
                    new->hop = temp->hop;
                    new->cost = temp->cost;
                    new->next = NULL;
                    p->next = new;
                    //printf("I got new one\n");
                    temp_old = new;
                }
                p = temp_old;
                temp_old = temp_old->next;
                temp = temp->next;
            }
        }
        
    }
    new = NULL;
    temp = NULL;
    temp_old = NULL;
    p = NULL;

    return change;
}

int change_list( int **Matrix, Router *router, int size )
{    int i,j;
     double neibo_cost;
     Node *i_temp;
     Node *j_temp;
     Node *new;
     Node *p;
     for( i = 0; i < size; i++)
         for( j = 0; j < size; j++){
            if( Matrix[i][j] > 0 && Matrix[i][j] < 1000 ){
                i_temp = router[i].old_list;
                j_temp = router[j].list;
                p = j_temp;
                //printf("ddd\n");
                //if(p == NULL)
                while ( p && p->destination != i ){
                    //printf("%c\n",p->destination+65);
                    p = p->next;                          //find out the cost to neighbohood!
                }
                neibo_cost = p->cost;
                //printf("%lf\n", p->cost);
                while( i_temp != NULL ){
                    if( i_temp->destination == j ){       //kick out the one to himself
                        i_temp = i_temp->next;
                        continue;
                    }
                    j_temp = router[j].list;
                    while( j_temp->next && j_temp->destination != i_temp->destination ){
                        //printf("%c\t", i_temp->destination+65);
                        j_temp = j_temp->next;
                    }
                    //printf("%c:%c\t", j_temp->destination+65, i_temp->destination+65);
                    if( j_temp->destination == i_temp->destination ){
                        //printf("I'm update!\n");
                        if( j_temp->cost > i_temp->cost + neibo_cost ){
                            j_temp->cost = i_temp->cost + neibo_cost;
                            j_temp->hop = i;
                        }
                    }else{
                        //printf("I'm added!\n");  
                        new = (Node *)malloc( sizeof( Node ) );
                        new->destination = i_temp->destination;
                        new->cost = i_temp->cost + neibo_cost;
                        new->hop = i;
                        new->next = NULL;
                        j_temp->next = new;
                    }
                    i_temp = i_temp->next;
                }
            }
         }
    return 0;
}





































