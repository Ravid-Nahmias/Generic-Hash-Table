
#include "GenericHashTable.h"
#include <stdio.h>
#include <string.h>
int main()
{

    Table *table = createTable(4, INT_TYPE, 3);
    int x = 0;
    int y = -4;
    int z = 8;
    printf("%d\n", add(table, &x));
    printf("%d\n", add(table, &y));
    printf("%d\n", add(table, &z));
    printf("%d\n", add(table, &x));
    printf("%d\n", add(table, &y));
    printf("%d\n", add(table, &z));
    printf("-------BEFORE REMOVE-----\n");
    printTable(table);

    printf("-------REMOVE HEAD-----\n");
    removeObj(table, &x);
    printTable(table);
    printf("%d\n", add(table, &x));
    printf("-------ADD X=0 again-----\n");
    printTable(table);

    printf("-------REMOVE MIDDLE-----\n");
    removeObj(table, &z);
    printTable(table);
    printf("%d\n", add(table, &z));
    printf("-------ADD Z=8 again-----\n");
    printTable(table);

    printf("-------REMOVE END-----\n");
    removeObj(table, &z);
    printTable(table);
    printf("%d\n", add(table, &z));
    printf("-------ADD Z=8 again-----\n");
    printTable(table);

    printf("\n");
    Table *t = NULL;
    int *p = NULL;
    printf("-------REMOVE CHECK NULL-----\n");

    printf("**table is NULL\n");
    removeObj(t, &z);
    printf("**data is NULL\n");
    removeObj(table, p);

    freeTable(table);
}