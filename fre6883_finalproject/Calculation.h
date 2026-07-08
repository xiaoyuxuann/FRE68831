#include"Extractdata.h"

 namespace final{
     
void cal_AAR(Matrix &aar, BigMatrix &abmatrix); //calculate AAR
void cal_CAAR(Matrix &caar,Matrix &aar); //calculate CAAR
Vec calculate_expectation(const Matrix & matrix); //calculate average
Vec calculate_std(const Matrix & matrix,const Vec &exp_aar); //calculate std
BigMatrix cal_final_matrix(const BigMatrix & arr_matrix,const BigMatrix &caar_matrix); //get the avg AAR, AAR-STD, avg CAAR, CAAR-STD matrix

}