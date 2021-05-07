#define _CRT_SECURE_NO_WARNINGS
#include "mtm_map/map.h"
#include "area.h"
#include "assist.h"
#include "tribe.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


#define UNDEFINED_ID -1


struct area_t
{
    int id;
    char* name;
    Tribe tribe;
    Area next;
};

Area areaCreate();
void areaDestroy(Area area);
AreaResult areaAddTribe(Area area, int tribe_id, const char* tribe_name);
AreaResult areaAdd(Area area, int area_id, const char* area_name);
char* areaGetTribeName(Area area, int tribe_id);
AreaResult areaAddVote(Area area, int area_id, int tribe_id, int num_of_votes);
AreaResult areaRemoveVote(Area area, int area_id, int tribe_id, int num_of_votes);
AreaResult areaSetTribeName(Area area, int tribe_id, const char* tribe_name);
AreaResult areaRemoveTribe(Area area, int tribe_id);
AreaResult areaRemove(Area area, AreaConditionFunction should_delete_area);
Map areaComputeAreasToTribesMapping(Area area);
static void areaElementsDelete(Area area);
static AreaResult handleResult(TribeResult result);
static Area getAreaById(Area area, int area_id);
static bool areaPutElemtnes(Area area, int area_id, const char* area_name);
AreaResult areaUpdateVote(Area area, int area_id, int tribe_id, int num_of_votes, UpdateVotesCondition condition);
static void swapArea(Area area1, Area area2);
static void removeNodeArea(Area area);
static Area getFormerAreaNode(Area area, int area_id);
bool areaContains(Area area, int area_id);
bool areaTribeContains(Area area, int tribe_id);

bool areaTribeContains(Area area, int tribe_id)
{
    if (area == NULL)
    {
        return false;
    }
    return tribeContains(area->tribe, tribe_id);
}

bool areaContains(Area area, int area_id)
{
    if (getAreaById(area, area_id) == NULL)
    {
        return false;
    }
    return true;

}

Area areaCreate()
{
    Area area = malloc(sizeof(*area));
    if (area == NULL)
    {
        return NULL;//allocation failed
    }
    area->tribe = tribeCreate();
    if (area->tribe == NULL)
    {
        free(area);
        return NULL;//allocation failed
    }
    area->id = UNDEFINED_ID;
    area->name = NULL;
    area->next = NULL;
    return area;
}

void areaDestroy(Area area)
{
    while (area != NULL)//if the id is undefined the area elements are already NULL
    {
        Area area_to_Delete = area;
        area = area->next;
        areaElementsDelete(area_to_Delete);
        free(area_to_Delete);
    }
}

AreaResult areaAddTribe(Area area, int tribe_id, const char* tribe_name)
{
    assert(area != NULL && tribe_name != NULL);
    assert(tribe_id >= 0);
    TribeResult result;
    while (area != NULL)
    {
        result = tribeAdd(area->tribe, tribe_id, tribe_name);
        if (result != TRIBE_SUCCESS)
        {
            return handleResult(result);
        }
        area = area->next;
    }
    return handleResult(result);
}

AreaResult areaAdd(Area area, int area_id, const char* area_name)
{
    assert(area != NULL && area_name != NULL);
    assert(area_id >= 0);
    if (getAreaById(area, area_id) != NULL) //found an area with the given id
    {
        return AREA_ALREADY_EXIST;
    }
    if (area->id == UNDEFINED_ID) //only one empty area
    {
        if (!areaPutElemtnes(area, area_id, area_name))
        {
            return AREA_OUT_OF_MEMORY;
        }
        tribeSetAllVotesToZero(area->tribe);
    }
    else
    {
        Area new_area = areaCreate();
        if (new_area == NULL)
        {
            return AREA_OUT_OF_MEMORY;
        }
        tribeDestroy(new_area->tribe);
        destroyString(new_area->name);
        new_area->tribe = tribeCopy(area->tribe);//copy the tribe 
        if (new_area->tribe == NULL)//check if copying tribe failed
        {
            return AREA_OUT_OF_MEMORY;
        }
        if (!areaPutElemtnes(new_area, area_id, area_name))
        {
            areaDestroy(new_area);
            return AREA_OUT_OF_MEMORY;
        }
        tribeSetAllVotesToZero(new_area->tribe);
        while (area->next != NULL)
        {
            area = area->next;
        }
        area->next = new_area;
    }
    return AREA_SUCCESS;
}

