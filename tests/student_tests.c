#include "unit_tests.h"
#include "hw7.h"

//  
// helper: compare two matrices for equality
//  
int matrices_equal_student(matrix_sf *a, matrix_sf *b) {
    if (a->num_rows != b->num_rows) return 0;
    if (a->num_cols != b->num_cols) return 0;
    for (unsigned int i = 0; i < a->num_rows * a->num_cols; i++)
        if (a->values[i] != b->values[i]) return 0;
    return 1;
}

//  
// TEST SUITES
//  
TestSuite(student_operator_return, .timeout=TEST_TIMEOUT);
TestSuite(student_bst, .timeout=TEST_TIMEOUT);
TestSuite(student_create_matrix, .timeout=TEST_TIMEOUT);
TestSuite(student_infix2postfix, .timeout=TEST_TIMEOUT);
TestSuite(student_evaluate_expr, .timeout=TEST_TIMEOUT);
TestSuite(student_execute, .timeout=TEST_TIMEOUT);
TestSuite(student_execute_valgrind, .timeout=TEST_TIMEOUT);

//  
// TEST 1: add 2x2 matrices
//  
Test(student_operator_return, student_add01) {
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *B = copy_matrix(2, 2, (int[]){5, 6, 7, 8});
    matrix_sf *C = add_mats_sf(A, B);
    expect_matrices_equal(C, 2, 2, (int[]){6, 8, 10, 12});
    free(A); free(B); free(C);
}

//  
// TEST 2: add with negative numbers
//  
Test(student_operator_return, student_add02) {
    matrix_sf *A = copy_matrix(2, 2, (int[]){-1, -2, -3, -4});
    matrix_sf *B = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *C = add_mats_sf(A, B);
    expect_matrices_equal(C, 2, 2, (int[]){0, 0, 0, 0});
    free(A); free(B); free(C);
}

//  
// TEST 3: multiply non-square matrices
//  
Test(student_operator_return, student_mult01) {
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *B = copy_matrix(3, 2, (int[]){7, 8, 9, 10, 11, 12});
    matrix_sf *C = mult_mats_sf(A, B);
    expect_matrices_equal(C, 2, 2, (int[]){58, 64, 139, 154});
    free(A); free(B); free(C);
}

//  
// TEST 4: dot product
//  
Test(student_operator_return, student_mult02) {
    matrix_sf *A = copy_matrix(1, 3, (int[]){1, 2, 3});
    matrix_sf *B = copy_matrix(3, 1, (int[]){4, 5, 6});
    matrix_sf *C = mult_mats_sf(A, B);
    expect_matrices_equal(C, 1, 1, (int[]){32});
    free(A); free(B); free(C);
}

//  
// TEST 5: transpose
//  
Test(student_operator_return, student_trans01) {
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *B = transpose_mat_sf(A);
    expect_matrices_equal(B, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    free(A); free(B);
}

//  
// TEST 6: double transpose
//  
Test(student_operator_return, student_trans02) {
    matrix_sf *A  = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *T1 = transpose_mat_sf(A);
    matrix_sf *T2 = transpose_mat_sf(T1);
    expect_matrices_equal(T2, 2, 3, (int[]){1, 2, 3, 4, 5, 6});
    free(A); free(T1); free(T2);
}

//  
// TEST 7: BST
//  
Test(student_bst, student_bst01) {
    bst_sf *root = NULL;
    matrix_sf *A = malloc(sizeof(matrix_sf)); A->name = 'A';
    matrix_sf *M = malloc(sizeof(matrix_sf)); M->name = 'M';
    matrix_sf *Z = malloc(sizeof(matrix_sf)); Z->name = 'Z';

    root = insert_bst_sf(M, root);
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(Z, root);

    cr_expect_eq(find_bst_sf('M', root), M);
    cr_expect_eq(find_bst_sf('A', root), A);
    cr_expect_eq(find_bst_sf('Z', root), Z);
    cr_expect_eq(find_bst_sf('B', root), NULL);

    free(A); free(M); free(Z);
}

//  
// TEST 8: create matrix
//  
Test(student_create_matrix, student_create01) {
    matrix_sf *mat = create_matrix_sf('A', "3 2 [4 5; 19 -34 ; 192 -9110; ]");
    expect_matrices_equal(mat, 3, 2, (int[]){4, 5, 19, -34, 192, -9110});
    cr_expect_eq(mat->name, 'A');
    free(mat);
}

//  
// TEST 9: create matrix negative
//  
Test(student_create_matrix, student_create02) {
    matrix_sf *mat = create_matrix_sf('G', "2 5 [ 8 4 9 1 13 ;-5 0 6 22 24 ;]");
    expect_matrices_equal(mat, 2, 5, (int[]){8, 4, 9, 1, 13, -5, 0, 6, 22, 24});
    cr_expect_eq(mat->name, 'G');
    free(mat);
}

//  
// TEST 10: postfix
//  
Test(student_infix2postfix, student_postfix01) {
    char *result = infix2postfix_sf("A+B");
    cr_expect_arr_eq(result, "AB+", 3);
    free(result);
}

//  
// TEST 11: postfix PDF
//  
Test(student_infix2postfix, student_postfix02) {
    char *result = infix2postfix_sf("P'*K+A");
    cr_expect_arr_eq(result, "P'K*A+", 6);
    free(result);
}

//  
// TEST 12: evaluate
//  
Test(student_evaluate_expr, student_expr01) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1,2,3,4}); A->name='A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){5,6,7,8}); B->name='B';

    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);

    matrix_sf *result = evaluate_expr_sf('C', "A+B", root);
    expect_matrices_equal(result, 2, 2, (int[]){6,8,10,12});
    cr_expect_eq(result->name, 'C');

    free(result);
    free_bst_sf(root);
}

//  
// TEST 13: transpose expr
//  
Test(student_evaluate_expr, student_expr02) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 3, (int[]){1,2,3,4,5,6}); A->name='A';
    root = insert_bst_sf(A, root);

    matrix_sf *result = evaluate_expr_sf('B', "A'", root);
    expect_matrices_equal(result, 3, 2, (int[]){1,4,2,5,3,6});
    cr_expect_eq(result->name, 'B');

    free(result);
    free_bst_sf(root);
}

//  
// TEST 14–15: scripts 
//  
Test(student_execute, student_script01) {
    run_script_without_valgrind("student_script01");
}
Test(student_execute_valgrind, student_script01) {
    expect_no_valgrind_errors(run_script_with_valgrind("student_script01"));
}

Test(student_execute, student_script02) {
    run_script_without_valgrind("student_script02");
}
Test(student_execute_valgrind, student_script02) {
    expect_no_valgrind_errors(run_script_with_valgrind("student_script02"));
}