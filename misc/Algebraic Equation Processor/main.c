/* 
 * File:   main.c
 * Author: Cengiz
 *
 * Created on June 2, 2012, 12:52 AM
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "token.h"
#include "PostfixConvertor.h"

#define STATE0 0
#define STATE1 1

char compareSign(char a, char b){
    if (b=='(') return 1;
    if (a=='(') return 1;
    if (a=='+' && b=='/') return 1;
    if (a=='+' && b=='*') return 1;
    if (a=='-' && b=='/') return 1;
    if (a=='-' && b=='*') return 1;
    return 0;
}

void test1(){
    EXPR expr;
    
    Einit(&expr, "123 / 456 * ( 6 + 4 )");
    
    getNextToken(&expr);
    while(globalToken.tokenType!=EOS_TOKEN){
        if (globalToken.tokenType==INT_TOKEN){
            printf("%d\n", globalToken.type.decimal);
        }
        else if (globalToken.tokenType==SIGN_TOKEN || globalToken.tokenType==LPAREN_TOKEN || globalToken.tokenType==RPAREN_TOKEN){
            printf("%c\n", globalToken.type.sign);
        }
        getNextToken(&expr);
    }
}

int main(int argc, char** argv) {
        
    char exprstr[100];
    char postfix[100]={0};
    char itoabuffer[100];
    STACK stack;
    Sinit(&stack);
    
    printf("Aritmetik ifadeyi giriniz : ");
    gets(exprstr);
    
    EXPR expr;
    Einit(&expr, &exprstr[0]);
    
    getNextToken(&expr);
    char state=0;
    int parencount=0;
    while(globalToken.tokenType!=EOS_TOKEN){
        if (state==STATE0){
            if (globalToken.tokenType==INT_TOKEN){
                state=STATE1;
                sprintf(itoabuffer, "%d", globalToken.type.decimal);
                strcat(postfix, itoabuffer);
                strcat(postfix, " ");
            }
            else if (globalToken.tokenType==LPAREN_TOKEN){
                state=STATE0;
                Spush(&stack, (int)globalToken.type.sign);
                parencount++;
            }
            else{
                printf("Girdi hatali.\n");
                goto end;
            }
        }
        else if(state==STATE1){
            if (globalToken.tokenType==RPAREN_TOKEN){
                if (parencount==0){
                    printf("Girdi hatali.\n");
                    goto end;
                }
                parencount--;
                state=STATE1;
                char tempchr=(char)Spop(&stack);
                while (tempchr!='('){
                    postfix[strlen(postfix)+1]=' ';
                    postfix[strlen(postfix)+2]=0;
                    postfix[strlen(postfix)]=tempchr;
                    tempchr=(char)Spop(&stack);
                }
            }
            else if (globalToken.tokenType==SIGN_TOKEN){
                state=STATE0;
                if(SisEmpty(&stack)){
                    Spush(&stack, (int) globalToken.type.sign);
                }
                else{
                    while(!SisEmpty(&stack)){
                        if(compareSign((char)Speek(&stack), globalToken.type.sign )){
                            Spush(&stack, (int) globalToken.type.sign);
                            break;
                        }
                        else{
                            postfix[strlen(postfix)+1]=' ';
                            postfix[strlen(postfix)+2]=0;
                            postfix[strlen(postfix)]=(char) Spop(&stack);
                        }  
                    }
                    if (SisEmpty(&stack)){
                        Spush(&stack, (int) globalToken.type.sign);
                    }
                }
            }
            else {
                printf("Girdi hatali.\n");
                goto end;
            }
        }
        getNextToken(&expr);
    }

    while (!SisEmpty(&stack)){
        postfix[strlen(postfix)+1]=' ';
        postfix[strlen(postfix)+2]=0;
        postfix[strlen(postfix)]=(char) Spop(&stack);
    }
    
    postfix[strlen(postfix)-1]=0;
    
    printf("\nPostfix : %s\n", postfix);
    
    Einit(&expr, &postfix[0]);
    Sinit(&stack);
    
    getNextToken(&expr);
    while(globalToken.tokenType!=EOS_TOKEN){
        if (globalToken.tokenType==INT_TOKEN){
            Spush(&stack, globalToken.type.decimal);
        }
        else if (globalToken.tokenType==SIGN_TOKEN){
            int num1=Spop(&stack);
            int num2=Spop(&stack);
            switch (globalToken.type.sign){
                case '+':
                    Spush(&stack, num2+num1);
                    break;
                case '-':
                    Spush(&stack, num2-num1);
                    break;
                case '*':
                    Spush(&stack, num2*num1);
                    break;
                case '/':
                    Spush(&stack, num2/num1);
                    break;
            }
        }
        getNextToken(&expr);
    }
    
    printf("\nIslem sonucu : %d\n", Spop(&stack));

    end:
    return 0;
    
    
}

