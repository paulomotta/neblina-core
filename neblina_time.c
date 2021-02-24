#include "neblina_time.h"
#include "libneblina.h"
#include <time.h>
#include <sys/time.h>


package_t * getTimePackage() {
    package_t * pkg   = package_init( 3, "time" );

    int ty_params[]   = { };
    int ty_returns[]  = { T_INT };
    
    pkg->functions[0] = package_new_func( "micro",       0, 1, ty_params, ty_returns, neblina_time_micro ); 
    pkg->functions[1] = package_new_func( "sec",   0, 1, ty_params, ty_returns, neblina_time_sec ); 
    pkg->functions[2] = package_new_func( "milli", 0, 1, ty_params, ty_returns, neblina_time_milli ); 
    
    
    return pkg;
}

void ** neblina_time_sec( void ** i, int * status ) {
    object_t  out;
    ivalue( out ) = time( NULL );
    type( out ) = T_INT;
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input( i, 0 );
    return ret;
}

void ** neblina_time_milli( void ** i, int * status ) {
    object_t out;
    type( out ) = T_INT;
    struct timeval tv;
    gettimeofday( &tv, NULL );

    ivalue( out ) = 1000 * tv.tv_sec + tv.tv_usec/1000;   
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input( i, 0 );
    return ret;
}

void ** neblina_time_micro( void ** i, int * status ) {
    object_t out;
    type( out ) = T_INT;
    struct timeval tv;
    gettimeofday( &tv, NULL );

    ivalue( out ) = 1000000 * tv.tv_sec + tv.tv_usec;   
    
    
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input( i, 0 );
    return ret;
}

