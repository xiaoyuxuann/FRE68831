#include "Extractdata.h"
#include <unistd.h>
#include <mutex>

namespace final{
    void* myrealloc(void* ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}
int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;
	mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string findStartDate(string day0, int N, vector<string> DATE) //day0 is the earning announcement date of each stock 
{ 
	auto it=find(DATE.begin(),DATE.end(),day0);
	it-=N;
    return *it;
}

string findEndDate(string day0, int N, vector<string> DATE) //day0 is the earning announcement date of each stock 
{
	string end_date="";
	int n=-1;
	for(vector<string>:: iterator ite=DATE.begin();ite!=DATE.end();ite++)
	{
		if(n>=0)
	    {
			n+=1;
		}
		if(n==N)
		{
			end_date=*ite;
			break;
		}
		if (*ite==day0)
		{
			n+=1;
		}
	}
    return end_date;
}


vector<double> extractIWV(vector<PAIR> &IWVData,string start_date,string end_date)
{
	vector<double> IWV;
	int i=-1;
	for(vector<PAIR>:: iterator ite=IWVData.begin();ite!=IWVData.end();ite++)
	{
		if(i==0)
		{
			IWV.push_back(ite->second);
			if(ite->first==end_date)
			{
				break;
			}
		}
		else
		{
			if(ite->first==start_date)
			{
				IWV.push_back(ite->second);
				i++;
			}
		}
	}
	return IWV;
}
// Get dates (similar to extractIWV function in Extractdata.cpp)
vector<string> getDates(string start, string end, vector<PAIR> &IWVData){
    vector<string> IWV;
    int i=-1;
	for(vector<PAIR>:: iterator ite=IWVData.begin();ite!=IWVData.end();ite++){
		if(i==0)
		{
			IWV.push_back(ite->first);
			if(ite->first==end)
			{
				break;
			}
		}
		else
		{
			if(ite->first==start)
			{
				IWV.push_back(ite->first);
				i++;
			}
		}
	    
	}
	return IWV;
}


vector<PAIR> retrieveIWVData()
{
    vector<PAIR> IWVData;
	// declaration of an object CURL 
	CURL* handle;

	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();

	// if everything's all right with the easy handle... 
	if (handle)
	{
		string url_common = "https://eodhistoricaldata.com/api/eod/";
		string start_date = "2022-01-03";
		string end_date = "2022-12-02";
		string api_token = "637e6a8c585406.82227762";  // You must replace this API token with yours

		struct MemoryStruct data;
		data.memory = NULL;
		data.size = 0;

  string symbol="IWV";
		string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
		curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

		//adding a user agent
		curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
		//curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

		// perform, then store the expected code in result
		curl_easy_perform(handle);

		stringstream sData;
		sData.str(data.memory);
		string sDate, sPrice;
		double dPrice=0;
		string line;
		while (getline(sData, line)) {
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				sDate = line.substr(0, line.find_first_of(','));
				line.erase(line.find_last_of(','));
				sPrice = line.substr(line.find_last_of(',') + 1);
				dPrice = strtod(sPrice.c_str(), NULL);
				IWVData.push_back(PAIR(sDate,dPrice));
			}
		}
		free(data.memory);
		data.size = 0;
	}
	curl_easy_cleanup(handle);
	curl_global_cleanup();

	return IWVData;
}
vector<double> CalculateDailyReturns(vector<double> &priceSeries){  
    vector<double> dailyReturns;
    double returns;
    int n=priceSeries.size();
    for (int i=0; i < n-1; i++){
        returns = (priceSeries[i+1] - priceSeries[i])/ priceSeries[i];
        dailyReturns.push_back(returns);
    }
    return dailyReturns;
}

vector<double> calculateAbnormalPrices(vector<double> &stockPrices, vector<double> &IWVPrices,vector<double> &cumulativereturn)
{
	vector<double> stockDailyReturns=CalculateDailyReturns(stockPrices);
	
	int size=(int)stockDailyReturns.size();
	cumulativereturn.resize(size);
	cumulativereturn[0]=stockDailyReturns[0];
	for(int i=1;i<size;i++){
		 
		cumulativereturn[i]=((stockDailyReturns[i]+1)*(stockDailyReturns[i-1]+1)-1);
	}
	vector<double> IWVDailyReturns=CalculateDailyReturns(IWVPrices);
	vector<double> abnormalPrices;
    abnormalPrices=stockDailyReturns-IWVDailyReturns;
	return abnormalPrices;
}
StockMap retrieveStock_IWVData(StockGroup& stockgroup, vector<PAIR> IWVData, int N)
{

	vector<string> tradingDates;
	for(vector<PAIR>:: iterator ite=IWVData.begin();ite!=IWVData.end();ite++)
	{  
		tradingDates.push_back(ite->first);
	}
	

    StockMap sm1, sm2, sm3;
    int id1=1,id2=2,id3=3;
    threadParams params1 = {
            stockgroup[0],
            tradingDates,
            IWVData,
            N,
            id1,
            sm1,
    };

    threadParams params2 = {
            stockgroup[1],
            tradingDates,
            IWVData,
            N,
            id2,
            sm2,
    };

    threadParams params3 = {
            stockgroup[2],
            tradingDates,
            IWVData,
            N,
            id3,
            sm3,
    };

	cout << endl;
	thread t1(retrieveStock_Helper, params1);
    //cout << "Thread 1 dispatched" << endl;
    thread t2(retrieveStock_Helper, params2);
    //cout << "Thread 2 dispatched" << endl;
    thread t3(retrieveStock_Helper, params3);
    //cout << "Thread 3 dispatched" << endl;

	cout << endl;
    t1.join();
    //cout << "Thread 1 joined" << endl;
    t2.join();
    //cout << "Thread 2 joined" << endl;
    t3.join();
    //cout << "Thread 3 joined" << endl;

	double progress = 100.0;
	string progress_Str = "[" + string((int)progress, '*') + string(100 - (int)progress, ' ') + "]";
	cout << fixed << showpoint;
	cout << setprecision(3);
	cout << "\r\033[F"<< "progress: [" << progress << "%]" << progress_Str << endl;

	sm1.insert(sm2.begin(), sm2.end());
    sm1.insert(sm3.begin(), sm3.end());
    


	return sm1;
}

