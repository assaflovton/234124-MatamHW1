#ifndef MTM_AREA_H
#define MTM_AREA_H

#include "election.h"
#include "assist.h"
#include "mtm_map/map.h"

/**
*Implements an Area type as a list. area is a pointer of node with 3 elements,
*area_id (int) and area_name (char*) and next(area)- a pointer to the next node.
*tribe_id is a positive number
**/

/** Type for defining an Area */
typedef struct area_t* Area;
/** Type used for returning error codes from area functions */
typedef enum AreaResult_t {
    AREA_OUT_OF_MEMORY,
    AREA_NULL_ARGUMENT,
    AREA_SUCCESS,
    AREA_INVALID_ID,
    AREA_TRIBE_ALREADY_EXIST,
    AREA_INVALID_NAME,
    AREA_ALREADY_EXIST,
    AREA_TRIBE_NOT_EXIST,
    AREA_NOT_EXIST,
    AREA_INVALID_VOTES
} AreaResult;

/*
*areaCreate: Allocates a new empty map.
*@return
* 	NULL - if allocations failed.
* 	A new Area in case of success.
*/
Area areaCreate();
/*
* areaDestroy: Deallocates an existing area. Clears all elements.
* @param area - Target area to be deallocated. If area is NULL nothing will be
* done. area is a list all list will be deallocated.
*/
void areaDestroy(Area area);
/*
*areaAddTribe: add a new tribe to all area nodes in the area list.
*@return
*AREA_TRIBE_ALREADY_EXIST if a tribe with the same id exsits in the tribes map
*AREA_OUT_OF_MEMORY if any memory allocation failed
*AREA_SUCCESS if tribe was succsessfully added to all areas
*/
AreaResult areaAddTribe(Area area, int tribe_id, const char* tribe_name);
/*
*areaAdd: add a new area to the area list.
*to the added area a copy of the tribe map is created and initialized with zero vodts
*@return
*AREA_ALREADY_EXIST if an area with the same id exsits in the list
*AREA_OUT_OF_MEMORY if any memory allocation failed
*AREA_SUCCESS if an area was succsessfully added to the list
*/
AreaResult areaAdd(Area area, int area_id, const char* area_name);
/*
*areaGetTribeName: return a copy of an area_name(char*) by the given area id
*@return
* NULL if no area with the give id exists or memeory allocation failed
* if not return a pointer to the copy of the area name
*/
char* areaGetTribeName(Area area, int tribe_id);
/*
*areaUpdateVote: update the number of votes
(adding or removing votes depends on condition) of the area with the specified id
*if the number of votes becomes negative after remove, set to 0
*@return
*AREA_NOT_EXIST if there is no area with the given id in the areas list
*AREA_TRIBE_NOT_EXIST if there is no tribe with the given id in the tribes map
*AREA_OUT_OF_MEMORY if any memory allocation failed
*AREA_SUCCESS if an area was succsessfully added to the list
*/
AreaResult areaUpdateVote(Area area, int area_id, int tribe_id, int num_of_votes, UpdateVotesCondition condition);
/*areaSetTribeName change the name of the tribe with the specified id, update the change is all the
*areas since each one has its own tribes map
*name conssists only spaces and low letter case
*@return
*AREA_TRIBE_DOES_NOT_EXIST if there is no tribe with the give is the tribe map
*AREA_SUCCSESS if went well
*/
AreaResult areaSetTribeName(Area area, int tribe_id, const char* tribe_name);
/*
*areaRemoveTribe:
*remove the tribe with the specified id from all of the areas in the list
*@return
*AREA_TRIBE_DOES_NOT_EXIST if there is no tribe with the give is the tribe map
*AREA_SUCCSESS if went well
*/
AreaResult areaRemoveTribe(Area area, int tribe_id);
/*
*areaRemove: removes areas from the list that thier id AreaConditionFunction return true for
*@return
*AREA_OUT_OF_MEMORY if any memory allocation failed
*AREA_SUCCSES otherwise
*/
AreaResult areaRemove(Area area, AreaConditionFunction should_delete_area);
/*
*areaComputeAreasToTribesMapping:
*finds for every area to which tribe most of the votes went and put them in a map
*when the key is the area id and the data id the tribe with most votes
*in case of a tie in the votes we will take the tribe with the smaller id
*in case of memory allocation fail return null. if threre are no areas or tribes retuen an empty map
*/
Map areaComputeAreasToTribesMapping(Area area);
/*
get a list of areas and return true if an area with the given exists, otherwise return false
*/
bool areaContains(Area area, int area_id);
/*
get a list of areas ans return true if a tribe with the givem id exsits, otherwise return false
*/
bool areaTribeContains(Area area, int tribe_id);
#endif //MTM_AREA_H

