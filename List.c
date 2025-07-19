/***
* Andrew Byi
* abyi
* 2024 Winter CSE101 PA4
* List.c
* List that can be modified by functions
***/ 

#include "List.h"

typedef struct NodeObj * Node;

typedef struct NodeObj
{
    void* data;
    Node next;
    Node previous;
}NodeObj;

typedef struct ListObj
{
    Node front; //element in the front
    Node back;  //element in the back
    Node cursor; //points at the node current on
    int length; //number of elements in array
    int index; //the order value of current element
} ListObj;

Node newNode(void* data) //Node constructor
{
    Node N = malloc(sizeof(NodeObj));
    assert(N != NULL);
    N->data = data;
    N->next = NULL;
    N->previous = NULL;
    return(N);
}
void freeNode(Node* pN) //Node destructor
{
    if(pN != NULL && *pN != NULL)
    {
        
        free(*pN);
        *pN = NULL;
    }
}
List newList() //List constructor
{
    List L;
    L = malloc(sizeof(ListObj));
    assert(L != NULL);
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1;
    return(L);
}
void freeList(List * pL){ //List destructor
    if(pL != NULL && *pL != NULL)
    {
        if(length(*pL) > 0)
        {
            clear(*pL);
        }
        free(*pL);
        *pL = NULL;
    }
}
int length(List L){
    if(L == NULL){
        printf("List Error: at Length(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(L->length > -1)
    {
        return(L->length);
    }
    else
    {
        return 0;
    }
}
//cursor element
int index(List L){

    if(L == NULL){
        printf("List Error: at index(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if((L->index <= L->length) && (L->index >= 0))
    {
        if(L->cursor != NULL)
        {
            return L->index;
        }
    }
    L->index = -1;
    return L->index;
    // Returns index of cursor element if defined, -1 otherwise.
}
void* front(List L){
    if(L == NULL){
        printf("List Error: at front(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at front(), empty list ");
        exit(EXIT_FAILURE);
    }
    
    return(L->front->data);
    // Returns front element of L. Pre: length()>0
}
void* back(List L){ 
    if(L == NULL){
        printf("List Error: at back(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at back(), empty list ");
        exit(EXIT_FAILURE);
    }
    return(L->back->data);
    // Returns back element of L. Pre: length()>0
}
void* get(List L){
    if(L == NULL){
        printf("List Error: at get(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at get(), empty list ");
        exit(EXIT_FAILURE);
    }
    if(index(L) < 0)
    {
        printf("List Error: at get(), index out of bounds");
        exit(EXIT_FAILURE);
    }
    return L->cursor->data;
    // Returns cursor element of L. Pre: length()>0, index()>=0
}
void clear(List L){
    while(length(L) > 0)
    {
        deleteFront(L);
    }
    L->index = -1;
    L->cursor = NULL;
    // Resets L to its original empty state.
}
void set(List L, void* x) //
{
    if(L == NULL){
        printf("List Error: at set(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at set(), empty List");
        exit(EXIT_FAILURE);
    }
    if(index(L) != -1)
    {
        L->cursor->data = x;
    }
    // Overwrites the cursor element’s data with x. 
    // Pre: length()>0, index()>=0
}
void moveFront(List L){ //
    if(L == NULL){
        printf("List Error: at moveFront(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at moveFront(), empty List");
        exit(EXIT_FAILURE);
    }
    L->cursor = L->front;
    L->index = 0;
    // If L is non-empty, sets cursor under the front element,
    // otherwise does nothing.
}
void moveBack(List L){ //
    if(L == NULL){ 
        printf("List Error: at moveBack(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at moveBack(), empty List");
        exit(EXIT_FAILURE);
    }
    L->cursor = L->back;
    L->index = length(L) - 1;
    // If L is non-empty, sets cursor under the back element,
    // otherwise does nothing.
}
void movePrev(List L){ 
    if(L == NULL){
        printf("List Error: at movePrev(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at movePrev(), wrong List length");
        exit(EXIT_FAILURE);
    }
    if(L->cursor == NULL)
    {
        printf("List Error: at movePrev(), null List pointer");
        exit(EXIT_FAILURE);
    }
    
    if(L->cursor == L->front)
    {
        L->index = -1;
        L->cursor = NULL;
    }
    else if(index(L) != -1)
    {
        List hold = newList();//new ListObj to hold current cursor
        hold->cursor = L->cursor;
        L->cursor = L->cursor->previous;
        L->index--;
        freeList(&hold);
    }
    
    // If cursor is defined and not at front, move cursor one 
    // step toward the front of L; if cursor is defined and at 
    // front, cursor becomes undefined; if cursor is undefined 
    // do noth
}
void moveNext(List L)
{
    if(L == NULL){
        printf("List Error: at moveNext(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at moveNext(), wrong List length");
        exit(EXIT_FAILURE);
    }
    if(index(L) == -1)
    {
        printf("List Error: at moveNext(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(L->cursor == L->back)
    {
        L->index = -1;
        L->cursor = NULL;
    }
    else if(index(L) != -1)
    {
        List hold = newList();//new ListObj to hold current cursor
        hold->cursor = L->cursor;
        L->cursor = L->cursor->next;
        L->index++;
        freeList(&hold);
    }
    // If cursor is defined and not at back, move cursor one 
    // step toward the back of L; if cursor is defined and at 
    // back, cursor becomes undefined; if cursor is undefined 
    // do nothing
}
void prepend(List L, void* x){ 
    // Insert new element into L. If L is non-empty, 
    // insertion takes place before front element.
    
    if(L == NULL){
        printf("List Error: at append(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(L->length == 0)
    {
        Node N = newNode(x);
        L->front = L->back = N; //set the front of the list 
    }
    else
    {
        Node N = newNode(x);
        N->next = L->front;
        L->front->previous = N;
        L->front = N;
    }
    if(index(L) != -1)
    {
        L->index++;
    }
    L->length++;
}
void append(List L, void* x){ 
    
    if(L == NULL){
        printf("List Error: at append(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(L->length == 0)
    {
        Node N = newNode(x);
        L->front = L->back = N; //set the front of the list 
    }
    else
    {
        Node N = newNode(x);
        N->previous = L->back;
        L->back->next = N;
        L->back = N;
    }
    L->length++;
}
void insertBefore(List L, void* x){ //

    if(L == NULL){
        printf("List Error: at insertBefore(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at insertBefore(), empty List");
        exit(EXIT_FAILURE);
    }
    if(index(L) == -1)
    {
        printf("List Error: at insertBefore(), NULL List node");
        exit(EXIT_FAILURE);
    }
    Node hold = newNode(x);
    hold->next = L->cursor; //have the new element next point to cursor
    if(L->cursor->previous != NULL)
    {
        L->cursor->previous->next = hold; //set the cursor's previous node point to hold
    }
    hold->previous = L->cursor->previous; //set the new node's previous point to cursor's previous
    L->cursor->previous = hold; //have cursor's previous point to held node
    if(L->front->previous != NULL)
    {
        L->front = L->front->previous;
    }
    L->length++;
    L->index++;
}
void insertAfter(List L, void* x){
    
    
    if(L == NULL){
        printf("List Error: at insertAfter(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at insertAfter(), empty List");
        exit(EXIT_FAILURE);
    }
    if(index(L) == -1)
    {
        printf("List Error: at insertAfter(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(L->cursor == NULL)
    {
        printf("List Error: at insertAfter(), NULL List cursor");
        exit(EXIT_FAILURE);
    }
    //printf("%s%d", "cursor: ", L->cursor->data);
    //printf("%s%p\n", ". next: ", L->cursor->next);
    if(L->cursor->next != NULL)
    {
        Node hold = newNode(x);
        hold->next = L->cursor->next; //have the new element hold list after cursor
        L->cursor->next->previous = hold; //set the next node's previous point to hold
        hold->previous = L->cursor; //have the cursor be hold's previous
        L->cursor->next = hold; //after cursor be switched to hold
    }
    else
    {
        append(L, x);
        L->length--;
    }
    L->length++;
}
void deleteFront(List L){ 
    Node N = NULL; //to hold the node so it can be freed
    if(L == NULL){
        printf("List Error: at deleteFront(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at deleteFront(), empty List");
        exit(EXIT_FAILURE);
    }
    N = L->front; //set N to the node so it can be freed
    if(length(L) > 1 && L->front != NULL)
    {
        //printf("%s%d\n", "next to front: ", L->front->next->data);
        L->front = L->front->next;
        L->front->previous = NULL;
    }
    else
    {
        L->front = L->back = NULL;
    }
    if(index(L) >= 0)
    {
        L->index--;
    }
    freeNode(&N);
    L->length--;
}
void deleteBack(List L){ 
    Node N = NULL; //to hold the node so it can be freed
    if(L == NULL){
        printf("List Error: at deleteBack(), NULL List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at deleteBack(), empty List");
        exit(EXIT_FAILURE);
    }
    N = L->back; //set N to the node so it can be freed
    if(length(L) > 1 && L->back != NULL)
    {
        L->back = L->back->previous;
        L->back->next = NULL;
    }
    else
    {
        L->back = L->front = NULL;
    }
    if(index(L) == (length(L) - 1))
    {
        L->index = -1;
    }
    freeNode(&N);
    //printf("%s%p\n", "should be null N: ", N);
    L->length--;
}
void delete(List L){ 
    if(L == NULL){
        printf("List Error: at delete(), NULL List");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at delete(), empty List");
        exit(EXIT_FAILURE);
    }
    if(index(L) < 0)
    {
        printf("List Error: at delete(), empty List");
        exit(EXIT_FAILURE);
    }
    if(L->cursor == NULL)
    {
        printf("List Error: at delete(), NULL List cursor");
        exit(EXIT_FAILURE);
    }
    Node hold = NULL;
    hold = L->cursor;
    //printf("%s%d\n", "cursor: ", L->cursor->data);
    //printf("%s%p\n", "L->cursor->previous should be : ", L->cursor->previous);
    //printf("%s%p\n", "L->cursor->next should be : ", L->cursor->next);
    if(L->cursor == L->front)
    {
        L->front = L->front->next;
    }
    else if(L->cursor == L->back)
    {
        L->back = L->back->previous;
    }
    else if(L->cursor->previous != NULL && L->cursor->next != NULL)
    {
        L->cursor->previous->next = L->cursor->next;
        L->cursor->next->previous = L->cursor->previous;
    }
    freeNode(&hold);
    L->index = -1;
    L->length--;
}

/*
void printList(FILE* out, List L){  ////////
    if(L == NULL){
        printf("Queue Error: at printList(), Null List node");
        exit(EXIT_FAILURE);
    }
    if(length(L) <= 0){
        printf("List Error: at delete(), empty List");
        exit(EXIT_FAILURE);
    }
    Node N = NULL;
    N = L->front;
    while(N != NULL)
    {
        fprintf(out, "%d%s", N->data, " ");
        N = N->next;
    }
    fprintf(out, "\n");
}*/
/*
List concatList(List A, List B){
// Returns a new List which is the concatenation of 
 // A and B. The cursor in the new List is undefined,
 // regardless of the states of the cursors in A and B.
 // The states of A and B are unchanged.
}
*/