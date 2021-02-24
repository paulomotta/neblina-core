#include <string.h>
#include <stdlib.h>

#include "neblina.h"
#include "neblina_vm.h"
#include "neblina_sym.h"

symbol_table_t      * global    = NULL;
function_table_t    * functions = NULL;
object_t        ** memory;
unsigned char   * mem_avail;
int               mem_curr_size = 512;
int      data_location = 0;

int get_position_avail( int len ) {
    int i = 0;
    for( i = 0; i < mem_curr_size; i++ ) {
        if( mem_avail[i] == MEM_AVAIL ) {
            mem_avail[i] = MEM_ALLOCATED;
            return i;
        }            
    }    
    return 0;
}


int sym_put_global( const char sym_name[256] ) {

      symbol_table_t *ptr;
      ptr = (symbol_table_t *) malloc ( sizeof(symbol_table_t) );
      if( !ptr )
        return SYM_MEM_FAIL;

      ptr->value = (symbol_t *) malloc ( sizeof(symbol_t) );
      if( !ptr->value )
        return SYM_MEM_FAIL;

      strcpy (ptr->value->name,sym_name);
      ptr->value->position = get_position_avail( 1 );
      ptr->next = global;
      global = ptr;
      return SYM_NO_ERROR;

}

symbol_t * sym_get_global( const char sym_name[256] ) {

    symbol_table_t * tmp = global;
    while( tmp != NULL ) {
        if( strcmp( tmp->value->name, sym_name ) == 0 )
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}
 
/**
 * 1, if are equals,
 * 0, otherwise
 */
int obj_cmp( object_t v1, object_t v2 ) {

    if( type(v1) != type(v2) )    
        return 0;
    else {
        int ret = 0;
        switch( type(v1) ) {
            
            case T_INT:
                ret = (ivalue(v1) == ivalue(v2))? 1 : 0;
                break;
            case T_STRING:
            case T_STRTOREL:
                ret = ( strcmp( svalue(v1), svalue(v2) ) ) ? 0 : 1;
                break;
            case T_FLOAT:
                ret = ( fvalue(v1) == fvalue(v2) )? 1 : 0;
                break;
            default:
                ret = 0;
                
        }
        
        return ret;
    }
    
}


int sym_install_global( const char sym_name[256] ) {
    if( sym_get_global( sym_name ) )
        return SYM_ALREADY_IN_TABLE;

    sym_put_global( sym_name );
    return SYM_NO_ERROR;
}

int func_add( function_t * f ) {
    
    function_table_t * ptr = functions;
    while( ptr ) {
        if( strcmp( ptr->func->name, f->name ) == 0 && f->nparams == ptr->func->nparams)
            return FUNC_ALREADY_IN_TABLE;
        ptr = ptr->next;
    }     
    

    ptr = (function_table_t *) malloc( sizeof(function_table_t) );

    ptr->func = f;
    ptr->scope = NULL;
    ptr->next = functions;
    functions = ptr;
    return FUNC_NO_ERROR;
}
void func_put( const char func_name[256], int init, int final, int nparams ) {
    function_table_t * ptr = (function_table_t*) malloc( sizeof(function_table_t) );

    ptr->func = (function_t*) malloc( sizeof(function_t) );
    strcpy( ptr->func->name, func_name );
    ptr->func->init_addr = init;
    ptr->func->final_addr = final;
    ptr->func->nparams = nparams;


    ptr->scope = NULL;

    ptr->next = functions;
    functions = ptr;
}

function_t * func_get( const char func_name[256], int nparams ) {
    function_table_t * ptr = functions;
    int flExistFunc = 0;
    
    while( ptr ) {
        if( strcmp( ptr->func->name, func_name ) == 0 ) {
            if( ptr->func->nparams == nparams )    
                return ptr->func;
            else
                flExistFunc = 1;        
        }
        ptr = ptr->next;
    }
    
    if( flExistFunc )
        return (function_t *) FUNC_INVALID_PARAMS;
    return (function_t *) 0;
}

int func_install_parms( const char func_name[256], int init, int final, int nparams ) {

    if( !func_get(func_name, nparams) )
        return FUNC_ALREADY_IN_TABLE;
    else {
        func_put( func_name, init, final, nparams );
    }
    return FUNC_NO_ERROR;
}

int func_install( function_t * f, int nparams ) {
    function_table_t * ptr = functions;
	
	while( ptr ) {
	    
	    if( ptr->func->nparams == nparams && strcmp(ptr->func->name, f->name ) == 0 ) {
	        return FUNC_ALREADY_IN_TABLE;
	    }
	    
	    ptr =  ptr->next;
	
	}
	
	f->nparams = nparams;
	return FUNC_NO_ERROR;
}


void func_add_scope() {

	scope_table_t * ptr = (scope_table_t *) malloc( sizeof(scope_table_t) );
	ptr->table = NULL;
	ptr->next = functions->scope;
	functions->scope = ptr;

}

int func_put_sym( const char sym_name[256] ) {
	symbol_table_t *ptr;
	ptr = (symbol_table_t *) malloc ( sizeof(symbol_table_t) );
	if( !ptr )
		return SYM_MEM_FAIL;

	ptr->value = (symbol_t *) malloc ( sizeof(symbol_t) );

	if( !ptr->value )
		return SYM_MEM_FAIL;
	ptr->value->type = T_NDEF;
	strcpy(ptr->value->name,sym_name);
	ptr->value->position = get_position_avail( 1 );
		
	if( !functions->scope ) 
	    func_add_scope();
	    
	ptr->next = functions->scope->table;
	
	functions->scope->table = ptr;
	return SYM_NO_ERROR;
}

symbol_t * func_get_sym( const char sym_name[256] ) {
    
    if( !functions )
        return NULL;
    
	scope_table_t * scopes = functions->scope;

	while( scopes ) {
		symbol_table_t * tmp = scopes->table;

		while( tmp != NULL ) {
			if( strcmp( tmp->value->name, sym_name ) == 0 )
				return tmp->value;
			tmp = tmp->next;
		}

		scopes  = scopes->next;
	}
    
	return sym_get_global(sym_name);
}
void func_release_sym( symbol_t * ptr ) {
    if( ptr ){    
        // mem_avail[ptr->position] = MEM_AVAIL;
        // ibytecode( RELMEM, ptr->position );
        free( ptr );
    }
	
}

void func_release_sym_table( symbol_table_t * ptr ) {
	symbol_table_t * tmp = ptr;
	while( tmp ) {
		tmp = ptr->next;
		func_release_sym( ptr->value );
		free( ptr );
		ptr = tmp;		
	}
}

void func_release_scope() {
    scope_table_t * ptr = NULL;
    if( functions->scope ) {
    	ptr = functions->scope->next;
    	func_release_sym_table( functions->scope->table  );
    	free( functions->scope );
    }

    functions->scope = ptr;
}

function_t * func_new() {
    return (function_t*) malloc( sizeof(function_t) );
}

/**
 * Have main function? 1: yes
 *                     0: no
 *
 */

int func_has_main() {

    function_table_t * ptr = functions;
    while( ptr ) {
        if( strcmp( ptr->func->name, "main" ) == 0 )
            return 1;
        ptr = ptr->next;
    }
    return 0;
}

