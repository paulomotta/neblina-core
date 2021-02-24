#ifndef __PACKAGENEB__
#define __PACKAGENEB__

#include <stdlib.h>
#include <string.h>
#include "neblina.h"




typedef struct __package_t {
    char                       name[256];     
    int                        nfunctions;    // Number of functions in package
    func_info_t             ** functions;     // Array of functions
    struct __package_t       * next;
} package_t;


extern package_t  *        packages[1024];
extern int                 npackages;

package_t * package_init( int nfunctions, char name[256] );




func_info_t * package_new_func( char name[256], 
                          int nparams, 
                          int nreturns, 
                          int * type_params, 
                          int * type_returns, 
                          void ** (*ptr_func)( void ** input, int * status ) ) ;

int package_is_defined( char name[256] );

func_info_t * package_func_get( char name[256], int nparams );

func_info_t * package_func_get_any( char name[256] );


#endif
