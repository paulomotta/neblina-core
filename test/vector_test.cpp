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

class NeblinaCoreFixture : public ::testing::Test {
protected:
  cl_int status;
public:

    NeblinaCoreFixture() {
        // initialization code here
    }

    protected:
    static void SetUpTestSuite() {
        std::cerr << "TestSuiteSetup" << std::endl;
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

    ~NeblinaCoreFixture() {
        // cleanup any pending stuff, but no exceptions allowed
    }

};

TEST_F(NeblinaCoreFixture, showDeviceList) {
    //showDevicesList();

    EXPECT_EQ(1, 1);

}

TEST_F(NeblinaCoreFixture, addVectorF) {

    int n = 3;

    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * b = vector_new(n, T_FLOAT);
    vector_t * r = vector_new(n, T_FLOAT);

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }

    vecreqdev(a);
    vecreqdev(b);
    vecreqdev(r);

    r->extra = addVectorF((cl_mem)a->extra, (cl_mem)b->extra, b->len);

    double * out = (double *) malloc(n * sizeof (double));
    status = clEnqueueReadBuffer(clinfo.q, (cl_mem)r->extra, CL_TRUE, 0, n * sizeof (double), out, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., out[i]);
    }
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);
    free(out);
}

TEST_F(NeblinaCoreFixture, vec_add) {

    int n = 3;

    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * b = vector_new(n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_add((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., r->value.f[i]);
    }
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_len) {

    int n = 3;

    vector_t * a = vector_new(n, T_FLOAT);

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
    }

    object_t ** in = convertToObject(a, NULL);

    int len = vec_len((void **) in, NULL);

    EXPECT_EQ(3, len);
    vector_delete(a);

}