char* areaGetTribeName(Area area, int tribe_id)
{
    if (area == NULL || area->tribe == NULL)
    {
        return NULL;
    }
    return tribeGetName(area->tribe, tribe_id);
}

AreaResult areaUpdateVote(Area area, int area_id, int tribe_id, int num_of_votes, UpdateVotesCondition condition)
{
    TribeResult result;
    assert(area != NULL && area->tribe != NULL && area_id >= 0 && tribe_id >= 0 && num_of_votes >= 0);
    Area area_to_update = getAreaById(area, area_id);//look for the area with the given id
    if (area_to_update == NULL)
    {
        return AREA_NOT_EXIST;
    }
    result = tribeUpdateVote(area_to_update->tribe, tribe_id, num_of_votes, condition);
    return handleResult(result);
}

AreaResult areaSetTribeName(Area area, int tribe_id, const char* tribe_name)
{
    assert(area != NULL && tribe_id >= 0 && tribe_name != NULL);
    TribeResult result;
    while (area != NULL)
    {//run on all area nodes, need to change the name in all the areas since each one has a tribe map
        result = tribeSetName(area->tribe, tribe_id, tribe_name);
        if (result != TRIBE_SUCCESS)
        {
            return handleResult(result);
        }
        area = area->next;
    }
    return handleResult(result);
}

AreaResult areaRemoveTribe(Area area, int tribe_id)
{
    assert(area != NULL && tribe_id >= 0);
    TribeResult result;
    while (area != NULL)// need to remove from all the area since each area has a tribe map of itself
    {
        if (area->tribe != NULL) {
            result = tribeRemove(area->tribe, tribe_id);
        }
        if (result != TRIBE_SUCCESS)
        {
            return handleResult(result);
        }
        area = area->next;
    }
    return handleResult(result);
}

AreaResult areaRemove(Area area, AreaConditionFunction should_delete_area)
{
    assert(area != NULL);
    if (area->next == NULL) //first and only node
    {
        if (should_delete_area(area->id))//need to delete the only area in the list
        {
            areaElementsDelete(area);//only delete  the node elements, don't delete the list itself
        }
        return AREA_SUCCESS;
    }
    Area tmp = area;
    while (tmp != NULL)
    {
        Area formerNode = getFormerAreaNode(area, tmp->id);
        if (should_delete_area(tmp->id))
        {
            if (tmp->next == NULL)
            {
                if (formerNode == NULL)//only one node exsits in the list and we want to delete it
                {
                    areaElementsDelete(tmp);//keep an empty list
                    tmp->tribe = NULL;
                }
                else
                {
                    formerNode->next = NULL;//we need to delete the last node in the list
                    areaElementsDelete(tmp);
                    free(tmp);
                }
                return AREA_SUCCESS;
            }
            if (formerNode == NULL)//need to delete the first node, but its not the only node in the list
            {
                swapArea(tmp, tmp->next);//swap and deletes the second node 
                removeNodeArea(tmp);
            }
            else
            {
                Area toDelete = tmp;
                formerNode->next = tmp->next;//remove a note from the middle of the list
                tmp = tmp->next;
                areaElementsDelete(toDelete);
                free(toDelete);
            }
        }
        else // progress in the list only if we dont need to delete this node
        {
            tmp = tmp->next;
        }
    }
    return AREA_SUCCESS;
}

