#include "gtest/gtest.h"
#include "clutils.h"
#include "oclvector.h"
#include "neblina_std.h"
#include "libneblina.h"

using namespace std;

class NeblinaCoreFixture : public ::testing::Test {
protected:
    cl_int err;
    cl_uint num_platforms;
    cl_int status;
public:

    NeblinaCoreFixture() {
        // initialization code here
    }

    void SetUp() {
        err = clGetPlatformIDs(0, NULL, &num_platforms);
        if (err == CL_SUCCESS) {
            //std::cout << "Success. Platforms available: " << num_platforms
            //        << std::endl;
        } else {
            //std::cout << "Error. Platforms available: " << num_platforms
            //        << std::endl;
        }

        InitCLEngine();
    }

    void TearDown() {
        ReleaseCLInfo(clinfo);
    }

    ~NeblinaCoreFixture() {
        // cleanup any pending stuff, but no exceptions allowed
    }

};

TEST_F(NeblinaCoreFixture, showDeviceList) {
    showDevicesList();

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

    r->mem = addVectorF(a->mem, b->mem, b->len);

    double * out = (double *) malloc(n * sizeof (double));
    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * sizeof (double), out, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., out[i]);
    }

}

TEST_F(NeblinaCoreFixture, vec_add) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * b = vector_new(n, T_FLOAT);
    vector_t * r;
    vector_t * out = vector_new(n, T_FLOAT);

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }
    
    object_t ** in = convertToObject(a,b);
    
    r = (vector_t *) vec_add((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * sizeof (double), out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., out->value.f[i]);
    }

}

TEST_F(NeblinaCoreFixture, vec_prod_WithFloat) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * b = vector_new(n, T_FLOAT);
    vector_t * r;
    vector_t * out = vector_new(n, T_FLOAT);

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
        b->value.f[i] = 2.;
    }
    
    object_t ** in = convertToObject(a,b);
    
    r = (vector_t *) vec_prod((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * sizeof (double), out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(4., out->value.f[i]);
    }

}

TEST_F(NeblinaCoreFixture, vec_prod_WithComplex) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * b = vector_new(n, T_COMPLEX);
    vector_t * r;
    vector_t * out = vector_new(n, T_COMPLEX);

    for (int i = 0; i < 2*a->len; i+=2) {
        a->value.f[i] = 2.;
        a->value.f[i+1] = 2.;
        b->value.f[i] = 2.;
        b->value.f[i+1] = 2.;
    }
    
    object_t ** in = convertToObject(a,b);
    
    r = (vector_t *) vec_prod((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * COMPLEX_SIZE, out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < 2*a->len; i+=2) {
        EXPECT_EQ(0., out->value.f[i]);
        EXPECT_EQ(8., out->value.f[i+1]);
    }

}

TEST_F(NeblinaCoreFixture, vec_conj) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * r;
    vector_t * out = vector_new(n, T_FLOAT);

    for (int i = 0; i < 2 * a->len; i+=2) {
        a->value.f[i] = 2.;
        a->value.f[i+1] = 2.;
    }
    
    object_t ** in = convertToObject(a, NULL);
    
    r = (vector_t *) vec_conj((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * sizeof (double), out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(8., out->value.f[i]);
    }

}

TEST_F(NeblinaCoreFixture, vec_add_off) {

    int n = 4;
    
    vector_t * a = vector_new(n, T_FLOAT);
    vector_t * r;
    vector_t * out = vector_new(n, T_FLOAT);

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }
    int offset = 2;
    object_t ** in = convertToObject2(offset, a);
    
    r = (vector_t *) vec_add_off((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, offset * sizeof (double), out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < offset; ++i) {
        EXPECT_EQ(4., out->value.f[i]);
    }

}

TEST_F(NeblinaCoreFixture, addVectorFC) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * b = vector_new(n, T_COMPLEX);
    
    vector_t * r = vector_new(n, T_COMPLEX);
    

    for (int i = 0; i < 2*a->len; i+=2) {
        a->value.f[i] = 1.;
        a->value.f[i+1] = 1.;
        b->value.f[i] = 1.;
        b->value.f[i+1] = 1.;
    }

    vecreqdev(a);
    vecreqdev(b);
    
    vecreqdev(r);

    r->mem = addVectorFC(a->mem, b->mem, b->len);
   
    double * out = (double *) malloc(n * COMPLEX_SIZE);
    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * COMPLEX_SIZE, out, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < 2*a->len; i+=2) {
        EXPECT_EQ(2., out[i]);
        EXPECT_EQ(2., out[i+1]);
    }

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

    r->mem = subVector(a->mem, b->mem, b->len);
   
    double * out = (double *) malloc(n * sizeof(double));
    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * sizeof(double), out, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(0., out[i]);
    }

}
