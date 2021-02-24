/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "neblina.y" /* yacc.c:339  */


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

#line 329 "neblina.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "neblina.tab.h".  */
#ifndef YY_YY_NEBLINA_TAB_H_INCLUDED
# define YY_YY_NEBLINA_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NAME = 258,
    DNAME = 259,
    STRING = 260,
    INT = 261,
    FLOAT = 262,
    IF = 263,
    ELSE = 264,
    FOR = 265,
    WHILE = 266,
    FUNCTION = 267,
    NEGT = 268,
    EOL = 269,
    SPARSE = 270,
    ENDFILE = 271,
    PRINT = 272,
    RETURN = 273,
    END = 274,
    SHIFTL = 275,
    SHIFTR = 276,
    AND = 277,
    OR = 278,
    C_EQ = 279,
    INCR = 280,
    DECR = 281,
    ENDPROG = 282,
    INCLUDE = 283,
    DT_INT = 284,
    DT_FLOAT = 285,
    DT_STRING = 286,
    DT_COMPLEX = 287,
    USING = 288,
    DOLAR = 289,
    LESSEQ = 290,
    DIFF = 291,
    GREATEQ = 292,
    TODEV = 293,
    TOHOST = 294,
    PLUS = 295,
    ASSIG = 296,
    TWO_DOTS = 297,
    NOT = 298
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 264 "neblina.y" /* yacc.c:355  */

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

#line 427 "neblina.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_NEBLINA_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 442 "neblina.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   722

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  206

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   298

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    45,    44,     2,
      53,    54,    48,    47,    57,    46,     2,    49,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    52,
      59,     2,    58,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    55,     2,    56,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    43,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   318,   318,   320,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   337,   348,   349,   350,   352,   352,   353,
     353,   352,   363,   371,   376,   371,   383,   384,   385,   388,
     389,   391,   391,   400,   400,   405,   405,   438,   439,   440,
     441,   442,   443,   444,   454,   463,   454,   492,   493,   494,
     497,   498,   499,   500,   501,   502,   504,   505,   506,   508,
     509,   510,   513,   514,   516,   525,   533,   534,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   555,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   568,   569,   571,   579,   580,
     581,   584,   585,   588,   589,   590,   591
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "DNAME", "STRING", "INT",
  "FLOAT", "IF", "ELSE", "FOR", "WHILE", "FUNCTION", "NEGT", "EOL",
  "SPARSE", "ENDFILE", "PRINT", "RETURN", "END", "SHIFTL", "SHIFTR", "AND",
  "OR", "C_EQ", "INCR", "DECR", "ENDPROG", "INCLUDE", "DT_INT", "DT_FLOAT",
  "DT_STRING", "DT_COMPLEX", "USING", "DOLAR", "LESSEQ", "DIFF", "GREATEQ",
  "TODEV", "TOHOST", "PLUS", "ASSIG", "TWO_DOTS", "'|'", "'&'", "'%'",
  "'-'", "'+'", "'*'", "'/'", "NOT", "'^'", "';'", "'('", "')'", "'['",
  "']'", "','", "'>'", "'<'", "$accept", "program", "stms", "stm", "$@1",
  "$@2", "$@3", "$@4", "$@5", "$@6", "if_stm", "if_part", "$@7",
  "else_part", "$@8", "callfunc", "$@9", "rparams", "function", "$@10",
  "$@11", "argparams", "argtype", "vector_declare", "argmov_value",
  "argmov", "memcpy", "matrix_declare", "exp", "cond", "expcond", "str",
  "number", "data_type", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   124,    38,    37,    45,    43,    42,    47,
     298,    94,    59,    40,    41,    91,    93,    44,    62,    60
};
# endif

#define YYPACT_NINF -93

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-93)))

