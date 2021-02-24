#ifndef __NEBLINAMATH__
#define __NEBLINAMATH__



#define DEG_TO_RAD 0.017453292519943295
#define RAD_TO_DEG 57.29577951308232
/**
 * Time Package
 */
package_t * getMathPackage();

void ** neblina_math_sin( void ** i, int * status );
void ** neblina_math_cos( void ** i, int * status );
void ** neblina_math_tan( void ** i, int * status );
void ** neblina_math_sinh( void ** i, int * status );
void ** neblina_math_cosh( void ** i, int * status );
void ** neblina_math_tanh( void ** i, int * status );
void ** neblina_math_asin( void ** i, int * status );
void ** neblina_math_acos( void ** i, int * status );
void ** neblina_math_atan( void ** i, int * status );
void ** neblina_math_rad( void ** i, int * status );
void ** neblina_math_deg( void ** i, int * status );


void ** neblina_math_log( void ** i, int * status );
void ** neblina_math_loga( void ** i, int * status );

void ** neblina_math_sqrt( void ** i, int * status );
void ** neblina_math_exp( void ** i, int * status );
void ** neblina_math_floor( void ** i, int * status );
void ** neblina_math_ceil( void ** i, int * status );
void ** neblina_math_round( void ** i, int * status );
void ** neblina_math_abs( void ** i, int * status );
void ** neblina_math_seed( void ** i, int * status );
void ** neblina_math_randomf( void ** i, int * status );
void ** neblina_math_randomi1( void ** i, int * status );
void ** neblina_math_randomi2( void ** i, int * status );


#endif
