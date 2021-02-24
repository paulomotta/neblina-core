%x incl 
%x IN_COMMENT
%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "neblina.tab.h"
#include "neblina.h"
#define MAX_INCLUDE_DEPTH 100
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int   curr_line[MAX_INCLUDE_DEPTH];
char  file_names[MAX_INCLUDE_DEPTH][256];
char  file_names_hist[MAX_INCLUDE_DEPTH][256];
int   include_hist = 0;
int   include_stack_ptr = 0;
int   current_file = 0;
int   ant_file[MAX_INCLUDE_DEPTH];
int   ant_top = 0;
void yyerror(const char *s);
int alreadyInclude( char filename[256] ) {
    int i = 0;

    for( i = 1; i < include_hist; i++ ) {
        if( strcmp( filename, file_names_hist[i] ) == 0 ) {
    //        ant_file = current_file;
            current_file = include_hist;
            return 1;        
        }
    }
    ant_file[ant_top++] = current_file;
    current_file = include_hist;
    return 0;
}

%}

%%
<INITIAL>{
     "/*"              BEGIN(IN_COMMENT);
}
<IN_COMMENT>{
"*/"      BEGIN(INITIAL);
[^*\n]+   // eat comment in chunks
"*"       // eat the lone star
\n        curr_line[include_stack_ptr-1]++; yylineno++;
}

"--".*					 ;
[\n]                { curr_line[include_stack_ptr-1]++; return EOL; }			
[ \t]               {                    }  
[:]                      { return TWO_DOTS;  }
include             BEGIN(incl);


<incl>[ \t]*      /* eat the whitespace */
<incl>[^ \t\n]+   { /* got the include file name */
            if ( include_stack_ptr >= MAX_INCLUDE_DEPTH )
            {
                yyerror("includes nested too deeply" );
                exit( 1 );
            }

        include_stack[include_stack_ptr] = YY_CURRENT_BUFFER;      
        curr_line[include_stack_ptr] = 0;

        strcpy( file_names[include_stack_ptr], yytext );
        strcat( file_names[include_stack_ptr], ".nbl" );
        if( !alreadyInclude( file_names[include_stack_ptr])  ) {
            char fileName[1024];
            fileName[0] = 0;
            strcat( fileName, "/etc/neblina/" );
            strcat( fileName, file_names[include_stack_ptr] ); 
            yyin = fopen( fileName, "r" );
            if( yyin ) {
                strcpy( file_names[include_stack_ptr], fileName );
            } else {
                yyin = fopen( file_names[include_stack_ptr], "r" );
                if ( !yyin ) {
                    char err[1024];
                    sprintf(err, "file '%s' not found", file_names[include_stack_ptr] );
                    yyerror(err );
                    exit( 1 );        
                }
            }
            strcpy( file_names_hist[include_hist++], file_names[include_stack_ptr] );
            include_stack_ptr++;
            yy_switch_to_buffer(yy_create_buffer( yyin, YY_BUF_SIZE ) );


        }
            BEGIN(INITIAL);
        }


using                    { return USING;     }
if                       { return IF;        }
end                      { return END;       }
return                   { return RETURN;    }
for                      { return FOR;       }
while                    { return WHILE;     }
and                      { return AND;       }
or                       { return OR;        }
else                     { return ELSE;      }
def                      { return FUNCTION;  }
int                      { yylval.vInt = T_INT; return DT_INT;             }
float                    { yylval.vInt = T_FLOAT; return DT_FLOAT;         }
string                   { yylval.vInt = T_STRING; return DT_STRING;       }
complex                  { yylval.vInt = T_COMPLEX; return DT_COMPLEX;     }
endprog                  { return ENDPROG;   }
sparse                   { return SPARSE;    }
[!]                      { return NEGT;      }
==                       { return C_EQ;      } 
[+][ \t]*[+]             { return INCR;      }
[-][ \t]*[-]             { return DECR;      }
=                        { return ASSIG;     }
[>][>][>]                { return TODEV; }
[<][<][<]                { return TOHOST; }
[>][>]                   { return SHIFTR; }
[<][<]                   { return SHIFTL; }

[<][=]                   { return LESSEQ; }  
[>][=]                   { return GREATEQ; } 
[!][=]                   { return DIFF; }  
[a-zA-Z_][a-zA-Z0-9_]*  {
					        strcpy( yylval.pChar, yytext);
					        return NAME;
				        }
[$][a-zA-Z_][a-zA-Z0-9_]*  {
					        strcpy( yylval.pChar, yytext);
					        return DNAME;
				        }
\'[^\']*\'			  {
                        yylval.pStr = (char *) malloc( sizeof(char) * strlen(yytext) - 1);
                        strncpy( yylval.pStr, yytext + 1, strlen(yytext) - 2 );
                        yylval.pStr[strlen(yytext) - 2] = 0;
				        return STRING;
				      }				    
\"[^\"]*\"			  {

                        yylval.pStr = (char *) malloc( sizeof(char) * strlen(yytext) - 1);
                        strncpy( yylval.pStr, yytext + 1, strlen(yytext) - 2 );
                        yylval.pStr[strlen(yytext) - 2] = 0;
				        return STRING;
	      }				        
[0-9]+  |
0[xX][0-9a-fA-F]+      {     
                                            if( strlen( yytext ) >= 2 && toupper( yytext[1] ) == 'X' ) {
                                               yylval.vInt = (int) strtoul(yytext, NULL, 16 );
                                            } else {
                                                yylval.vInt = atoi(yytext);
                    				        }
                    				        return INT;           	    }				      
[0-9]+("."[0-9]*)	|
([0-9]+)?"."[0-9]+	|
[0-9]+("."[0-9]*)([dDeEgG][+-])?[0-9]+ |
([0-9]+)?"."[0-9]+(([dDeEgG][+-])?[0-9]+)? {
				                            yylval.vFloat = atof(yytext);
                    				        return FLOAT;
				                      }

				                      
	        

.					{ return  yytext[0]; }

<<EOF>> {
        if ( --include_stack_ptr < 1 )
            {
            yyterminate();
            }   
        else
            {
        
            current_file = ant_file[--ant_top];
            yy_delete_buffer( YY_CURRENT_BUFFER );
            yy_switch_to_buffer(
                 include_stack[include_stack_ptr] );
            }

}

