#ifndef MTM_ASSIST_H
#define MTM_ASSIST_H
/*
typdef for votes update operation (adding or removing)
*/
typedef int (*UpdateVotesCondition)(int, int);
/*
gets a pointer to a string and disallocates it
*/
void destroyString(char *str);
/*
allocates a string in the length+1 of the given int also add '\0' in the last cell
return a pointer to the string if allocation failed return NULL
*/
char *createString(int length);
/*
get the number of votes and add the given number
*/
int addVotes(int votes, int votes_to_add);
/*
get the number of votes and remove the given number if votes
became a negative number return 0
*/
int removeVotes(int votes, int votes_to_remove);
/*
get a number and return a string of the number if allocation failed return NULL
*/
char *intToString(int number);
/*
gets a string that contains only numbers and return an int
*/
int stringToInt(const char *str);

#endif //MTM_ASSIST_H

