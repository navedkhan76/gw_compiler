#include <stdio.h>
#include "gw_basic.h"
#include "y.tab.h"
static int lbl = 0;
static int lineno = 1;
void findNxt(typeNode *p, int *l);
int prev;
int tree_walk(typeNode *p)
{
    int lbl1 = 0, lbl2 = 0;
    if (!p)
    {
        return 0;
    }
    switch (p->type)
    {
    case conType:
        switch (p->con.type)
        {
        case intType:
            printf("%d ", p->con.i);
            break;
        case floatType:
            printf("%f ", p->con.f);
            break;
        case stringType:
            printf("%s ", p->con.s);
            break;
        }
        break;
    case idType:
        switch (p->id.type)
        {
        case intType:
            printf("%s", integers[p->id.loc]);
            break;
        case floatType:
            printf("%s", floats[p->id.loc]);
            break;
        case stringType:
            printf("%s", strings[p->id.loc]);
            break;
        }
        break;
    case oprType:
        switch (p->opr.oper)
        {
        case WHILE:
            prev = lineno;
            tree_walk(p->opr.op[1]);
            printf("%d. if(", lineno++);
            tree_walk(p->opr.op[0]);
            printf(") then goto (%d)\n", prev);
            break;
        case IF:
            if (p->opr.nops > 2)
            {
                /* if else*/
                if (p->opr.op[0]->opr.oper != AND &&p->opr.op[0]->opr.oper != OR)
                    printf("%d. if(", lineno++);
                tree_walk(p->opr.op[0]);
                printf(") then goto (%d)\n", lineno + 1);
                int nxt = lineno + 1;
                findNxt(p->opr.op[1], &nxt);
                printf("%d. goto(%d)\n", lineno++, nxt + 1);
                tree_walk(p->opr.op[1]);
                nxt = lineno + 1;
                findNxt(p->opr.op[2], &nxt);
                printf("%d. goto(%d)\n", lineno++, nxt);
                tree_walk(p->opr.op[2]);
            }
            else
            {
                /* if */
                if (p->opr.op[0]->opr.oper != AND &&p->opr.op[0]->opr.oper != OR)
                {
                    printf("%d. if(", lineno++);
                    tree_walk(p->opr.op[0]);
                    printf(") then goto (%d)\n", lineno + 1);
                }
                else
                    tree_walk(p->opr.op[0]);
                int nxt = lineno + 1;
                findNxt(p->opr.op[1], &nxt);
                printf("%d. goto(%d)\n", lineno++, nxt);
                tree_walk(p->opr.op[1]);
            }
            break;


        case PRINT:
            printf("%d. print", lineno++);
            tree_walk(p->opr.op[0]);
            printf("\n");
            break;
        case '=':
            switch (p->opr.op[0]->id.type)
            {
            case intType:
                switch (p->opr.op[1]->type)
                {
                case oprType:
                    tree_walk(p->opr.op[1]);
                    printf("%d. %s = t%d\n", lineno++, integers[p->opr.op[0]->id.loc], p->opr.op[1]->opr.label);
                    break;
                default:
                    printf("%d. %s  = ", lineno++, integers[p->opr.op[0]->id.loc]);
                    tree_walk(p->opr.op[1]);
                    printf("\n");
                    break;
                }
                break;
            case floatType:
                switch (p->opr.op[1]->type)
                {
                case oprType:
                    tree_walk(p->opr.op[1]);
                    printf("%d. %s = t%d\n", lineno++, floats[p->opr.op[0]->id.loc], p->opr.op[1]->opr.label);
                    break;
                default:
                    printf("%d. %s  = ", lineno++, floats[p->opr.op[0]->id.loc]);
                    tree_walk(p->opr.op[1]);
                    printf("\n");
                    break;
                }
                break;
            case stringType:
                switch (p->opr.op[1]->type)
                {
                case oprType:
                    tree_walk(p->opr.op[1]);
                    printf("%d. %s = t%d\n", lineno++, strings[p->opr.op[0]->id.loc], p->opr.op[1]->opr.label);
                    break;
                default:
                    printf("%d. %s  = ", lineno++, strings[p->opr.op[0]->id.loc]);
                    tree_walk(p->opr.op[1]);
                    printf("\n");
                    break;
                }
                break;
            }
            break;
        case UMINUS:
            tree_walk(p->opr.op[0]);
            printf("\tneg\n");
            break;
        default:
            p->opr.label = lbl++;
            switch (p->opr.oper)
            {
            case '+':
                if (p->opr.op[0]->type == oprType)
                {
                    tree_walk(p->opr.op[0]);
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d + t%d\n", lineno++, p->opr.label, p->opr.op[0]->opr.label, p->opr.op[1]->opr.label);
                    }
                    else
                    {
                        printf("%d. t%d = t%d + ", lineno++, p->opr.label, p->opr.op[0]->opr.label);
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }
                else
                {
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d + ", lineno++, p->opr.label, p->opr.op[1]->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf("\n");
                    }
                    else
                    {
                        printf("%d. t%d = ", lineno++, p->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf(" + ");
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }


                break;
            case '-':
                if (p->opr.op[0]->type == oprType)
                {
                    tree_walk(p->opr.op[0]);
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d - t%d\n", lineno++, p->opr.label, p->opr.op[0]->opr.label, p->opr.op[1]->opr.label);
                    }
                    else
                    {
                        printf("%d. t%d = t%d -", lineno++, p->opr.label, p->opr.op[0]->opr.label);
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }
                else
                {
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d - ", lineno++, p->opr.label, p->opr.op[1]->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf("\n");
                    }
                    else
                    {
                        printf("%d. t%d = ", lineno++, p->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf(" - ");
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }
                break;
            case '*':
                if (p->opr.op[0]->type == oprType)
                {
                    tree_walk(p->opr.op[0]);
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d * t%d\n", lineno++, p->opr.label, p->opr.op[0]->opr.label, p->opr.op[1]->opr.label);
                    }
                    else
                    {
                        printf("%d. t%d = t%d * ", lineno++, p->opr.label, p->opr.op[0]->opr.label);
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }
                else
                {
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d * ", lineno++, p->opr.label, p->opr.op[1]->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf("\n");
                    }
                    else
                    {
                        printf("%d. t%d = ", lineno++, p->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf(" * ");
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }
                break;
            case '/':
                if (p->opr.op[0]->type == oprType)
                {
                    tree_walk(p->opr.op[0]);
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d / t%d\n", lineno++, p->opr.label, p->opr.op[0]->opr.label, p->opr.op[1]->opr.label);
                    }
                    else
                    {
                        printf("%d. t%d = t%d / ", lineno++, p->opr.label, p->opr.op[0]->opr.label);
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }
                else
                {
                    if (p->opr.op[1]->type == oprType)
                    {
                        tree_walk(p->opr.op[1]);
                        printf("%d. t%d = t%d / ", lineno++, p->opr.label, p->opr.op[1]->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf("\n");
                    }
                    else
                    {
                        printf("%d. t%d = ", lineno++, p->opr.label);
                        tree_walk(p->opr.op[0]);
                        printf(" / ");
                        tree_walk(p->opr.op[1]);
                        printf("\n");
                    }
                }
                break;
            case '<':
                tree_walk(p->opr.op[0]);
                printf("<");
                tree_walk(p->opr.op[1]);
                break;
            case '>':
                tree_walk(p->opr.op[0]);
                printf(">");
                tree_walk(p->opr.op[1]);
                break;
            case GE:
                tree_walk(p->opr.op[0]);
                printf(">=");
                tree_walk(p->opr.op[1]);
                break;
            case LE:
                tree_walk(p->opr.op[0]);
                printf("<=");
                tree_walk(p->opr.op[1]);
                break;
            case NE:
                tree_walk(p->opr.op[0]);
                printf("<>");
                tree_walk(p->opr.op[1]);
                break;
            case EQ:
                tree_walk(p->opr.op[0]);
                printf("==");
                tree_walk(p->opr.op[1]);
                break;
            case AND:
                if (p->opr.op[0]->opr.oper != AND &&p->opr.op[0]->opr.oper != OR)
                {
                    printf("%d. if(", lineno++);
                    tree_walk(p->opr.op[0]);
                    printf(") then goto (%d)\n", lineno + 1);
                }
                else
                    tree_walk(p->opr.op[0]);
                printf("%d. goto(%d)\n", lineno++, lineno + 2);
                printf("%d. if(", lineno++);
                // printf("%d",p->opr.op[1]->type);
                tree_walk(p->opr.op[1]);
                printf(") then goto (%d)\n", lineno + 1);
                break;
            case OR:
                if (p->opr.op[0]->opr.oper != AND && p->opr.op[0]->opr.oper != OR)
                {
                    printf("%d. if(", lineno++);
                    tree_walk(p->opr.op[0]);
                    printf(") then goto (%d)\n", lineno + 2);
                }
                else
                    tree_walk(p->opr.op[0]);
                printf("%d. if(", lineno++);
                // printf("%d",p->opr.op[1]->type);
                tree_walk(p->opr.op[1]);
                printf(") then goto (%d)\n", lineno + 1);
                break;
            default:
                tree_walk(p->opr.op[0]);
                tree_walk(p->opr.op[1]);
            }
        }
    }
    return 0;
}


void findNxt(typeNode *p, int *l)
{
    // printf("called %d\n",p->type==oprType);
    if (p && p->type == oprType)
    {
        *l = *l + 1;
        findNxt(p->opr.op[0], l);
        findNxt(p->opr.op[1], l);
    }
}