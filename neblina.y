%{
#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include "neblina_sym.h"
#include "neblina_vm.h"
#include "package.h"
#include "neblina_str.h"
#include "neblina_std.h"
#include "neblina_packages.h"
#include "libneblina.h"
#include "clutils.h"
#define YYDEBUG       1
#define GLOBAL_CTX    0
#define LOCAL_CTX     1

int yylex();
void yyerror(const char *s);

#define MAX_INCLUDE_DEPTH 100
#define cyyerror( str, err_strchar )    \
                char err[1024];  sprintf(err, str, err_strchar ); yyerror( err );



extern FILE * yyin;
int                 reg_max_store = 0;
int                 reg_min_store = 0xFFFFFFF;
extern int          curr_line[MAX_INCLUDE_DEPTH];
int                 errors        = 0;
int                 curr_context  = GLOBAL_CTX;
int                 reg_location  = 0;
int                 reg_arg_len   = 0;
function_t *        reg_func      = NULL;
int                 id_constants  = 0;
char                 include_names[1024][1024];
int                 reg_include = 0;
char    curr_func_name[256];
char   reg_param[64][256];
int    reg_param_idx = 0;
cl_device_type  proctype = CL_DEVICE_TYPE_ALL;
int            id_device = 0;
extern int    line_numbers[MAX_INCLUDE_DEPTH];
extern char  file_names[MAX_INCLUDE_DEPTH][256];
extern int include_stack_ptr;
extern int  include_hist;

unsigned int stack_location[256];
int          top_location = 0;
extern instruction_t  *  code;
extern int nkernelsRmat;
void pop_return_values( int nvalues ) {
    int i = 0;
    for( i = 0; i < nvalues; i++ )
        ibytecode( POP, 0 );                
}

void push_location( unsigned int v ) {
    stack_location[top_location++] = v;
}

unsigned int pop_location() {
    return stack_location[--top_location];
}

typedef struct __for_label_t {
    int goto_addr;
    int jmp_false_addr;
    int increment;
} for_label_t;


typedef struct __while_label_t {
    int goto_addr;
    int jmp_false_addr;
} while_label_t;

typedef struct __arg_list_t {
    struct __arg_list_t * next;
    struct __object_t     arg;
} arg_list_t;

arg_list_t * list = NULL;

struct __arg_list_t * new_arglist() {
    return (struct __arg_list_t *) malloc( sizeof(struct __arg_list_t) );
}

void add_arg( struct __object_t  arg ) {
    struct __arg_list_t * ptr = new_arglist();
    ptr->next = list;
    ptr->arg = arg;
    
    list = ptr;
}
int push_args() {
    struct __arg_list_t * ptr = list;
    int ret = 0; 
    while( list ) {
        object_t v = list->arg;
        
        if( type(v) != T_ADDR )
            bytecode( PUSH, v ); 
        else
            ibytecode( LOAD, avalue( v ) ); 
        ptr = list->next;
        free( list );
        list = ptr;
        ret++;
    }
    
    return ret;
    
}

struct __for_label_t * new_forlabel() {
    return (struct __for_label_t *) malloc( sizeof(struct __for_label_t) );
}

void release_forlabel( struct __for_label_t * ptr ) {
    free( ptr );
}

struct __while_label_t * new_whilelabel() {
    return (struct __while_label_t *) malloc( sizeof(struct __while_label_t) );
}

void release_whilelabel( struct __while_label_t * ptr ) {
    free( ptr );
}

void context_check( enum code_ops operation, char *sym_name ) {  
   if( curr_context == GLOBAL_CTX ) {
       symbol_t * t = sym_get_global( sym_name ); 
       if( t ) 
            ibytecode( operation, t->position );   
       else { 
            printf( "%s", sym_name );
            printf( "%s\n", " is an undeclared identifier" );
       }
   } else {
       symbol_t * t = func_get_sym( sym_name ); 
       if( t ) 
            ibytecode( operation, t->position );   
       else {
            char str_err[256]; 
            sprintf(str_err,  "\'%s\' is an undeclared identifier", sym_name );
            yyerror( str_err );
       }
   }
}

unsigned int get_addr( char *sym_name ) {  
   if( curr_context == GLOBAL_CTX ) {
       symbol_t * t = sym_get_global( sym_name ); 
       if( t ) 
            return t->position;
       else { 
            char str_err[256]; 
            sprintf(str_err,  "%s is an undeclared identifier", sym_name );
            yyerror( str_err );       }
   } else {
       symbol_t * t = func_get_sym( sym_name ); 
       if( t ) 
            return t->position;
       else {
            char str_err[256]; 
            sprintf(str_err,  "%s is an undeclared identifier", sym_name );
            yyerror( str_err );
       }
   }
   return 0;
}

void context_install( enum code_ops operation, char *sym_name ) {  


   if( curr_context == GLOBAL_CTX ) {
       symbol_t * t = sym_get_global( sym_name ); 
       if( !t ) {
         sym_put_global( sym_name );
         t = sym_get_global( sym_name );
       }
       
       ibytecode( operation, t->position );   
        
   } else {
       symbol_t * t = func_get_sym( sym_name ); 
       if( !t ) {
           func_put_sym( sym_name );
           t = func_get_sym( sym_name );
       }
                  
       ibytecode( operation, t->position );      
   }
}
void store_rparams() {
    int i = 0;
    
    for(i = reg_param_idx - 1; i >= 0; i-- ) {
        if( reg_param[i][0] == '#' )
            context_install( STOREV, reg_param[i] + 1 );
        else
            context_install( STORE, reg_param[i] );
    }

    reg_param_idx = 0;

}
void store_rparams_func() {
    int i = 0;
    
    for(i = reg_param_idx - 1; i >= 0; i-- ) {
        context_install( STORE, reg_param[i] );
    }

    reg_param_idx = 0;

}

//void read_object( int data_type, char id[256], int lenght ) {
//  
//    symbol_t * sym = func_get_sym( id );
//    if( !sym ) {
//        context_install( LOAD, id );
//        sym = func_get_sym( id );
//    } else {
//        ibytecode( LOAD, get_addr( id ) ); 
//        printf("TY : %d\n", sym->type );
//    }

//    
//        
//    switch( data_type ) {
// 
//        case DT_INT:      
//            if( sym->type == T_INT || sym->type == T_NDEF )
//                ibytecode( READ_INT, 0 ); 
//            else
//                yyerror( "Invalid read, data type: int" );
//                
//            break;
//        case DT_FLOAT:
//            if( sym->type == T_FLOAT || sym->type == T_NDEF )
//                ibytecode( READ_FLOAT, 0 ); 
//            else
//                yyerror( "Invalid read, data type: double" );
//            break;    
//        case DT_STRING:
//            if( sym->type == T_STRING || sym->type == T_NDEF )
//                ibytecode( READ_STRING, lenght ); 
//            else
//                yyerror( "Invalid read, data type: double" );
//            break;    
//        default:
//            yyerror( "Invalid read data type" );
//    }
//    ibytecode( STORE, get_addr( id ) );

//}
%}