// print progress by Tsai
void printProgress(size_t cur_index, size_t last_index, vector<Stock> stocks_in_group)
{
	static std::mutex m;
	const std::lock_guard<std::mutex> lock(m);
	
	cout << fixed << showpoint;
	cout << setprecision(3);

	//if((int)cur_index==1) cout << "\r\033[F";

	//calculate progress
	double progress = 100 * (double)cur_index / (double)last_index;
	// print progress
	if (progress != 0.0){
		string progress_Str = "[" + string((int)progress, '*') + string(100 - (int)progress, ' ') + "]";
		cout << "\r\033[F"<< "progress: [" << progress << "%]" << progress_Str << endl;
	}
		
	// usleep(100000);
}

StockMap retrieveStock_Helper(threadParams params) {
	
    vector<Stock>& stocks_in_group = params.stocks_in_group;
    
    vector<string>&tradingDates = params.tradingDates;
    
    vector<PAIR>& IWVData = params.IWVData;
    int N = params.N;
    int id=params.ID;
    StockMap& stockPriceMap = params.stockPriceMap;
	//StockMap stockPriceMap;
	string symbol;
	string day0;
	string start_date;
	string end_date;
	vector<double> IWVPrices;
	//int n=0;
	//int m=0;
	
	
	CURL* handle;
		
	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);
	
	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();
	// if everything's all right with the easy handle... 
	if (handle){
	
	 string url_common = "https://eodhistoricaldata.com/api/eod/";
	 string api_token = "63798a9e62a577.42161245";  // You must replace this API token with yours
	 
	for(vector<Stock>:: iterator ite=stocks_in_group.begin();ite!=stocks_in_group.end();ite++)
	{   
		// print progress by Tsai
			// convert itr to index
		size_t cur_index = distance(std::begin(stocks_in_group), ite);
		size_t last_index = distance(std::begin(stocks_in_group), std::end(stocks_in_group));
		printProgress(cur_index, last_index, stocks_in_group);

		// retrieve stock data    	
		vector<double> stockPrices;
		vector<double> IWVPrices;
		symbol=ite->getSymbol();
		day0=ite->getEarningAnnouncementDate();
		start_date=findStartDate(day0,N,tradingDates);
		end_date=findEndDate(day0,N,tradingDates);
		//cout<<	start_date<<end_date<<endl;
		//n+=1;
		//cout<<symbol<<" "<<n<<endl;
		if(end_date=="")
		{
			// cout<<"not enough data for "<<2*N+1<<" days"<<endl;
			//m+=1;
			continue;
		}
		
		
		ite->setStartDate(start_date);
		ite->setEndDate(end_date);
		IWVPrices=extractIWV(IWVData,start_date,end_date);
		
	
			struct MemoryStruct data;
			data.memory = NULL;
			data.size = 0;
	
			string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
			curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
	
			//adding a user agent
			curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
		
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
			// perform
			curl_easy_perform(handle);
			
			stringstream sData;
			sData.str(data.memory);
			string sDate, sPrice;
			double dPrice;
			string line;
			while (getline(sData, line)) {
				size_t found = line.find('-');
				if (found != std::string::npos)
				{
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
					sPrice = line.substr(line.find_last_of(',') + 1);
					dPrice = strtod(sPrice.c_str(), NULL);
					stockPrices.push_back(dPrice);
				}
			}
			ite->setgroupID(id);
			ite->setPrices(stockPrices);
			vector<double> cumulativereturn;
			//cout<<"{{{{{{{{{{{{{{{{{{{"<<ite->getPrices().size()<<endl;
			ite->setAbnormalPrices(calculateAbnormalPrices(stockPrices,IWVPrices,cumulativereturn));
			ite->setCummulativeReturn(cumulativereturn);
			
			//vector<double> v=ite->getAbnormalPrices();
			//cout<<v[0]<<endl;
			stockPriceMap.insert({symbol,&(*ite)});
			
			free(data.memory);
			data.size = 0;
		}	
	
	}
	//cout<<m<<endl;
	curl_easy_cleanup(handle);
	curl_global_cleanup();
	return stockPriceMap;
}


vector<string> getOutofMapStocks(vector<Stock> &totalStock,StockMap& pricemaps){
	vector<string> stocksymbols;
	string symbol;
	 for(vector<Stock>::iterator itr= totalStock.begin();itr!=totalStock.end();itr++){
         symbol=itr->getSymbol();
         if (pricemaps.find(symbol) == pricemaps.end()){
              stocksymbols.push_back(symbol);
           }
      }
   return stocksymbols;
}
}