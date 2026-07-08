#include "DataProcessing.h"

namespace final{
   
 void populatedata(vector<Stock> & stock){

   	const char* Russell3000EarningFile = "Russell3000EarningsAnnouncements.csv";
     ifstream fin;
   	fin.open(Russell3000EarningFile, ios::in);
   	string line, symbol, earningAnnouncementDate, periodEnding, estimated, reportedEarnings, surprise, surprisePercentage, other;
   	getline(fin, line);
   	while (!fin.eof())
   	{
     		getline(fin, line);
     		stringstream sin(line);
     		getline(sin, symbol, ',');
     		getline(sin, earningAnnouncementDate,',');
     		earningAnnouncementDate=formatDate(earningAnnouncementDate);
     		getline(sin, periodEnding,',');
     		getline(sin, estimated,',');
     		getline(sin, reportedEarnings,',');
     		getline(sin, surprise,',');
     		getline(sin, surprisePercentage,',');
     		stringstream ss1(estimated);
     		double d1,d2,d3,d4;
     		
     		ss1 >> d1;
     		stringstream ss2(reportedEarnings);
     		ss2 >> d2;
     		stringstream ss3(surprise);
     		ss3 >> d3;
     		stringstream ss4(surprisePercentage);
     		ss4 >> d4;
     		
     		Stock s(symbol,earningAnnouncementDate,periodEnding,d1,d2,d3,d4);
     		stock.push_back(s);
	}
 }
   void sort_stock_based_on_suprise(vector<Stock> &stock){
           sort(stock.begin(),stock.end(),cmp_val);
    }
   bool cmp_val(Stock &left, Stock &right) {
        	return left.getSurprisePercentage() < right.getSurprisePercentage();
   }

       
 void dividestock(vector<Stock> &stock,StockGroup & stockgroup){
         
        int n=(int)stock.size()/3-1;
         stockgroup.resize(3);
         int i=0;
	       for(;i<n;i++){
    	      stockgroup[0].push_back(stock[i]);
    		     stockgroup[1].push_back(stock[n+i]);
    		    stockgroup[2].push_back(stock[2*n+1+i]);
        	}
      stockgroup[1].push_back(stock[2*n]);
    	 stockgroup[2].push_back(stock[2*n+2+i]);
          
     }
     
 string formatDate(string date)
 {
	   time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo=localtime(&rawtime);
    strptime(date.c_str(),"%d-%b-%g", timeinfo);
    char buf[20];
    strftime(buf,sizeof(buf),"%Y-%m-%d",timeinfo);
    string formattedDate(buf);
    
	   return formattedDate;
   }
}