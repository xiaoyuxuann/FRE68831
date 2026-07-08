#include <vector>
#include <cmath>
#include "iostream"
#include <iomanip>
#include <stdio.h>
#include <string.h>
using namespace std;
namespace final{
    
typedef vector<double> Vec;
typedef vector<Vec> Matrix;
typedef vector<Matrix> BigMatrix;

Vec operator-(const Vec& V, const Vec& W);

Vec& operator+=(Vec & U,const Vec & V);
Vec& operator-=(Vec & U,const Vec & V);
Vec& operator/=(Vec & U,int a);
Matrix operator*(const Matrix&U,const Matrix&V);
Vec operator*(const Vec& V, const Vec& W);
Vec sqrt(const Vec &V);
std::ostream& operator<<(std::ostream &out,Vec &V );
std::ostream& operator<<(std::ostream &out,Matrix &W );
std:: ostream& operator<<(std::ostream &out,BigMatrix &H);
std::ostream& operator<<(std::ostream &out,vector<string> &V );
}