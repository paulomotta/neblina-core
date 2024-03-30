#include "gtest/gtest.h"
#include "libneblina.h"
#include "bridge_api.h"
#include "neblina_std.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

using namespace std;

class NeblinaCoreFixture : public ::testing::Test {
protected:
public:
    bridge_manager_t m;
    int idx;

    NeblinaCoreFixture() {
        idx = 0;
        string plugin_name = "/usr/local/lib64/libneblina-cpu-bridge.so";
        load_plugin(&m, const_cast<char *>(plugin_name.c_str()), idx);
        m.bridges[idx].InitEngine_f(0);
    }

    protected:
    static void SetUpTestSuite() {
        std::cerr << "TestSuiteSetup" << std::endl;
    }

    static void TearDownTestSuite() {
    }
    
    void SetUp() {
        
    }

    void TearDown() {
        //getchar();
        
    }

    ~NeblinaCoreFixture() {
        // cleanup any pending stuff, but no exceptions allowed
    }

};

TEST_F(NeblinaCoreFixture, vec_add_float) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_add(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., r->value.f[i]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);
    
    EXPECT_EQ(1, 1);

}

// TEST_F(NeblinaCoreFixture, vec_len) {

//     int n = 3;

//     vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );

//     for (int i = 0; i < a->len; i++) {
//         a->value.f[i] = 1.;
//     }

//     object_t ** in = convertToObject(a, NULL);

//     int len = vec_len(&m, idx, (void **) in, NULL);

//     EXPECT_EQ(3, len);
//     delete_object_array(in, 1);
//     m.bridges[idx].vector_delete(a);
// }

TEST_F(NeblinaCoreFixture, vec_add_complex) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 1.;
        a->value.f[idx + 1] = 1.;
        
        b->value.f[idx] = 1.;
        b->value.f[idx + 1] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_add(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        int idx = 2 * (i);
        EXPECT_EQ(2., r->value.f[idx]);
        EXPECT_EQ(2., r->value.f[idx + 1]);
    }
    
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_sub_float) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_sub(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(0., r->value.f[i]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_sub_complex) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 1.;
        a->value.f[idx + 1] = 1.;
        
        b->value.f[idx] = 1.;
        b->value.f[idx + 1] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_sub(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        int idx = 2 * (i);
        EXPECT_EQ(0., r->value.f[idx]);
        EXPECT_EQ(0., r->value.f[idx + 1]);
    }

    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);

}


TEST_F(NeblinaCoreFixture, scalar_vec_float) {

    int n = 3;
    
    double scalar = 2.0;
    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    
    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    object_t ** in = convertScaVecToObject(scalar, a);
    
    vector_t * r = (vector_t *) vec_mulsc(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].vecreqhost(r);
    
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(4., r->value.f[i]);
    }

    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(r);
}

TEST_F(NeblinaCoreFixture, complex_scalar_float_vec) {

    int n = 3;
    
    complex_t * scalar = m.bridges[idx].complex_new(2.0, 2.0);
    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    
    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    vector_t * r = (vector_t *) vec_mul_complex_scalar(&m, idx, scalar, a);
    
    m.bridges[idx].vecreqhost(r);
    
    for (int i = 0; i < r->len; i++) {
        int idx = 2 * (i);

        EXPECT_EQ(4., r->value.f[idx]);
        EXPECT_EQ(2., r->value.f[idx + 1]);
    }

    m.bridges[idx].vector_delete(a);
    m.bridges[idx].complex_delete(scalar);
    m.bridges[idx].vector_delete(r);
}

TEST_F(NeblinaCoreFixture, complex_scalar_complex_vec) {

    int n = 3;
    
    complex_t * scalar = m.bridges[idx].complex_new(2.0, 2.0);
    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    
    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 2.;
        a->value.f[idx+1] = 2.;
    }

    vector_t * r = (vector_t *) mul_complex_scalar_complex_vec(&m, idx, scalar, a);
    
    m.bridges[idx].vecreqhost(r);
    
    for (int i = 0; i < r->len; i++) {
        int idx = 2 * (i);

        EXPECT_EQ(4., r->value.f[idx]);
        EXPECT_EQ(4., r->value.f[idx + 1]);
    }

    m.bridges[idx].vector_delete(a);
    m.bridges[idx].complex_delete(scalar);
    m.bridges[idx].vector_delete(r);
}

