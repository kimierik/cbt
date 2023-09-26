#ifndef VECTOR
#define VECTOR




//vector 
//all elements are treated as void pointers
typedef struct Vector{
    void* elements;
    int size;
    int elemSize;
    int capasity;

}Vector;


//function used to make a vector please use this instead of making one manually
//elemsize is the size of the element that is being stored in this vector
Vector makeVector(int elemsize);


//push
void pushVector(Vector* stack, void*elem);
//pop
void * popVector(Vector *stack);

/*
 * print stack information
 * */
void printVector(struct Vector stack);

//destroy vector
void destroyVector(Vector *stack);

/*
 * used when pointers are stored on the stack
 * this function deallocates the pointers with the ElementCleaningFunction 
 * 
 * */
void destroyPointerVector(Vector *stack, void(*ElementCleaningFunction)(void *) );

//internal function that increases the cap of the vector
void _IncreaseVectorCap(struct Vector* stack);




#endif
