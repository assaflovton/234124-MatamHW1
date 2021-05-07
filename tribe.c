#define _CRT_SECURE_NO_WARNINGS
#include "mtm_map/map.h"
#include "assist.h"
#include "tribe.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define SEPERATOR '$'
#define BASE_TEN 10

#define SEPERATOR '$'
#define BASE_TEN 10


struct tribe_t
{
    Map tribe_map;
};

Tribe tribeCreate();
void tribeDestroy(Tribe tribe);
TribeResult tribeAdd(Tribe tribe, int tribe_id, const char* tribe_name);
TribeResult tribeSetName(Tribe tribe, int tribe_id, const char* tribe_name);
TribeResult tribeRemove(Tribe tribe, int tribe_id);
Tribe tribeCopy(Tribe tribe);
static char* combineStringsWithSperation(const char* str1, const char* str2);
static char* createStringOfTribeNameAndVotes(const char* tribe_name, int num_of_votes);
char* tribeGetName(Tribe tribe, int tribe_id);
static int getIntAfterSperator(const char* str);
static char* getStringBeforeSperator(const char* str);
static int getVotesFromTribeById(Tribe tribe, int tribe_id);
TribeResult tribeUpdateVote(Tribe tribe, int tribe_id, int num_of_votes, UpdateVotesCondition condition);
static TribeResult handleMapResult(MapResult result);
char* tribeGetMaxVotesForArea(Tribe tribe);
bool tribeContains(Tribe tribe, int tribe_id);

bool tribeContains(Tribe tribe, int tribe_id)
{
    if (tribe == NULL)
    {
        return false;
    }
    char* string_id = intToString(tribe_id);
    bool result = mapContains(tribe->tribe_map, string_id);
    destroyString(string_id);
    return result;
}
Tribe tribeCreate()
{
    Tribe tribe = malloc(sizeof(*tribe));
    if (tribe == NULL)
    {
        return NULL;
    }
    tribe->tribe_map = mapCreate();
    if (tribe->tribe_map == NULL) //allocation failed
    {
        free(tribe);
        return NULL;
    }
    return tribe;
}

void tribeDestroy(Tribe tribe)
{
    if (tribe != NULL)
    {
        mapDestroy(tribe->tribe_map);
        free(tribe);
    }
}

