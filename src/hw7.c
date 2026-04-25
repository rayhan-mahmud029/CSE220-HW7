#include "hw7.h"

// helper function to free BST except the last matrix
void free_bst_except_sf(bst_sf *root, char skip_name) {
    if (root == NULL) return;
    // post-order traversal: free children BEFORE parent
    free_bst_except_sf(root->left_child, skip_name);
    free_bst_except_sf(root->right_child, skip_name);
    if (root->mat != NULL && root->mat->name != skip_name) {
        free(root->mat);  // only free if NOT the last matrix
    }
    free(root);
}

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    // "The function creates a new BST if root is NULL"
    if (root == NULL) {
        bst_sf *new_node = malloc(sizeof(bst_sf));
        new_node->mat = mat;            // "without making a copy of mat"
        new_node->left_child  = NULL;
        new_node->right_child = NULL;
        return new_node;               // "Return a pointer to the root"
    }

    // "the sorted property of the BST is maintained"
    // sort by matrix name (char comparison)
    if (mat->name < root->mat->name)
        root->left_child  = insert_bst_sf(mat, root->left_child);
    else
        root->right_child = insert_bst_sf(mat, root->right_child);

    // "Return a pointer to the root of the updated BST"
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    // "Given a pointer to the bst_sf struct root, which could be NULL"
    // "If no matrix of the given name is in the BST, return NULL"
    if (root == NULL) return NULL;

    // found it, return pointer to the matrix
    if (name == root->mat->name) return root->mat;

    // search left or right based on name comparison
    if (name < root->mat->name)
        return find_bst_sf(name, root->left_child);
    else
        return find_bst_sf(name, root->right_child);
}

void free_bst_sf(bst_sf *root) {
    // "which could be NULL" → nothing to free, stop
    if (root == NULL) return;

    // post-order traversal: free children BEFORE parent
    // otherwise we lose the pointers to children
    free_bst_sf(root->left_child);   // free left subtree first
    free_bst_sf(root->right_child);  // free right subtree second
    free(root->mat);                 // "free all matrix_sf structs pointed to by BST nodes"
    free(root);                      // "free all the nodes of the tree"
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) { // Matrices has to be same size
    // initializing result matrix by allocating memory using malloc
    matrix_sf *result = malloc(sizeof(matrix_sf) + mat1->num_rows * mat1->num_cols * sizeof(int));
    result->name = '0';  // non-alphabetical, marks it as a temp matrix
    result->num_rows = mat1->num_rows;
    result->num_cols = mat1->num_cols;
    for (unsigned int i = 0; i < mat1->num_rows * mat1->num_cols; i++) {
        result->values[i] = mat1->values[i] + mat2->values[i];
    }
    return result;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    // mat1 is m x n, mat2 is n x p, result is m x p
    unsigned int m = mat1->num_rows;
    unsigned int n = mat1->num_cols;  // has to be == mat2->num_rows
    unsigned int p = mat2->num_cols;

    // initializing result matrix by allocating memory using malloc
    matrix_sf *result = malloc(sizeof(matrix_sf) + m * p * sizeof(int));
    result->name = '0';  // non-alphabetical, marks it as a temp matrix
    result->num_rows = m;
    result->num_cols = p;

    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < p; j++) {
            int sum = 0;
            for (unsigned int k = 0; k < n; k++) {
                // mat1[i][k] * mat2[k][j]
                // i * n + k gives the index for mat1[i][k]
                // k * p + j gives the index for mat2[k][j]
                sum += mat1->values[i * n + k] * mat2->values[k * p + j];
            }
            result->values[i * p + j] = sum;  // i * p + j gives the index for result[i][j]
        }
    }
    return result;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    // flip the matrix over its diagonal, so mat[i][j] becomes mat[j][i]
    unsigned int m = mat->num_rows;
    unsigned int n = mat->num_cols;

    // initializing result matrix by allocating memory using malloc
    matrix_sf *result = malloc(sizeof(matrix_sf) + n * m * sizeof(int));
    result->name = '0';  // non-alphabetical, marks it as a temp matrix
    result->num_rows = n;  // rows and cols are SWAPPED after transpose
    result->num_cols = m;  // rows and cols are SWAPPED after transpose

    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            // j * m + i gives the index for result[j][i]
            // i * n + j gives the index for mat[i][j]
            result->values[j * m + i] = mat->values[i * n + j];
        }
    }
    return result;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    // expr is of the form:
    // a positive integer NR (number of rows)
    // one or more spaces
    // a positive integer NC (number of columns)
    // zero or more spaces
    // a left square bracket
    // zero or more spaces
    // NC integers separated by spaces and terminated by a semicolon
    // NR-1 additional lists of NC integers
    // a right square bracket
    unsigned int nr, nc;

    // getting nr and nc using sscanf which will avoid the spaces and get number as unsigned int
    const char *ptr = expr;
    sscanf(ptr, "%u %u", &nr, &nc);

    // initializing the matrix
    matrix_sf *result = malloc(sizeof(matrix_sf) + nr * nc * sizeof(int));
    result->name = name;
    result->num_rows = nr;
    result->num_cols = nc;

    // moving the pointer to the first left square bracket
    // strchr will return a pointer to the first occurrence of '[' in the string
    // or NULL if it is not found
    ptr = strchr(ptr, '[');
    ptr++; // move past '[', now ptr points to the first number in the matrix

    for (unsigned int i = 0; i < nr * nc; i++) {
        int val, chars_read;
        // sscanf with "%d" will skip whitespace automatically
        // but semicolons are not whitespace so skip them manually
        // %n will store the number of characters read so far
        // which we can use to advance the pointer
        while (*ptr == ' ' || *ptr == ';') ptr++;
        sscanf(ptr, "%d%n", &val, &chars_read);
        result->values[i] = val;
        ptr += chars_read; // advance ptr by how many chars sscanf consumed
    }
    return result;
}