%union {
    int   vInt;
    long  vLong;
    double vFloat;
    char pChar[256];
    char * pStr;
 
    struct __object_t      * vObj;
    struct __for_label_t   * vFor;
    struct __while_label_t * vWhile;
    struct __function_t    * vFunc;
    struct __arg_list_t    * vArg;   
}

%token <pChar>  NAME 
%token <pChar>  DNAME 
%token <pStr>   STRING 
%token <vInt>   INT 
%token <vFloat> FLOAT 

%token <vInt> IF
%token <vInt> ELSE
%token <vFor> FOR
%token <vWhile> WHILE
%token <vFunc > FUNCTION

%token NEGT EOL SPARSE ENDFILE PRINT RETURN END SHIFTL SHIFTR AND OR C_EQ INCR DECR ENDPROG INCLUDE DT_INT DT_FLOAT DT_STRING DT_COMPLEX USING DOLAR LESSEQ DIFF GREATEQ TODEV TOHOST PLUS 
%token ASSIG TWO_DOTS

%type <vInt> exp data_type
%type <vInt> if_stm if_part else_part
%type <vInt> argparams argmov_value argmov
%type <vInt> callfunc
%type <vInt> argtype 
%start program


/*=========================================================================
                           OPERATOR PRECEDENCE
=========================================================================*/

