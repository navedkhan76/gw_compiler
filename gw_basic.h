typedef enum {conType,idType,oprType} nodeEnum;
typedef enum {intType,floatType,stringType} idEnum;
// Constant Node having type and its value
typedef struct{
    idEnum type;
    union{
        int i;
        double f;
        char* s;
    };
} conTypeNode;
// Varibles having name,its type and its location
typedef struct{
    idEnum type;
    int loc;
}  idTypeNode;
// node having type of the node and data
typedef struct{
    int oper;
    int nops;
    int label;
    struct nodeTypeTag **op;
} oprTypeNode;
typedef struct nodeTypeTag{
    nodeEnum type;


    union{
        idTypeNode id;
        conTypeNode con; 
        oprTypeNode opr;
    };
} typeNode;


extern char integers[100][40];


extern char strings[100][40];


extern char floats[100][40];


extern int symInt[100];


extern float symFloat[100];


extern char symString[100][100];