Map areaComputeAreasToTribesMapping(Area area)
{
    char* string_area_id, * tribe_max_vote_id;
    Map map_of_max = mapCreate();//create the map of the mapped areas to tribes
    if (map_of_max == NULL)
    {
        return NULL;
    }
    if (area == NULL)
    {
        return map_of_max;
    }
    MapResult result;
    while (area != NULL)//do for all areas in the list
    {
        string_area_id = intToString(area->id);
        tribe_max_vote_id = tribeGetMaxVotesForArea(area->tribe);// map the tribe with the most votes to the area
        if (tribe_max_vote_id == NULL)
        {
            destroyString(string_area_id);
            return map_of_max;
        }
        result = mapPut(map_of_max, (const char*)string_area_id, (const char*)tribe_max_vote_id);
        destroyString(string_area_id);
        if (result != MAP_SUCCESS)//allocation failed
        {
            mapDestroy(map_of_max);
            return NULL;
        }
        area = area->next;

    }
    return map_of_max;
}

/*
getAreaById: get a pointer to the areas list and return a pointer to the area with the given id
if no area with the specified id exists return NULL
*/
static Area getAreaById(Area area, int area_id)
{
    if (area == NULL)
    {
        return NULL;
    }
    Area tmp = area;
    while (tmp != NULL)//run on all the areas
    {
        if (tmp->id == area_id)
        {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

/*
areaPutElemtnes: get an initialized area and update its elements by copying the given
varibels (by value) if any memeory allocation failed return false otherwise true
*/
static bool areaPutElemtnes(Area new_area, int area_id, const char* area_name)
{
    new_area->id = area_id;
    new_area->name = createString(strlen(area_name));//allocate memory for the area name
    if (new_area->name == NULL)//check if allocation failed
    {
        return false;
    }
    strcpy(new_area->name, area_name);//copy the name
    return true;
}

/*
get a pointer to a area and free all of the area varibels and afterwards set them to NULL
*/
static void areaElementsDelete(Area area)
{
    assert(area != NULL);
    if (area->tribe != NULL)
    {
        tribeDestroy(area->tribe);
    }
    area->next = NULL;
    destroyString(area->name);
    area->name = NULL;
    area->id = UNDEFINED_ID;
}

/*
handle the results from tribe
*/
static AreaResult handleResult(TribeResult result)
{
    switch (result)
    {
    case TRIBE_SUCCESS:
        return AREA_SUCCESS;
    case TRIBE_OUT_OF_MEMORY:
        return AREA_OUT_OF_MEMORY;
    case TRIBE_NULL_ARGUMENT:
        return AREA_NULL_ARGUMENT;
    case TRIBE_ITEM_ALREADY_EXISTS:
        return AREA_TRIBE_ALREADY_EXIST;
    case TRIBE_ITEM_DOES_NOT_EXIST:
        return AREA_TRIBE_NOT_EXIST;
    default:
        return AREA_SUCCESS;
    }
}
/*
getFormerAreaNode get an area id and return a pointer to the node before the node with this id
if there is no node with the given id, return NULL
*/
static Area getFormerAreaNode(Area area, int area_id)
{
    if (area == NULL || area->next == NULL)
    {
        return NULL;
    }
    if (area->id == area_id)
    {
        return NULL;
    }
    while (area->next != NULL)
    {
        if (area->next->id == area_id)
        {
            return area;
        }
        area = area->next;
    }
    return NULL;
    /*
     if (area != NULL && area->next != NULL && area->id == undef)
    {
        while (area->next != NULL)
        {
            if (area->next->id == area_id)
            {
                return area;
            }
        }
    }
    return NULL;
    */
}
/*
removeNodeArea: gets a pointer to the node before the node we want to remove from the list
disconnect the node from the list and disallocates its elenents
*/
static void removeNodeArea(Area area)
{
    Area toDelete = area->next;
    area->next = area->next->next;//go over the node we want to delete
    areaElementsDelete(toDelete);//free the node elements
    free(toDelete);
}
/*
swapArea: swaps between two area elements
*/
static void swapArea(Area area1, Area area2)
{
    char* tmp = area1->name;
    area1->name = area2->name;
    area2->name = tmp;
    int tmp2 = area1->id;
    area1->id = area2->id;
    area2->id = tmp2;
    Tribe tmp3 = area1->tribe;
    area1->tribe = area2->tribe;
    area2->tribe = tmp3;
}