%left '|'
%left '&'
%left SHIFTR SHIFTL
%left '%'
%left '-' '+'
%left '*' '/'
%left NOT
%right '^'
%left OR
%left AND
%left NEGT
%%

program :   
            
            | stms   { ibytecode( HALT, 0 ); }

//include: 
//        | INCLUDE NAME EOL;
//        | include INCLUDE NAME EOL;
      
        
stms: // NULL
     | stms stm EOL         {} 
     | stms stm ';'         {} 
     | stms stm ';' EOL     {} 
stm: 
    |  function        {}
    |  NAME INCR       { context_check( INC, $<pChar>2 ); }
    |  NAME DECR       { context_check( DEC, $<pChar>2 ); }
    |  memcpy          {}

    |  RETURN argparams  { if( curr_context == LOCAL_CTX ){ 
                                ibytecode( GOTO_ST, 0 ); 
                                if( reg_func->rparams == 0 ){
                                    reg_func->rparams = $<vInt>2; 
                                } else {
                                    if(reg_func->rparams != $<vInt>2) {
                                        yyerror("different number of return values");
                                    }
                                }
                                
                           } else { yyerror("unexpected 'return' statement");  } } 
    |  PRINT exp         { ibytecode( WRITE, 0 ); } 
    |  PRINT str         { ibytecode( WRITE, 0 ); } 
    |  if_stm
    
    |  FOR { if(curr_context == LOCAL_CTX ){ func_add_scope(); }} NAME ASSIG exp {  $<vFor>1 = new_forlabel(); context_install( STORE, $<pChar>3 ); } 
        TWO_DOTS { $<vFor>1->goto_addr = cur_location(); } exp {   
                           context_check( LOAD, $<pChar>3 ); 
                           ibytecode( GEQ, 0 ); 
                           $<vFor>1->jmp_false_addr = reserve_location(); } 
         stms END {  ibytecode_addr( JMP_FALSE, cur_location() + 2, $<vFor>1->jmp_false_addr);
                     context_check( INC, $<pChar>3 );
                     if(curr_context == LOCAL_CTX ) func_release_scope();
                     ibytecode(GOTO, $<vFor>1->goto_addr); 
                     release_forlabel($<vFor>1 );   
                   }
    | USING NAME          {             if( package_add( $<pChar>2 ) == -1 ) {

                                            cyyerror( "package '%s' not found", $<pChar>2 );
                                            exit( 1 );
                                        }
                           
                           }
    
    | WHILE {      if(curr_context == LOCAL_CTX ){ func_add_scope(); }
                    
                   $<vWhile>1 = new_whilelabel(); 
                   $<vWhile>1->goto_addr = cur_location(); 
                    }
        cond { $<vWhile>1->jmp_false_addr = reserve_location(); }
        stms END {
                     ibytecode_addr( JMP_FALSE, cur_location() + 1, $<vWhile>1->jmp_false_addr);
                     ibytecode(GOTO, $<vWhile>1->goto_addr); 
                     release_whilelabel( $<vWhile>1 );
                     if(curr_context == LOCAL_CTX ) func_release_scope();
                 }
       | callfunc                      {  pop_return_values( $1 ); if( $1 != 0 ) { yyerror("function returns one or more values"); } }
       | rparams ASSIG argparams     { if($<vInt>1 != $<vInt>3 ) yyerror("invalid number of assignment variables"); if( $3 == 0 ) { yyerror("syntax error, expected a assignment value"); } else { store_rparams(); } }
       | exp      { store_rparams(); }        
          

