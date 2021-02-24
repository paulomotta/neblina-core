#ifndef __SYMBOL_H_
#define __SYMBOL_H_

#include <string.h>
#include <stdlib.h>

#include "neblina.h"
#include "neblina_vm.h"

int get_position_avail( int len );


int sym_put_global( const char sym_name[256] );

symbol_t * sym_get_global( const char sym_name[256] ) ;
 
/**
 * 1, if are equals,
 * 0, otherwise
 */
int obj_cmp( object_t v1, object_t v2 );


int sym_install_global( const char sym_name[256] ) ;

int func_add( function_t * f );
void func_put( const char func_name[256], int init, int final, int nparams );
function_t * func_get( const char func_name[256], int nparams ) ;

int func_install_parms( const char func_name[256], int init, int final, int nparams );

int func_install( function_t * f, int nparams );
void func_add_scope();
int func_put_sym( const char sym_name[256] );
symbol_t * func_get_sym( const char sym_name[256] ) ;
void func_release_sym( symbol_t * ptr ) ;

void func_release_sym_table( symbol_table_t * ptr );

void func_release_scope();

function_t * func_new() ;
/**
 * Have main function? 1: yes
 *                     0: no
 *
 */

int func_has_main();




#endif

