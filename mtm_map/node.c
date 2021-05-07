#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct Node_t
{
    char *key;
    char *data;
    struct Node_t *next;
};

Node nodeCreate();
void nodeDestroy(Node node);
char *nodeGet(Node node, const char *key);
Node nodeCopy(Node node);
int nodeGetSize(Node node);
NodeResult nodePut(Node node, const char *key, const char *data);
bool nodeContains(Node node, const char *kay);
NodeResult nodeClear(Node node);
NodeResult nodeRemove(Node node, const char *key);
static bool allocString(const char *str, char **returnStr);
Node getNodeByKey(Node node, const char *key);
static bool allocAndInsertNode(Node node, const char *key, const char *data);
static Node createNewNode(const char *key, const char *data);
static Node getForemerNodeByKey(Node node, const char *key);
static void NodeSwap(Node node1, Node node2);
static void nodeElementsDelete(Node node);
char* nodeGetKey(Node node);
char* nodeGetData(Node node);
Node nodeGetNext(Node node);
void nodeSetKey(Node node,char* key);
void nodeSetData(Node node,char *data);
void nodeSetNext(Node node, Node next);
static void destroyString(char *str);


Node nodeCreate() //create the first Node
{
    Node new_node = malloc(sizeof(*new_node));
    if (new_node == NULL)
    {
        return NULL;
    }
    new_node->key = NULL;
    new_node->data = NULL;
    new_node->next = NULL;
    return new_node;
}
/*
get a pointer to a node and free all the list
*/
void nodeDestroy(Node node) 
{
    while (node != NULL)
    {
        Node toDelete = node;
        nodeElementsDelete(toDelete);
        node = node->next;
        free(toDelete);
    }
}
/*
get a pointer to a node and free all of the node varibels, afterwards set them to NULL
*/
static void nodeElementsDelete(Node node)
{
    assert(node != NULL);
    destroyString(node->key);
    destroyString(node->data);
    node->key=NULL;
    node->data=NULL;
}

Node nodeCopy(Node node) 
{
    if (node != NULL) //if node exists
    {
        if (node->key != NULL) //if node is not empty
        {
            Node new_node = createNewNode((const char *)(node->key), (const char *)(node->data));
            if (new_node == NULL) //creat first node failed
            {
                return NULL;
            }
            node = node->next;              //already copied the first node
            Node node_to_return = new_node; //ptr to the first node of the new node
            while (node != NULL)
            {
                new_node->next = createNewNode((const char *)(node->key), (const char *)(node->data));
                if (new_node->next == NULL) //creating a node failed
                {
                    nodeDestroy(node_to_return);
                    return NULL;
                }
                node = node->next;
                new_node = new_node->next;
            }
            return node_to_return;
        }
        return nodeCreate(); //return empty node 
    }
    return NULL; //node does not exist
}

int nodeGetSize(Node node)
{
    int node_count = 0;
    while (node != NULL && node->key != NULL) //node may be empty, yet not null
    {
        node_count++;
        node = node->next;
    }
    return node_count;
}

bool nodeContains(Node node, const char *key)
{
   assert(node != NULL && key != NULL);
    if (getNodeByKey(node, key) == NULL)//if we got NULL there is no node with the given key
    {
        return false;
    }
    return true;
}

NodeResult nodePut(Node node, const char *key, const char *data)
{
    assert(node!=NULL);
    if (node->key == NULL) //node exsits but empty, need to allocate  the node, key and data
    {
        if (!allocAndInsertNode(node, key, data))
        {
            return NODE_OUT_OF_MEMORY;
        }
        return NODE_SUCCESS;
    }
    Node node_node_to_put = getNodeByKey(node, key); //if returned null node does not exists
    if (node_node_to_put != NULL)           //we found node matching the key so we want to overwrite its data
    {
        destroyString(node_node_to_put->data);
        char* str_return=malloc(strlen(data)+1);
        strcpy(str_return, data);//copy by value data
        node_node_to_put->data = str_return;
    }
    else //the node does not exits in the node, we want to creata a new one at the end
    {
        while (node->next != NULL)//progress node to be the last node
        {
            node = node->next;
        }
        node->next = createNewNode(key, data);
        if (node->next == NULL)//creating the new node failed
        {
            return NODE_OUT_OF_MEMORY;
        }
    }
    return NODE_SUCCESS;
}

char* nodeGet(Node node, const char *key)
{
    assert(key != NULL);
    if (node == NULL)
    {
        return NULL;
    }
    Node node_to_return_data = getNodeByKey(node, key);
    if (node_to_return_data == NULL)
    {
        return NULL;
    }
    return node_to_return_data->data;
}

