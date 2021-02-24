/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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
#line 264 "neblina.y" /* yacc.c:1909  */

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

#line 112 "neblina.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_NEBLINA_TAB_H_INCLUDED  */
