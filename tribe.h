#ifndef MTM_TRIBE_H
#define MTM_TRIBE_H

#include "assist.h"
/**
* Tribe tribe
* Implements a Tribe type.
* The type of tribe_map is a map
*key is a char* type and the value is the tribe_id (not negative)
*data is a char* type and the value is tha tribe_name + tribe_votes
*the data is sperated by a defined sign SPERATOR
*tribe name consists of lower case letters and spaces
* tribe_votes is a positive number
**/

/** Type for defining a Tribe */
typedef struct tribe_t* Tribe;

/** Type used for returning error codes from tribe functions */
typedef enum TribeResult_t
{
    TRIBE_SUCCESS,
    TRIBE_OUT_OF_MEMORY,
    TRIBE_NULL_ARGUMENT,
    TRIBE_ITEM_ALREADY_EXISTS,
    TRIBE_ITEM_DOES_NOT_EXIST,
    TRIBE_ERROR
} TribeResult;

/**
* tribeCreate: Allocates a new empty tribe.
* @return
* 	NULL - if allocations failed.
* 	A new Tribe in case of success.
*/
Tribe tribeCreate();
/**
* tribeDestroy: Deallocates an existing tribe. Clears all elements.
* @param tribe - Target tribe to be deallocated. If tribe is NULL nothing will be
* done
*/
void tribeDestroy(Tribe tribe);
/**
*gets a pointer to tribe map and initilaze it with the given id as key
*and the data value as one string : "tribe name" SPERATOR "votes"
*votes initishial value is zero.
*@return
return NULL if tribe is NUll or tribe does not exists or memeory allocation failed
*/
TribeResult tribeAdd(Tribe tribe, int tribe_id, const char* tribe_name);
/**
*gets a pointer to tribe and find the tribe in the tribe map with the given id as key
* sperate the tribe name from the string sored in data and return a pointer to a copy
*of the tribe name(by value)
*@return
*TRIBE_NULL_ARGUMENT if one of the given arguments is NULL
*TRIBE_INVALID_ID if tribe_id is negative
*TRIBE_INVALID_VOTES if votes is not a posite
*/
char* tribeGetName(Tribe tribe, int tribe_id);
/**
*gets a pointer to tribe and find the tribe in the tribe map with the given id as key
*update votes value in data by the given condition and number
*@return
*return NULL if tribe is NUll or tribe does not exists or memeory allocation failed
*/
TribeResult tribeUpdateVote(Tribe tribe, int tribe_id, int num_of_votes, UpdateVotesCondition condition);
/*
*get a tribe id to set its name to tribe_name
*return TRIBE_OUT_MEMORY if name allocation failed
*TRIBE_SUCSESS if update went well
*/
TribeResult tribeSetName(Tribe tribe, int tribe_id, const char* tribe_name);
/*
*gets a trie it and remove it from all the tribes,
*@return TRIBE_ITEM_DOES_NOT_EXIST if the the tribe isn't one of all the tribes
*TRIBE_SUCSESS if update went well
*/
TribeResult tribeRemove(Tribe tribe, int tribe_id);
/*
gets a pointer to tribe, run on all tribe map and change every vote to 0
*/
void tribeSetAllVotesToZero(Tribe tribe);
/*
gets a pointer to a tribe adt and return a copy (by value) of the tribe
*return a copy (by value) of the given tribe
*@return NULL if memory allocation failed otherwise return a pointer to the copy
*/
Tribe tribeCopy(Tribe tribe);
/*
get a tribe map and return the tribe id of the tribe with the highest amount of votes
return NULL if tribe is NULL or the tribe map is NULL
*/
char* tribeGetMaxVotesForArea(Tribe tribe);
/*
gets a tribe map and a id and return true id a tribe with this id exsits otherwise
retrun false
*/
bool tribeContains(Tribe tribe, int tribe_id);
#endif //MTM_TRIBE_H


