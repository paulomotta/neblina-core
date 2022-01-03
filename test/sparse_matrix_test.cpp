#include "gtest/gtest.h"
#include "libneblina.h"
#include "clutils.h"
#include "oclvector.h"
#include "neblina_std.h"
//#include "neblina_vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

using namespace std;

class SparseMatrixFixture : public ::testing::Test {
protected:
  cl_int status;
public:

    SparseMatrixFixture() {
        // initialization code here
    }

    protected:
    static void SetUpTestSuite() {
        std::cerr << "Sparse Matrix TestSuiteSetup" << std::endl;
        cl_int err;
        cl_uint num_platforms;
        
        err = clGetPlatformIDs(0, NULL, &num_platforms);
        if (err == CL_SUCCESS) {
            //std::cout << "Success. Platforms available: " << num_platforms
            //        << std::endl;
        } else {
            //std::cout << "Error. Platforms available: " << num_platforms
            //        << std::endl;
        }

        InitCLEngine(0);
    }

    static void TearDownTestSuite() {
        ReleaseCLInfo(clinfo);
    }
    
    void SetUp() {
        
    }

    void TearDown() {
        //printf("check");
        //getchar();
        
    }

    ~SparseMatrixFixture() {
        // cleanup any pending stuff, but no exceptions allowed
    }

};


TEST_F(SparseMatrixFixture, smatvec_multiply_WithSparseMatrixFloat) {

    int n = 10;

    vector_t * a = vector_new(n, T_FLOAT);
    smatrix_t * b = smatrix_new(n, n, T_FLOAT);
    vector_t * out;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 3.;
    }

    smatrix_set_real_value(b, 0, 0, 3.);
    smatrix_set_real_value(b, 0, 1, 3.);
    smatrix_set_real_value(b, 0, 9, 3.);

    smatrix_set_real_value(b, 1, 1, 3.);
    smatrix_set_real_value(b, 1, 5, 3.);
    smatrix_set_real_value(b, 1, 8, 3.);

    smatrix_set_real_value(b, 2, 2, 3.);
    smatrix_set_real_value(b, 2, 4, 3.);
    smatrix_set_real_value(b, 2, 7, 3.);

    smatrix_set_real_value(b, 3, 3, 3.);
    smatrix_set_real_value(b, 3, 1, 3.);
    smatrix_set_real_value(b, 3, 6, 3.);

    smatrix_pack(b);

    //    for (int i=0; i < b->nrow * b->maxcols; i++) {
    //        printf("b->idx_col[%d]=%d b->[%d]=%lf \n",i,b->idx_col[i],i,b->m[i]);
    //    }

    out = (vector_t *) smatvec_multiply(b, a);

    EXPECT_EQ(27., out->value.f[0]);
    EXPECT_EQ(27., out->value.f[1]);
    EXPECT_EQ(27., out->value.f[2]);
    EXPECT_EQ(27., out->value.f[3]);
    EXPECT_EQ(0., out->value.f[4]);
    EXPECT_EQ(0., out->value.f[5]);
    EXPECT_EQ(0., out->value.f[6]);
    EXPECT_EQ(0., out->value.f[7]);
    EXPECT_EQ(0., out->value.f[8]);
    EXPECT_EQ(0., out->value.f[9]);
    
    vector_delete(a);
    smatrix_delete(b);
    vector_delete(out);

}