char* infix2postfix_sf(char *infix) {
    int len = strlen(infix);

    char *output = malloc(len + 1);
    char *stack  = malloc(len + 1);

    int out_top   = 0;
    int stack_top = 0;

    // precedence helper: higher number = higher precedence
    // ' (transpose) = 3, * (multiply) = 2, + (add) = 1
    #define PREC(c) ((c) == '\'' ? 3 : (c) == '*' ? 2 : (c) == '+' ? 1 : 0)

    for (int i = 0; i < len; i++) {
        char c = infix[i];

        if (c == ' ') continue;  // skip spaces

        // matrix name → goes straight to output
        if (c >= 'A' && c <= 'Z') {
            output[out_top++] = c;
        }
        // left paren → push to stack
        else if (c == '(') {
            stack[stack_top++] = c;
        }
        // right paren → pop to output until left paren
        else if (c == ')') {
            while (stack_top > 0 && stack[stack_top - 1] != '(') {
                output[out_top++] = stack[--stack_top];
            }
            stack_top--; // discard '('
        }
        // operator: +, *, '
        else if (c == '+' || c == '*' || c == '\'') {
            if (c == '\'') {
                // strictly greater for right-associative unary operator
                // A'' must evaluate as (A')' correctly
                while (stack_top > 0 &&
                       stack[stack_top - 1] != '(' &&
                       PREC(stack[stack_top - 1]) > PREC(c)) {  // strictly greater
                    output[out_top++] = stack[--stack_top];
                }
            } else {
                // greater or equal for left-associative binary operators + and *
                while (stack_top > 0 &&
                       stack[stack_top - 1] != '(' &&
                       PREC(stack[stack_top - 1]) >= PREC(c)) {  // greater or equal
                    output[out_top++] = stack[--stack_top];
                }
            }
            stack[stack_top++] = c;
        }
    }

    // pop remaining operators to output
    while (stack_top > 0) {
        output[out_top++] = stack[--stack_top];
    }

    output[out_top] = '\0';
    free(stack);       // "Any memory allocated must be deallocated except postfix string"
    return output;     // "Return the newly allocated string containing postfix expression"
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    // "Use the function infix2postfix_sf to convert it to postfix"
    char *postfix = infix2postfix_sf(expr);

    // "create a stack of pointers to matrix_sf structs"
    matrix_sf **stack = malloc(strlen(postfix) * sizeof(matrix_sf*));
    int stack_top = 0;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        // matrix name → find in BST and push pointer
        if (c >= 'A' && c <= 'Z') {
            // "Call find_bst_sf to get a pointer to that matrix"
            stack[stack_top++] = find_bst_sf(c, root);
        }
        // "pop one (for ') matrix off the stack"
        // transpose is a unary operator
        else if (c == '\'') {
            matrix_sf *mat = stack[--stack_top];
            matrix_sf *result = transpose_mat_sf(mat);
            result->name = '0'; // "give it a non-alphabetical name"

            // "you can easily identify it as a matrix evaluate_expr_sf created"
            // "you can then safely free() it"
            if (mat->name < 'A' || mat->name > 'Z') free(mat);

            stack[stack_top++] = result;
        }
        // "pop two (for *) matrices off the stack"
        else if (c == '*') {
            matrix_sf *mat2 = stack[--stack_top]; // right operand first
            matrix_sf *mat1 = stack[--stack_top]; // left operand second
            matrix_sf *result = mult_mats_sf(mat1, mat2);
            result->name = '0'; // "give it a non-alphabetical name"

            // "If either or both of the popped matrices do NOT have
            //  an alphabetical name, free() the applicable matrix/matrices"
            if (mat1->name < 'A' || mat1->name > 'Z') free(mat1);
            if (mat2->name < 'A' || mat2->name > 'Z') free(mat2);

            stack[stack_top++] = result;
        }
        // "pop two (for +) matrices off the stack"
        else if (c == '+') {
            matrix_sf *mat2 = stack[--stack_top];
            matrix_sf *mat1 = stack[--stack_top];
            matrix_sf *result = add_mats_sf(mat1, mat2);
            result->name = '0'; // "give it a non-alphabetical name"

            // "If either or both of the popped matrices do NOT have
            //  an alphabetical name, free() the applicable matrix/matrices"
            if (mat1->name < 'A' || mat1->name > 'Z') free(mat1);
            if (mat2->name < 'A' || mat2->name > 'Z') free(mat2);

            stack[stack_top++] = result;
        }
    }

    // "storing the resulting matrix in a new matrix called name"
    matrix_sf *result = stack[--stack_top];
    result->name = name;  // assign the actual name to final result

    // "the function must deallocate the postfix expression string"
    free(postfix);
    // "the function must deallocate any memory it allocates"
    free(stack);

    // "Return a pointer to the new matrix" (not freed, caller handles it)
    return result;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

