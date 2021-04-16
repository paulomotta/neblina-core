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

TEST_F(NeblinaCoreFixture, matvec_mul3_WithFloat) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_FLOAT);
    matrix_t * b = matrix_new( n, n, T_FLOAT ); 
    vector_t * r;
    vector_t * out = vector_new(n, T_FLOAT);

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }
    
    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            b->value.f[i*b->ncol + j] = 3.;
        }
    }
    
    object_t ** in = convertToObject3(a,b);
    
    r = (vector_t *) matvec_mul3((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * sizeof(double), out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < a->len; i++) {
        EXPECT_EQ(18., out->value.f[i]);
    }

}

TEST_F(NeblinaCoreFixture, matvec_mul3_WithComplex) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_COMPLEX);
    matrix_t * b = matrix_new( n, n, T_COMPLEX ); 
    vector_t * r;
    vector_t * out = vector_new(n, T_COMPLEX);

    for (int i = 0; i < 2 * a->len; i+=2) {
        a->value.f[i] = 2.;
        a->value.f[i+1] = 2.;
    }
    
    for (int i = 0; i < b->ncol; i++) {
        for (int j = 0; j < b->nrow; j++) {
            int idx = 2*(i*b->ncol + j);
            b->value.f[idx] = 3.;
            b->value.f[idx+1] = 3.;
        }
    }
    
    object_t ** in = convertToObject3(a,b);
    
    r = (vector_t *) matvec_mul3((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * COMPLEX_SIZE, out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < 2 * a->len; i+=2) {
        EXPECT_EQ(0., out->value.f[i]);
        EXPECT_EQ(36., out->value.f[i+1]);
    }

}

TEST_F(NeblinaCoreFixture, smatvec_multiply_WithSparseMatrixFloat) {

    int n = 10;
    
    vector_t * a = vector_new(n, T_FLOAT);
    smatrix_t * b = smatrix_new( n, n, T_FLOAT ); 
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
    
    out = (vector_t *) smatvec_multiply(b, a );

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

}

TEST_F(NeblinaCoreFixture, matvec_mul3_WithSparseMatrixFloat) {

    int n = 10;
    
    vector_t * a = vector_new(n, T_FLOAT);
    smatrix_t * b = smatrix_new( n, n, T_FLOAT ); 
    vector_t * r;
    vector_t * out = vector_new(n, T_FLOAT);

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

    object_t ** in = convertToObject4(a,b);
    
    r = (vector_t *) matvec_mul3((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * sizeof(double), out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

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

}

TEST_F(NeblinaCoreFixture, matvec_mul3_WithSparseMatrixComplex) {

    int n = 10;
    
    vector_t * a = vector_new(n, T_COMPLEX);
    smatrix_t * b = smatrix_new( n, n, T_COMPLEX ); 
    vector_t * r;
    vector_t * out = vector_new(n, T_COMPLEX);

    for (int i = 0; i < 2 * a->len; i+=2) {
        a->value.f[i] = 3.;
        a->value.f[i+1] = 3.;
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
    
    smatrix_pack_complex(b);

    object_t ** in = convertToObject4(a,b);
    
    r = (vector_t *) matvec_mul3((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * COMPLEX_SIZE, out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    EXPECT_EQ(54., out->value.f[0]);
    EXPECT_EQ(0., out->value.f[1]);
    EXPECT_EQ(54., out->value.f[2]);
    EXPECT_EQ(0., out->value.f[3]);
    EXPECT_EQ(54., out->value.f[4]);
    EXPECT_EQ(0., out->value.f[5]);
    EXPECT_EQ(54., out->value.f[6]);
    EXPECT_EQ(0., out->value.f[7]);
    EXPECT_EQ(0., out->value.f[8]);
    EXPECT_EQ(0., out->value.f[9]);
    EXPECT_EQ(0., out->value.f[10]);
    EXPECT_EQ(0., out->value.f[11]);
    EXPECT_EQ(0., out->value.f[12]);
    EXPECT_EQ(0., out->value.f[13]);
    EXPECT_EQ(0., out->value.f[14]);
    EXPECT_EQ(0., out->value.f[15]);
    EXPECT_EQ(0., out->value.f[16]);
    EXPECT_EQ(0., out->value.f[17]);
    EXPECT_EQ(0., out->value.f[18]);
    EXPECT_EQ(0., out->value.f[19]);

}

TEST_F(NeblinaCoreFixture, vec_conj) {

    int n = 3;
    
    vector_t * a = vector_new(n, T_COMPLEX);
    vector_t * r;
    vector_t * out = vector_new(n, T_COMPLEX);

    for (int i = 0; i < 2 * a->len; i+=2) {
        a->value.f[i] = 2.;
        a->value.f[i+1] = 2.;
    }
    
    object_t ** in = convertToObject(a, NULL);
    
    r = (vector_t *) vec_conj((void **) in, NULL );

    status = clEnqueueReadBuffer(clinfo.q, r->mem, CL_TRUE, 0, n * COMPLEX_SIZE, out->value.f, 0, NULL, NULL);
    CLERR
    EXPECT_EQ(0, status);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., out->value.f[2*i]);
        EXPECT_EQ(-2., out->value.f[2*i+1]);
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

TEST_F(NeblinaCoreFixture, vec_sum) {

    int n = 4;
    
    vector_t * a = vector_new(n, T_FLOAT);
    object_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    object_t ** in = convertToObject(a, NULL);
    
    r = (object_t *) vec_sum((void **) in, NULL );

    EXPECT_EQ(8., r->value.f);

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