#define YYTABLE_NINF -53

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       8,    27,   200,   -93,     6,   -93,   -93,   143,   -93,   -93,
      34,   400,   367,    61,   466,   466,   122,   122,   122,     4,
     -93,    39,   -93,    10,   -19,   -93,   -93,   638,   -93,   -93,
     -93,   122,    17,    20,   -93,   143,   143,   -93,   653,    22,
     207,    49,    97,   143,   -93,   653,    49,    35,    21,   -93,
     -93,   -93,   -93,   -11,    45,   -93,   -93,   -93,   653,    49,
       3,    62,   -93,    67,   -93,   -93,    73,   -93,    73,   -93,
     -93,   593,   -93,   109,   -93,   -93,   367,   121,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   411,   367,   122,
     -93,   608,    13,   400,   143,   143,   -93,   400,   400,   400,
     400,   400,   400,   400,    94,    22,    83,   122,    85,   367,
      86,   122,   466,   -93,   -93,   -93,    45,    89,    66,    66,
     659,   643,   213,    71,    71,   -93,   -93,   -15,   122,   -29,
     428,   -93,    71,   -93,   -93,   116,   217,   -93,   -93,   -93,
     -93,   -93,   -93,   673,   -93,   122,   -93,   138,   381,   122,
     -93,    98,   443,   623,   -93,   265,   122,   396,   -93,   -93,
     122,   638,   282,   100,    11,   122,   488,   367,   -93,   122,
     -93,   458,    -8,   503,   110,   -93,   122,   -93,   518,   122,
      12,   533,   -93,   122,   -93,   -93,   473,   -93,   -93,   548,
     -93,   -93,   563,   122,   -93,   122,   330,   -93,   -93,   638,
     578,   -93,   -93,   -93,   347,   -93
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     8,     1,    80,   101,   102,     0,    17,    23,
       0,     0,    47,     0,     0,     0,     0,     0,     0,     0,
      16,     0,    30,    78,     0,     9,    12,    28,    79,    10,
      11,     0,     0,    80,    97,     0,     0,    78,    95,    31,
       0,    96,     0,     0,    44,    14,    15,    80,     0,   103,
     104,   105,   106,    78,    13,    48,    54,    55,    53,    51,
      79,     0,    22,    59,    60,    62,    64,    61,    65,    83,
      84,     0,     5,     6,    33,    29,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
      86,    95,     0,     0,     0,     0,     4,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,     0,     0,    77,     7,     4,    27,    41,    76,    75,
      73,    72,    74,    69,    68,    70,    71,    81,     0,     0,
       0,    85,    68,   100,    93,    94,     8,    91,    87,    92,
      88,    89,    90,    99,    98,     0,     4,    37,     0,     0,
      49,     0,     0,     0,    63,     8,     0,     0,    36,    81,
       0,    18,     8,    38,     0,     0,     0,    47,    56,     0,
      34,     0,    82,     0,     0,    25,     0,    45,     0,     0,
       0,     0,    42,     0,    82,    19,     0,     4,    57,     0,
      58,    66,     0,     0,    39,     0,     8,    67,    43,    20,
       0,    46,     4,    40,     8,    21
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -93,   -93,   -92,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     -93,   -93,   -93,   -93,   -93,     9,   -93,    14,   -93,   -93,
     -93,   -69,    48,   -13,    47,   147,   -93,    -9,    -2,   -23,
     612,    28,    18,   118
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    19,    42,   174,   193,   202,    43,   146,
      20,    21,    96,    22,   115,    37,    32,    24,    25,   106,
     187,    54,    55,    56,    65,    66,    26,    57,    38,    39,
      40,    41,    28,    61
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      27,    64,    64,   -50,   136,    67,    67,   116,    -2,    45,
      58,    23,    90,    92,    69,    70,    71,   -52,    72,   129,
     105,    53,    76,   155,   -26,   158,   -39,     3,   109,    87,
      60,    29,    30,   -40,    91,    94,    95,    44,    77,    46,
      59,   -50,   -39,   -50,    94,    95,   -50,   -38,    74,   -40,
      49,    50,    51,    52,   162,   -52,    73,   -52,    75,   -35,
     -52,    31,   -26,   -38,    62,   177,   190,   131,    77,   109,
      88,   134,   135,   -35,    58,    89,   118,   119,   120,   121,
     122,   123,   124,   125,   126,    53,    58,   130,   -35,    33,
     107,   132,     5,     6,    60,   196,   103,    53,   180,    64,
     104,   143,   109,    67,    59,   148,    60,    58,   152,   153,
     204,    82,    83,    84,    85,    86,    59,   110,    53,    85,
      86,   133,   111,   114,   117,    33,   157,    60,     5,     6,
     112,   144,    16,    17,    27,   145,   147,    59,    94,    18,
     149,   163,   151,   161,   156,    23,    33,   166,    34,     5,
       6,   167,   185,    27,   171,   176,    35,   150,   173,   154,
      27,   164,    68,   178,    23,    58,   108,   181,    16,    17,
       0,    23,     0,     0,   186,    18,    53,   189,     0,     0,
       0,   192,     0,     0,     0,    60,     0,     0,     0,    16,
      17,   199,     0,   200,    27,    59,    36,     0,     0,     0,
      -3,     0,    27,     4,     0,    23,     5,     6,     7,     0,
       8,     9,    10,    23,     0,     0,     0,    11,    12,     0,
       4,     0,     0,     5,     6,     7,   -32,     8,     9,    10,
       0,    97,     0,    13,    11,    12,   -32,     0,    14,    15,
       0,   -37,    98,    99,   100,     0,    16,    17,     0,     0,
      13,     0,     0,    18,     0,    14,    15,   -37,   -37,    83,
      84,    85,    86,    16,    17,   101,   102,     0,     4,     0,
      18,     5,     6,     7,   -37,     8,     9,    10,     0,     0,
       0,     0,    11,    12,   170,     4,     0,     0,     5,     6,
       7,     0,     8,     9,    10,     0,     0,     0,    13,    11,
      12,   175,     0,    14,    15,     0,   -37,     0,     0,     0,
       0,    16,    17,     0,     0,    13,     0,     0,    18,     0,
      14,    15,   -37,   -37,     0,     0,     0,     0,    16,    17,
       0,     0,     0,     4,     0,    18,     5,     6,     7,   -37,
       8,     9,    10,     0,     0,     0,     0,    11,    12,   201,
       4,     0,     0,     5,     6,     7,     0,     8,     9,    10,
       0,     0,     0,    13,    11,    12,   205,     0,    14,    15,
      47,   -37,    34,     5,     6,     0,    16,    17,     0,     0,
      13,     0,    48,    18,     0,    14,    15,   -37,   -37,     0,
       0,     0,     0,    16,    17,     0,    49,    50,    51,    52,
      18,    78,    79,    33,   -37,    34,     5,     6,     0,     0,
       0,     0,     0,    16,    17,     0,    78,    79,     0,     0,
      18,     0,     0,   165,    80,    81,    82,    83,    84,    85,
      86,    78,    79,     0,     0,     0,     0,   159,   160,    80,
      81,    82,    83,    84,    85,    86,    16,    17,    78,    79,
       0,     0,   172,    18,    80,    81,    82,    83,    84,    85,
      86,     0,     0,    78,    79,     0,     0,   127,   128,    63,
       0,    80,    81,    82,    83,    84,    85,    86,    78,    79,
       0,    48,     0,     0,   159,   160,    80,    81,    82,    83,
      84,    85,    86,    78,    79,    49,    50,    51,    52,   168,
     169,    80,    81,    82,    83,    84,    85,    86,    78,    79,
       0,     0,     0,     0,   182,   183,    80,    81,    82,    83,
      84,    85,    86,    78,    79,     0,     0,     0,     0,   194,
     195,    80,    81,    82,    83,    84,    85,    86,    78,    79,
       0,     0,     0,     0,     0,   179,    80,    81,    82,    83,
      84,    85,    86,    78,    79,     0,     0,     0,     0,   184,
       0,    80,    81,    82,    83,    84,    85,    86,    78,    79,
       0,     0,     0,     0,   188,     0,    80,    81,    82,    83,
      84,    85,    86,    78,    79,     0,     0,     0,     0,   191,
       0,    80,    81,    82,    83,    84,    85,    86,    78,    79,
       0,     0,     0,     0,   197,     0,    80,    81,    82,    83,
      84,    85,    86,    78,    79,     0,     0,     0,     0,   198,
       0,    80,    81,    82,    83,    84,    85,    86,    78,    79,
       0,     0,     0,     0,   203,     0,    80,    81,    82,    83,
      84,    85,    86,    78,    79,     0,     0,   113,     0,     0,
       0,    80,    81,    82,    83,    93,    85,    86,    78,    79,
       0,     0,   113,    78,    79,   165,    80,    81,    82,    83,
      84,    85,    86,    78,    79,     0,     0,     0,     0,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    82,    83,
      84,    85,    86,    78,    79,     0,    80,    81,    82,    83,
      93,    85,    86,    81,    82,    83,    84,    85,    86,   137,
     138,   139,   140,   141,   142,     0,    80,    81,    82,     0,
       0,    85,    86
};

