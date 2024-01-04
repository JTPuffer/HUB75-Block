#include <iostream>
#include <cmath>
namespace bob {


        template <typename T, int8_t size>class Vector;
        template <typename T, int8_t row, int8_t colum>class Matrix;

        typedef Matrix<float, 4, 4> Matrix4f;
        typedef Matrix<float, 3, 3> Matrix3f;

        typedef Vector<float, 4> Vector4f;
        typedef Vector<float, 3> Vector3f;



        template <typename T, int8_t row, int8_t colum>class Matrix {
        public:
            T data[row][colum];
            Matrix() {
                for (int i = 0; i < row; i++) {
                    for (int j = 0; j < colum; j++) {
                        data[i][j] = (i == j) ? 1 : 0;  // Set the diagonal elements to 'value', others to 0
                    }
                }
            }
            template <typename... Args>
            Matrix(Args... args) :data{ static_cast<T>(args)... } {
                static_assert(row > 0 && colum > 0, "Matrix dimensions must be greater than zero");
                static_assert(sizeof...(args) / (row * sizeof(T)) <= colum, "too many colums");
                static_assert(sizeof...(args) / (colum * sizeof(T)) <= row, "too many rows");
            };
            ~Matrix();

            static Matrix<T, 4, 4> rotateX(T angle) {// this will be changed soon promise ( :
                T cosTheta = std::cos(angle);
                T sinTheta = std::sin(angle);

                return Matrix<T, 4, 4>(
                    1, 0, 0, 0,
                    0, cosTheta, -sinTheta, 0,
                    0, sinTheta, cosTheta, 0,
                    0, 0, 0, 1);
            }

            // Function to create a rotation matrix around the y-axis
            static Matrix<T, 4, 4>rotateY(T angle) {
                T cosTheta = std::cos(angle);
                T sinTheta = std::sin(angle);

                return Matrix<T, 4, 4>(
                    cosTheta, 0, sinTheta, 0,
                    0, 1, 0, 0,
                    -sinTheta, 0, cosTheta, 0,
                    0, 0, 0, 1);
            }

            static Matrix<T, 4, 4>rotateZ(T angle) {
                T cosTheta = std::cos(angle);
                T sinTheta = std::sin(angle);

                return Matrix<T, 4, 4>(
                    cosTheta, -sinTheta, 0, 0,
                    sinTheta, cosTheta, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                );
            }

        
            Vector<T, colum> operator*(const Vector<T, colum>& v) {
                Vector<T, colum> temp;
                for (int8_t i = 0; i < colum; i++) {
                    for (int8_t j = 0; j < row; j++) {// can do it here though so i guess if you want to do that it has to be in this order see vector class operation
                        temp.data[i] += data[i][j] * v.data[j];
                    }
                }
                return temp;
            }
            //assumming its the same size matrix i know it doesnt have to be but its faster slightly
            Matrix operator*(const Matrix<T, row, colum>& m) {
                Matrix< T, row, colum > temp = Matrix< T, row, colum >(0.0);
                for (int i = 0; i < row; ++i) {
                    for (int k = 0; k < colum; ++k) {
                        for (int j = 0; j < row; ++j) {
                            temp.data[i][j] += data[i][k] * m.data[k][j];
                        }
                    }
                }
                return temp;
            }
            void print_matrix() {
                for (int i = 0; i < row; i++) {
                    for (int j = 0; j < colum; j++) {
                        printf("%f\t", data[i][j]);
                    }
                    printf("\n");
                }
            }
        };


        template <typename T, int8_t size>class Vector
        {
        public:
            T data[size];

            template <typename... Args>
            Vector(Args... args) :data{ static_cast<T>(args)... } {
                static_assert(sizeof...(args) <= size, "too many elements in vector");
            }
            Vector() :data{} {}

            ~Vector();

            Vector operator+(const Vector& v) {
                Vector result;
                for (int8_t i = 0; i < size; ++i) {
                    result.data[i] = data[i] + v.data[i];
                }
                return result;
            }
            Vector operator*(const Vector& v) {
                Vector result;
                for (int8_t i = 0; i < size; ++i) {
                    result.data[i] = data[i] * v.data[i];
                }
                return result;
            }
            Vector operator*(const Matrix<T, size, size>& m) {
                Vector<T, size> temp;
                for (int8_t i = 0; i < size; i++) {
                    for (int8_t j = 0; j < size; j++) {// colums is not compile time for efficiency can assume it is a square matrix
                        temp.data[i] += m.data[i][j] * data[j];
                    }
                }
                return temp;
            }
            T operator[](const int num) {
                return data[num];
            }

            Vector<T, size> normalise() {
                float total = 0;
                for (int8_t i = 0; i < size; i++) {
                    total += (data[i] * data[i]);
                }

                total = std::sqrt(total);
                Vector<T, size> temp;

                for (int8_t i = 0; i < size; i++) {
                    temp.data[i] = data[i] / total;
                }
                return temp;
            }
            float dotProduct(Vector<T, size>& v1) {
                float total = 0;
                for (int8_t i = 0; i < size; i++) {
                    total += (data[i] * v1.data[i]);
                }
                return total;
            }
        };

        template <typename T>
        Vector<T, 3> crossProduct3(Vector<T, 3>& edge1, Vector<T, 3>& edge2) {
            return Vector<T, 3>(edge1.data[1] * edge2.data[2] - edge1.data[2] * edge2.data[1],// cross product
                edge1.data[2] * edge2.data[0] - edge1.data[0] * edge2.data[2],
                edge1.data[0] * edge2.data[1] - edge1.data[1] * edge2.data[0]);
        }
        template <typename T>
        Vector<T, 3> normal3(Vector<T, 3>& v0, Vector<T, 3>& v1, Vector<T, 3>& v2) {
            Vector<T, 3> edge1 = Vector<T, 3>(v1[0] - v0.data[0], v1.data[1] - v0.data[1], v1.data[2] - v0.data[2]);
            Vector<T, 3> edge2 = Vector<T, 3>(v2.data[0] - v0.data[0], v2.data[1] - v0.data[1], v2.data[2] - v0.data[2]);

            return crossProduct3(edge1, edge2);
        }

        template<typename T, int8_t row, int8_t colum>
        inline Matrix<T, row, colum>::~Matrix()
        {
        }
        template<typename T, int8_t size>
        inline Vector<T, size>::~Vector()
        {
        }


    
}