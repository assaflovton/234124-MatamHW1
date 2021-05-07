#define _CRT_SECURE_NO_WARNINGS
#include "mtm_map/map.h"         
#include "election.h"
#include "area.h"
#include "assist.h"
#include "tribe.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


#define SPACE ' '
#define FIRST_LETTER 'a'
#define LAST_LETTER 'z'

struct election_t
{
    Area area_list;
};
/**
* Implements an Election type.
* Election has a list of Areas each, Area has name (string) and id (id) and a map of tirbes
*each tribe in the map has tribe id as a key and name+votes as one string
*each area has a copy of all the existing tribes in the election systeme that keeps a record of the votes they got
*from each tribe
**/
Election electionCreate();
void electionDestroy(Election election);
ElectionResult electionAddTribe(Election election, int tribe_id, const char* tribe_name);
ElectionResult electionAddArea(Election election, int area_id, const char* area_name);
char* electionGetTribeName(Election election, int tribe_id);
ElectionResult electionAddVote(Election election, int area_id, int tribe_id, int num_of_votes);
ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes);
ElectionResult electionSetTribeName(Election election, int tribe_id, const char* tribe_name);
ElectionResult electionRemoveTribe(Election election, int tribe_id);
ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area);
Map electionComputeAreasToTribesMapping(Election election);
static bool isValidVotes(int num_of_votes);
static bool isValidId(int id);
static bool isValidName(const char* name);
static ElectionResult isAddArgumentsValid(Election election, int id, const char* name);
static ElectionResult handleResult(AreaResult result);

Election electionCreate()
{
    Election election = malloc(sizeof(*election));
    if (election == NULL)
    {
        return NULL;
    }
    election->area_list = areaCreate();
    if (election->area_list == NULL)
    {
        free(election);
        return NULL;
    }
    return election;
}

void electionDestroy(Election election)
{
    if (election != NULL)
    {
        areaDestroy(election->area_list);
        free(election);
    }
}

ElectionResult electionAddTribe(Election election, int tribe_id, const char* tribe_name)
{
    ElectionResult result_arguments_valid = isAddArgumentsValid(election, tribe_id, tribe_name);
    if (result_arguments_valid == ELECTION_NULL_ARGUMENT || result_arguments_valid == ELECTION_INVALID_ID)
    {
        return result_arguments_valid;
    }
    if (areaTribeContains(election->area_list, tribe_id))
    {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (result_arguments_valid != ELECTION_SUCCESS)
    {
        return result_arguments_valid;
    }
    AreaResult result = areaAddTribe(election->area_list, tribe_id, tribe_name);
    return handleResult(result);
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name)
{
    ElectionResult result_arguments_valid = isAddArgumentsValid(election, area_id, area_name);
    if (result_arguments_valid == ELECTION_NULL_ARGUMENT || result_arguments_valid == ELECTION_INVALID_ID)
    {
        return result_arguments_valid;
    }
    if (areaContains(election->area_list, area_id))
    {
        return ELECTION_AREA_ALREADY_EXIST;
    }
    if (result_arguments_valid != ELECTION_SUCCESS)
    {
        return result_arguments_valid;
    }
    AreaResult result = areaAdd(election->area_list, area_id, area_name);
    return handleResult(result);
}

char* electionGetTribeName(Election election, int tribe_id)
{
    if (election == NULL || !isValidId(tribe_id))
    {
        return NULL;
    }
    return areaGetTribeName(election->area_list, tribe_id);
}

ElectionResult electionAddVote(Election election, int area_id, int tribe_id, int num_of_votes)
{
    if (election == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidId(area_id) || !isValidId(tribe_id))
    {
        return ELECTION_INVALID_ID;
    }
    if (!isValidVotes(num_of_votes))
    {
        return ELECTION_INVALID_VOTES;
    }
    AreaResult result = areaUpdateVote(election->area_list, area_id, tribe_id, num_of_votes, addVotes);
    return handleResult(result);
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes)
{
    if (election == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidId(area_id) || !isValidId(tribe_id))
    {
        return ELECTION_INVALID_ID;
    }
    if (!isValidVotes(num_of_votes))
    {
        return ELECTION_INVALID_VOTES;
    }
    AreaResult result = areaUpdateVote(election->area_list, area_id, tribe_id, num_of_votes, removeVotes);
    return handleResult(result);
}

ElectionResult electionSetTribeName(Election election, int tribe_id, const char* tribe_name)
{
    ElectionResult result_arguments_valid = isAddArgumentsValid(election, tribe_id, tribe_name);
    if (result_arguments_valid == ELECTION_NULL_ARGUMENT || result_arguments_valid == ELECTION_INVALID_ID)
    {
        return result_arguments_valid;
    }
    if (!areaTribeContains(election->area_list, tribe_id))
    {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (result_arguments_valid != ELECTION_SUCCESS)
    {
        return result_arguments_valid;
    }
    AreaResult result = areaSetTribeName(election->area_list, tribe_id, tribe_name);
    return handleResult(result);
}

ElectionResult electionRemoveTribe(Election election, int tribe_id)
{
    if (election == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidId(tribe_id))
    {
        return ELECTION_INVALID_ID;
    }
    AreaResult result = areaRemoveTribe(election->area_list, tribe_id);
    return handleResult(result);
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area)
{
    if (election == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    AreaResult result = areaRemove(election->area_list, should_delete_area);
    return handleResult(result);
}

Map electionComputeAreasToTribesMapping(Election election)
{
    if (election == NULL)
    {
        return NULL;
    }
    return areaComputeAreasToTribesMapping(election->area_list);
}
/*
validates the given arguments and return the matched error to to the argument
if all arguments are valid returns ELECTION_SUCCSESS
*/
static ElectionResult isAddArgumentsValid(Election election, int id, const char* name)
{
    if (election == NULL || name == NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (!isValidId(id))
    {
        return ELECTION_INVALID_ID;
    }
    if (!isValidName(name))
    {
        return ELECTION_INVALID_NAME;
    }
    return ELECTION_SUCCESS;
}
/*
validates the number of votes is a positive number
*/
static bool isValidVotes(int num_of_votes)
{
    if (num_of_votes <= 0)
    {
        return false;
    }
    return true;
}
/*
validets the id  is a positive bumber
*/
static bool isValidId(int id)
{
    if (id < 0)
    {
        return false;
    }
    return true;
}
/*
validets the name consists only low letter case and spaces
*/
static bool isValidName(const char* name)
{
    while (*name)
    {
        if (*name != SPACE && (*name > LAST_LETTER || *name < FIRST_LETTER))
        {
            return false;
        }
        name++;
    }
    return true;
}
/*
handle the results from tribe
*/
static ElectionResult handleResult(AreaResult result)
{
    switch (result)
    {
    case AREA_SUCCESS:
        return ELECTION_SUCCESS;
    case AREA_OUT_OF_MEMORY:
        return ELECTION_OUT_OF_MEMORY;
    case AREA_NULL_ARGUMENT:
        return ELECTION_NULL_ARGUMENT;
    case AREA_NOT_EXIST:
        return ELECTION_AREA_NOT_EXIST;
    case AREA_ALREADY_EXIST:
        return ELECTION_AREA_ALREADY_EXIST;
    case AREA_TRIBE_NOT_EXIST:
        return ELECTION_TRIBE_NOT_EXIST;
    case AREA_TRIBE_ALREADY_EXIST:
        return ELECTION_TRIBE_ALREADY_EXIST;
    default:
        return ELECTION_SUCCESS;
    }
}