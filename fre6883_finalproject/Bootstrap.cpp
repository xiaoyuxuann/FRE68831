#include "Bootstrap.h"
#include "curl/curl.h"

namespace final {
    
    void Bootstrap_model:: get_sample_group(StockGroup & samplegroups) {
        samplegroups.resize(3);
        samplegroups[0]=randomStockSelection(stockgroup[0]);
        samplegroups[1]=randomStockSelection(stockgroup[1]);
        samplegroups[2]=randomStockSelection(stockgroup[2]);
        //samplegroups now include three groups and each group has lenghth=80
    }
   

  BigMatrix Bootstrap_model:: getAbnormalReturn(StockMap &pricemaps) {
         
        StockGroup samplegroups;
        get_sample_group(samplegroups);
        string symbol;

        BigMatrix abnormalreturn;
         abnormalreturn.resize(3);
        
        for(int i=0;i<3;i++){
            for(vector<Stock>::iterator itr= samplegroups[i].begin();itr!=samplegroups[i].end();itr++){
                symbol=itr->getSymbol();
                if (pricemaps.find(symbol) != pricemaps.end()){
                    auto it=pricemaps.find(symbol);
                    abnormalreturn[i].push_back((it->second)->getAbnormalPrices());
                }
            }
        }
        return abnormalreturn;
     }
     
     void Bootstrap_model::finish_40_times_bootstramp(StockMap &pricemaps,BigMatrix & aar_matrix,BigMatrix & caar_matrix){
        BigMatrix abmatrix;
        for(int n=0;n<40;n++){
            Matrix aar(3),caar(3);
            srand(n);
            abmatrix=getAbnormalReturn(pricemaps);
         
            cal_AAR(aar, abmatrix);
            cal_CAAR(caar,aar);
          
            aar_matrix[0].push_back(aar[0]);
            aar_matrix[1].push_back(aar[1]);
            aar_matrix[2].push_back(aar[2]);
            caar_matrix[0].push_back(caar[0]);
            caar_matrix[1].push_back(caar[1]);
            caar_matrix[2].push_back(caar[2]);
  }
     }
     
     
    
    vector<Stock> Bootstrap_model:: randomStockSelection(vector<Stock> group)
        {
        	vector<Stock> randomSampling;
        	//srand((unsigned)time(NULL));
        	int size=(int)group.size();
            int nums[size];
            for(int i=0;i<size;i++){
                nums[i]=i;
            }
            random_shuffle(nums,nums+size);
            int number;
            for(int i=0;i<80;i++){
                number=nums[i];
                randomSampling.push_back(group[number]);
            }
            return randomSampling;
        }
}