TEST_F(SparseMatrixFixture, matvec_mul3_WithSparseMatrixFloat) {

    int n = 10;

    vector_t * a = vector_new(n, T_FLOAT);
    smatrix_t * b = smatrix_new(n, n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 3.;
    }

    smatrix_set_real_value(b, 0, 0, 3.);
    smatrix_set_real_value(b, 0, 1, 3.);
    smatrix_set_real_value(b, 0, 9, 3.);

    smatrix_set_real_value(b, 1, 1, 3.);
    smatrix_set_real_value(b, 1, 5, 3.);
    smatrix_set_real_value(b, 1, 8, 3.);

    smatrix_set_real_value(b, 2, 2, 3.);
    smatrix_set_real_value(b, 2, 4, 3.);
    smatrix_set_real_value(b, 2, 7, 3.);

    smatrix_set_real_value(b, 3, 3, 3.);
    smatrix_set_real_value(b, 3, 1, 3.);
    smatrix_set_real_value(b, 3, 6, 3.);

    smatrix_pack(b);

    object_t ** in = convertToObject4(a, b);

    r = (vector_t *) matvec_mul3((void **) in, NULL);

    vecreqhost(r);

    EXPECT_EQ(27., r->value.f[0]);
    EXPECT_EQ(27., r->value.f[1]);
    EXPECT_EQ(27., r->value.f[2]);
    EXPECT_EQ(27., r->value.f[3]);
    EXPECT_EQ(0., r->value.f[4]);
    EXPECT_EQ(0., r->value.f[5]);
    EXPECT_EQ(0., r->value.f[6]);
    EXPECT_EQ(0., r->value.f[7]);
    EXPECT_EQ(0., r->value.f[8]);
    EXPECT_EQ(0., r->value.f[9]);
    
    vector_delete(a);
    smatrix_delete(b);
    vector_delete(r);

}

TEST_F(SparseMatrixFixture, matvec_mul3_WithSparseMatrixComplex) {

    int n = 7000;

    vector_t * a = vector_new(n, T_COMPLEX);
    smatrix_t * b = smatrix_new(n, n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[2 * i] = 3.;
        a->value.f[2 * i + 1] = 3.;
    }

    smatrix_set_complex_value(b, 0, 0, 3., 3.);
    smatrix_set_complex_value(b, 0, 1, 3., 3.);
    smatrix_set_complex_value(b, 0, 9, 3., 3.);

    smatrix_set_complex_value(b, 1, 1, 3., 3.);
    smatrix_set_complex_value(b, 1, 5, 3., 3.);
    smatrix_set_complex_value(b, 1, 8, 3., 3.);

    smatrix_set_complex_value(b, 2, 2, 3., 3.);
    smatrix_set_complex_value(b, 2, 4, 3., 3.);
    smatrix_set_complex_value(b, 2, 7, 3., 3.);

    smatrix_set_complex_value(b, 3, 3, 3., 3.);
    smatrix_set_complex_value(b, 3, 1, 3., 3.);
    smatrix_set_complex_value(b, 3, 6, 3., 3.);
    
    for (int i = 0; i < a->len; i++) {
        smatrix_set_complex_value(b, 10, i, 3., 3.);
    }

    smatrix_pack_complex(b);

    object_t ** in = convertToObject4(a, b);

    r = (vector_t *) matvec_mul3((void **) in, NULL);

    vecreqhost(r);

    EXPECT_EQ(0., r->value.f[0]);
    EXPECT_EQ(54., r->value.f[1]);
    EXPECT_EQ(0., r->value.f[2]);
    EXPECT_EQ(54., r->value.f[3]);
    EXPECT_EQ(0., r->value.f[4]);
    EXPECT_EQ(54., r->value.f[5]);
    EXPECT_EQ(0., r->value.f[6]);
    EXPECT_EQ(54., r->value.f[7]);
    EXPECT_EQ(0., r->value.f[8]);
    EXPECT_EQ(0., r->value.f[9]);
    EXPECT_EQ(0., r->value.f[10]);
    EXPECT_EQ(0., r->value.f[11]);
    EXPECT_EQ(0., r->value.f[12]);
    EXPECT_EQ(0., r->value.f[13]);
    EXPECT_EQ(0., r->value.f[14]);
    EXPECT_EQ(0., r->value.f[15]);
    EXPECT_EQ(0., r->value.f[16]);
    EXPECT_EQ(0., r->value.f[17]);
    EXPECT_EQ(0., r->value.f[18]);
    EXPECT_EQ(0., r->value.f[19]);
    
    vector_delete(a);
    smatrix_delete(b);
    vector_delete(r);

}