if_stm:   if_part END    { ibytecode_addr( JMP_FALSE, cur_location(), pop_location() );  if(curr_context == LOCAL_CTX ) func_release_scope(); }
        | else_part 

if_part:  IF cond {  
                      if(curr_context == LOCAL_CTX )
                      { 
                        func_add_scope();
                        push_location( reserve_location() );
                      }
                   } 
          stms    {}

else_part: if_part ELSE {  ibytecode_addr( JMP_FALSE, cur_location() + 1, pop_location() ); push_location( reserve_location() );   
                if(curr_context == LOCAL_CTX ) {  func_release_scope(); func_add_scope(); } } 
           stms END { ibytecode_addr( GOTO, cur_location(), pop_location() );  if(curr_context == LOCAL_CTX ) func_release_scope();  }       


callfunc: NAME { if( package_func_get_any( $<pChar>1 ) == NULL ) push_location( reserve_location() );  } '(' argparams ')'  {   

func_info_t * f; 
if( ( f = package_func_get( $<pChar>1, $<vInt>4 )) != NULL ) {                                          
    object_t o; pvalue( o ) = f; type( o ) = T_CFUNC; $$ = f->nreturns; bytecode( CALL_C, o );
} else {                                                                        
    function_t * t = func_get($<pChar>1, $<vInt>4); 
    if( t == NULL ) { 
        if( package_func_get_any( $<pChar>1 ) == NULL ) {
            char strerr[1024]; sprintf(strerr, "function '%s' not defined", $<pChar>1); yyerror( strerr ); 
        } else {
            char strerr[1024]; sprintf(strerr, "function '%s' with %d arguments not defined", $<pChar>1, $<vInt>4); yyerror( strerr ); 
        }

        exit( 1 );
    }
    else if( t == (function_t *)FUNC_INVALID_PARAMS ) {
        char err[1024]; sprintf( err, "invalid number of parameters on call '%s'", $<pChar>1);
        yyerror(err);
        exit( 1 );
    } else { 
        ibytecode(CALL, t->init_addr +1 );  
        int loc = pop_location();  
        ibytecode_addr(PUSH_PC, cur_location() - loc - 1, loc );
        
        $$ = t->rparams;
    }  
                                                                              
                                                                              }
                                                                             }


                                                                             
rparams:                            { $<vInt>$ = 0;}
        | NAME                      { strcpy( reg_param[reg_param_idx++], $<pChar>1 ); $<vInt>$ = 1; }
        | NAME '[' exp ']'          { strcpy(reg_param[reg_param_idx], "#" );  strcat( reg_param[reg_param_idx++], $<pChar>1 ); $<vInt>$ = 1; }
        | NAME '[' exp ',' exp ']'  { strcpy(reg_param[reg_param_idx], "#" );  strcat( reg_param[reg_param_idx++], $<pChar>1 ); $<vInt>$ = 1; }
        | rparams ',' NAME          { if( $<vInt>$ == 0 ) { yyerror("unexpected ','"); } else{ strcpy( reg_param[reg_param_idx++], $<pChar>3 ); $<vInt>$++;} }
        | rparams ',' NAME '[' exp ']' {if( $<vInt>$ == 0 ) { yyerror("unexpected ','"); } else{ strcpy(reg_param[reg_param_idx], "#" ); strcat( reg_param[reg_param_idx++], $<pChar>3 ); $<vInt>$++;} }
        | rparams ',' NAME '[' exp ',' exp ']' {if( $<vInt>$ == 0 ) { yyerror("unexpected ','"); } else{ strcpy(reg_param[reg_param_idx], "#" ); strcat( reg_param[reg_param_idx++], $<pChar>3 ); $<vInt>$++;} }
     
function_list:     
                  function lines                          {}
               |  function_list function lines            {}
                                                  
lines:  
     | EOL              {}
     | lines EOL        {}
                                 
