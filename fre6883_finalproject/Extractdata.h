 #include "curl/curl.h"
 #include <map>
 #include <cstring>
 #include <stdio.h>
#include <string> 
 #include <thread>
#include <time.h>
 #include <locale>
 #include <iomanip>
 #include "DataProcessing.h"
 #include "Operator.h"
 #ifndef Extractdata_H
 #define Extractdata_H

 using namespace std;
 
 
namespace final{
typedef pair<string,double> PAIR;
typedef map<string,Stock*> StockMap;

struct MemoryStruct {
	char* memory;
	size_t size;
    };
  struct threadParams {
    vector<Stock>& stocks_in_group;
    vector<string>& tradingDates;
    vector<PAIR>& IWVData;
    int N;
    int ID;
    StockMap& stockPriceMap;
};
void* myrealloc(void* ptr, size_t size);
int write_data2(void* ptr, size_t size, size_t nmemb, void* data);
string findStartDate(string day0, int N, vector<string> DATE); // find start date of a stock for 2N+1 days
string findEndDate(string day0, int N, vector<string> DATE); // find end date of a stock for 2N+1 days
vector<PAIR> retrieveIWVData(); //retrieve the IWV data with specific time period
vector<double> CalculateDailyReturns(vector<double> &priceSeries);
vector<double> calculateAbnormalPrices(vector<double> &stockPrices, vector<double> &IWVPrices,vector<double> &cumulativereturn);
vector<double> extractIWV(vector<PAIR> &IWVData,string start_date,string end_date); //extract IWV data for 2022
vector<string> getDates(string start, string end, vector<PAIR> &IWVData);
StockMap retrieveStock_IWVData(StockGroup& stockgroup, vector<PAIR> IWVData, int N);
StockMap retrieveStock_Helper(threadParams params);
vector<string> getOutofMapStocks(vector<Stock> &totalStock,StockMap& pricemaps);
 }

#endif
