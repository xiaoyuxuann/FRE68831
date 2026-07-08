#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Operator.h"
using namespace std;
namespace final{
void plotResults(double* xData, double* yData, double* yData2,double* yData3, int dataSize);

void plotGnuplot(Matrix& CAAR_avg) {
    int i;
    int nIntervals = CAAR_avg[0].size()-1;
    int intervalSize = nIntervals;
    int stepSize = intervalSize/nIntervals;
    double* xData = (double*)malloc((nIntervals+1) * sizeof(double));
    double* yData1 = (double*)malloc((nIntervals+1) * sizeof(double));
    double* yData2 = (double*)malloc((nIntervals+1) * sizeof(double));
    double* yData3 = (double*)malloc((nIntervals+1) * sizeof(double));
    xData[0] =(double)-(nIntervals+1)/2 ;
    double x0 = 0.0;
    for (i = 0; i < nIntervals; i++) {
         x0 = xData[i];
        xData[i + 1] = x0 + stepSize;
    }
    for (i = 0; i <= nIntervals; i++) {

        yData1[i] = CAAR_avg[0][i];
    }
    for (i = 0; i <= nIntervals; i++) {

        yData2[i] = CAAR_avg[1][i];
    }
    for (i = 0; i <= nIntervals; i++) {

        yData3[i] = CAAR_avg[2][i];
    }
   
    
    plotResults(xData, yData1, yData2,yData3, nIntervals);
   
}
void plotResults(double* xData, double* yData, double* yData2,double* yData3, int dataSize) {
    FILE *gnuplotPipe,*tempDataFile;
    const char* tempDataFileName1 = "Missgroup";
    const char* tempDataFileName2 = "Meetgroup";
    const char* tempDataFileName3 = "Beatgroup";
    double x1, y1, x2, y2, x3, y3;
    int i;
    const char *title = "CAAR average for three groups";
    const char *yLabel = "Y";
    gnuplotPipe = popen("gnuplot -persist", "w");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set title '%s'\n", title);
    fprintf(gnuplotPipe, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red'\n");
    fprintf(gnuplotPipe, "set xlabel 'Announcement Date'\nset ylabel '%s'\n", yLabel);
    if (gnuplotPipe) {
        fprintf(gnuplotPipe,"plot \"%s\" with lines, \"%s\" with lines,\"%s\" with lines\n",tempDataFileName1, tempDataFileName2,tempDataFileName3);
        fflush(gnuplotPipe);
        tempDataFile = fopen(tempDataFileName1,"w");
        for (i=0; i <= dataSize; i++) {
            x1 = xData[i];
            y1 = yData[i];
            fprintf(tempDataFile,"%lf %lf\n",x1,y1);
        }
        fclose(tempDataFile);
        tempDataFile = fopen(tempDataFileName2,"w");
        for (i=0; i <= dataSize; i++) {
            x2 = xData[i];
            y2 = yData2[i];
            fprintf(tempDataFile,"%lf %lf\n",x2,y2);
        }
        fclose(tempDataFile);
        tempDataFile = fopen(tempDataFileName3,"w");
        for (i=0; i <= dataSize; i++) {
            x3 = xData[i];
            y3 = yData3[i];
            fprintf(tempDataFile,"%lf %lf\n",x3,y3);
        }
        fclose(tempDataFile);
        getchar();
        fprintf(gnuplotPipe,"exit \r");
    } else {        
        printf("gnuplot not found...");    
    }
} 
}