/* 
 * File:   PostfixConvertor.h
 * Author: Cengiz
 *
 * Created on June 2, 2012, 12:56 PM
 */

#ifndef POSTFIXCONVERTOR_H
#define	POSTFIXCONVERTOR_H

typedef struct pstfx{
    char* string;
    int indexPointer;
} EXPR ;

void Einit(EXPR* expr, char* str){
    expr->string=str;
    expr->indexPointer=0;
}

TOKEN globalToken;

char getNextToken(EXPR* expression){
    if (expression->indexPointer!=0 && expression->string[expression->indexPointer-1]==0){
        globalToken.tokenType=EOS_TOKEN;
        return 0;
    }
    char buffer[15];
    char currentChar=expression->string[expression->indexPointer++];
    int bufferIndex=0;
    
    switch(currentChar){
        case '+':
            globalToken.tokenType=SIGN_TOKEN;
            globalToken.type.sign='+';
            expression->indexPointer++;
            return 1;
        case '-':
            globalToken.tokenType=SIGN_TOKEN;
            globalToken.type.sign='-';
            expression->indexPointer++;
            return 1;
        case '*':
            globalToken.tokenType=SIGN_TOKEN;
            globalToken.type.sign='*';
            expression->indexPointer++;
            return 1;
        case '/':
            globalToken.tokenType=SIGN_TOKEN;
            globalToken.type.sign='/';
            expression->indexPointer++;
            return 1;
        case ')':
            globalToken.tokenType=RPAREN_TOKEN;
            globalToken.type.sign=')';
            expression->indexPointer++;
            return 1;  
        case '(':
            globalToken.tokenType=LPAREN_TOKEN;
            globalToken.type.sign='(';
            expression->indexPointer++;
            return 1;
            
        default:            
            while(currentChar!=' ' && currentChar!=0){
                if (currentChar>='0' && currentChar<='9'){
                    buffer[bufferIndex++]=currentChar;
                    currentChar=expression->string[expression->indexPointer++];
                }
                else{
                    printf("Giris dogru girilmedi : %c\n", currentChar);
                    return 0;
                } 
            }
            
            buffer[bufferIndex]=0;
            globalToken.tokenType=INT_TOKEN;
            globalToken.type.decimal=atoi(buffer);
            return 1;
    }
}


#endif	/* POSTFIXCONVERTOR_H */

