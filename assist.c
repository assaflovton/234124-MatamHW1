#define _CRT_SECURE_NO_WARNINGS
#include "assist.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#define END_OF_STRING '\0'
#define BASE_TEN 10

void destroyString(char *str);
char *createString(int length);
int addVotes(int votes, int votes_to_add);
int removeVotes(int votes, int votes_to_remove);
char *intToString(int number);
int stringToInt(const char *str);

void destroyString(char *str)
{
    if (str != NULL)
    {
        free(str);
    }
    str = NULL;
}

char *createString(int length)
{
    char *new_str = malloc(length + 1);
    if (new_str == NULL)
    {
        return NULL;
    }
    new_str[length] = END_OF_STRING; //add '\0' in the last cell
    return new_str;
}

int addVotes(int votes, int votes_to_add)
{
    return votes + votes_to_add;
}

int removeVotes(int votes, int votes_to_remove)
{
    return (votes - votes_to_remove) > 0 ? (votes - votes_to_remove) : 0;
}

char *intToString(int number)
{
    int tmp = number, digits_num = 0, reverse = 0;
    while (tmp > 0) //calculate the number of digits and reverse the number
    {
        reverse *= BASE_TEN;
        reverse += tmp % BASE_TEN;
        digits_num++;
        tmp /= BASE_TEN;
    }
    char *str = createString(digits_num); //alloc a string in the size of the number
    int i=0;
    while(i<digits_num){
        str[i] = (reverse % BASE_TEN) + '0';
        reverse /= BASE_TEN;
        i++;
    }
    return str;
}

int stringToInt(const char *str)
{
    int int_from_str = 0, cou = 0;
    assert(str != NULL);
    while (str[cou])
    {
        int_from_str *= BASE_TEN;
        int_from_str += (str[cou] - '0'); //value is in ASCII
        cou++;
    }
    return int_from_str;
}



