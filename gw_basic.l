%{
#include <stdlib.h>
#include <string.h>
#include "gw_basic.h"
#include "y.tab.h"
void yyerror(char *);
%}
%%
"while" {return WHILE;}
"if" {return IF;}
"else" {return ELSE;}
"print" {return PRINT;} 
"run" {return RUN;}
"wend" {return WEND;}
"then" { return THEN;}
"and" {return AND;}
"or" {return OR;}
[a-zA-Z_][0-9a-zA-Z_]*[$%#] {
    yylval.s = strdup(yytext);
    return VARIABLE;
}
[0-9]+[.][0-9]* {
    yylval.fValue = atof(yytext);
    return FLOAT;
}


[0-9]+[eE][+-][0-9]* {
    yylval.fValue = atof(yytext);
    return FLOAT;
}
[0-9]* {
    yylval.iValue = atoi(yytext);
    return INTEGER;
}
\"[^"\n]*["\n] {
    yylval.s = strdup(yytext+1);
    if (yylval.s[yyleng-2] != '"')
        printf("improperly terminated string");
    else
        yylval.s[yyleng-2] = 0; 
    return STRING;
}
[-()<>=+*/,\n] {
    return *yytext;
}


">=" {return GE;}
"<=" {return LE;}
"==" {return EQ;}
"<>" {return NE;}
[ \t]+  /* ignore whitespace */
. {yyerror("Unknown character"); }
%%


int yywrap(void)
{
    return 1;
}