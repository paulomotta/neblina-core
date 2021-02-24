#include <math.h>
#include "package.h"
#include "neblina_vm.h"
#include "neblina_math.h"

package_t * getMathPackage() {
    package_t * pkg   = package_init( 23, "math" );

    int ty_out[]   = { T_FLOAT };
    int ty_outa[]   = { T_ANY };
    int ty_in1[]  = { T_ANY };
    int ty_in2[]  = { T_ANY, T_ANY };
    
    int i = 0;
    pkg->functions[i++] = package_new_func( "sin",      1, 1, ty_in1, ty_out, neblina_math_sin );
    pkg->functions[i++] = package_new_func( "cos",      1, 1, ty_in1, ty_out, neblina_math_cos ); 
    pkg->functions[i++] = package_new_func( "tan",      1, 1, ty_in1, ty_out, neblina_math_tan );
    pkg->functions[i++] = package_new_func( "sinh",      1, 1, ty_in1, ty_out, neblina_math_sinh );
    pkg->functions[i++] = package_new_func( "cosh",      1, 1, ty_in1, ty_out, neblina_math_cosh ); 
    pkg->functions[i++] = package_new_func( "tanh",      1, 1, ty_in1, ty_out, neblina_math_tanh );
    pkg->functions[i++] = package_new_func( "asin",      1, 1, ty_in1, ty_out, neblina_math_asin );
    pkg->functions[i++] = package_new_func( "acos",      1, 1, ty_in1, ty_out, neblina_math_acos ); 
    pkg->functions[i++] = package_new_func( "atan",      1, 1, ty_in1, ty_out, neblina_math_atan );
    pkg->functions[i++] = package_new_func( "deg",      1, 1, ty_in1, ty_out, neblina_math_deg );
    pkg->functions[i++] = package_new_func( "rad",      1, 1, ty_in1, ty_out, neblina_math_rad ); 
    pkg->functions[i++] = package_new_func( "log",      1, 1, ty_in1, ty_out, neblina_math_log ); 
    pkg->functions[i++] = package_new_func( "log",      2, 1, ty_in2, ty_out, neblina_math_loga );
    pkg->functions[i++] = package_new_func( "floor",      1, 1, ty_in1, ty_out, neblina_math_floor );
    pkg->functions[i++] = package_new_func( "ceil",      1, 1, ty_in1, ty_out, neblina_math_ceil );
    pkg->functions[i++] = package_new_func( "round",      1, 1, ty_in1, ty_out, neblina_math_round );
    pkg->functions[i++] = package_new_func( "sqrt",      1, 1, ty_in1, ty_out, neblina_math_sqrt );
    pkg->functions[i++] = package_new_func( "abs",      1, 1, ty_in1, ty_outa, neblina_math_abs );
    pkg->functions[i++] = package_new_func( "exp",      1, 1, ty_in1, ty_out, neblina_math_exp );
    pkg->functions[i++] = package_new_func( "seed",      1, 0, ty_in1, NULL, neblina_math_seed );
    pkg->functions[i++] = package_new_func( "random",    0, 1, NULL, ty_out, neblina_math_randomf );
    pkg->functions[i++] = package_new_func( "random",    1, 1, ty_in1, ty_out, neblina_math_randomi1 );
    pkg->functions[i++] = package_new_func( "random",    2, 1, ty_in2, ty_out, neblina_math_randomi2 ); 
    
    return pkg;
}

