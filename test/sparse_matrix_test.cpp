#include "gtest/gtest.h"
#include "libneblina.h"
#include "bridge_api.h"
#include "neblina_std.h"
//#include "neblina_vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <string>

using namespace std;

class SparseMatrixFixture : public ::testing::Test {
protected:
public:
    bridge_manager_t m;
    int idx;
    
    SparseMatrixFixture() {
        idx = 0;
        string plugin_name = "/usr/local/lib64/libneblina-cpu-bridge.so";
        load_plugin(&m, const_cast<char *>(plugin_name.c_str()), idx);
        m.bridges[idx].InitEngine_f(0);
    }

    protected:
    static void SetUpTestSuite() {
        std::cerr << "Sparse Matrix TestSuiteSetup" << std::endl;
    }

    static void TearDownTestSuite() {
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


// TEST_F(SparseMatrixFixture, smatvec_multiply_WithSparseMatrixFloat) {
//     GTEST_SKIP();
//     int n = 10;

//     vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
//     smatrix_t * b = m.bridges[idx].smatrix_new(n, n, T_FLOAT);
//     vector_t * out;

//     for (int i = 0; i < a->len; i++) {
//         a->value.f[i] = 3.;
//     }

//     m.bridges[idx].smatrix_set_real_value(b, 0, 0, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 0, 1, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 0, 9, 3.);

//     m.bridges[idx].smatrix_set_real_value(b, 1, 1, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 1, 5, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 1, 8, 3.);

//     m.bridges[idx].smatrix_set_real_value(b, 2, 2, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 2, 4, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 2, 7, 3.);

//     m.bridges[idx].smatrix_set_real_value(b, 3, 3, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 3, 1, 3.);
//     m.bridges[idx].smatrix_set_real_value(b, 3, 6, 3.);

//     m.bridges[idx].smatrix_pack(b);

//     //    for (int i=0; i < b->nrow * b->maxcols; i++) {
//     //        printf("b->idx_col[%d]=%d b->[%d]=%lf \n",i,b->idx_col[i],i,b->m[i]);
//     //    }

//     // out = (vector_t *) smatvec_multiply(&m, idx, b, a);

//     EXPECT_EQ(27., out->value.f[0]);
//     EXPECT_EQ(27., out->value.f[1]);
//     EXPECT_EQ(27., out->value.f[2]);
//     EXPECT_EQ(27., out->value.f[3]);
//     EXPECT_EQ(0., out->value.f[4]);
//     EXPECT_EQ(0., out->value.f[5]);
//     EXPECT_EQ(0., out->value.f[6]);
//     EXPECT_EQ(0., out->value.f[7]);
//     EXPECT_EQ(0., out->value.f[8]);
//     EXPECT_EQ(0., out->value.f[9]);
    
//     m.bridges[idx].vector_delete(a);
//     m.bridges[idx].smatrix_delete(b);
//     m.bridges[idx].vector_delete(out);

// }

TEST_F(SparseMatrixFixture, matvec_mul3_WithSparseMatrixFloat) {

    int n = 10;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1, NULL );
    smatrix_t * b = m.bridges[idx].smatrix_new(n, n, T_FLOAT);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 3.;
    }


    m.bridges[idx].smatrix_set_real_value(b, 0, 0, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 0, 1, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 0, 9, 3.);


    m.bridges[idx].smatrix_set_real_value(b, 1, 1, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 1, 5, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 1, 8, 3.);


    m.bridges[idx].smatrix_set_real_value(b, 2, 2, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 2, 4, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 2, 7, 3.);


    m.bridges[idx].smatrix_set_real_value(b, 3, 3, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 3, 1, 3.);
    m.bridges[idx].smatrix_set_real_value(b, 3, 6, 3.);


    m.bridges[idx].smatrix_pack(b);


    object_t ** in = convertToObject4(a, b);


    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);


    m.bridges[idx].vecreqhost(r);


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


    delete_object_array(in, 2);

    m.bridges[idx].vector_delete(a);

    m.bridges[idx].vector_delete(r);

    m.bridges[idx].smatrix_delete(b);


}

TEST_F(SparseMatrixFixture, matvec_mul3_WithSparseMatrixComplex) {

    int n = 70;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1, NULL );
    smatrix_t * b = m.bridges[idx].smatrix_new(n, n, T_COMPLEX);
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[2 * i] = 3.;
        a->value.f[2 * i + 1] = 3.;
    }

    m.bridges[idx].smatrix_set_complex_value(b, 0, 0, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 0, 1, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 0, 9, 3., 3.);

    m.bridges[idx].smatrix_set_complex_value(b, 1, 1, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 1, 5, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 1, 8, 3., 3.);

    m.bridges[idx].smatrix_set_complex_value(b, 2, 2, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 2, 4, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 2, 7, 3., 3.);

    m.bridges[idx].smatrix_set_complex_value(b, 3, 3, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 3, 1, 3., 3.);
    m.bridges[idx].smatrix_set_complex_value(b, 3, 6, 3., 3.);
    
    for (int i = 0; i < a->len; i++) {
        m.bridges[idx].smatrix_set_complex_value(b, 10, i, 3., 3.);
    }

    m.bridges[idx].smatrix_pack_complex(b);

    object_t ** in = convertToObject4(a, b);

    r = (vector_t *) matvec_mul3(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

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
    
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].smatrix_delete(b);
    m.bridges[idx].vector_delete(r);

}

