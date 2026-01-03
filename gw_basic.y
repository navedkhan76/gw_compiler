%{
    #include<stdio.h>
    #include <string.h>
    #include<stdlib.h>
    #include<stdarg.h>
    // #include "gw_basic.h"
    #include "compiler_2.h"
    typeNode *opr(int oper, int nops, ...);
    typeNode *id(char*s);
    typeNode *con(int iValue, float fValue, char* sValue,int type);
    typeNode *intr(int value);
    typeNode *flt(float value);
    typeNode *st(char* s);
    int lookIntTable(char* s);
    int lookStringTable(char* s);
    int lookFloatTable(char* s);
    void freeNode(typeNode *p);
    int tree_walk(typeNode *p);
    int yylex(void);
    void yyerror(char* s);
    char integers[100][40];
    char strings[100][40];
    char floats[100][40];
    int symInt[100];
    float symFloat[100];
    char symString[100][100];
    %}


    %union{
        int iValue;
        float fValue;
        char *s;
        typeNode *nptr;
    }
    %token<s> VARIABLE
    %token <iValue> INTEGER
    %token <fValue> FLOAT
    %token <s> STRING
    %token WHILE IF PRINT RUN WEND THEN
    %nonassoc IFX
    %nonassoc ELSE


    %left OR AND
    %left GE LE EQ NE '>' '<'
    %left '+' '-'
    %left '*' '/'
    %nonassoc UMINUS


    %type<nptr> stmt expr stmt_list




    %%
    program: function {}


    function: function stmt {tree_walk($2); freeNode($2);}
    |
    ;
    stmt: 
        expr '\n' {$$=$1;}
        | PRINT expr '\n' {$$ = opr(PRINT,1,$2);}
        | VARIABLE '=' expr '\n' {$$ = opr('=',2,id($1),$3);}
        | WHILE  expr '\n' stmt WEND {$$ = opr(WHILE, 2, $2, $4);}
        | IF expr THEN stmt %prec IFX {$$ = opr(IF,2,$2,$4);}
        | IF expr THEN stmt ELSE stmt {$$ = opr(IF,3,$2,$4,$6);}
        | stmt_list RUN '\n'{$$=$1;}
        ;
        
    stmt_list:INTEGER stmt {$$=$2;}
            | stmt_list INTEGER stmt {$$ = opr('\n',2,$1,$3);}
            ;
    expr:
        INTEGER { $$ = intr($1); }
        |FLOAT { $$ = flt($1); }
        |STRING { $$ = st($1); }
        |VARIABLE { $$ = id($1); }
        | expr '+' expr { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr { $$ = opr('*', 2, $1, $3); }
        | expr '/' expr { $$ = opr('/', 2, $1, $3); }
        | expr '<' expr { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr { $$ = opr('>', 2, $1, $3); }
        | expr GE expr { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr { $$ = opr(EQ, 2, $1, $3); }
        | expr AND expr { $$ = opr(AND, 2, $1, $3); }
        | expr OR expr { $$ = opr(OR, 2, $1, $3); }
        | '(' expr ')' { $$ = $2; }
 ; 
    %%
    
typeNode *intr(int value) {
 typeNode *p;
 /* allocate node */
 if ((p = malloc(sizeof(typeNode))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = conType;
 p->con.type = intType;
 p->con.i=value;
 return p;
}
typeNode *flt(float value) {
 typeNode *p;
 /* allocate node */
 if ((p = malloc(sizeof(typeNode))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = conType;
 p->con.type = floatType;
 p->con.f=value;
 return p;
}
typeNode *st(char* s) {
 typeNode *p;
 /* allocate node */
 if ((p = malloc(sizeof(typeNode))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = conType;
 p->con.type = stringType;
 p->con.s=strdup(s);
 return p;
}
typeNode *id(char* s) {
 typeNode *p;
 /* allocate node */
 if ((p = malloc(sizeof(typeNode))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = idType;
 char last = s[strlen(s)-1];
 s[strlen(s)-1]='\0';
 if(last=='%')
 {
    p->id.loc = lookIntTable(s);
    p->id.type = intType;
    // printf("%s , %d\n",s,p->id.loc);
 }
 else if(last=='$')
 {
    p->id.loc = lookStringTable(s);
    p->id.type = stringType;
 }
 else if(last=='#')
 {
    p->id.loc = lookFloatTable(s);
    p->id.type = floatType;
 }
//  printf("%s\n",p->id.s);
 return p;
} 


typeNode *opr(int oper, int nops, ...) {
 va_list ap;
 typeNode *p;
 int i;
 /* allocate node */
    // printf("oper %d\n",oper);
 if ((p = malloc(sizeof(typeNode))) == NULL)
 yyerror("out of memory");
 if ((p->opr.op = malloc(nops * sizeof(typeNode))) == NULL)
 yyerror("out of memory");
 /* copy information */
 p->type = oprType;
 p->opr.oper = oper;
 p->opr.nops = nops;
 va_start(ap, nops);
 for (i = 0; i < nops; i++)
 p->opr.op[i] = va_arg(ap, typeNode*);
 va_end(ap);
 return p;
}




void freeNode(typeNode *p) {
 int i;
 if (!p) return;
 if (p->type == oprType) {
 for (i = 0; i < p->opr.nops; i++)
 freeNode(p->opr.op[i]);
 free(p->opr.op);
 }
 free (p);
//  printf("Freed\n");
}




void yyerror(char *s) {
 fprintf(stdout, "%s\n", s);
}




int main(void) {
 yyparse();
 system("pause");
 getchar();
 return 0;
} 




int lookIntTable(char *s)
{
    int i;
    for(i=0;i<100&&integers[i][0]!='\0';i++)
    {
        if(strcmp(s,integers[i])==0)
        return i;
    }
    strcpy(integers[i],s);
    return i;
}




int lookFloatTable(char *s)
{
    int i;
    for(i=0;i<100&&floats[i][0]!='\0';i++)
    {
        if(strcmp(s,floats[i])==0)
        return i;
    }
    strcpy(floats[i],s);
    return i;
}




int lookStringTable(char *s)
{
    int i;
    for(i=0;i<100&&strings[i][0]!='\0';i++)
    {
        if(strcmp(s,strings[i])==0)
        return i;
    }
    strcpy(strings[i],s);
    return i;
}