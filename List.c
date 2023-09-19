/****************************************************************************************
* Akhror Rakhmatov, arakhmat@ucsc.edu, PA4
*  List.c
*  Public section for List ADT
*  Used Queue.c from examples
*****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include"List.h"


// structs --------------------------------------------------------------------

// private Node type
typedef struct NodeObj* Node;

// private NodeObj type
typedef struct NodeObj{
   void* data;
   Node next;
   Node prev;
} NodeObj;

// private ListObj type
typedef struct ListObj{
   Node cursor;
   Node front;
   Node back;
   int length;
   int index;
} ListObj;



// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
Node newNode(void* data){
   Node N = malloc(sizeof(NodeObj));
   assert( N!=NULL );
   N->data = data;
   N->next = NULL;
   return(N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

// newList()
// Returns reference to new empty List object.
List newList(){
   List L;
   L = malloc(sizeof(ListObj));
   assert( L!=NULL );
   L->front = L->back = NULL;
   L->length = 0;
   L->index = -1;
   return(L);
}

// freeList()
// Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL){
   if(pL!=NULL && *pL!=NULL) { 
      while( !isEmpty(*pL) ) { 
         deleteFront(*pL); 
      }
      free(*pL);
      *pL = NULL;
   }
}


// Access functions -----------------------------------------------------------

// isEmpty()
// Returns true if Q is empty, otherwise returns false.
bool isEmpty(List L){
   if( L==NULL ){
      printf("List Error: calling isEmpty() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   return(L->length==0);
}

// Length()
// Returns the length of L.
int length(List L){
   if( L==NULL ){
      printf("List Error: calling length() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   return(L->length);
}

// Index()
// Returns index of cursor element if defined, -1 otherwise.
int index(List L) {
   if( L==NULL ){
      return -1;
   }
   return (L->index);
}

bool checkFront(List L) {
   return L->front == NULL;
}


// front()
// Returns the value at the front of L.
// Pre: length()>0
void* front(List L){
   if( L==NULL ){
      printf("List Error: calling Front() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( isEmpty(L) ){
      printf("List Error: calling Front() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   return(L->front->data);
}

// back()
// Returns back element of L. Pre: length()>0
void* back(List L) {
   if( L==NULL ){
      printf("List Error: calling back() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( isEmpty(L) ){
      printf("List Error: calling back() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   return(L->back->data);
}

// get() 
// Returns cursor element of L. Pre: length()>0, index()>=0
void* get(List L) {
   if( L==NULL ){
      printf("List Error: calling get() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( isEmpty(L) ){
      printf("List Error: calling get() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   if( index(L) == -1){
      printf("List Error: Cursor undefined\n");
      exit(EXIT_FAILURE);
   }
   return(L->cursor->data);
}


// equals()
// Returns true if A is same int sequence as B, false otherwise.
/*bool equals(List A, List B){
   if( A==NULL || B==NULL ){
      printf("List Error: calling equals() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }

   bool eL;
   Node N, M;

   eL = ( A->length == B->length );
   N = A->front;
   M = B->front;
   while( eL && N!=NULL){
      eL = ( N->data==M->data );
      N = N->next;
      M = M->next;
   }
   return eL;
}
*/

// Manipulation procedures ----------------------------------------------------

// clear() 
// Resets L to its original empty state.
void clear(List L) {
   while (L->length != 0) {
      deleteFront(L);
   }
}

// set()
// Overwrites the cursor element’s data with x.
// Pre: length()>0, index()>=0
void set(List L, void* x) {
   if(L->length > 0 && L->index >= 0) {
      L->cursor->data = x;
   }
}

// moveFront()
// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L){
   if( isEmpty(L) ){
      printf("List Error: calling moveFront() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   L->cursor = L->front;
   L->index = 0;
}

// moveBack()
// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L) {
   if( isEmpty(L) ){
      printf("List Error: calling moveBack() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   L->cursor = L->back;
   int temp = L->length;
   L->index = temp - 1;
}

// movePrev()
// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L) {
   if (index(L) == -1) {
      L->index = -1;
   }
   if (index(L) == 0) {
      L->index = -1;
   }
   if (index(L) > 0) {
      L->cursor = L->cursor->prev;
      L->index--;
   }
}

// moveNext()
// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L) {
   if (index(L) == -1) {
      L->index = -1;
   }  
   else if (index(L) < length(L) - 1 && index(L) != -1) {
      L->cursor = L->cursor->next;
      L->index++;
   }
   else if (index(L) == length(L) - 1) {
      L->index = -1;
   }
}