NodeResult nodeRemove(Node node, const char *key)
{
    assert(key!=NULL);
    if (node == NULL)
    {
        return NODE_NULL_ARGUMENT;
    }
    Node node_to_remove = getNodeByKey(node, key);
    if (node_to_remove == NULL)
    {
        return NODE_ITEM_DOES_NOT_EXIST;
    }
    if (node->next == NULL) //node has only one node, it is the elemet we want to remove
    {
        nodeElementsDelete(node); //delete the node elements the node but does not delete it
        return NODE_SUCCESS;
    }
    Node former_node = getForemerNodeByKey(node, key); //node has more than on node
    if (former_node == NULL)//we want to remove the first node list has more then one node
    {
        NodeSwap(node, node->next);//swap the first and second node because we can't delete the first node
        node_to_remove = node->next;//create a pointer to the node we would like to delete (second node)
        node->next = node->next->next;//set the first node to point to the third node 
    }
    else
    {
        former_node->next = node_to_remove->next;//set the former node to point to the next node of the node we delete
    }
    node_to_remove->next = NULL;//make sure the node we delete not points to the list
    nodeDestroy(node_to_remove);
    return NODE_SUCCESS;
}

NodeResult nodeClear(Node node)
{
    assert(node!=NULL);
    while (node != NULL && node->key != NULL)
    {
        Node toRemove = node;
        nodeRemove(toRemove, (const char *)(toRemove->key));
    }
    return NODE_SUCCESS;
}
/*
gets two nodes and replace the pointers of key and data
*/
static void NodeSwap(Node node1, Node node2)
{
    assert(node1 != NULL && node2 != NULL);
    char *tmp = node1->key;
    node1->key = node2->key;
    node2->key = tmp;
    tmp = node1->data;
    node1->data = node2->data;
    node2->data = tmp;
}

/*
get a node and return the former node to the node pointing to the element with the specified given key
return NULL if the key mathch with the first node or no node has this key
*/
static Node getForemerNodeByKey(Node node, const char *key)
{
    assert(node != NULL);
    if (!strcmp((const char *)(node->key), key))//the first node is the node with the key we were looking
    {
        return NULL;// therefore there is no former node
    }
    while (node->next != NULL)
    {
        if (!strcmp((const char *)(node->next->key), key))
        {
            return node;
        }
        node = node->next;
    }
    return node;
}


Node getNodeByKey(Node node, const char *key)
{
    assert(key != NULL);
    while (node != NULL && node->key != NULL)//node may be null or empty
    {
        if (!strcmp((const char *)(node->key), key))
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

/*
gets a pointer to node key and data, create a copy (by value) to key and data and insert them to node
return false if allocation failed and destroy the node.
*/
static bool allocAndInsertNode(Node node, const char *key, const char *data)
{
    assert(key != NULL && data != NULL);
    free(node->key);
    free(node->data);
    if ((!allocString(key, &node->key)) || (!allocString(data, &node->data)))//if one of the allocations faild
    {
        nodeDestroy(node);
        return false;
    }
    strcpy(node->key, key);
    strcpy(node->data, data);
    return true;
}
/*get a string and a pointer to string, copy(by value) the string
return false i allocarion failed
*/
static bool allocString(const char *str, char **str_return)
{
    assert(str != NULL);
    *str_return = malloc(strlen(str) + 1);
    if (*str_return == NULL)
    {
        return false;
    }
    return true;
}

/*
gets a key and data and create a new node, if create went well returns a poiner to this node else NULL
*/
static Node createNewNode(const char *key, const char *data)
{
    Node new_node = nodeCreate();
    if (new_node == NULL)
    {
        return NULL;
    }
    if (!allocAndInsertNode(new_node, key, data))
    {
        nodeDestroy(new_node);
        return NULL;
    }
    return new_node;
}


char* nodeGetKey(Node node)
{
    if(node==NULL)
    {
        return NULL;
    }
    return node->key;
}

char* nodeGetData(Node node)
{
    if(node==NULL)
    {
        return NULL;
    }
    return node->data;
}

Node nodeGetNext(Node node)
{
    if(node==NULL)
    {
        return NULL;
    }
    return node->next;
}

void nodeSetKey(Node node,char* key)
{
    if(node!=NULL)
    {
        node->key=key;
    }
}
    
void nodeSetData(Node node,char *data)
{
    if(node!=NULL)
    {
        node->data=data;
    }
}

void nodeSetNext(Node node, Node next)
{
    if(node!=NULL)
    {
        node->next=next;
    }
}
static void destroyString(char *str)
{
    if (str != NULL)
    {
        free(str);
        str=NULL;
    }
}