function: FUNCTION NAME { 
                     reg_min_store = 0x0FFFFFFF;
                     reg_max_store = 0;
                     curr_context = LOCAL_CTX; $<vFunc>1 = func_new(); reg_func = $<vFunc>1; $<vFunc>1->rparams = 0;
                     $<vFunc>1->init_addr = reserve_location(); 
                     strcpy( $<vFunc>1->name, $<pChar>2 );
                     strcpy( curr_func_name, $<pChar>2 );
                     func_add( $<vFunc>1 );         
                   } 
           '(' rparams ')' {  store_rparams_func(); $<vFunc>1->nparams = -1; 
                              if( func_install( $<vFunc>1, $<vInt>5 ) != FUNC_NO_ERROR ) {
                                char str[256];
                                sprintf(str, "redefinition of function %s", $<pChar>2);
                                yyerror( str );
                              }                      
                           }
                   
            stms END {  
                        $<vFunc>1->min_store = reg_min_store;
                        $<vFunc>1->max_store = reg_max_store;
                        
                               
                        $<vFunc>1->final_addr = cur_location(); 
                    
                        if( strcmp($<vFunc>1->name, "main") == 0 ) {
                            ibytecode_addr(GOTO, $<vFunc>1->init_addr + 1,  $<vFunc>1->init_addr );
                        } else {
                            ibytecode( GOTO_ST, 0 );
                            ibytecode_addr(GOTO, $<vFunc>1->final_addr + 1,  $<vFunc>1->init_addr );                   
                        }
                        
//                        printf("Last : %d POS : %d\n", reg_laststore, reg_store - $<vInt>5);
                        
                        
                        curr_context = GLOBAL_CTX;
                     }
                                              
        
argparams:                            { $<vInt>$ = 0; } 
         | argtype                    { $<vInt>$ = $1; }
         | argparams ',' argtype      { if($<vInt>$ == 0) yyerror("syntax error"); $<vInt>$ += $3;   }  


argtype:    callfunc                  { $$ = $1; }   
         |  str                       { $$ = 1;  }
         |  number                    { $$ = 1;  }
         |  exp                       { $$ = 1;  }
         |  vector_declare            { $$ = 1;  }
         |  matrix_declare            { $$ = 1;  }
         
vector_declare:    data_type '[' exp ']'      { object_t arg; avalue(arg) = ($<vInt>1 & 0xFF) | (T_VECTOR << 8);  bytecode( ALLOCV, arg);  }
                |  NAME '[' exp TWO_DOTS exp ']'     { context_check( LOAD, $<pChar>1 ); object_t arg; avalue(arg) = (T_VECTOR & 0xFF)   | (T_NDEF << 8);  bytecode( ALLOCV, arg);  }
                | data_type '[' ']' '(' argparams ')' { object_t arg; avalue(arg) = ($<vInt>1 & 0xFF) | (T_VECTOR << 8) | ($<vInt>5 << 16);  bytecode( ALLOCV, arg); }

argmov_value:   NAME                          { $$ = 1; context_check( LOAD, $<pChar>1 ); }  
            | vector_declare                  { $$ = 1; }
            | matrix_declare                  { $$ = 1; }

argmov:                                       
        argmov_value                        { $<vInt>$ = 1; }               
        | argmov ',' argmov_value            { if($<vInt>$ == 0) yyerror("syntax error"); $<vInt>$ += $<vInt>1; }
        
memcpy:  TODEV argmov  { 
                                int i = 0;
                                if($<vInt>2 == 0) yyerror("syntax error");
                                for(i=0; i < $<vInt>2; i++ ) {
                                    func_info_t * f = package_func_get( "movetodev", 1 );             
                                    object_t o; pvalue( o ) = f; type( o ) = T_CFUNC; bytecode( CALL_C, o );
                                }
                       }
                              
    | TOHOST argmov {      
                                int i = 0;
                                if($<vInt>2 == 0) yyerror("syntax error");
                                for(i=0; i < $<vInt>2; i++ ) {
                                    func_info_t * f = package_func_get( "movetohost", 1 );             
                                    object_t o; pvalue( o ) = f; type( o ) = T_CFUNC; bytecode( CALL_C, o );
                                }      
                       }
