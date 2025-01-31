#include <reis.h>
#include <math.h>

// Matrix, WIP
typedef struct {
	int rows;
	int cols;
	double** data;
} Matrix;

Matrix* 
create_matrix(int n_rows, int n_cols) {
	Matrix *mat = (Matrix*)MALLOC(sizeof(Matrix));
	mat->rows = n_rows;
	mat->cols = n_cols;
	double** _data = (double**)MALLOC(sizeof(double*) * n_rows);

	for(int i=0;i<n_rows;i++) {
		_data[i] = (double*)CALLOC(n_cols, sizeof(double));
	}
	mat->data = _data;
	return mat;
}

Matrix* 
copy_matrix(double* data, int n_rows, int n_cols) {
	Matrix *mat = create_matrix(n_rows, n_cols);
	for(int i=0;i<n_rows;i++) {
		for(int j=0;j<n_cols;j++) {
			mat->data[i][j] = data[n_cols*i+j];
		}
	}
	return mat;
}

void 
print_matrix(Matrix* mat) {
    for(int i=0;i<mat->rows;i++) {
        printf("%s", "\n");
        for(int j=0;j<mat->cols;j++) {
            printf("%f\t", mat->data[i][j]);
        }
    }
}

// BINARY OPERATIONS

#define bset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bclear(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define bflip(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define bcheck(byte,nbit) ((byte) &   (1<<(nbit)))

int 
bcount(int number) {
	return (int)log2(number)+1;
}

int 
isNum(const char *str) {
		while(*str) {
				if(!isdigit(*str))
						return 0;
				str++;
		}
		return 1;
}

// TODO implement the defines
/*int slctb(int num, size_t start, size_t end) {
	assert(end >= start);

	const int mask = (1 << (end-start+1)) - 1;
	const int shift = start - 1;

	return (num & (mask << shift)) >> shift;
}*/
