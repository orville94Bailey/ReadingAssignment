ase Study: Mat r ices, Cont inued
1 #ifndef MATRIX4_H
2 #define MATRIX4_H
3
4 #include <iostream>
5 #include <stdexcept>
6
7 using namespace std;
8
9 /**
10 Matrix exception class for indexing error.
11 */
12 class MatrixIndexException : public out_of_range
13 {
14 public:
15 MatrixIndexException(int i);
16 private:
17 string format_message(int i);
18 };
19
690 CHAPTER 17 • Exception Handling
20 /**
21 Matrix exception class for mismatched argument error.
22 */
23 class MatrixMismatchException : public invalid_argument
24 {
25 public:
26 MatrixMismatchException();
27 };
28
29 /**
30 This class describes a row in a matrix.
31 */
32 class Matrix;
33 class MatrixRow
34 {
... Same as in matrix2.h
48 double& operator[](int j) throw (MatrixIndexException);
...
53 };
...
80 /**
81 This class describes a matrix with arbitrary rows and columns.
82 */
83 class Matrix
84 {
85 public:
...
116 double& operator()(int i, int j) throw (MatrixIndexException);
...
131 MatrixRow operator[](int i) throw (MatrixIndexException);
...
145 Matrix& operator+=(const Matrix& right)
146 throw (MatrixMismatchException);
147
148 private:
...
163 };
...
170 Matrix operator+(const Matrix& left, const Matrix& right)
171 throw (MatrixMismatchException);
...
178 Matrix operator*(const Matrix& left, const Matrix& right)
179 throw (MatrixMismatchException);
...
203
204 inline MatrixIndexException::MatrixIndexException(int idx)
205 : out_of_range(format_message(idx)) {}
206
207 inline MatrixMismatchException::MatrixMismatchException()
208 : invalid_argument("Matrix arguments have incompatible sizes") {}
209
...
235 inline MatrixRow Matrix::operator[](int i) throw (MatrixIndexException)
236 {
17.4 • Case Study: Matrices, Continued 691
Methods that used assert in the previous version have been rewritten in
matrix4.cpp to throw exceptions on error. Notice that the matrix addition function
is declared as potentially throwing an exception even though none appears in the
body, because the exception from the operator += can propagate through the function.
The same is true for the subscript operators in MatrixRow and ConstMatrixRow.
ch17/matrix4.cpp
237 return MatrixRow(this, i);
238 }
239
...
247
248 inline double& MatrixRow::operator[](int j)
throw (MatrixIndexException)
249 {
250 return (*mat)(i,j);
251 }
...
267 #endif
1 #include <iomanip>
2 #include <sstream>
3
4 #include "matrix4.h"
5
6 string MatrixIndexException::format_message(int n)
7 {
8 ostringstream outstr;
9 outstr << "Matrix index " << n << " out of range";
10 return outstr.str();
11 }
12
... Same as in matrix2.cpp
38
39 double& Matrix::operator()(int i, int j) throw (MatrixIndexException)
40 {
41 if (i < 0 || i >= rows)
42 throw MatrixIndexException(i);
43 if (j < 0 || j >= columns)
44 throw MatrixIndexException(j);
45 return elements[i * get_columns() + j];
46 }
...
56
57 Matrix& Matrix::operator+=(const Matrix& right)
58 throw (MatrixMismatchException)
59 {
60 if (rows != right.rows || columns != right.columns)
61 throw MatrixMismatchException();
62 for (int i = 0; i < rows; i++)
63 for (int j = 0; j < columns; j++)
64 (*this)(i, j) += right(i, j);
692 CHAPTER 17 • Exception Handling
ch17/matrixtest4.cpp
Program Run
65 return *this;
66 }
67
68 Matrix operator+(const Matrix& left, const Matrix& right)
69 throw (MatrixMismatchException)
70 {
71    Matrix result = left;
72    result += right;
73 return result;
74 }
75
76 Matrix operator*(const Matrix& left, const Matrix& right)
77 throw (MatrixMismatchException)
78 {
79 if (left.get_columns() != right.get_rows())
80 throw MatrixMismatchException();
81    Matrix result(left.get_rows(), right.get_columns());