TribeResult tribeAdd(Tribe tribe, int tribe_id, const char* tribe_name)
{
    assert(tribe != NULL);
    char* tribe_id_in_string = intToString(tribe_id);
    if (mapContains(tribe->tribe_map, tribe_id_in_string))
    {
        free(tribe_id_in_string);
        return TRIBE_ITEM_ALREADY_EXISTS;
    }
    if (tribe_id_in_string == NULL)
    {
        return TRIBE_OUT_OF_MEMORY;
    }                                                                           //the tribe is new
    char* combined_votes_name = createStringOfTribeNameAndVotes(tribe_name, 0); //initial value of votes 0
    if (combined_votes_name == NULL)
    {
        destroyString(tribe_id_in_string);
        return TRIBE_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(tribe->tribe_map, (const char*)tribe_id_in_string, (const char*)combined_votes_name);
    free(tribe_id_in_string);
    free(combined_votes_name);
    return handleMapResult(result);
}

char* tribeGetName(Tribe tribe, int tribe_id)
{
    assert(tribe != NULL);
    const char* tribe_id_in_string = intToString(tribe_id);
    if (tribe_id_in_string == NULL)
    {
        return NULL;
    }
    if (!mapContains(tribe->tribe_map, tribe_id_in_string))
    {
        destroyString((char*)tribe_id_in_string);
        return NULL;
    }
    char* string_name_and_votes = mapGet(tribe->tribe_map, tribe_id_in_string);
    destroyString((char*)tribe_id_in_string);
    return getStringBeforeSperator(string_name_and_votes);//sperate the name from the string and return it
}

TribeResult tribeSetName(Tribe tribe, int tribe_id, const char* tribe_name)
{
    assert(tribe != NULL);
    char* id_str = intToString(tribe_id);
    if (id_str == NULL)
    {
        return TRIBE_OUT_OF_MEMORY;
    }
    if (!mapContains(tribe->tribe_map, id_str))
    {
        destroyString(id_str);
        return TRIBE_ITEM_DOES_NOT_EXIST;
    }
    int votes = getVotesFromTribeById(tribe, tribe_id);
    char* combined_votes_and_new_name = createStringOfTribeNameAndVotes(tribe_name, votes);
    if (combined_votes_and_new_name == NULL)
    {
        return TRIBE_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(tribe->tribe_map, id_str, combined_votes_and_new_name);
    destroyString(id_str);
    destroyString(combined_votes_and_new_name);
    return handleMapResult(result);
}

TribeResult tribeRemove(Tribe tribe, int tribe_id)
{
    assert(tribe != NULL);
    char* id_str = intToString(tribe_id);
    if (id_str == NULL)
    {
        return TRIBE_OUT_OF_MEMORY;
    }
    MapResult result = mapRemove(tribe->tribe_map, id_str);
    destroyString(id_str);
    return handleMapResult(result);
}

TribeResult tribeUpdateVote(Tribe tribe, int tribe_id, int num_of_votes, UpdateVotesCondition condition)
{
    assert(tribe_id >= 0 && num_of_votes >= 0 && tribe != NULL);
    int previeous_votes, updated_votes;
    char* id_str = intToString(tribe_id);
    if (id_str == NULL)
    {
        return TRIBE_OUT_OF_MEMORY;
    }
    if (!mapContains(tribe->tribe_map, id_str))
    {
        destroyString(id_str);
        return TRIBE_ITEM_DOES_NOT_EXIST;
    }
    char* name = tribeGetName(tribe, tribe_id);
    if (name == NULL)
    {
        return TRIBE_OUT_OF_MEMORY;
    }
    previeous_votes = getVotesFromTribeById(tribe, tribe_id);
    updated_votes = condition(previeous_votes, num_of_votes);
    char* combined_updated_votes_name = createStringOfTribeNameAndVotes((const char*)name, updated_votes);
    destroyString(tribeGetName(tribe, tribe_id));
    MapResult result = mapPut(tribe->tribe_map, id_str, combined_updated_votes_name);
    destroyString(id_str);
    destroyString(name);
    destroyString(combined_updated_votes_name);
    return handleMapResult(result);
}

Tribe tribeCopy(Tribe tribe)
{
    Tribe tribe_copy = tribeCreate();
    if (tribe_copy == NULL)
    {
        return NULL;
    }
    Map tribe_map_copy = mapCopy(tribe->tribe_map);
    if (tribe_map_copy == NULL)
    {

        tribeDestroy(tribe_copy);
        return NULL;
    }
    mapDestroy(tribe_copy->tribe_map);
    tribe_copy->tribe_map = tribe_map_copy;
    return tribe_copy;
}

char* tribeGetMaxVotesForArea(Tribe tribe)
{
    if (tribe == NULL || tribe->tribe_map == NULL)
    {
        return NULL;
    }
    int max_votes = 0, current_tribe_votes;
    char* max_id;
    Map tribe_map_copy = tribe->tribe_map;
    if (tribe_map_copy == NULL)
    {
        return NULL;
    }
    char* current_key = mapGetFirst(tribe_map_copy);
    if (current_key == NULL)
    {
        return NULL;
    }
    max_id = current_key;
    max_votes = getVotesFromTribeById(tribe, stringToInt(current_key));
    while (current_key != NULL)
    {
        current_tribe_votes = getVotesFromTribeById(tribe, stringToInt(current_key));
        if (current_tribe_votes > max_votes)
        {
            max_votes = getVotesFromTribeById(tribe, stringToInt(current_key));
            max_id = current_key;
        }
        if (current_tribe_votes == max_votes)//if it has the same amount of votes choose the one with the lower tribe id
        {
            if (stringToInt(max_id) > stringToInt(current_key))//found a new max
            {
                max_id = current_key;//update the max id
                max_votes = getVotesFromTribeById(tribe, stringToInt(current_key));// update the max vote
            }
        }
        current_key = mapGetNext(tribe_map_copy);
    }

    return max_id;
}

/*
gets two strings, return a new string with a char speration between them(see define SPERATOR)
return NULL if allocation failed
*/
static char* combineStringsWithSperation(const char* str1, const char* str2)
{
    assert(str1 != NULL && str2 != NULL);
    int new_str_size = strlen(str1) + 1 + strlen(str2); //1 for sperator, 1 for end of string
    char* new_str = createString(new_str_size);
    if (new_str == NULL)
    {
        return NULL;
    }
    strncpy(new_str, (const char*)str1, strlen(str1));                    //copy the name
    strncpy(new_str + strlen(str1) + 1, (const char*)str2, strlen(str2)); //copy the votes
    new_str[strlen(str1)] = SEPERATOR;                                     //add sperator
    return new_str;
}
/*
get the tribe name and the num of votes to the tribe, create a strig (copy by value) of the two with the
SPERATOR sign in the middle if the memory allocation failed return NULL
*/
static char* createStringOfTribeNameAndVotes(const char* tribe_name, int num_of_votes)
{
    assert(tribe_name != NULL);
    char* votes_str = intToString(num_of_votes);
    if (votes_str == NULL)
    {
        return NULL;
    }
    char* combined_str = combineStringsWithSperation(tribe_name, (const char*)votes_str);
    if (combined_str == NULL)
    {
        destroyString(votes_str);
        return NULL;
    }
    destroyString(votes_str);
    return combined_str;
}
/*
get a string and return the str after the SPERATOR
*/
static int getIntAfterSperator(const char* str)
{
    assert(str != NULL);
    char* sperator_place = strchr(str, SEPERATOR);
    assert(sperator_place != NULL);
    return stringToInt((const char*)(sperator_place + 1));
}
/*
get a pointer to a string and return
*/
static char* getStringBeforeSperator(const char* str)
{
    assert(str != NULL);
    char* sperator_place = strchr(str, SEPERATOR);  //return a pointer to the first charachter equals to Sperator
    char* name = createString((int)(sperator_place - str)); //we dont include sperator so -1
    if (name == NULL)
    {
        return NULL;
    }
    strncpy(name, (const char*)str, sperator_place - str);
    return name;
}
/*
look for a tribe by the given id and return the number of votes from this tribe
*/
static int getVotesFromTribeById(Tribe tribe, int tribe_id)
{
    assert(tribe != NULL);
    char* tribe_id_in_string = intToString(tribe_id);
    assert(tribe_id_in_string != NULL);
    char* combined_name_and_votes = mapGet(tribe->tribe_map, (const char*)tribe_id_in_string);
    assert(combined_name_and_votes != NULL);
    destroyString(tribe_id_in_string);
    return getIntAfterSperator(combined_name_and_votes);
}
/*
handle the results from map
*/
static TribeResult handleMapResult(MapResult result)
{
    switch (result)
    {
    case MAP_SUCCESS:
        return TRIBE_SUCCESS;
    case MAP_OUT_OF_MEMORY:
        return TRIBE_OUT_OF_MEMORY;
    case MAP_NULL_ARGUMENT:
        return TRIBE_NULL_ARGUMENT;
    case MAP_ITEM_ALREADY_EXISTS:
        return TRIBE_ITEM_ALREADY_EXISTS;
    case MAP_ITEM_DOES_NOT_EXIST:
        return TRIBE_ITEM_DOES_NOT_EXIST;
    case MAP_ERROR:
        return TRIBE_ERROR;
    default:
        return TRIBE_SUCCESS;
    }
}
/*
get a pointer to
*/
void tribeSetAllVotesToZero(Tribe tribe)
{
    int votes_to_remove, tribe_id;
    assert(tribe != NULL && tribe->tribe_map != NULL);
    char* current_key = mapGetFirst(tribe->tribe_map);
    while (current_key != NULL)
    {
        tribe_id = stringToInt(current_key);
        votes_to_remove = getVotesFromTribeById(tribe, tribe_id);
        tribeUpdateVote(tribe, tribe_id, votes_to_remove, removeVotes);
        current_key = mapGetNext(tribe->tribe_map);
    }
}


