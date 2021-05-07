#ifndef NODE_H_
#define NODE_H_

#include <stdbool.h>
#include <string.h>
/**
* Node Container
*
* Implements a node container type.
* The type of the key and the value is string (char *)
* The node has an internal iterator for external use. For all functions
* where the state of the iterator after calling that function is not stated,
* it is undefined. That is you cannot assume anything about it.
*
* The following functions are available:
*   nodeCreate		- Creates a new empty node
*   nodeDestroy		- Deletes an existing node and frees all resources
*   nodeCopy		- Copies an existing node
*   nodeGetSize		- Returns the size of a given node
*   nodeContains	- returns weather or not a key exists inside the node.
*   nodePut		    - Gives a specific key a given value.
*   				  If the key exists, the value is overridden.
*   nodeGet  	    - Returns the data paired to a key which matches the given key.
*					  Iterator status unchanged
*   nodeRemove		- Removes a pair of (key,data) elements for which the key
*                    matches a given element (using the strcmp function).
*   nodeGetFirst	- Sets the internal iterator to the first key in the
*   				  node, and returns it.
*   nodeGetNext		- Advances the internal iterator to the next key and
*   				  returns it.
*	 nodeClear		- Clears the contents of the node. Frees all the elements of
*	 				  the node using the free function.
* 	 NODE_FOREACH	- A macro for iterating over the node's elements.
*/

/** Type for defining the node */
typedef struct Node_t* Node;

/** Type used for returning error codes from node functions */
typedef enum NodeResult_t {
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARGUMENT,
    NODE_ITEM_ALREADY_EXISTS,
    NODE_ITEM_DOES_NOT_EXIST,
    NODE_ERROR
} NodeResult;

/**
* nodeCreate: Allocates a new empty node.
*
* @return
* 	NULL - if allocations failed.
* 	A new Node in case of success.
*/
Node nodeCreate();

/**
* nodeDestroy: Deallocates an existing node. Clears all elements.
*
* @param node - Target node to be deallocated. If node is NULL nothing will be
* 		done
*/
void nodeDestroy(Node node);

/**
* nodeCopy: Creates a copy of target node.
* Iterator values for both nodes is undefined after this operation.
*
* @param node - Target node.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Node containing the same elements as node otherwise.
*/
Node nodeCopy(Node node);

/**
* nodeGetSize: Returns the number of elements in a node
* @param node - The node which size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the node.
*/
int nodeGetSize(Node node);

/**
* nodeContains: Checks if a key element exists in the node. The key element will be
* considered in the node if one of the key elements in the node it determined equal
* using the strcmp function from string.h.
*
* @param node - The node to search in
* @param key - The key to look for.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true - if the key element was found in the node.
*/
bool nodeContains(Node node, const char* key);

/**
*	nodePut: Gives a specified key a specific value.
*  Iterator's value is undefined after this operation.
*
* @param node - The node for which to assign/reassign the data element
* @param keyElement - The key element which need to be assigned/reassigned.
*       A copy of the key element will be inserted.
* @param dataElement - The new data element to associate with the given key.
*      A copy of the data element will be inserted and old data memory would be deleted.
* @return
* 	NODE_NULL_ARGUMENT if one of the params is NULL
* 	NODE_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	NODE_SUCCESS the paired elements had been inserted successfully
*/
NodeResult nodePut(Node node, const char* key, const char* data);

/**
*	nodeGet: Returns the data associated with a specific key in the node(not a copy).
*			Iterator status unchanged
*
* @param node - The node for which to get the data element from.
* @param keyElement - The key element which need to be found and who's data
we want to get.
* @return
*  NULL if a NULL pointer was sent or if the node does not contain the requested key.
* 	A pointer to the data element associated with the key otherwise.
*/
char* nodeGet(Node node, const char* key);

/**
* 	nodeRemove: Removes a pair of key and data elements from the node. The elements
*  are found using the comparison function strcmp. Once found,
*  the elements are removed and deallocated.
*  Iterator's value is undefined after this operation.
*
* @param node -
* 	The node to remove the elements from.
* @param keyElement
* 	The key element to find and remove from the node. The key element will be freed.
* 	The data element associated with this key will also be freed.
* @return
* 	NODE_NULL_ARGUMENT if a NULL was sent to the function
*  NODE_ITEM_DOES_NOT_EXIST if an equal key item does not already exists in the node
* 	NODE_SUCCESS the paired elements had been removed successfully
*/
NodeResult nodeRemove(Node node, const char* key);

/*
looks for a node in the list by a given key
return a pointer to the node if it was found and return null if not
*/
Node getNodeByKey(Node node, const char *key);

/**
* nodeClear: Removes all key and data elements from target node.
* The elements are deallocated.
* @param node
* 	Target node to remove all element from.
* @return
* 	NODE_NULL_ARGUMENT - if a NULL pointer was sent.
* 	NODE_SUCCESS - Otherwise.
*/
NodeResult nodeClear(Node node);
/*
gets a node and return a pointer to key
*/
char* nodeGetKey(Node node);
/*
gets a node and return a pointer to data
*/
char* nodeGetData(Node node);
/*
gets a node and return a pointer to next node
*/
Node nodeGetNext(Node node);
/*
set the given key to the node
*/
void nodeSetKey(Node node,char* key);
/*
set the given data to the node
*/
void nodeSetData(Node node,char *data);
/*
set the given pointer to the next node to the current node
*/
void nodeSetNext(Node node, Node next);

/*!
* Macro for iterating over a node.
* Declares a new iterator for the loop.
*/

#endif /* NODE_H_ */
