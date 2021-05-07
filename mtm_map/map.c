#include "map.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define NULL_MAP -1

Map mapCreate();
void mapDestroy(Map map);
Map mapCopy(Map map);
int mapGetSize(Map map);
bool mapContains(Map map, const char *key);
MapResult mapPut(Map map, const char *key, const char *data);
char *mapGet(Map map, const char *key);
MapResult mapRemove(Map map, const char *key);
char *mapGetFirst(Map map);
char *mapGetNext(Map map);
MapResult mapClear(Map map);
char *mapGetFirst(Map map);
char *mapGetNext(Map map);

struct Map_t
{
    Node node;
    char*  iterator;
};

Map mapCreate()
{
    Map map = malloc(sizeof(*map));
    if (map == NULL)
    {
        return NULL;
    }
    Node new_node = nodeCreate();
    if (new_node == NULL)
    {
        free(map);
        return NULL;
    }
    map->iterator=NULL;
    map->node = new_node;
    return map;
}

void mapDestroy(Map map)
{
    if (map != NULL)
    {
        nodeDestroy(map->node);
        free(map);
    }
}

Map mapCopy(Map map)
{
    if(map==NULL)
    {
        return NULL;
    }
    Map map_copy = mapCreate();
    if (map_copy == NULL)
    {
        return NULL;
    }
    Node node_copy = nodeCopy(map->node);
    if (node_copy == NULL)
    {
        mapDestroy(map_copy);
        return NULL;
    }
    free(map_copy->node);
    map_copy->node = node_copy;
    map_copy->iterator=NULL;
    return map_copy;
}
int mapGetSize(Map map)
{
    if (map == NULL)
    {
        return NULL_MAP;
    }
    if(nodeGetKey(map->node)==NULL)
    {
        return 0;
    }
    return nodeGetSize(map->node);
}

bool mapContains(Map map, const char *key)
{
    if (map == NULL || key == NULL)
    {
        return false;
    }
    return nodeContains(map->node,key);
}

MapResult mapPut(Map map, const char *key, const char *data)
{
    if (map == NULL || key == NULL || data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    assert(map->node != NULL);//assums node cant be NULL if map not NULL
    NodeResult result = nodePut(map->node,key,data);
    if (result == NODE_OUT_OF_MEMORY)
    {
        mapDestroy(map);
        return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS;
}

char* mapGet(Map map, const char *key)
{
    if (map == NULL || key == NULL)
    {
        return NULL;
    }
    return nodeGet(map->node, key);
}

MapResult mapRemove(Map map, const char *key)
{
    if (map == NULL || key == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    assert(map->node != NULL);
    NodeResult result = nodeRemove(map->node,key);
    if (result == NODE_ITEM_DOES_NOT_EXIST)
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    return MAP_SUCCESS;
}
MapResult mapClear(Map map)
{
    if (map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    assert(map->node != NULL);
    nodeClear(map->node);
    return MAP_SUCCESS;
}

char* mapGetFirst(Map map)
{
    if (map == NULL)
    {
        return NULL;
    }
    map->iterator = nodeGetKey(map->node);
    return map->iterator;
}

char* mapGetNext(Map map)
{
    if (map == NULL || map->iterator == NULL)
    {
        return NULL;
    }
    Node current_node=getNodeByKey(map->node,map->iterator);
    if(current_node==NULL || nodeGetNext(current_node)==NULL)
    {
        return NULL;
    }
    map->iterator=nodeGetKey(nodeGetNext(current_node));
    return map->iterator;
}
