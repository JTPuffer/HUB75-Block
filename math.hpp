#include <iostream>
#include <cmath>
namespace bob {

    template <typename T>class Vector4;

    template <typename T>class Matrix4 {
    public:
        T data[4][4];
        Matrix4(const T num);
        Matrix4(std::initializer_list<std::initializer_list<T>> inputList);
        ~Matrix4();

        static Matrix4<double> rotateX(const double angle) {
            double cosTheta = std::cos(angle);
            double sinTheta = std::sin(angle);

            return Matrix4<double>({
                {1, 0, 0, 0},
                {0, cosTheta, -sinTheta, 0},
                {0, sinTheta, cosTheta, 0},
                {0, 0, 0, 1} });
        }

        // Function to create a rotation matrix around the y-axis
        static Matrix4<double> rotateY(double angle) {
            double cosTheta = std::cos(angle);
            double sinTheta = std::sin(angle);

            return Matrix4<double>({
                {cosTheta, 0, sinTheta, 0},
                {0, 1, 0, 0},
                {-sinTheta, 0, cosTheta, 0},
                {0, 0, 0, 1} });
        }

        static Matrix4<double> rotateZ(double angle) {
            double cosTheta = std::cos(angle);
            double sinTheta = std::sin(angle);

            return Matrix4<double>({
                {cosTheta, -sinTheta, 0, 0},
                {sinTheta, cosTheta, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}
                });
        }

        Vector4<T> operator*(const Vector4<T>& v) {
            Vector4<T> temp = Vector4<T>();
            for (int i = 0; i < 4; i++) {
                *temp[i] += data[i][0] * v.x;
                *temp[i] += data[i][1] * v.y;
                *temp[i] += data[i][2] * v.z;
                *temp[i] += data[i][3] * v.w;
            }
            return temp;
        }
        T operator()(int row, int colum) {
            return data[row][colum];
        }
        Matrix4 operator*(const Matrix4<T>& m) {
            Matrix4<T> temp = Matrix4<T>(0.0);
            for (int i = 0; i < 4; ++i) {
                for (int k = 0; k < 4; ++k) {
                    for (int j = 0; j < 4; ++j) {
                        temp.data[i][j] += data[i][k] * m.data[k][j];
                    }
                }
            }
            return temp;
        }
        void print_matrix() {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    printf("%f\t",data[i][j]);
                }
                printf("\n");
            }
        }
    };


    template <typename T>class Vector4
    {
    public:
        T x;
        T y;
        T z;
        T w;

        Vector4(T uX, T uY, T uZ, T uW) :x(uX), y(uY), z(uZ), w(uW) {}
        Vector4() :x(0), y(0), z(0), w(0) {}

        ~Vector4();

        Vector4 operator+(const Vector4& v) {
            return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
        }
        Vector4 operator*(const Vector4& v) {
            return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
        }
        Vector4 operator*(const Matrix4<T>& v) {
            Vector4<T> temp = Vector4<T>();
            for (int i = 0; i < 4; i++) {
                *temp[i] += v.data[i][0] * x;
                *temp[i] += v.data[i][1] * y;
                *temp[i] += v.data[i][2] * z;
                *temp[i] += v.data[i][3] * w;
            }
            return temp;
        }
        T* operator[](const int num) {
              switch (num)
            {
            case 0:
                return &x;
            case 1:
                return &y;
            case 2:
                return &z;   
            case 3:
                return &w;
            }
			return &x;
        }

    };


    template <typename T>
    inline Matrix4<T>::Matrix4(T num)
    {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                data[i][j] = (i == j) ? num : 0;  // Set the diagonal elements to 'value', others to 0
            }
        }
    }

    template<typename T>
    inline Matrix4<T>::Matrix4(std::initializer_list<std::initializer_list<T>> inputList) {
        if (inputList.size() == 4) {
            int i = 0;
            for (const auto& row : inputList) {
                if (row.size() == 4) {
                    int j = 0;
                    for (const auto& value : row) {
                        data[i][j] = value;
                        ++j;
                    }
                }
                else {
                    //throw std::invalid_argument("Each row must have exactly 4 elements.");
                }
                ++i;
            }
        }
        else {
            //throw std::invalid_argument("The initializer list must contain exactly 4 rows.");
        }
    }

    template <typename T>
    Matrix4<T>::~Matrix4()
    {
    }

    template<typename T>
    inline Vector4<T>::~Vector4()
    {
    }

}