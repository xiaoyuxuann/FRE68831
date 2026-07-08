#include "Stock.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>  
#include <algorithm>

namespace final{
typedef vector<vector<Stock>> StockGroup;

void populatedata(vector<Stock> &totalStock); //parse EPS information to stocks
void sort_stock_based_on_suprise(vector<Stock> &totalStock); //sort stocks by surprisePercentage
void dividestock(vector<Stock> &totalStock,StockGroup &stockgroup); //divide stocks into three groups
string formatDate(string date);
bool cmp_val(Stock &left, Stock &right); //compare surprisePercentage

}