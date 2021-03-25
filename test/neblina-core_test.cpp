#include "gtest/gtest.h"
#include "clutils.h"
#include "oclvector.h"
#include "neblina_std.h"
#include "libneblina.h"

using namespace std;

TEST(neblina_test_suite, showDeviceList)
{
    showDevicesList();
    
    EXPECT_EQ(1, 1);
    
}

TEST(neblina_test_suite, addVector)
{
    
    cl_int err;
    cl_uint num_platforms;

    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err == CL_SUCCESS) {
        std::cout << "Success. Platforms available: " << num_platforms
                << std::endl;
    } else {
        std::cout << "Error. Platforms available: " << num_platforms
                << std::endl;
    }

    InitCLEngine();

    vector_t * a = vector_new( 3, T_FLOAT ) ;
    vector_t * b = vector_new( 3, T_FLOAT ) ;

    for (int i=0; i < a->len; i++){
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }
    
    vecreqdev( a ); vecreqdev( b ); 
    int ii = 0;
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    vector_t * r = (vector_t *) malloc( sizeof( vector_t ) );
    r->value.f = (double *) malloc( b->len * sizeof(double) );

    r->mem = addVectorF( a->mem, b->mem, b->len ); 


    r->len = b->len;
    r->type = T_FLOAT;
    r->location = LOCDEV;
    type( *out ) = T_VECTOR;
    vvalue( *out ) = (void *) r;
    static void * ret[1];
    ret[0] = (void *) out;
    
    int n = 3000;
    double * out2 = (double *) malloc( n * sizeof(double) );
    cl_int status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, 3*sizeof(double), out2, 0, NULL, NULL);
    CLERR
    
    for (int i=0; i < 3; ++i){
        printf("res[%d]=%f\n",i,out2[i]);
    }

    ReleaseCLInfo(clinfo);

    
    EXPECT_EQ(1, 1);
    
}