matrix_declare:    data_type '[' exp ',' exp ']' { object_t arg; avalue(arg) = ($<vInt>1 & 0xFF) | (T_MATRIX << 8);  bytecode( ALLOCV, arg);  }
                | SPARSE data_type '[' exp ',' exp ']' { object_t arg; avalue(arg) = ($<vInt>2 & 0xFF) | (T_SMATRIX << 8);  bytecode( ALLOCV, arg);  }

exp:    exp '+' exp            { ibytecode( ADD, 0 ); }
      | exp '-' exp            { ibytecode( SUB, 0 ); }
      | exp '*' exp            { ibytecode( MUL, 0 ); }
      | exp '/' exp            { ibytecode( DIV, 0 ); }
      | exp '&' exp            { ibytecode( ANDA, 0 ); }
      | exp '|' exp            { ibytecode( ORA, 0 ); }
      | exp '%' exp            { ibytecode( MOD, 0 ); }
      | exp SHIFTR exp            { ibytecode( SHR, 0 ); }
      | exp SHIFTL exp            { ibytecode( SHL, 0 ); }      
      | '(' exp ')'            {}
      | callfunc               {}
      | number                 {}
      | NAME                        { context_check( LOAD, $<pChar>1 ); }  
      | NAME '[' exp ']'            { context_check( LOADV, $<pChar>1 ); }
      | NAME '[' exp ',' exp ']'    { context_check( LOADV, $<pChar>1 ); }
      | '-' exp  %prec NOT        { ibytecode( SIGCH, 0 ); }
      | '+' exp  %prec NOT        {}
      ;

cond:  '(' cond  ')' 
 
      | NEGT cond %prec NEGT              { ibytecode( NEGL, 0 );  }
      | expcond LESSEQ expcond            { ibytecode( LEQ, 0 );  }
      | expcond GREATEQ expcond           { ibytecode( GEQ, 0 );  }
      | expcond '>' expcond               { ibytecode( GT, 0 );   }
      | expcond '<' expcond               { ibytecode( LT, 0 );   } 
      | expcond C_EQ expcond              { ibytecode( EQ, 0 );   }
      | expcond DIFF expcond              { ibytecode( NEQ, 0 );  }
      | cond AND cond                     { ibytecode( ANDL, 0 ); }
      | cond OR  cond                     { ibytecode( ORL, 0 );  }
      

expcond:  exp
        | str                  
      
str: STRING                      { 
                                   object_t v; 
                                   type(v) = T_STRING; 
                                   svalue(v) = (char *) malloc( sizeof(char)*(strlen($1) + 1) ); 
                                   strcpy( svalue(v), $1 ); 
                                   bytecode( ALLOCS, v ); 
                                 } 
                                    
   | str '+' str                 { ibytecode( APPEND, 0 ); } 
   | str '+' exp                 { ibytecode( APPEND, 0 ); } 
   | exp '+' str                 { ibytecode( APPEND, 0 ); }
   ;
                                             
number:   INT            { object_t v; type(v) = T_INT; ivalue(v) = $<vInt>1;      bytecode( PUSH, v ); }
        | FLOAT          { object_t v; type(v) = T_FLOAT ; fvalue(v) = $<vFloat>1; bytecode( PUSH, v ); }   
           
        ;
data_type:  DT_INT         { $<vInt>$ = $<vInt>1; }
          | DT_FLOAT       { $<vInt>$ = $<vInt>1; }
          | DT_STRING      { $<vInt>$ = $<vInt>1; }
          | DT_COMPLEX     { $<vInt>$ = $<vInt>1; }
        ;
%%
/*=========================================================================
                                   MAIN
=========================================================================*/

