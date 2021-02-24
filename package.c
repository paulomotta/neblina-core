#include "package.h"

package_t  *        packages[1024];
int                 npackages = 0;
package_t * package_init( int nfunctions, char name[256] ) {
    package_t * pkg = (package_t *)malloc( sizeof( package_t ) );
    pkg->next = NULL;
    pkg->nfunctions = nfunctions;
    pkg->functions = (func_info_t **) malloc( nfunctions * sizeof( func_info_t *));
    strncpy( pkg->name, name, 256 );    
    return pkg;
}




func_info_t * package_new_func( char name[256], 
                          int nparams, 
                          int nreturns, 
                          int * type_params, 
                          int * type_returns, 
                          void ** (*ptr_func)( void ** input, int * status ) ) {

    func_info_t * ret = (func_info_t *) malloc( sizeof(func_info_t) );
    int i = 0;
    
    strcpy( ret->name, name );
    ret->nreturns = nreturns;
    ret->nparams = nparams;
    ret->type_params = (int *)malloc( sizeof( int ) * nparams );
    ret->type_returns = (int *)malloc( sizeof( int ) * nreturns );
    
    for( i = 0; i < nparams; i++ )
        ret->type_params[i] = type_params[i];
    
    for( i = 0; i < nreturns; i++ )
        ret->type_returns[i] = type_returns[i];
        
    ret->ptr_func =  ptr_func;
    
    return ret;
}



int package_is_defined( char name[256] ) {
    int i;    
    for( i = 0; i < npackages; i++ )
        if( strcmp( name, packages[i]->name ) == 0 )
            return 1;
            
    return 0;
    
}

func_info_t * package_func_get( char name[256], int nparams ) {
    int i, j;
    for( i = 0; i < npackages; i++ ) {
        for( j = 0; j < packages[i]->nfunctions; j++ ) {
            if( strcmp( name, packages[i]->functions[j]->name ) == 0 && packages[i]->functions[j]->nparams == nparams ) {
                return packages[i]->functions[j];
            }
        }
    }
    return NULL;
}

func_info_t * package_func_get_any( char name[256] ) {
    int i, j;
    for( i = 0; i < npackages; i++ ) {
        for( j = 0; j < packages[i]->nfunctions; j++ ) {
            if( strcmp( name, packages[i]->functions[j]->name ) == 0) {
                return packages[i]->functions[j];
            }
        }
    }
    return NULL;
}
