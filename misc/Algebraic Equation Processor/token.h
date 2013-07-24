/* 
 * File:   token.h
 * Author: Cengiz
 *
 * Created on June 2, 2012, 12:53 AM
 */

#ifndef TOKEN_H
#define	TOKEN_H

#define INT_TOKEN 0
#define LPAREN_TOKEN 1
#define RPAREN_TOKEN 2
#define SIGN_TOKEN 3
#define EOS_TOKEN 4

typedef union tokentype{
    int decimal;
    char sign;
} TOKENTYPE ;

typedef struct token{
    char tokenType;
    TOKENTYPE type;
} TOKEN ;



#endif	/* TOKEN_H */