TEST_F(NeblinaCoreFixture, vec_add_complex) {

    int n = 3;

    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * b = vector_new(n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 1.;
        a->value.f[idx + 1] = 1.;
        
        b->value.f[idx] = 1.;
        b->value.f[idx + 1] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_add((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        int idx = 2 * (i);
        EXPECT_EQ(2., r->value.f[idx]);
        EXPECT_EQ(2., r->value.f[idx + 1]);
    }
    
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_sub) {

    int n = 3;

    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * b = vector_new(n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_sub((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(0., r->value.f[i]);
    }
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_sub_WithComplex) {

    int n = 3;

    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * b = vector_new(n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 1.;
        a->value.f[idx + 1] = 1.;
        
        b->value.f[idx] = 1.;
        b->value.f[idx + 1] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_sub((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        int idx = 2 * (i);
        EXPECT_EQ(0., r->value.f[idx]);
        EXPECT_EQ(0., r->value.f[idx + 1]);
    }

    vector_delete(a);
    vector_delete(b);
    vector_delete(r);

}


TEST_F(NeblinaCoreFixture, scalar_vec) {

    int n = 3;
    
    double scalar = 2.0;
    vector_t * a = vector_new(n, T_FLOAT);
    
    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    object_t ** in = convertScaVecToObject(scalar, a);
    
    vector_t * r = (vector_t *) vec_mulsc((void **) in, NULL);
    
    vecreqhost(r);
    
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(4., r->value.f[i]);
    }

    vector_delete(a);
    vector_delete(r);
}

TEST_F(NeblinaCoreFixture, complex_scalar_float_vec) {

    int n = 3;
    
    complex_t * scalar = complex_new(2.0, 2.0);
    vector_t * a = vector_new(n, T_FLOAT);
    
    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    vector_t * r = (vector_t *) vec_mul_complex_scalar(scalar, a);
    
    vecreqhost(r);
    
    for (int i = 0; i < r->len; i++) {
        int idx = 2 * (i);

        EXPECT_EQ(4., r->value.f[idx]);
        EXPECT_EQ(2., r->value.f[idx + 1]);
    }

    vector_delete(a);
    complex_delete(scalar);
    vector_delete(r);
}

TEST_F(NeblinaCoreFixture, complex_scalar_complex_vec) {

    int n = 3;
    
    complex_t * scalar = complex_new(2.0, 2.0);
    vector_t * a = vector_new(n, T_COMPLEX);
    
    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 2.;
        a->value.f[idx+1] = 2.;
    }

    vector_t * r = (vector_t *) mul_complex_scalar_complex_vec(scalar, a);
    
    vecreqhost(r);
    
    for (int i = 0; i < r->len; i++) {
        int idx = 2 * (i);

        EXPECT_EQ(4., r->value.f[idx]);
        EXPECT_EQ(4., r->value.f[idx + 1]);
    }

    vector_delete(a);
    complex_delete(scalar);
    vector_delete(r);
}

TEST_F(NeblinaCoreFixture, float_scalar_complex_vec) {

    int n = 3;
    
    double scalar = 2.0;
    vector_t * a = vector_new(n, T_COMPLEX);
    
    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 2.;
        a->value.f[idx+1] = 2.;
    }

    vector_t * r = (vector_t *) mul_float_scalar_complex_vec(scalar, a);
    
    vecreqhost(r);
    
    for (int i = 0; i < r->len; i++) {
        int idx = 2 * (i);

        EXPECT_EQ(4., r->value.f[idx]);
        EXPECT_EQ(2., r->value.f[idx + 1]);
    }

    vector_delete(a);
    vector_delete(r);
}

TEST_F(NeblinaCoreFixture, vec_prod_WithFloat) {

    int n = 3;

    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * b = vector_new(n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
        b->value.f[i] = 2.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_prod((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(4., r->value.f[i]);
    }
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_prod_WithComplex) {

    int n = 3;

    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * b = vector_new(n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 2.;
        a->value.f[i + 1] = 2.;
        b->value.f[i] = 2.;
        b->value.f[i + 1] = 2.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_prod((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < 2 * a->len; i += 2) {
        EXPECT_EQ(0., r->value.f[i]);
        EXPECT_EQ(8., r->value.f[i + 1]);
    }
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_conj) {

    int n = 3;

    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 2.;
        a->value.f[i + 1] = 2.;
    }

    object_t ** in = convertToObject(a, NULL);

    r = (vector_t *) vec_conj((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., r->value.f[2 * i]);
        EXPECT_EQ(-2., r->value.f[2 * i + 1]);
    }
    
    vector_delete(a);
    vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_add_off) {

    int n = 4;

    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }
    int offset = 2;
    object_t ** in = convertToObject2(offset, a);

    r = (vector_t *) vec_add_off((void **) in, NULL);

    vecreqhost(r);

    for (int i = 0; i < offset; ++i) {
        EXPECT_EQ(4., r->value.f[i]);
    }
    vector_delete(a);
    vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_sum) {

    int n = 4;

    vector_t * a = vector_new(n, T_FLOAT);
    object_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    object_t ** in = convertToObject(a, NULL);

    r = (object_t *) vec_sum((void **) in, NULL);

    EXPECT_EQ(8., r->value.f);
    
    vector_delete(a);

}

TEST_F(NeblinaCoreFixture, addVectorC) {

    int n = 3;

    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * b = vector_new(n, T_COMPLEX);

    vector_t * r = vector_new(n, T_COMPLEX);


    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 1.;
        a->value.f[i + 1] = 1.;
        b->value.f[i] = 1.;
        b->value.f[i + 1] = 1.;
    }

    vecreqdev(a);
    vecreqdev(b);

    vecreqdev(r);

    r->extra = addVectorC((cl_mem)a->extra, (cl_mem)b->extra, b->len);

    double * out = (double *) malloc(n * COMPLEX_SIZE);
    status = clEnqueueReadBuffer(clinfo.q, (cl_mem)r->extra, CL_TRUE, 0, n * COMPLEX_SIZE, out, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < 2 * a->len; i += 2) {
        EXPECT_EQ(2., out[i]);
        EXPECT_EQ(2., out[i + 1]);
    }
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);
    free(out);

}

TEST_F(NeblinaCoreFixture, subVector) {

    int n = 3;

    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * b = vector_new(n, T_FLOAT);

    vector_t * r = vector_new(n, T_FLOAT);


    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }

    vecreqdev(a);
    vecreqdev(b);

    vecreqdev(r);

    r->extra = subVector((cl_mem)a->extra, (cl_mem)b->extra, b->len);

    double * out = (double *) malloc(n * sizeof (double));
    status = clEnqueueReadBuffer(clinfo.q, (cl_mem)r->extra, CL_TRUE, 0, n * sizeof (double), out, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(0., out[i]);
    }
    
    vector_delete(a);
    vector_delete(b);
    vector_delete(r);
    free(out);

}
