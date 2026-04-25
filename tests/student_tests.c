#include "unit_tests.h"
#include "hw7.h"

// ─────────────────────────────────────────────
// helper: compare two matrices for equality
// ─────────────────────────────────────────────
int matrices_equal_student(matrix_sf *a, matrix_sf *b) {
    if (a->num_rows != b->num_rows) return 0;
    if (a->num_cols != b->num_cols) return 0;
    for (unsigned int i = 0; i < a->num_rows * a->num_cols; i++)
        if (a->values[i] != b->values[i]) return 0;
    return 1;
}

// ─────────────────────────────────────────────
// TEST SUITES (matching the style in unit_tests.c)
// ─────────────────────────────────────────────
TestSuite(student_operator_return, .timeout=TEST_TIMEOUT);
TestSuite(student_operator_valgrind, .timeout=TEST_TIMEOUT);
TestSuite(student_bst, .timeout=TEST_TIMEOUT);
TestSuite(student_create_matrix, .timeout=TEST_TIMEOUT);
TestSuite(student_infix2postfix, .timeout=TEST_TIMEOUT);
TestSuite(student_evaluate_expr, .timeout=TEST_TIMEOUT);
TestSuite(student_execute, .timeout=TEST_TIMEOUT);
TestSuite(student_execute_valgrind, .timeout=TEST_TIMEOUT);

// ─────────────────────────────────────────────
// TEST 1: add 2x2 matrices
// ─────────────────────────────────────────────
Test(student_operator_return, student_add01, .description="Add two 2x2 matrices") {
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *B = copy_matrix(2, 2, (int[]){5, 6, 7, 8});
    matrix_sf *C = add_mats_sf(A, B);
    expect_matrices_equal(C, 2, 2, (int[]){6, 8, 10, 12});
    free(A); free(B); free(C);
}
Test(student_operator_valgrind, student_add01) {
    expect_no_valgrind_errors(run_with_valgrind("student_add01"));
}

// ─────────────────────────────────────────────
// TEST 2: add with negative numbers
// ─────────────────────────────────────────────
Test(student_operator_return, student_add02, .description="Add matrices with negative numbers") {
    matrix_sf *A = copy_matrix(2, 2, (int[]){-1, -2, -3, -4});
    matrix_sf *B = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *C = add_mats_sf(A, B);
    expect_matrices_equal(C, 2, 2, (int[]){0, 0, 0, 0});
    free(A); free(B); free(C);
}
Test(student_operator_valgrind, student_add02) {
    expect_no_valgrind_errors(run_with_valgrind("student_add02"));
}

// ─────────────────────────────────────────────
// TEST 3: multiply non-square matrices
// ─────────────────────────────────────────────
Test(student_operator_return, student_mult01, .description="Multiply 2x3 by 3x2 matrices") {
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *B = copy_matrix(3, 2, (int[]){7, 8, 9, 10, 11, 12});
    matrix_sf *C = mult_mats_sf(A, B);
    // C[0][0]=1*7+2*9+3*11=58, C[0][1]=1*8+2*10+3*12=64
    // C[1][0]=4*7+5*9+6*11=139, C[1][1]=4*8+5*10+6*12=154
    expect_matrices_equal(C, 2, 2, (int[]){58, 64, 139, 154});
    free(A); free(B); free(C);
}
Test(student_operator_valgrind, student_mult01) {
    expect_no_valgrind_errors(run_with_valgrind("student_mult01"));
}

// ─────────────────────────────────────────────
// TEST 4: multiply 1x3 by 3x1 (dot product → 1x1)
// ─────────────────────────────────────────────
Test(student_operator_return, student_mult02, .description="Multiply 1x3 by 3x1 giving 1x1") {
    matrix_sf *A = copy_matrix(1, 3, (int[]){1, 2, 3});
    matrix_sf *B = copy_matrix(3, 1, (int[]){4, 5, 6});
    matrix_sf *C = mult_mats_sf(A, B);
    // 1*4 + 2*5 + 3*6 = 4+10+18 = 32
    expect_matrices_equal(C, 1, 1, (int[]){32});
    free(A); free(B); free(C);
}
Test(student_operator_valgrind, student_mult02) {
    expect_no_valgrind_errors(run_with_valgrind("student_mult02"));
}