// prepend()
// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, void* x) {
   Node N = newNode(x);

   if( L==NULL ){
      printf("List Error: calling prepend() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   
   if( isEmpty(L) ) { 
      L->front = L->back = N;
   }else{ 
      N->next = L->front;
      L->front->prev = N; 
      L->front = N;
      L->index++;
      }
   L->length++;
}

// append()
// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, void* x)
{
   Node N = newNode(x);

   if( L==NULL ){
      printf("List Error: calling append() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   
   if( isEmpty(L) ) { 
      L->front = L->back = N;
   }else{
      L->back->next = N;
      N->prev = L->back;
      if(index(L) == length(L) - 1) {
         L->cursor = L->back;
      }
      L->back = N;
   }
   L->length++;
}

// insertBefore()
// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, void* x) {
   if( L==NULL ){
      printf("List Error: calling append() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if(L->length > 0 && index(L) > 0) { 
      Node N = newNode(x);
      L->cursor->prev->next = N;
      N->prev = L->cursor->prev;
      L->cursor->prev = N;
      N->next = L->cursor;
      L->length++;
      L->index++;
   }
   else {
      prepend(L, x);
   }
}

// insertAfter()
// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, void* x) { 
   if( L==NULL ){
      printf("List Error: calling InsertAfter() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if(index(L) >= 0) { 
      if (L->cursor->data == L->back->data) {
         append(L, x);
      } else {
         Node N = newNode(x);
         L->cursor->next->prev = N;
         N->next =  L->cursor->next;
         L->cursor->next = N;
         N->prev = L->cursor;
         L->length++;
      }
   }
}


// deleteFront()
// Delete the front element. Pre: length()>0
void deleteFront(List L) {
   Node N = NULL;

   if( L==NULL ){
      printf("List Error: calling deleteFront() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( isEmpty(L) ){
      printf("List Error: calling deleteFront() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   if (index(L) >= 0) {
         L->index--;
   }
   N = L->front;
   if( length(L)>1 ) { 
      L->front->next->prev = NULL;
      L->front = L->front->next;
   }else{ 
      L->front = L->back = NULL; 
   }
   L->length--;
   freeNode(&N);
}

// deleteBack()
// Delete the back element. Pre: length()>0
void deleteBack(List L) {
   Node N = NULL;

   if( L==NULL ){
      printf("List Error: calling deleteBack() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( isEmpty(L) ){
      printf("List Error: calling deleteBack() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   N = L->back;
   if (index(L) == length(L) -1) {
      L->index = -1;
   }
   if( length(L)>1 ) {
      L->back->prev->next = NULL;
      L->back = L->back->prev;
   }
   if (length(L) == 1){ 
      L->front = L->back = NULL; 
   }
   L->length--;
   freeNode(&N);
}

// delete()
// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L) {
   Node N = NULL;
   if (index(L) < 0) {
      printf("List Error: calling delete() on undefined reference\n");
      exit(EXIT_FAILURE);
   }
   if( L==NULL ){
      printf("List Error: calling delete() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( isEmpty(L) ){
      printf("List Error: calling delete() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   N = L->cursor;
   if (L->cursor) {
      if (index(L) == length(L) - 1) {
         deleteBack(L);
      }
   }
   if (L->cursor) {
      if (index(L) == 0) {
         deleteFront(L);
      }
   }
   if (L->cursor) {
      if (index(L) > 0 && index(L) < length(L) - 1) {
         L->cursor->prev->next = L->cursor->next;
         L->cursor->next->prev = L->cursor->prev;
         L->length--;
         L->index = -1;
         freeNode(&N);
      }
   }
}

// Other Functions ------------------------------------------------------------

// printList()
// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
void printList(FILE* out, List L) {
   Node N = NULL;

   if( L==NULL ){
      printf("List Error: calling printList() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }

   for(N = L->front; N != NULL; N = N->next){
      printf(FORMAT" ", N->data);
   }
   fprintf(out, "\n");
}


// copyList()
// Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.
List copyList(List L) {
   List Q;
   Q = malloc(sizeof(ListObj));
   assert( Q!=NULL );
   Q->front = Q->back = NULL;
   Q->length = 0;
   Q->index = -1;
   for (Node tempp = L->front; tempp; tempp = tempp->next) {
      append(Q, tempp->data);
   }
   return(Q);
}