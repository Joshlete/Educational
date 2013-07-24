/* 
 * File:   stack.h
 * Author: Cengiz
 *
 * Created on June 2, 2012, 12:25 PM
 */

#ifndef STACK_H
#define	STACK_H

#define STACK_SIZE 256

typedef struct stack {
    int stackpointer;
    int datablock[STACK_SIZE];
} STACK ;

void Sinit(STACK* stack){
    stack->stackpointer=0;
}

char SisFull(STACK* stack){
    if (stack->stackpointer==STACK_SIZE){
        return 1;
    }
    else{
        return 0;
    }
}

char SisEmpty(STACK* stack){
    if (stack->stackpointer==0){
        return 1;
    }
    else{
        return 0;
    }
}

void Spush(STACK* stack, int element){
    if (!SisFull(stack)){
        stack->datablock[stack->stackpointer++]=element;
    }
    else{
        printf("Stack dolu\n");
    }
}

int Spop(STACK* stack){
    if (!SisEmpty(stack)){
        return stack->datablock[--stack->stackpointer];
    }
    else{
        return 0;
    }
}

int Speek(STACK* stack){
    if (!SisEmpty(stack)){
        return stack->datablock[stack->stackpointer-1];
    }
    else{
        printf("Stack bos\n");
        return 0;
    }
}

#endif	/* STACK_H */

