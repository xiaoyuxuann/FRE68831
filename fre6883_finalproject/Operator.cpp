#include "Operator.h"
namespace final{
    
    Vec operator-(const Vec& V, const Vec& W)
{
    int d = (int)V.size();
    Vec U(d);
    for (int j = 0; j < d; j++) U[j] = V[j] - W[j];
    return U;
}


    Vec& operator+=(Vec & U,const Vec & V){
 int d=(int)V.size();
 for(int j=0;j<d;j++){
     U[j]+=V[j];
  
 }
 return U;
}
Vec& operator-=(Vec &U,const Vec &V){
 int d=(int)V.size();
 for(int j=0;j<d;j++){
    U[j]-=V[j];
  
 }
 return U;
}

 Vec& operator/=(Vec & U,int a){
  int d=(int)U.size();
 for(int j=0;j<d;j++){
    U[j]=U[j]/a;
  
 }
 return U;
  
 }
 Matrix operator*(const Matrix&U,const Matrix&V){
    int n=(int)U.size();
    Matrix W(n);
    for(int i=0;i<n;i++){
      W[i]=U[i]*V[i];
    }
    return W;
  
 }
 Vec operator*(const Vec& V, const Vec& W)
{
    int d = (int)V.size();
    Vec U(d);
    for (int j = 0; j < d; j++) U[j] = V[j] * W[j];
    return U;
}
Vec sqrt(const Vec &V){
    int d=(int) V.size();
    Vec U(d);
    for(int j=0;j<d;j++){
        U[j]=std::sqrt(V[j]);
    }
    return U;
}

        std::ostream& operator<<(std::ostream &out,Matrix &W ){
            
        for(Matrix::iterator itr=W.begin();itr!=W.end();itr++){
               cout<<"Now print a new line "<<endl;
                 out<<*itr;
            }
             out<<endl;
             return out;
        }
             
    std::ostream& operator<<(std::ostream &out,BigMatrix &H ){
            for(BigMatrix::iterator itr=H.begin();itr!=H.end();itr++){
            cout<<"Now print a new group"<<endl;
                 out<<*itr;
             }
             out<<endl;
             return out;
        }
        
    std::ostream& operator<<(std::ostream &out,Vec &V ){
        out << fixed << showpoint;
        out << setprecision(3);
        for(Vec::iterator ite=V.begin();ite!=V.end();ite++){
            if(*ite>0){ out << *ite << "     ";}
            else{out << *ite << "    ";}
            size_t cur_index = distance(std::begin(V), ite); // get itr index
		    if(((int)cur_index+1)%10==0) out<<endl; //return if (index+1) mode 10 = 0 
        }
        out<<endl;
        out << endl;
          
        return out;
         }
      std::ostream& operator<<(std::ostream &out,vector<string> &V ){
        
        for(vector<string>:: iterator ite=V.begin();ite!=V.end();ite++){
            if(strlen((*ite).c_str())==2){out<<*ite<<"       ";}
            else if(strlen((*ite).c_str())==3){out<<*ite<<"   ";}
            else{out<<*ite<<"       ";}
            size_t cur_index = distance(std::begin(V), ite); // get itr index
		    if(((int)cur_index+1)%10==0) out<<endl; //return if (index+1) mode 10 = 0 
        }
        out<<endl;
        out << endl;
          
        return out;
         }
}