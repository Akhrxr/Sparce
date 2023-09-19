/***************************************************************************************
* Akhror Rakhmatov, arakhmat@ucsc.edu, PA4
*  List.h
*  Private section for List ADT
*  Used Queue.h from examples
****************************************************************************************/
#include<stdbool.h>
#ifndef List_H_INCLUDE_
#define List_H_INCLUDE_


#define FORMAT "%p"  // format string for ListElement


// Exported types -------------------------------------------------------------
typedef struct ListObj* List;


// Constructors-Destructors ---------------------------------------------------

// newList()
// Returns reference to new empty List object. 
List newList();

// freeList()
// Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL);


// Access functions -----------------------------------------------------------

// isEmpty()
// Returns true if Q is empty, otherwise returns false.
bool isEmpty(List L);

// Length()
// Returns the length of L.
int length(List L);

// Index()
// Returns index of cursor element if defined, -1 otherwise.
int index(List L);

// front()
// Returns front element of L. Pre: length()>0
void* front(List L);

// back()
// Returns back element of L. Pre: length()>0
void* back(List L);

// get() 
// Returns cursor element of L. Pre: length()>0, index()>=0
void* get(List L);

// equals()
// Returns true if A is same int sequence as B, false otherwise.
//bool equals(List A, List B);

// Manipulation procedures ----------------------------------------------------

// clear() 
// Resets L to its original empty state.
void clear(List L);

// set()
// Overwrites the cursor element’s data with x.
// Pre: length()>0, index()>=0
void set(List L, void* x);

// moveFront()
// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L);

// moveBack()
// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L);

// movePrev()
// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L);

// moveNext()
// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L);

// prepend()
// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, void* x);

// append()
// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, void* x);

// insertBefore()
// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, void* x);

// insertAfter()
// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, void* x);

// deleteFront()
// Delete the front element. Pre: length()>0
void deleteFront(List L);

// deleteBack()
// Delete the back element. Pre: length()>0
void deleteBack(List L); 

// delete()
// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L);



// Other Functions ------------------------------------------------------------

// printList()
// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.

void printList(FILE* out, List L); 

// copyList()
// Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.
List copyList(List L);

#endif