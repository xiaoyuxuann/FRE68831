//#include <stdio.h>
#include <string> 
#include <iostream>
#include <sstream>  
//#include <vector>
#include <fstream>
#include <locale>
#include <iomanip>
//#include <algorithm>
#include <cmath>
//#include <map>
#include "Calculation.h"
using namespace std;

namespace final{
 class Bootstrap_model{
   private:
    //int N;
    StockGroup stockgroup;// three groups of stocks
    Bootstrap_model(){}//default constructer
 
    public:
     Bootstrap_model(StockGroup group_):stockgroup(group_){}//constructor with parameters
    
     void get_sample_group(StockGroup & samplegroups); //get one sampling
     vector<Stock> randomStockSelection(vector<Stock> group); //randomly select 80 stocks from each group
     BigMatrix getAbnormalReturn(StockMap &pricemaps); //get abnormal returns for a sampling
     void finish_40_times_bootstramp(StockMap &pricemaps,BigMatrix & arr_matrix,BigMatrix & carr_matrix); //40 samplings
     
~ Bootstrap_model(){}

};

}


