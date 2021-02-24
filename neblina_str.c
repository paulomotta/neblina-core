#include <string.h>
#include "neblina_str.h"
#include "package.h"

package_t * getStrPackage() {
    package_t * pkg   = package_init( 1, "str" );
    int ty_params[]   = { T_STRING };
    int ty_returns[]  = { T_INT };
    pkg->functions[0] = package_new_func( "len", 1, 1, ty_params, ty_returns, str_len ); 
    
    return pkg;
}

void ** str_len( void ** i, int * status ) {
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    object_t ** in = (object_t **) i;
    type( out[0] ) = T_INT;
    ivalue( out[0] ) = strlen( svalue( *in[0] ) );  
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}