static const yytype_int16 yycheck[] =
{
       2,    14,    15,    14,    96,    14,    15,    76,     0,    11,
      12,     2,    35,    36,    16,    17,    18,    14,    14,    88,
      43,    12,    41,   115,    14,    54,    41,     0,    57,    31,
      12,    25,    26,    41,    36,    22,    23,     3,    57,    11,
      12,    52,    57,    54,    22,    23,    57,    41,     9,    57,
      29,    30,    31,    32,   146,    52,    52,    54,    19,    53,
      57,    55,    52,    57,     3,    54,    54,    54,    57,    57,
      53,    94,    95,    53,    76,    55,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    76,    88,    89,    53,     3,
      55,    93,     6,     7,    76,   187,    47,    88,   167,   112,
       3,   103,    57,   112,    76,   107,    88,   109,   110,   111,
     202,    45,    46,    47,    48,    49,    88,    55,   109,    48,
      49,    93,    55,    14,     3,     3,   128,   109,     6,     7,
      57,   103,    46,    47,   136,    41,    53,   109,    22,    53,
      55,     3,    56,   145,    55,   136,     3,   149,     5,     6,
       7,    53,    42,   155,   156,    55,    13,   109,   160,   112,
     162,   147,    15,   165,   155,   167,    48,   169,    46,    47,
      -1,   162,    -1,    -1,   176,    53,   167,   179,    -1,    -1,
      -1,   183,    -1,    -1,    -1,   167,    -1,    -1,    -1,    46,
      47,   193,    -1,   195,   196,   167,    53,    -1,    -1,    -1,
       0,    -1,   204,     3,    -1,   196,     6,     7,     8,    -1,
      10,    11,    12,   204,    -1,    -1,    -1,    17,    18,    -1,
       3,    -1,    -1,     6,     7,     8,     9,    10,    11,    12,
      -1,    24,    -1,    33,    17,    18,    19,    -1,    38,    39,
      -1,    41,    35,    36,    37,    -1,    46,    47,    -1,    -1,
      33,    -1,    -1,    53,    -1,    38,    39,    57,    41,    46,
      47,    48,    49,    46,    47,    58,    59,    -1,     3,    -1,
      53,     6,     7,     8,    57,    10,    11,    12,    -1,    -1,
      -1,    -1,    17,    18,    19,     3,    -1,    -1,     6,     7,
       8,    -1,    10,    11,    12,    -1,    -1,    -1,    33,    17,
      18,    19,    -1,    38,    39,    -1,    41,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    33,    -1,    -1,    53,    -1,
      38,    39,    57,    41,    -1,    -1,    -1,    -1,    46,    47,
      -1,    -1,    -1,     3,    -1,    53,     6,     7,     8,    57,
      10,    11,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
       3,    -1,    -1,     6,     7,     8,    -1,    10,    11,    12,
      -1,    -1,    -1,    33,    17,    18,    19,    -1,    38,    39,
       3,    41,     5,     6,     7,    -1,    46,    47,    -1,    -1,
      33,    -1,    15,    53,    -1,    38,    39,    57,    41,    -1,
      -1,    -1,    -1,    46,    47,    -1,    29,    30,    31,    32,
      53,    20,    21,     3,    57,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,    46,    47,    -1,    20,    21,    -1,    -1,
      53,    -1,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    20,    21,    -1,    -1,    -1,    -1,    56,    57,    43,
      44,    45,    46,    47,    48,    49,    46,    47,    20,    21,
      -1,    -1,    56,    53,    43,    44,    45,    46,    47,    48,
      49,    -1,    -1,    20,    21,    -1,    -1,    56,    57,     3,
      -1,    43,    44,    45,    46,    47,    48,    49,    20,    21,
      -1,    15,    -1,    -1,    56,    57,    43,    44,    45,    46,
      47,    48,    49,    20,    21,    29,    30,    31,    32,    56,
      57,    43,    44,    45,    46,    47,    48,    49,    20,    21,
      -1,    -1,    -1,    -1,    56,    57,    43,    44,    45,    46,
      47,    48,    49,    20,    21,    -1,    -1,    -1,    -1,    56,
      57,    43,    44,    45,    46,    47,    48,    49,    20,    21,
      -1,    -1,    -1,    -1,    -1,    57,    43,    44,    45,    46,
      47,    48,    49,    20,    21,    -1,    -1,    -1,    -1,    56,
      -1,    43,    44,    45,    46,    47,    48,    49,    20,    21,
      -1,    -1,    -1,    -1,    56,    -1,    43,    44,    45,    46,
      47,    48,    49,    20,    21,    -1,    -1,    -1,    -1,    56,
      -1,    43,    44,    45,    46,    47,    48,    49,    20,    21,
      -1,    -1,    -1,    -1,    56,    -1,    43,    44,    45,    46,
      47,    48,    49,    20,    21,    -1,    -1,    -1,    -1,    56,
      -1,    43,    44,    45,    46,    47,    48,    49,    20,    21,
      -1,    -1,    -1,    -1,    56,    -1,    43,    44,    45,    46,
      47,    48,    49,    20,    21,    -1,    -1,    54,    -1,    -1,
      -1,    43,    44,    45,    46,    47,    48,    49,    20,    21,
      -1,    -1,    54,    20,    21,    42,    43,    44,    45,    46,
      47,    48,    49,    20,    21,    -1,    -1,    -1,    -1,    20,
      21,    43,    44,    45,    46,    47,    48,    49,    45,    46,
      47,    48,    49,    20,    21,    -1,    43,    44,    45,    46,
      47,    48,    49,    44,    45,    46,    47,    48,    49,    97,
      98,    99,   100,   101,   102,    -1,    43,    44,    45,    -1,
      -1,    48,    49
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    61,    62,     0,     3,     6,     7,     8,    10,    11,
      12,    17,    18,    33,    38,    39,    46,    47,    53,    63,
      70,    71,    73,    75,    77,    78,    86,    88,    92,    25,
      26,    55,    76,     3,     5,    13,    53,    75,    88,    89,
      90,    91,    64,    68,     3,    88,    91,     3,    15,    29,
      30,    31,    32,    75,    81,    82,    83,    87,    88,    91,
      92,    93,     3,     3,    83,    84,    85,    87,    85,    88,
      88,    88,    14,    52,     9,    19,    41,    57,    20,    21,
      43,    44,    45,    46,    47,    48,    49,    88,    53,    55,
      89,    88,    89,    47,    22,    23,    72,    24,    35,    36,
      37,    58,    59,    47,     3,    89,    79,    55,    93,    57,
      55,    55,    57,    54,    14,    74,    81,     3,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    56,    57,    81,
      88,    54,    88,    91,    89,    89,    62,    90,    90,    90,
      90,    90,    90,    88,    91,    41,    69,    53,    88,    55,
      82,    56,    88,    88,    84,    62,    55,    88,    54,    56,
      57,    88,    62,     3,    77,    42,    88,    53,    56,    57,
      19,    88,    56,    88,    65,    19,    55,    54,    88,    57,
      81,    88,    56,    57,    56,    42,    88,    80,    56,    88,
      54,    56,    88,    66,    56,    57,    62,    56,    56,    88,
      88,    19,    67,    56,    62,    19
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    61,    62,    62,    62,    62,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    64,    65,    66,
      67,    63,    63,    68,    69,    63,    63,    63,    63,    70,
      70,    72,    71,    74,    73,    76,    75,    77,    77,    77,
      77,    77,    77,    77,    79,    80,    78,    81,    81,    81,
      82,    82,    82,    82,    82,    82,    83,    83,    83,    84,
      84,    84,    85,    85,    86,    86,    87,    87,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    90,    91,    91,    91,
      91,    92,    92,    93,    93,    93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     0,     3,     3,     4,     0,     1,
       2,     2,     1,     2,     2,     2,     1,     0,     0,     0,
       0,    12,     2,     0,     0,     6,     1,     3,     1,     2,
       1,     0,     4,     0,     5,     0,     5,     0,     1,     4,
       6,     3,     6,     8,     0,     0,     9,     0,     1,     3,
       1,     1,     1,     1,     1,     1,     4,     6,     6,     1,
       1,     1,     1,     3,     2,     2,     6,     7,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     4,     6,     2,     2,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     3,     3,
       3,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 320 "neblina.y" /* yacc.c:1646  */
    { ibytecode( HALT, 0 ); }
#line 1767 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 328 "neblina.y" /* yacc.c:1646  */
    {}
#line 1773 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 329 "neblina.y" /* yacc.c:1646  */
    {}
#line 1779 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 330 "neblina.y" /* yacc.c:1646  */
    {}
#line 1785 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 332 "neblina.y" /* yacc.c:1646  */
    {}
#line 1791 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 333 "neblina.y" /* yacc.c:1646  */
    { context_check( INC, (yyvsp[0].pChar) ); }
#line 1797 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 334 "neblina.y" /* yacc.c:1646  */
    { context_check( DEC, (yyvsp[0].pChar) ); }
#line 1803 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 335 "neblina.y" /* yacc.c:1646  */
    {}
#line 1809 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 337 "neblina.y" /* yacc.c:1646  */
    { if( curr_context == LOCAL_CTX ){ 
                                ibytecode( GOTO_ST, 0 ); 
                                if( reg_func->rparams == 0 ){
                                    reg_func->rparams = (yyvsp[0].vInt); 
                                } else {
                                    if(reg_func->rparams != (yyvsp[0].vInt)) {
                                        yyerror("different number of return values");
                                    }
                                }
                                
                           } else { yyerror("unexpected 'return' statement");  } }
#line 1825 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 348 "neblina.y" /* yacc.c:1646  */
    { ibytecode( WRITE, 0 ); }
#line 1831 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 349 "neblina.y" /* yacc.c:1646  */
    { ibytecode( WRITE, 0 ); }
#line 1837 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 352 "neblina.y" /* yacc.c:1646  */
    { if(curr_context == LOCAL_CTX ){ func_add_scope(); }}
#line 1843 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 352 "neblina.y" /* yacc.c:1646  */
    {  (yyvsp[-4].vFor) = new_forlabel(); context_install( STORE, (yyvsp[-2].pChar) ); }
#line 1849 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 353 "neblina.y" /* yacc.c:1646  */
    { (yyvsp[-6].vFor)->goto_addr = cur_location(); }
#line 1855 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 353 "neblina.y" /* yacc.c:1646  */
    {   
                           context_check( LOAD, (yyvsp[-6].pChar) ); 
                           ibytecode( GEQ, 0 ); 
                           (yyvsp[-8].vFor)->jmp_false_addr = reserve_location(); }
#line 1864 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 357 "neblina.y" /* yacc.c:1646  */
    {  ibytecode_addr( JMP_FALSE, cur_location() + 2, (yyvsp[-11].vFor)->jmp_false_addr);
                     context_check( INC, (yyvsp[-9].pChar) );
                     if(curr_context == LOCAL_CTX ) func_release_scope();
                     ibytecode(GOTO, (yyvsp[-11].vFor)->goto_addr); 
                     release_forlabel((yyvsp[-11].vFor) );   
                   }
#line 1875 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 363 "neblina.y" /* yacc.c:1646  */
    {             if( package_add( (yyvsp[0].pChar) ) == -1 ) {

                                            cyyerror( "package '%s' not found", (yyvsp[0].pChar) );
                                            exit( 1 );
                                        }
                           
                           }
#line 1887 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 371 "neblina.y" /* yacc.c:1646  */
    {      if(curr_context == LOCAL_CTX ){ func_add_scope(); }
                    
                   (yyvsp[0].vWhile) = new_whilelabel(); 
                   (yyvsp[0].vWhile)->goto_addr = cur_location(); 
                    }
#line 1897 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 376 "neblina.y" /* yacc.c:1646  */
    { (yyvsp[-2].vWhile)->jmp_false_addr = reserve_location(); }
#line 1903 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 377 "neblina.y" /* yacc.c:1646  */
    {
                     ibytecode_addr( JMP_FALSE, cur_location() + 1, (yyvsp[-5].vWhile)->jmp_false_addr);
                     ibytecode(GOTO, (yyvsp[-5].vWhile)->goto_addr); 
                     release_whilelabel( (yyvsp[-5].vWhile) );
                     if(curr_context == LOCAL_CTX ) func_release_scope();
                 }
#line 1914 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 383 "neblina.y" /* yacc.c:1646  */
    {  pop_return_values( (yyvsp[0].vInt) ); if( (yyvsp[0].vInt) != 0 ) { yyerror("function returns one or more values"); } }
#line 1920 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 384 "neblina.y" /* yacc.c:1646  */
    { if((yyvsp[-2].vInt) != (yyvsp[0].vInt) ) yyerror("invalid number of assignment variables"); if( (yyvsp[0].vInt) == 0 ) { yyerror("syntax error, expected a assignment value"); } else { store_rparams(); } }
#line 1926 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 385 "neblina.y" /* yacc.c:1646  */
    { store_rparams(); }
#line 1932 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 388 "neblina.y" /* yacc.c:1646  */
    { ibytecode_addr( JMP_FALSE, cur_location(), pop_location() );  if(curr_context == LOCAL_CTX ) func_release_scope(); }
#line 1938 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 391 "neblina.y" /* yacc.c:1646  */
    {  
                      if(curr_context == LOCAL_CTX )
                      { 
                        func_add_scope();
                        push_location( reserve_location() );
                      }
                   }
#line 1950 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 398 "neblina.y" /* yacc.c:1646  */
    {}
#line 1956 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 400 "neblina.y" /* yacc.c:1646  */
    {  ibytecode_addr( JMP_FALSE, cur_location() + 1, pop_location() ); push_location( reserve_location() );   
                if(curr_context == LOCAL_CTX ) {  func_release_scope(); func_add_scope(); } }
#line 1963 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 402 "neblina.y" /* yacc.c:1646  */
    { ibytecode_addr( GOTO, cur_location(), pop_location() );  if(curr_context == LOCAL_CTX ) func_release_scope();  }
#line 1969 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 405 "neblina.y" /* yacc.c:1646  */
    { if( package_func_get_any( (yyvsp[0].pChar) ) == NULL ) push_location( reserve_location() );  }
#line 1975 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 405 "neblina.y" /* yacc.c:1646  */
    {   

func_info_t * f; 
if( ( f = package_func_get( (yyvsp[-4].pChar), (yyvsp[-1].vInt) )) != NULL ) {                                          
    object_t o; pvalue( o ) = f; type( o ) = T_CFUNC; (yyval.vInt) = f->nreturns; bytecode( CALL_C, o );
} else {                                                                        
    function_t * t = func_get((yyvsp[-4].pChar), (yyvsp[-1].vInt)); 
    if( t == NULL ) { 
        if( package_func_get_any( (yyvsp[-4].pChar) ) == NULL ) {
            char strerr[1024]; sprintf(strerr, "function '%s' not defined", (yyvsp[-4].pChar)); yyerror( strerr ); 
        } else {
            char strerr[1024]; sprintf(strerr, "function '%s' with %d arguments not defined", (yyvsp[-4].pChar), (yyvsp[-1].vInt)); yyerror( strerr ); 
        }

        exit( 1 );
    }
    else if( t == (function_t *)FUNC_INVALID_PARAMS ) {
        char err[1024]; sprintf( err, "invalid number of parameters on call '%s'", (yyvsp[-4].pChar));
        yyerror(err);
        exit( 1 );
    } else { 
        ibytecode(CALL, t->init_addr +1 );  
        int loc = pop_location();  
        ibytecode_addr(PUSH_PC, cur_location() - loc - 1, loc );
        
        (yyval.vInt) = t->rparams;
    }  
                                                                              
                                                                              }
                                                                             }
#line 2010 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 438 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 0;}
#line 2016 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 439 "neblina.y" /* yacc.c:1646  */
    { strcpy( reg_param[reg_param_idx++], (yyvsp[0].pChar) ); (yyval.vInt) = 1; }
#line 2022 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 440 "neblina.y" /* yacc.c:1646  */
    { strcpy(reg_param[reg_param_idx], "#" );  strcat( reg_param[reg_param_idx++], (yyvsp[-3].pChar) ); (yyval.vInt) = 1; }
#line 2028 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 441 "neblina.y" /* yacc.c:1646  */
    { strcpy(reg_param[reg_param_idx], "#" );  strcat( reg_param[reg_param_idx++], (yyvsp[-5].pChar) ); (yyval.vInt) = 1; }
#line 2034 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 442 "neblina.y" /* yacc.c:1646  */
    { if( (yyval.vInt) == 0 ) { yyerror("unexpected ','"); } else{ strcpy( reg_param[reg_param_idx++], (yyvsp[0].pChar) ); (yyval.vInt)++;} }
#line 2040 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 443 "neblina.y" /* yacc.c:1646  */
    {if( (yyval.vInt) == 0 ) { yyerror("unexpected ','"); } else{ strcpy(reg_param[reg_param_idx], "#" ); strcat( reg_param[reg_param_idx++], (yyvsp[-3].pChar) ); (yyval.vInt)++;} }
#line 2046 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 444 "neblina.y" /* yacc.c:1646  */
    {if( (yyval.vInt) == 0 ) { yyerror("unexpected ','"); } else{ strcpy(reg_param[reg_param_idx], "#" ); strcat( reg_param[reg_param_idx++], (yyvsp[-5].pChar) ); (yyval.vInt)++;} }
#line 2052 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 454 "neblina.y" /* yacc.c:1646  */
    { 
                     reg_min_store = 0x0FFFFFFF;
                     reg_max_store = 0;
                     curr_context = LOCAL_CTX; (yyvsp[-1].vFunc) = func_new(); reg_func = (yyvsp[-1].vFunc); (yyvsp[-1].vFunc)->rparams = 0;
                     (yyvsp[-1].vFunc)->init_addr = reserve_location(); 
                     strcpy( (yyvsp[-1].vFunc)->name, (yyvsp[0].pChar) );
                     strcpy( curr_func_name, (yyvsp[0].pChar) );
                     func_add( (yyvsp[-1].vFunc) );         
                   }
#line 2066 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 463 "neblina.y" /* yacc.c:1646  */
    {  store_rparams_func(); (yyvsp[-5].vFunc)->nparams = -1; 
                              if( func_install( (yyvsp[-5].vFunc), (yyvsp[-1].vInt) ) != FUNC_NO_ERROR ) {
                                char str[256];
                                sprintf(str, "redefinition of function %s", (yyvsp[-4].pChar));
                                yyerror( str );
                              }                      
                           }
#line 2078 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 471 "neblina.y" /* yacc.c:1646  */
    {  
                        (yyvsp[-8].vFunc)->min_store = reg_min_store;
                        (yyvsp[-8].vFunc)->max_store = reg_max_store;
                        
                               
                        (yyvsp[-8].vFunc)->final_addr = cur_location(); 
                    
                        if( strcmp((yyvsp[-8].vFunc)->name, "main") == 0 ) {
                            ibytecode_addr(GOTO, (yyvsp[-8].vFunc)->init_addr + 1,  (yyvsp[-8].vFunc)->init_addr );
                        } else {
                            ibytecode( GOTO_ST, 0 );
                            ibytecode_addr(GOTO, (yyvsp[-8].vFunc)->final_addr + 1,  (yyvsp[-8].vFunc)->init_addr );                   
                        }
                        
//                        printf("Last : %d POS : %d\n", reg_laststore, reg_store - $<vInt>5);
                        
                        
                        curr_context = GLOBAL_CTX;
                     }
#line 2102 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 492 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 0; }
#line 2108 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 493 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = (yyvsp[0].vInt); }
#line 2114 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 494 "neblina.y" /* yacc.c:1646  */
    { if((yyval.vInt) == 0) yyerror("syntax error"); (yyval.vInt) += (yyvsp[0].vInt);   }
#line 2120 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 497 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = (yyvsp[0].vInt); }
#line 2126 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 498 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1;  }
#line 2132 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 499 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1;  }
#line 2138 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 500 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1;  }
#line 2144 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 501 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1;  }
#line 2150 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 502 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1;  }
#line 2156 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 504 "neblina.y" /* yacc.c:1646  */
    { object_t arg; avalue(arg) = ((yyvsp[-3].vInt) & 0xFF) | (T_VECTOR << 8);  bytecode( ALLOCV, arg);  }
#line 2162 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 505 "neblina.y" /* yacc.c:1646  */
    { context_check( LOAD, (yyvsp[-5].pChar) ); object_t arg; avalue(arg) = (T_VECTOR & 0xFF)   | (T_NDEF << 8);  bytecode( ALLOCV, arg);  }
#line 2168 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 506 "neblina.y" /* yacc.c:1646  */
    { object_t arg; avalue(arg) = ((yyvsp[-5].vInt) & 0xFF) | (T_VECTOR << 8) | ((yyvsp[-1].vInt) << 16);  bytecode( ALLOCV, arg); }
#line 2174 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 508 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1; context_check( LOAD, (yyvsp[0].pChar) ); }
#line 2180 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 509 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1; }
#line 2186 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 510 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1; }
#line 2192 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 513 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = 1; }
#line 2198 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 514 "neblina.y" /* yacc.c:1646  */
    { if((yyval.vInt) == 0) yyerror("syntax error"); (yyval.vInt) += (yyvsp[-2].vInt); }
#line 2204 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 516 "neblina.y" /* yacc.c:1646  */
    { 
                                int i = 0;
                                if((yyvsp[0].vInt) == 0) yyerror("syntax error");
                                for(i=0; i < (yyvsp[0].vInt); i++ ) {
                                    func_info_t * f = package_func_get( "movetodev", 1 );             
                                    object_t o; pvalue( o ) = f; type( o ) = T_CFUNC; bytecode( CALL_C, o );
                                }
                       }
#line 2217 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 525 "neblina.y" /* yacc.c:1646  */
    {      
                                int i = 0;
                                if((yyvsp[0].vInt) == 0) yyerror("syntax error");
                                for(i=0; i < (yyvsp[0].vInt); i++ ) {
                                    func_info_t * f = package_func_get( "movetohost", 1 );             
                                    object_t o; pvalue( o ) = f; type( o ) = T_CFUNC; bytecode( CALL_C, o );
                                }      
                       }
#line 2230 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 533 "neblina.y" /* yacc.c:1646  */
    { object_t arg; avalue(arg) = ((yyvsp[-5].vInt) & 0xFF) | (T_MATRIX << 8);  bytecode( ALLOCV, arg);  }
#line 2236 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 534 "neblina.y" /* yacc.c:1646  */
    { object_t arg; avalue(arg) = ((yyvsp[-5].vInt) & 0xFF) | (T_SMATRIX << 8);  bytecode( ALLOCV, arg);  }
#line 2242 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 536 "neblina.y" /* yacc.c:1646  */
    { ibytecode( ADD, 0 ); }
#line 2248 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 537 "neblina.y" /* yacc.c:1646  */
    { ibytecode( SUB, 0 ); }
#line 2254 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 538 "neblina.y" /* yacc.c:1646  */
    { ibytecode( MUL, 0 ); }
#line 2260 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 539 "neblina.y" /* yacc.c:1646  */
    { ibytecode( DIV, 0 ); }
#line 2266 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 540 "neblina.y" /* yacc.c:1646  */
    { ibytecode( ANDA, 0 ); }
#line 2272 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 541 "neblina.y" /* yacc.c:1646  */
    { ibytecode( ORA, 0 ); }
#line 2278 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 542 "neblina.y" /* yacc.c:1646  */
    { ibytecode( MOD, 0 ); }
#line 2284 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 543 "neblina.y" /* yacc.c:1646  */
    { ibytecode( SHR, 0 ); }
#line 2290 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 544 "neblina.y" /* yacc.c:1646  */
    { ibytecode( SHL, 0 ); }
#line 2296 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 545 "neblina.y" /* yacc.c:1646  */
    {}
#line 2302 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 546 "neblina.y" /* yacc.c:1646  */
    {}
#line 2308 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 547 "neblina.y" /* yacc.c:1646  */
    {}
#line 2314 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 548 "neblina.y" /* yacc.c:1646  */
    { context_check( LOAD, (yyvsp[0].pChar) ); }
#line 2320 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 549 "neblina.y" /* yacc.c:1646  */
    { context_check( LOADV, (yyvsp[-3].pChar) ); }
#line 2326 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 550 "neblina.y" /* yacc.c:1646  */
    { context_check( LOADV, (yyvsp[-5].pChar) ); }
#line 2332 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 551 "neblina.y" /* yacc.c:1646  */
    { ibytecode( SIGCH, 0 ); }
#line 2338 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 552 "neblina.y" /* yacc.c:1646  */
    {}
#line 2344 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 557 "neblina.y" /* yacc.c:1646  */
    { ibytecode( NEGL, 0 );  }
#line 2350 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 558 "neblina.y" /* yacc.c:1646  */
    { ibytecode( LEQ, 0 );  }
#line 2356 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 559 "neblina.y" /* yacc.c:1646  */
    { ibytecode( GEQ, 0 );  }
#line 2362 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 560 "neblina.y" /* yacc.c:1646  */
    { ibytecode( GT, 0 );   }
#line 2368 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 561 "neblina.y" /* yacc.c:1646  */
    { ibytecode( LT, 0 );   }
#line 2374 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 562 "neblina.y" /* yacc.c:1646  */
    { ibytecode( EQ, 0 );   }
#line 2380 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 563 "neblina.y" /* yacc.c:1646  */
    { ibytecode( NEQ, 0 );  }
#line 2386 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 564 "neblina.y" /* yacc.c:1646  */
    { ibytecode( ANDL, 0 ); }
#line 2392 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 565 "neblina.y" /* yacc.c:1646  */
    { ibytecode( ORL, 0 );  }
#line 2398 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 571 "neblina.y" /* yacc.c:1646  */
    { 
                                   object_t v; 
                                   type(v) = T_STRING; 
                                   svalue(v) = (char *) malloc( sizeof(char)*(strlen((yyvsp[0].pStr)) + 1) ); 
                                   strcpy( svalue(v), (yyvsp[0].pStr) ); 
                                   bytecode( ALLOCS, v ); 
                                 }
#line 2410 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 579 "neblina.y" /* yacc.c:1646  */
    { ibytecode( APPEND, 0 ); }
#line 2416 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 580 "neblina.y" /* yacc.c:1646  */
    { ibytecode( APPEND, 0 ); }
#line 2422 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 581 "neblina.y" /* yacc.c:1646  */
    { ibytecode( APPEND, 0 ); }
#line 2428 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 584 "neblina.y" /* yacc.c:1646  */
    { object_t v; type(v) = T_INT; ivalue(v) = (yyvsp[0].vInt);      bytecode( PUSH, v ); }
#line 2434 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 585 "neblina.y" /* yacc.c:1646  */
    { object_t v; type(v) = T_FLOAT ; fvalue(v) = (yyvsp[0].vFloat); bytecode( PUSH, v ); }
#line 2440 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 588 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = (yyvsp[0].vInt); }
#line 2446 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 589 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = (yyvsp[0].vInt); }
#line 2452 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 590 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = (yyvsp[0].vInt); }
#line 2458 "neblina.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 591 "neblina.y" /* yacc.c:1646  */
    { (yyval.vInt) = (yyvsp[0].vInt); }
#line 2464 "neblina.tab.c" /* yacc.c:1646  */
    break;


#line 2468 "neblina.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 593 "neblina.y" /* yacc.c:1906  */

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

