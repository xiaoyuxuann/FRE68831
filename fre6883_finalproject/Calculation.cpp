#include "Calculation.h"
namespace final{
    

void cal_AAR(Matrix &aar, BigMatrix &abmatrix){
 
 int d=(int)aar.size();
 int len=abmatrix[0].size();
 
  Vec  aar_one_group;
 for(int i=0;i<d;i++){
      aar_one_group=calculate_expectation(abmatrix[i]);
      aar[i]=(aar_one_group);
 }
}

void cal_CAAR(Matrix &caar,Matrix &aar){
 
 int d=(int)aar.size();
 int dateSize =  aar[0].size();
 
 for(int k = 0; k<d; k++){
  double cumulateRet = 0;
  caar[k].resize(dateSize);
  for(int i = 0; i<dateSize; i++){
   cumulateRet += aar[k][i];
   caar[k][i]=(cumulateRet);
  }
 }
}

 Vec calculate_expectation(const Matrix & matrix){
  Vec exp_aar=matrix[0];
  int n=(int)matrix.size();
  for(int i=1;i<n;i++){
    exp_aar+=matrix[i];
  }
  exp_aar/=n;
  return exp_aar;
}


 Vec calculate_std(const Matrix & matrix,const Vec &exp_aar){
  
  int n=(int)matrix.size();
  Matrix ma=matrix;
  for(int i=0;i<n;i++){
    ma[i]-=exp_aar;
  }
   ma=ma*ma;
   Vec var_caar=calculate_expectation(ma);
   Vec std_caar=sqrt(var_caar);
  
  
  return std_caar;
  //change arr_exp which include the expectation of arrs or caars for all three groups
}


BigMatrix cal_final_matrix(const BigMatrix & arr_matrix,const BigMatrix &caar_matrix){
  
   BigMatrix final_matrix(3);
   Vec exp_aar,std_aar,exp_caar,std_caar;
   for(int i=0;i<3;i++){
       final_matrix[i].resize(4);
       exp_aar=calculate_expectation(arr_matrix[i]);
       //cout<<"the exp_aar size"<<exp_aar.size()<<endl;
       final_matrix[i][0]=exp_aar;
       std_aar=calculate_std(arr_matrix[i],exp_aar);
       final_matrix[i][1]=std_aar;
       exp_caar=calculate_expectation(caar_matrix[i]);
       final_matrix[i][2]=exp_caar;
       std_caar=calculate_std(caar_matrix[i],exp_caar);
       final_matrix[i][3]=std_caar;
   }
  return final_matrix;
}
}