void declare_stds(int argc, char *argv[], int start) {

    object_t v1; type(v1) = T_FILE; vvalue(v1) = (void *) stdout; bytecode( PUSH, v1 ); context_install(STORE, "stdout" );
    object_t v2; type(v2) = T_FILE; vvalue(v2) = (void *) stderr; bytecode( PUSH, v2 ); context_install(STORE, "stderr" );
    object_t v3; type(v3) = T_FILE; vvalue(v3) = (void *) stdin;  bytecode( PUSH, v3 ); context_install(STORE, "stdin" );
    
    object_t v4; type(v4) = T_VECTOR; 
    //printf("argc  = %d\n", argc );
    //printf("start = %d\n", start );
    int lenArgs = argc - start;
    
    if( lenArgs > 0 ) {
        int i = 0;
        vector_t * varg = (vector_t *) malloc( sizeof( vector_t ) );
        varg->type = T_STRING;
        varg->len = lenArgs;
        varg->value.s = (void **) malloc( lenArgs * sizeof( char * ) );
        varg->location = LOCHOS;
        for(i=0; i < lenArgs; i++ ) {
            varg->value.s[i] = (void *) malloc( sizeof( char )*strlen( argv[start+i] ) + 1);
            char * tmp = (char *)varg->value.s[i];
            strcpy( tmp,  argv[start+i] );
        }
        vvalue(v4) =  (void *) varg;
        type(v4) = T_VECTOR;
        
        bytecode( PUSH, v4 ); context_install(STORE, "args" );
    }
}
int main( int argc, char *argv[] )
{ extern FILE *yyin;
  nkernelsRmat = 0;
  int i, idx_file, startIdxArgsToVM = 0;
  
  unsigned char showTrace = 0, alreadyOpen = 0;
    
  
  if( argc == 1 ) {
    yyerror("no input file.");
    exit( 1 );
  }    
      for(i=1; i < argc; i++ ) {
        if( strcmp( argv[i], "-l" ) == 0 ) {
             showDevicesList();
             exit( 0 );     
        } else if( strcmp( argv[i], "-S" ) == 0 ) {
            showTrace = 1;
        } else if( strcmp( argv[i], "-type" ) == 0 ) {
            i++;
            if( i >= argc ) {
                yyerror("indefined process type");
                exit( 1 );
            } 
            
            if( strcmp( argv[i], "cpu" ) == 0 ) {
                proctype =  CL_DEVICE_TYPE_CPU;
            } else if( strcmp( argv[i], "gpu" ) == 0 ){
                proctype =  CL_DEVICE_TYPE_GPU;
            } else {
              yyerror("undefined processor type");
              exit( 1 );  
            }
        } else if( strcmp( argv[i], "-id" ) == 0 ) {
            i++;
            if( i >= argc ) {
                yyerror("undefined id device");
                exit( 1 );
            } 
            id_device = atoi( argv[i] );
        } else {
            if( !alreadyOpen ) {
                yyin = fopen( argv[i], "r" );
                if( !yyin ) {
                    yyerror("input file not found");
                    exit(1);
                }
                idx_file = i;                
                alreadyOpen = 1;
            
            } else {
                if( startIdxArgsToVM == 0 )
                    startIdxArgsToVM = i - 1;
            }
        }
      }
      if( startIdxArgsToVM == 0 )
        startIdxArgsToVM = argc - 1;        
  current_file = 0;
  curr_line[include_stack_ptr] = 0;
  strcpy( file_names[include_stack_ptr++], argv[idx_file] );
  strcpy( file_names_hist[include_hist++], argv[idx_file] );
  init_machine();
  package_add_default();
  declare_stds(argc, argv, startIdxArgsToVM);
  yyparse();

  
  
  //showTrace = 1;
  if( errors == 0 ) {
       if( !func_has_main() )
          yyerror("function 'main' not found");
       else {    
            InitCLEngine();
            if( showTrace ){ 
                trace_code();
                }
            // print_info_source();
            execute();

       }
  }
  finalize_machine();
  return 0;
}

void yywrap () {}
void yyerror (const char *s ) {
  errors++;
  if( errors == 1 )
      printf(NEB_PRESENT); 
  fprintf ( stderr, "%s:%d error: %s\n", file_names[include_stack_ptr-1], curr_line[include_stack_ptr-1]+1, s);
}

