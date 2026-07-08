// #include "Bootstrap.h"
#include "UI.h"

using namespace std;
using namespace final;

int main(){
  vector<Stock> totalStock;
  StockGroup stockgroup;
  
  // stock data processing
  populatedata(totalStock);
  sort_stock_based_on_suprise(totalStock);
  dividestock(totalStock,stockgroup);//stockgroup include three vectors
  
  UI_execute(stockgroup, totalStock);
  
  cout<<"Done"<<endl;
  
  return 0;
}