TEST_F(NeblinaCoreFixture, float_scalar_complex_vec) {

    int n = 3;
    
    double scalar = 2.0;
    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    
    for (int i = 0; i < a->len; i++) {
        int idx = 2 * (i);
        a->value.f[idx] = 2.;
        a->value.f[idx+1] = 2.;
    }

    vector_t * r = (vector_t *) mul_float_scalar_complex_vec(&m, idx, scalar, a);
    
    m.bridges[idx].vecreqhost(r);
    
    for (int i = 0; i < r->len; i++) {
        int idx = 2 * (i);

        EXPECT_EQ(4., r->value.f[idx]);
        EXPECT_EQ(2., r->value.f[idx + 1]);
    }

    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(r);
}

TEST_F(NeblinaCoreFixture, vec_prod_float) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
        b->value.f[i] = 2.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_prod(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(4., r->value.f[i]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_prod_complex) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * r;

    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 2.;
        a->value.f[i + 1] = 2.;
        b->value.f[i] = 2.;
        b->value.f[i + 1] = 2.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_prod(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < 2 * a->len; i += 2) {
        EXPECT_EQ(0., r->value.f[i]);
        EXPECT_EQ(8., r->value.f[i + 1]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_conj_complex) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * r;

    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 2.;
        a->value.f[i + 1] = 2.;
    }

    object_t ** in = convertToObject(a, NULL);

    r = (vector_t *) vec_conj(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(2., r->value.f[2 * i]);
        EXPECT_EQ(-2., r->value.f[2 * i + 1]);
    }
    delete_object_array(in, 1);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_add_off) {

    int n = 4;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }
    int offset = 2;
    object_t ** in = convertToObject2(offset, a);

    r = (vector_t *) vec_add_off(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < offset; ++i) {
        EXPECT_EQ(4., r->value.f[i]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(r);

}

TEST_F(NeblinaCoreFixture, vec_sum) {

    int n = 4;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    object_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    object_t ** in = convertToObject(a, NULL);

    r = (object_t *) vec_sum(&m, idx, (void **) in, NULL);

    EXPECT_EQ(8., r->value.f);
    
    delete_object_array(in, 1);
    delete_object(r);
    m.bridges[idx].vector_delete(a);
}

TEST_F(NeblinaCoreFixture, vec_sum_complex) {

    int n = 4;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    object_t * r;

    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 2.;
    }

    object_t ** in = convertToObject(a, NULL);

    r = (object_t *) vec_sum(&m, idx, (void **) in, NULL);

    EXPECT_EQ(8., r->value.f);
    
    delete_object_array(in, 1);
    delete_object(r);
    m.bridges[idx].vector_delete(a);
}

TEST_F(NeblinaCoreFixture, add_vector_complex) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_COMPLEX, 1 );
    vector_t * r;


    for (int i = 0; i < 2 * a->len; i += 2) {
        a->value.f[i] = 1.;
        a->value.f[i + 1] = 1.;
        b->value.f[i] = 1.;
        b->value.f[i + 1] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_add(&m, idx, (void **) in, NULL);
    
    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < 2 * a->len; i += 2) {
        EXPECT_EQ(2., r->value.f[i]);
        EXPECT_EQ(2., r->value.f[i + 1]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);
}

TEST_F(NeblinaCoreFixture, sub_vector_float) {

    int n = 3;

    vector_t * a = m.bridges[idx].vector_new(n, T_FLOAT, 1 );
    vector_t * b = m.bridges[idx].vector_new(n, T_FLOAT, 1 );

    vector_t * r;


    for (int i = 0; i < a->len; i++) {
        a->value.f[i] = 1.;
        b->value.f[i] = 1.;
    }

    object_t ** in = convertToObject(a, b);

    r = (vector_t *) vec_sub(&m, idx, (void **) in, NULL);

    m.bridges[idx].vecreqhost(r);

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(0., r->value.f[i]);
    }
    delete_object_array(in, 2);
    m.bridges[idx].vector_delete(a);
    m.bridges[idx].vector_delete(b);
    m.bridges[idx].vector_delete(r);

}