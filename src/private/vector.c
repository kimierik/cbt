
#include "../utils/debug.c"
#include "../public/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




Vector makeVector(int elemsize){
    Vector a;
    a.capasity=10;
    a.size=0;
    a.elemSize=elemsize;
    a.elements= malloc(elemsize*a.capasity);

    return a;
}




void * popVector(Vector *stack){

    if (stack->size>0){
        //what
        void* ret= stack->elements + --stack->size *stack->elemSize; //?
        return ret;
    }

    printf("Stack Underflow attempring to pop from empty array\n");
    PrintStackTrace();
    exit(2);
}




void _IncreaseVectorCap(struct Vector* stack){

    stack->capasity*=2;
    //save
    void *l= malloc(stack->elemSize * stack->capasity);
    //move stack.list to l
    for (int i=0; i<stack->size;i++){
        //(*l)[i]=(*stack->list)[i];
        void * pos=stack->elements + i * stack->elemSize;
        memcpy( l + i * stack->elemSize, pos,  stack->elemSize );
    }
    free(stack->elements);//?
    stack->elements=l;
}



void* indexAtVector(Vector *vec,int i){
    //printf("attempting to index :%i \n",i);
    void* a= vec->elements + i*vec->elemSize;
    //printf("retrieved :%i=%c\n",a,a);
    return a;
}



//insert at position
void insertAtVector(Vector*vec,int index ,void* elem){
    //insert element at index
    //see if we need to increse capasity
    if (vec->size == vec->capasity){
        //inclrease stackcap
        _IncreaseVectorCap(vec);
    }

    //move all elements from index onward by one 
    //this needs to start at the latest element so we dont override anything by accident
    for(int i=vec->size-1; i>=0;i--){
        memcpy( (vec->elements+(vec->elemSize*i)) , (vec->elements+(vec->elemSize*i+1)), vec->elemSize );
    }
    memcpy(vec->elements+vec->elemSize*index, elem,vec->elemSize);
}



void pushVector(Vector* stack, void*elem){

    if (stack->size == stack->capasity){
        //inclrease stackcap
        _IncreaseVectorCap(stack);
    }

    //copy elem to elements + pos*elemsize
    //copy whatever is at the pointer elem to the list
    memcpy(stack->elements + stack->size*stack->elemSize , elem, stack->elemSize);
    stack->size++;
}



void destroyVector(Vector *stack){
    //ughhh
    //probably make a function that loops all elems and applies a function that is passed to them
    //this probably needs to free all enements not just the one
    free(stack->elements);
}

/*
 * used when pointers are stored on the stack
 * this function deallocates the pointers and the pointer array
 * 
 * */
void destroyPointerVector(Vector *stack, void(*ElementCleaningFunction)(void *) ){

    for(int i=stack->size-1 ;i>=0;i--){

        //casts element in the vector into a void pointer pointer
        //so a pointer that points to the pointer of whatever is stored?
        void** tok=((void**)(stack->elements+(i*stack->elemSize)));

        //free the stored element with the given function
        (*ElementCleaningFunction)(tok);
    }

    destroyVector(stack);
}


/*
 * print stack information
 * */
void printVector(struct Vector stack){
    printf("printingstack\n");
    printf("Vector size:%i, stack cap:%i, elemsize:%i \n", stack.size, stack.capasity, stack.elemSize);
    for(int i=0 ;i<stack.size;i++){
        int*p=((int*)(stack.elements+(i*stack.elemSize)));
        printf("index :%i is %i \n", i,*p );//shit
    }
}