// ─────────────────────────────────────────────
// TEST 5: transpose rectangular matrix
// ─────────────────────────────────────────────
Test(student_operator_return, student_trans01, .description="Transpose 2x3 matrix") {
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *B = transpose_mat_sf(A);
    expect_matrices_equal(B, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    free(A); free(B);
}
Test(student_operator_valgrind, student_trans01) {
    expect_no_valgrind_errors(run_with_valgrind("student_trans01"));
}

// ─────────────────────────────────────────────
// TEST 6: double transpose equals original
// ─────────────────────────────────────────────
Test(student_operator_return, student_trans02, .description="Double transpose equals original") {
    matrix_sf *A  = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *T1 = transpose_mat_sf(A);   // 3x2
    matrix_sf *T2 = transpose_mat_sf(T1);  // 2x3 again
    expect_matrices_equal(T2, 2, 3, (int[]){1, 2, 3, 4, 5, 6});
    free(A); free(T1); free(T2);
}
Test(student_operator_valgrind, student_trans02) {
    expect_no_valgrind_errors(run_with_valgrind("student_trans02"));
}

// ─────────────────────────────────────────────
// TEST 7: BST insert and find
// ─────────────────────────────────────────────
Test(student_bst, student_bst01, .description="Insert and find in BST") {
    bst_sf *root = NULL;
    matrix_sf *A = malloc(sizeof(matrix_sf)); A->name = 'A';
    matrix_sf *M = malloc(sizeof(matrix_sf)); M->name = 'M';
    matrix_sf *Z = malloc(sizeof(matrix_sf)); Z->name = 'Z';

    root = insert_bst_sf(M, root);  // root
    root = insert_bst_sf(A, root);  // left of M
    root = insert_bst_sf(Z, root);  // right of M

    cr_expect_eq(find_bst_sf('M', root), M);
    cr_expect_eq(find_bst_sf('A', root), A);
    cr_expect_eq(find_bst_sf('Z', root), Z);
    cr_expect_eq(find_bst_sf('B', root), NULL); // not in tree

    free(A); free(M); free(Z);
    // bst nodes not freed (matches unit_tests.c style)
}

// ─────────────────────────────────────────────
// TEST 8: create_matrix_sf with spaces variation
// ─────────────────────────────────────────────
Test(student_create_matrix, student_create01, .description="Create matrix with varying spaces") {
    // from PDF example
    matrix_sf *mat = create_matrix_sf('A', "3 2 [4 5; 19 -34 ; 192 -9110; ]");
    expect_matrices_equal(mat, 3, 2, (int[]){4, 5, 19, -34, 192, -9110});
    cr_expect_eq(mat->name, 'A');
    free(mat);
}

// ─────────────────────────────────────────────
// TEST 9: create_matrix_sf with negative numbers
// ─────────────────────────────────────────────
Test(student_create_matrix, student_create02, .description="Create matrix with negative numbers") {
    matrix_sf *mat = create_matrix_sf('G', "2 5 [ 8 4 9 1 13 ;-5 0 6 22 24 ;]");
    // this is the exact example from the PDF
    expect_matrices_equal(mat, 2, 5, (int[]){8, 4, 9, 1, 13, -5, 0, 6, 22, 24});
    cr_expect_eq(mat->name, 'G');
    free(mat);
}

// ─────────────────────────────────────────────
// TEST 10: infix2postfix simple
// ─────────────────────────────────────────────
Test(student_infix2postfix, student_postfix01, .description="Simple infix to postfix") {
    char *result = infix2postfix_sf("A+B");
    cr_expect_arr_eq(result, "AB+", 3, "Expected AB+ got %s", result);
    free(result);
}

// ─────────────────────────────────────────────
// TEST 11: infix2postfix PDF example
// ─────────────────────────────────────────────
Test(student_infix2postfix, student_postfix02, .description="PDF example P'*K+A") {
    char *result = infix2postfix_sf("P'*K+A");
    cr_expect_arr_eq(result, "P'K*A+", 6, "Expected P'K*A+ got %s", result);
    free(result);
}

// ─────────────────────────────────────────────
// TEST 12: evaluate_expr simple addition
// ─────────────────────────────────────────────
Test(student_evaluate_expr, student_expr01, .description="Evaluate simple A+B expression") {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4}); A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){5, 6, 7, 8}); B->name = 'B';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);

    matrix_sf *result = evaluate_expr_sf('C', "A+B", root);
    expect_matrices_equal(result, 2, 2, (int[]){6, 8, 10, 12});
    cr_expect_eq(result->name, 'C');

    free(result);
    free_bst_sf(root);
}

// ─────────────────────────────────────────────
// TEST 13: evaluate_expr transpose
// ─────────────────────────────────────────────
Test(student_evaluate_expr, student_expr02, .description="Evaluate transpose expression") {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6}); A->name = 'A';
    root = insert_bst_sf(A, root);

    matrix_sf *result = evaluate_expr_sf('B', "A'", root);
    expect_matrices_equal(result, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    cr_expect_eq(result->name, 'B');

    free(result);
    free_bst_sf(root);
}

// ─────────────────────────────────────────────
// TEST 14: execute_script addition
// ─────────────────────────────────────────────
Test(student_execute, student_script01, .description="Execute simple addition script") {
    run_script_without_valgrind("student_script01");
}
Test(student_execute_valgrind, student_script01) {
    expect_no_valgrind_errors(run_script_with_valgrind("student_script01"));
}

// ─────────────────────────────────────────────
// TEST 15: execute_script chained operations
// ─────────────────────────────────────────────
Test(student_execute, student_script02, .description="Execute chained operations script") {
    run_script_without_valgrind("student_script02");
}
Test(student_execute_valgrind, student_script02) {
    expect_no_valgrind_errors(run_script_with_valgrind("student_script02"));
}