void ** neblina_math_sin( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = sin( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = sin( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute sin()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_cos( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = cos( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = cos( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute cos()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_tan( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = tan( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = tan( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute tan()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_sinh( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = sinh( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = sinh( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute sinh()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_cosh( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = cosh( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = cosh( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute cos()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_tanh( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = tanh( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = tanh( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute tanh()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_asin( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = asin( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = asin( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute asin()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_acos( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = acos( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = acos( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute acos()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_atan( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = atan( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = atan( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute atan()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}       

void ** neblina_math_log( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t out;// = (object_t *) malloc( sizeof( object_t ) );
    type( out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( out ) = log( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( out ) = log( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute log" );
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input( i, 1 );
    return ret;
}    

void ** neblina_math_loga( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t out;// = (object_t *) malloc( sizeof( object_t ) );
    type( out ) = T_FLOAT;
    double v, base;
    if( type( *in[1] ) == T_FLOAT )
        v = fvalue( *in[1] );
    else if( type( *in[1] ) == T_INT )
        v = ivalue( *in[1] );
    else 
        runerror( "invalid type to compute log" );
        
    if( type( *in[0] ) == T_FLOAT )
        base = fvalue( *in[0] );
    else if( type( *in[0] ) == T_INT )
        base = ivalue( *in[0] );
    else 
        runerror( "invalid type to compute log" );
    
    fvalue( out ) = log(v)/log(base);
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input( i, 2 );
    return ret;
}

void ** neblina_math_sqrt( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t out;
    type( out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( out ) = sqrt( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( out ) = sqrt( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute sqrt()" );
    clear_input( i, 1 );
    static void * ret[1];
    ret[0] = (void *) &out;
    return ret;
}
 
void ** neblina_math_exp( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t out;
    
    if( type( *in[0] ) == T_FLOAT ) {
        type( out ) = T_FLOAT;
        fvalue( out ) = exp( fvalue( *in[0] ) );
    } else if( type( *in[0] ) == T_INT ) {
        type( out ) = T_FLOAT;
        fvalue( out ) = exp( ivalue( *in[0] ) );
    } else if( type( *in[0] ) == T_COMPLEX ) {
        complex_t * i = (complex_t *) vvalue( *in[0] );
        complex_t * o = (complex_t *) malloc(sizeof(complex_t));
        
        o->re = exp( i->re ) * cos( i->im );
        o->im = exp( i->re ) * sin( i->im );
        
        vvalue( out ) = (void *) o;
        type( out ) = T_COMPLEX;
    } else
        runerror( "invalid type to compute exp()" );
    
    clear_input( i, 1 );
    static void * ret[1];
    ret[0] = (void *) &out;
    return ret;
}   

void ** neblina_math_floor( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = floor( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = floor( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute floor()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_ceil( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t out;// = (object_t *) malloc( sizeof( object_t ) );
    type( out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( out ) = ceil( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( out ) = ceil( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute ceil()" );
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input( i, 1 );
    return ret;
}

void ** neblina_math_round( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = round( fvalue( *in[0] ) );
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = round( ivalue( *in[0] ) );
    else 
        runerror( "invalid type to compute round()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_rad( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = fvalue( *in[0] )*DEG_TO_RAD;
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = ivalue( *in[0] )*DEG_TO_RAD;
    else 
        runerror( "invalid type to compute rad()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_math_deg( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_FLOAT;
    if( type( *in[0] ) == T_FLOAT )
        fvalue( *out ) = fvalue( *in[0] )*RAD_TO_DEG;
    else if( type( *in[0] ) == T_INT )
        fvalue( *out ) = ivalue( *in[0] )*RAD_TO_DEG;
    else 
        runerror( "invalid type to compute deg()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}


void ** neblina_math_abs( void ** i, int * status ) {
    object_t ** in = (object_t **) i;
    object_t out;// = (object_t *) malloc( sizeof( object_t ) );
    
    if( type( *in[0] ) == T_FLOAT ) {
        type( out ) = T_FLOAT;
        fvalue( out ) = fabs( fvalue( *in[0] ) );
    } else if( type( *in[0] ) == T_INT ) {
        type( out ) = T_INT;
        ivalue( out ) = abs( ivalue( *in[0] ) );
    } else 
        runerror( "invalid type to compute abs()" );
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input(i, 1);
    return ret;
}

void ** neblina_math_seed( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    if( type( *in[0] ) == T_INT )
        srand( ivalue( *in[0] ) );
    return NULL;
}

void ** neblina_math_randomf( void ** i, int * status ){
    object_t out;// = (object_t *) malloc( sizeof( object_t ) );
    type( out ) = T_FLOAT;
    fvalue( out ) = (double)rand() / RAND_MAX;
    
    static void * ret[1];
    ret[0] = (void *) &out;
    clear_input( i, 0 );
    return ret;
}

void ** neblina_math_randomi1( void ** i, int * status ) {
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_INT;
    if( type( *in[0] ) == T_FLOAT ) {
        ivalue( *out ) = (rand() % (int) fvalue( *in[0] )) + 1;
    } else if( type( *in[0] ) == T_INT ) {
        ivalue( *out ) = (rand() % (int) ivalue( *in[0] )) + 1;
    } else 
        runerror( "invalid type to compute random()" );
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}
void ** neblina_math_randomi2( void ** i, int * status ){
    object_t ** in = (object_t **) i;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    type( *out ) = T_INT;
    int init, final;
    
    
    if( type( *in[1] ) == T_FLOAT )
        init = (int) fvalue( *in[1] );
    else if( type( *in[1] ) == T_INT )
        init = ivalue( *in[1] );
    else 
        runerror( "invalid type to compute random()" );
        
    if( type( *in[0] ) == T_FLOAT )
        final = (int)fvalue( *in[0] );
    else if( type( *in[0] ) == T_INT )
        final = ivalue( *in[0] );
    else 
        runerror( "invalid type to compute random()" );
    
    if( init > final )
        runerror( "invalid range to compute random()" );
    
    
    ivalue( *out ) = (rand() %(final - init + 1)) + init;
    
    static void * ret[1];
    ret[0] = (void *) out;
    return ret;
}
