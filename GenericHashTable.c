#include "GenericHashTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int checksForTable(int size, int dType, int listLength)
{
    // The function checks input variables for the create table function.
    //return -1 if the one of the variables is wrong and 0 if everything is ok.
    if (size <= 0)
    {
        fprintf(stderr, "Invallid value for size \n");
        return 1;
    }
    if (dType != INT_TYPE && dType != STR_TYPE)
    {
        fprintf(stderr, "Invalid value for type \n");
        return 1;
    }
    if (listLength <= 0)
    {
        fprintf(stderr, "Invalid value for list length \n");
        return 1;
    }
    else
        return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Table *createTable(int size, int dType, int listLength)
{
    int check = checksForTable(size, dType, listLength);
    if (check == 1)
        return NULL;

    Table *table = (Table *)malloc(sizeof(Table)); // pointer to the table
    if (table == NULL)                             //check if allocated
    {
        fprintf(stderr, "Couldn't allocate memory for the table \n");
        return NULL;
    }
    table->arr = (Object **)malloc(size * (sizeof(Object *))); //pointer to the table array
    if (table->arr == NULL)                                    //check if allocated
    {
        fprintf(stderr, "Couldn't allocate memory for the table array \n");
        exit(1);
    }

    // assign attributes
    table->M = size;
    table->N = size;
    table->D = table->N / table->M;
    table->t = listLength;
    table->type = dType;

    for (int i = 0; i < size; i++) // each cell in the array will point NULL
        table->arr[i] = NULL;

    return table;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void printTable(Table *table)
{ // The function print the table (the format is in the exe definition)
    if (table == NULL)
    {
        fprintf(stderr, "Invalid value for table variable. \n");
        return;
    }
    else
    {
        if (table == NULL)
            fprintf(stderr, "Table pointer is null");
        for (int i = 0; i < table->N; i++)
        {
            printf("[%d]", i); // print the index
            Object *temp = table->arr[i];

            for (; temp != NULL; temp = temp->next)
            {
                if (table->type == INT_TYPE) // if the table is int table
                    printf("\t%d\t-->", *(int *)temp->data);
                else // if the table is string table
                    printf("\t%s\t-->", (char *)temp->data);
            }

            printf("\n");
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int isEqual(int type, void *data1, void *data2)
{
    /**
* check the equality of the data of two objects. The implementation is different depending the type of the data.
* the function returns 0 if they are equal or some other value if they are not equal.
*/
    if (type != 0 && type != 1)
    {
        fprintf(stderr, "Invalid value for type. \n");
        return 1;
    }
    if (data1 == NULL || data2 == NULL)
    {
        fprintf(stderr, "Invalid value for data. \n");
        return 1;
    }
    if (type == INT_TYPE)
    { // if its int
        if (*(int *)data1 == *(int *)data2)
            return 0;
        else
            return 1;
    }
    else
        return (strcmp((char *)(data1), (char *)(data2)));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int intHashFun(int *key, int origSize)
{
    if (key == NULL)
    {
        fprintf(stderr, "Invalid value for key. \n");
        return -1;
    }
    if (origSize < 0)
    {
        fprintf(stderr, "Invalid value for size. \n");
        return -1;
    }
    int ans = *(key) % origSize;

    //* returns the hash value of an integer, which is key mod origSize
    return ans < 0 ? ans + origSize : ans;
    

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int strHashFun(char *key, int origSize)
{
    //returns the hash value of an string, which is m mod origSize, where m is the sum of the ascii value of all the
    //character in key.
    if (key == NULL)
    {
        fprintf(stderr, "Invalid value for key. \n");
        return -1;
    }
    if (origSize < 0)
    {
        fprintf(stderr, "Invalid value for size. \n");
        return -1;
    }
    int len = strlen(key);
    int count = 0;
    for (int i = 0; i < len; i++)
        count += key[i];
    return count % origSize;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Object *lastObjInList(Table *table, int idx)
{
    // this function returns the last object of the linkes list in the specific index in the table.
    // check input validation
    if (table == NULL)
    {
        fprintf(stderr, "Invalid value for table. \n");
        return NULL;
    }
    if (idx < 0)
    {
        fprintf(stderr, "Invalid value for index. \n");
        return NULL;
    }

    Object *before = table->arr[idx];
    Object *cur = table->arr[idx];
    if (cur == NULL)
    {
        // if there are no elements in the array
        return NULL;
    }
    if (cur->next == NULL)
    {
        // if there is one elements in the array
        table->arr[idx] = NULL;
        return cur;
    }
    for (int i = 0; i < table->t && cur != NULL; i++)
    { // if there are elements in the array, returns the last one and change the element before to point to null now
        if (cur->next == NULL)
        {
            before->next = NULL;
            return cur;
        }
        before = cur;
        cur = cur->next;
    }
    return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void freeTable(Table *table)
{
    if (table == NULL)
    {
        fprintf(stderr, "Invalid value for table variable.");
        return;
    }
    else
    {
        //The function release all the allocated members of struct Table.
        for (int i = 0; i < table->N; i++)
        {
            Object *temp = table->arr[i];
            while (temp != NULL)
            {
                //free all the objects in the array
                temp = lastObjInList(table, i);
                if (temp != NULL)
                    freeObject(temp, INT_TYPE);
            }
            free(table->arr[i]); // free the pointer of the list
        }
        free(table->arr); // free the pointer of the array
        free(table);      // free the pointer of the table
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Object *createObject(void *data)
{
    if (data == NULL)
    {
        fprintf(stderr, "Invalid value for data. \n");
        return NULL;
    }

    Object *obj = (Object *)malloc(sizeof(Object));
    if (obj == NULL)
        return NULL;

    obj->data = data;
    obj->next = NULL;
    obj->index = -1;
    return obj;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void freeObject(Object *obj, int type)
{ // This function frees an object, the data of the object should also be freed.
    if (obj == NULL)
    {
        fprintf(stderr, "Invalid value for table. \n");
        return;
    }
    else
    {
        free(obj->data);
        free(obj);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Object *objectByType(Table *table, void *data)
{
    // this function returns an object that its data is casting according the type of the table
    Object *obj;

    if (table->type == INT_TYPE)
    {                                              // if the type is int:
        int *new_ptr = (int *)malloc(sizeof(int)); //allocate memory for int pointer
        if (new_ptr == NULL)
            return NULL;
        *new_ptr = *((int *)data);
        obj = createObject(new_ptr);
    }
    else
    { // if the type is str:
        //allocate memory for the length of the string, the +1 is for the char /0
        char *new_ptr = (char *)malloc(1 + sizeof(char) * strlen((char *)data));
        if (new_ptr == NULL)
            return NULL;
        strcpy((char *)new_ptr, (char *)data); // copy the string to the pointer
        obj = createObject(new_ptr);
    }

    return obj;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int addToEnd(Table *table, Object *obj, int idx)
{
    // this function add an object to the end of the list in the specific index of the table
    if (table->arr[idx] == NULL)
    { // if its the first element in the list
        obj->index = idx;
        table->arr[idx] = obj;
        return idx;
    }
    int item_counter = 0; // count the number of objects in the list until reaching t
    Object *temp = table->arr[idx];
    for (; item_counter < table->t - 1; item_counter++)
    { // searching for a free place for the object in the list, check that it doesn't insert more than t elements to the list
        if (temp->next == NULL)
        {
            obj->index = idx;
            temp->next = obj;
            obj->next = NULL;
            return idx;
        }
        else
            temp = temp->next;
    }
    return -1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int findAPlaceInLists(Table *table, Object *obj, int idx)
{
    // this function receives an object and an index and insert the object to one of the list that are in
    // the scope. the scope is from idx to (idx+table->D -1) lists.
    int temp_idx = 0;
    int lists_counter = 0; // count the number of lists (idx) we can search for place until D
    int insert_to_list = 0;

    temp_idx = idx;
    for (; lists_counter < table->D; lists_counter++, temp_idx++)
    {
        //searching room in the other lists in the scope
        insert_to_list = addToEnd(table, obj, temp_idx);
        if (insert_to_list != -1)
            return insert_to_list;
    }
    return -1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void updateArray(Table *table, int idx)
{ // this function update some values in case of multipling the table and the idx pointer is not null
    //the vlues updates: the pointer of the array, the indexes of the objects

    // if there is a list, move it to the new index in the array
    table->arr[2 * idx] = table->arr[idx];
    Object *temp = table->arr[2 * idx];
    for (int j = 0; j < table->t && temp != NULL; j++)
    {
        // updating the index value of the objects in the list
        temp->index = temp->index * 2;
        temp = temp->next;
    }
    if (idx != 0) // the old index is "free" to recieve new list in the next time
        table->arr[idx] = NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int multiply_table(Table *table)
{ // this function multiply the size of the array and transfer the existed lists to their new indexs
    table->arr = realloc(table->arr, (table->N * 2 * sizeof(Object *)));
    if (table->arr == NULL)
    {
        fprintf(stderr, "The array realloc failed. \n");
        return -1;
    }

    for (int i = (table->N - 1); i >= 0; i--)
    {
        if (table->arr[i] != NULL)
        {
            updateArray(table, i);
        }
        else
            table->arr[2 * i] = NULL;
        table->arr[2 * i + 1] = NULL; // set the new indexes to null
    }

    // updating variables
    table->N = 2 * table->N;
    table->D = table->N / table->M;

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int add(Table *table, void *data)
{
    /**
* The function adds data to the hashtable (as described in the exe definition)
* On success, the function returns the array index of the added data, otherwise, it return -1.
*/
    if (table == NULL)
    {
        fprintf(stderr, "Invalid value for table. \n");
        return -1;
    }
    if (data == NULL)
    {
        fprintf(stderr, "Invalid value for data. \n");
        return -1;
    }
    int idx;
    int insert_to_list = 0;
    Object *obj = objectByType(table, data); // the pointer to the object based on the type

    //the hash function returns the index in the table
    if (table->type == INT_TYPE)
        idx = table->D * intHashFun((int *)obj->data, table->M);
    else
        idx = table->D * strHashFun((char *)obj->data, table->M);

    // try to insert the object to the lists in the scope of the index
    insert_to_list = findAPlaceInLists(table, obj, idx);

    if (insert_to_list == -1)
    {
        // need to multiply the table
        int multiply_succeed = multiply_table(table);
        if (multiply_succeed == -1)
            return -1;
        idx = idx * 2;                                       // updating the index
        insert_to_list = findAPlaceInLists(table, obj, idx); //insert to one of the new lists in the scope
    }
    return insert_to_list;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Object *search(Table *table, void *data)
{
    /**
* The function search for an object that its data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
*/
    if (data == NULL)
    {
        fprintf(stderr, "Invalid value for data variable. \n ");
        return NULL;
    }
    if (table == NULL)
    {
        fprintf(stderr, "Invalid value for table variable. \n ");
        return NULL;
    }
    int idx = 0;
    Object *temp;
    // finding the start index to search in
    if (table->type == INT_TYPE)
        idx = table->D * intHashFun((int *)data, table->M);
    else
        idx = table->D * strHashFun((char *)data, table->M);

    if (table->arr[idx] == NULL) // if the allocation failed
        return NULL;
    for (int i = 0; i < table->D; i++)
    {
        // the outer loop is for the scope of the lists
        temp = table->arr[idx];
        for (int j = 0; j < table->t && temp != NULL; j++)
        {
            // the inner loop is for the number of elements that can be in the list
            if (isEqual(table->type, temp->data, data) == 0)
                return temp;
            else
                temp = temp->next;
        }
        idx++;
    }
    return NULL;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int removeObj(Table *table, void *data)
{
    /**
* The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
* On success, the function returns the array index of the removed data, otherwise, it returns -1.
* -1 is also returned in the case where there is no such object.
*/
    if (data == NULL)
    {
        fprintf(stderr, "Invalid value for data variable. \n ");
        return -1;
    }
    if (table == NULL)
    {
        fprintf(stderr, "Invalid value for table variable. \n ");
        return -1;
    }
    Object *toRemove = search(table, data); // finding the object in the table
    if (toRemove == NULL)
        return -1;
    int idx = toRemove->index;
    Object *before = table->arr[idx];
    if (isEqual(table->type, before->data, data) == 0)
    {
        // if its the first element of the list
        table->arr[idx] = toRemove->next;
        freeObject(toRemove, INT_TYPE);
        return idx;
    }
    else
    { // searcing the element in the other elements in the list
        for (int i = 0; i < table->t && before != NULL && before->next != NULL; i++)
        {
            if (isEqual(table->type, before->next->data, data) == 0)
            {
                before->next = toRemove->next;
                freeObject(toRemove, INT_TYPE);
            }
            before = before->next;
        }
    }
    return -1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