matrix_sf* execute_script_sf(char *filename) {
    // open the file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    // setup for getline as shown in assignment
    char *line = NULL;
    size_t max_line_size = MAX_LINE_LEN;

    bst_sf *root  = NULL;  // BST symbol table
    matrix_sf *last = NULL;  // tracks the last matrix created

    // "getline reads a line of text from a file"
    while (getline(&line, &max_line_size, file) != -1) {
        char mat_name;
        int  offset = 0;

        // every line starts with "X = ..."
        // parse the matrix name and skip past "X ="
        // %c reads the name, %*c skips the '=', %n tracks offset
        sscanf(line, " %c %*c %n", &mat_name, &offset);

        // rest points to everything after "X ="
        char *rest = line + offset;

        // skip any leading spaces before first real character
        while (*rest == ' ') rest++;

        // HOW TO TELL IF ITS A DEFINITION OR FORMULA:
        // definition → starts with a digit (NR is a positive integer)
        // formula    → starts with a letter or '('
        // example:
        // "A = 3 2 [ 4 5; 19 -34; ]"  → rest = "3 2 [..." → digit
        // "Z = (A + B)' * C"           → rest = "(A..." → not digit
        if (*rest >= '0' && *rest <= '9') {
            // its a matrix definition
            // "create_matrix_sf when a line defines a new matrix by providing its contents"
            last = create_matrix_sf(mat_name, rest);
        } else {
            // its a formula
            // "evaluate_expr_sf when a line creates a new matrix by performing operations"
            last = evaluate_expr_sf(mat_name, rest, root);
        }

        // "followed by a call to insert_bst_sf to insert the new matrix into the BST"
        root = insert_bst_sf(last, root);
    }

    // check if file was empty
    if (last == NULL) {
        free(line);
        fclose(file);
        return NULL;
    }

    // use helper to free BST without freeing last matrix
    // free_bst_except_sf skips freeing the matrix with last->name
    free_bst_except_sf(root, last->name);

    // "You'll have to deallocate the memory yourself"
    // free the getline buffer
    free(line);

    // close the file
    fclose(file);

    // "return a pointer to the final named matrix created on the last line"
    return last;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}