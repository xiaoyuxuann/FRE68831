#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include "Bootstrap.h"
#include "Gnuplot.h"

using namespace std;

namespace final{

// print menu
void print_Options(){
    cout<<endl;
    cout<<endl;
    cout<<"1 - Enter N to retrieve 2N+1 days of historical price data for all stocks "<<endl;
    cout<<"2 - Pull information for one stock from one group "<<endl;
    cout<<"3 - Show AAR, AAR-STD, CAAR and CAAR-STD for one group "<<endl;
    cout<<"4 - Show CAAR for all 3 groups "<<endl;
    // cout<<"5 - Show Std CAAR for all 3 groups "<<endl;
    // cout<<"6 - Show Avg AAR for all 3 groups "<<endl;
    // cout<<"7 - Show Std CAAR for all 3 groups "<<endl;
    cout<<"5 - Exit "<<endl;
}

void option_1( vector<Stock> &totalStock,StockGroup &stockgroup, vector<PAIR> &IWVData,StockMap &pricemaps,BigMatrix &final_matrix){
    // Enetr N
    int N;
    bool nNot60 = true;
    
    while(nNot60){
        cout<<"Enter N (N >= 60 & N<=90): ";
        cin>>N;
        
        // test N >= 60
        if (N < 60){cout << "N must >= 60 and <=90!" << endl;}
        else if (N > 90){ cout << "N must >= 60 and <=90!" << endl;}
        else{ nNot60 = false;}
    }
    
    // Retrieve all stock and IWV data based on N
        //IWV
    cout<<"Retrieve IWV Data....."<<endl;
    IWVData=final::retrieveIWVData();
    vector<PAIR> IWVCopy;
    for (int i=0; i<IWVData.size(); i++) IWVCopy.push_back(IWVData[i]);
        //Stock
    cout<<"Retrieve Stock Daily Data....."<<endl;
    pricemaps=final::retrieveStock_IWVData(stockgroup,IWVCopy,N);
    
    // print 
    vector<string> pricesymbols=getOutofMapStocks(totalStock,pricemaps);
    cout<<endl<<"Warning: The stocks below do not have enough "<<2*N+1<<" days prices !"<<endl;
    
    for(vector<string>::iterator ite=pricesymbols.begin();ite!=pricesymbols.end();ite++){
            cout << *ite << " ";
    }
    cout<<endl;
    
    
    
    // 40 times bootstraping
    Bootstrap_model model(stockgroup);
    BigMatrix aar_matrix(3),caar_matrix(3);
    model.finish_40_times_bootstramp(pricemaps,aar_matrix,caar_matrix);
    final_matrix=cal_final_matrix(aar_matrix,caar_matrix);
    //return pricemaps;
    
    
}

void option_2(StockMap pricemaps, vector<PAIR> IWVData){
    string symbol;
    bool hasSymbol = true;
    
    // test has data for the symbol
    while(hasSymbol){
        cout<<"Enter Symbol for Detailed Information: ";
        cin>>symbol;
        if (pricemaps.find(symbol) != pricemaps.end()) {
            hasSymbol = false;
        }
        else cout << "There is No Data for the Symbol!" << endl;
    }
    
    cout << endl;
    // print stock info
    cout << "Ticker:                " << pricemaps[symbol]->getSymbol() << endl;
    cout << "Earnings Call Date:    " << pricemaps[symbol]->getEarningAnnouncementDate() << endl;
    cout << "Period Ending:         " << pricemaps[symbol]->getPeriodEnding() << endl;
    cout << "Reported EPS:          " << pricemaps[symbol]->getActEPS() << endl;
    cout << "Estimated EPS:         " << pricemaps[symbol]->getEstEPS() << endl;
    cout << "Surprise:              " << pricemaps[symbol]->getSurprise() << endl;
    cout << "Surprise%:             " << pricemaps[symbol]->getSurprisePercentage() << endl;
    cout << endl << "Group:                 " << pricemaps[symbol]->getgroupName() << endl;

    cout << endl;
    
    //Dates
    cout << "Daily Price Dates: " <<endl;
        //Get Dates by IWVData and start/end date of Stock
    vector<string> Dates;
    Dates = getDates(pricemaps[symbol]->getStartDate(), pricemaps[symbol]->getEndDate(), IWVData);
        //Print Dates
    cout<<Dates<<endl;
    
    //Daily Prices
    cout << endl;
    cout << endl;
    cout << "Daily Prices: " << endl;
    vector<double> prices = pricemaps[symbol]->getPrices();

    cout<<prices<<endl;
    cout << "Cumulative Daily Returns: " << endl;
    vector<double> CReturn = pricemaps[symbol]->getCummulativeReturn();
    cout<<CReturn<<endl;
}

void option_3(StockGroup &stockgroup, StockMap &pricemaps,const BigMatrix &final_matrix)
{
    string group;
    bool hasGroup = true;
    map<string, int> groupList = {{"Miss",0}, {"Meet",1}, {"Beat",2}};
    
    // test has group
    while(hasGroup){
        cout<<"Enter Group (Beat, Meet, Miss) for Detailed Information: ";
        cin>>group;
        if(groupList.find(group)!=groupList.end()){
            hasGroup = false;
        }
        else cout << "Try Again..." << endl;
    }
    
    
    
    // Print Avg AAR, AAR STD, Avg CAAR, CAAR STD
    vector<string> calculationType = {"Avg AAR", "AAR STD", "Avg CAAR", "CAAR STD"};
    
    for(int i=0; i<=3; i++){ // 0-3: Avg AAR ~ CAAR STD
        cout<<endl;
        cout<< calculationType[i] << " for Group "<<group<<endl;
        vector<double> result_Vec;
        result_Vec = final_matrix[(int)groupList[group]][i]; //(int)groupList[group] equals (int)groupList["Beat"] return 0
        cout<<result_Vec<<endl;
    }

   // return final_matrix;
}

void option_4(const BigMatrix &final_matrix)
{
    Matrix c(3);//matrix for caar average
    for (int i = 0; i < 3; i++)
    {
        c[i] = (final_matrix[i][2]);
    }
    plotGnuplot(c);
}

void UI_execute(StockGroup stockgroup, vector<Stock> &totalStock){
    string num = "0";
    StockMap pricemaps;
    vector<PAIR> IWVData;
    BigMatrix final_matrix;
    bool getData = false;

    while (num != "5")
    {
        print_Options();
        cout<<endl<<"Enter number and Press Return: ";
        cin>>num;
        
        // 1 - Enter N to retrieve 2N+1 days of historical price data for all stocks
        if(num=="1"){
             option_1(totalStock,stockgroup, IWVData,pricemaps,final_matrix);
             getData = true;
        }
        else if(!getData && num != "5"){
            cout<< "Please Enter 1 to Get Data First!" << endl;
            continue;
        }
        // 2 - Pull information for one stock from one group
        else if (num == "2")
        {
            option_2(pricemaps, IWVData);
            
        }
        // 3 - Show AAR, AAR-STD, CAAR and CAAR-STD for one group
        else if (num == "3")
        {
            option_3(stockgroup, pricemaps,final_matrix);
        }
        // 4 - Show CAAR for all 3 groups
        else if (num == "4")
        {
            option_4(final_matrix);
            usleep(2000000);
        }
        else if (num != "5")
        {
            cout<<endl<< "Please Enter Right Number (1~5)!" << endl;
        }
    }
}

}
