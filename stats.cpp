/**
 * Author: Alan Tong
 * Date: June 22. 2019
 */
#include "stats.h"

stats::stats(PNG & im){

    /* YOUR CODE HERE */

    // To find the cumulative color info, we run through the whole
    // image matrix and compute each pixel value individually

    // resizing vectors
    hist.resize(im.width());
    sumHueX.resize(im.width());
    sumHueY.resize(im.width());
    sumSat.resize(im.width());
    sumLum.resize(im.width());


    for (int rowIdx = 0; rowIdx < im.width(); ++rowIdx) {

        // resizing vectors
        hist[rowIdx].resize(im.height());
        sumHueX[rowIdx].resize(im.height());
        sumHueY[rowIdx].resize(im.height());
        sumSat[rowIdx].resize(im.height());
        sumLum[rowIdx].resize(im.height());

        for (int colIdx = 0; colIdx < im.height(); ++colIdx) {


            // initialization
            double currentsumHueX = 0;
            double currentsumHueY = 0;
            double currentsumSat = 0;
            double currentsumLum = 0;
            hist[rowIdx][colIdx].resize(36);

            for (int histIdx = 0; histIdx < 36; ++histIdx) {
                hist[rowIdx][colIdx][histIdx] = 0;
            }


            // compute cumulative values in sub-rectangle
            HSLAPixel currentPixel = *im.getPixel(rowIdx, colIdx);

            if (rowIdx == 0 && colIdx == 0){
                // at (0,0)
                currentsumHueX = cos(currentPixel.h*PI/180.0);
                currentsumHueY = sin(currentPixel.h*PI/180.0);
                currentsumSat = currentPixel.s;
                currentsumLum = currentPixel.l;

                int current_histIdx = currentPixel.h/10;
                hist[rowIdx][colIdx][current_histIdx]++;

            }else if (rowIdx == 0 && colIdx != 0){
                // on the left edge
                // look one unit above
                currentsumHueX = cos(currentPixel.h*PI/180.0)+sumHueX[rowIdx][colIdx-1];
                currentsumHueY = sin(currentPixel.h*PI/180.0)+sumHueY[rowIdx][colIdx-1];
                currentsumSat = currentPixel.s+sumSat[rowIdx][colIdx-1];
                currentsumLum = currentPixel.l+sumLum[rowIdx][colIdx-1];

                hist[rowIdx][colIdx] = hist[rowIdx][colIdx-1];
                int current_histIdx = currentPixel.h/10;
                hist[rowIdx][colIdx][current_histIdx]++;

            }else if (rowIdx != 0 && colIdx == 0){
                // on the top edge
                // look one unit left
                currentsumHueX = cos(currentPixel.h*PI/180.0)+sumHueX[rowIdx-1][colIdx];
                currentsumHueY = sin(currentPixel.h*PI/180.0)+sumHueY[rowIdx-1][colIdx];
                currentsumSat = currentPixel.s+sumSat[rowIdx-1][colIdx];
                currentsumLum = currentPixel.l+sumLum[rowIdx-1][colIdx];

                hist[rowIdx][colIdx] = hist[rowIdx-1][colIdx];
                int current_histIdx = currentPixel.h/10;
                hist[rowIdx][colIdx][current_histIdx]++;


            }else{
                // inside
                // +top + left - (-1,-1)
                currentsumHueX = cos(currentPixel.h*PI/180.0)+sumHueX[rowIdx-1][colIdx]
                                +sumHueX[rowIdx][colIdx-1]-sumHueX[rowIdx-1][colIdx-1];
                currentsumHueY = sin(currentPixel.h*PI/180.0)+sumHueY[rowIdx-1][colIdx]
                                +sumHueY[rowIdx][colIdx-1]-sumHueY[rowIdx-1][colIdx-1];
                currentsumSat = currentPixel.s+sumSat[rowIdx-1][colIdx]
                                +sumSat[rowIdx][colIdx-1]
                                -sumSat[rowIdx-1][colIdx-1];
                currentsumLum = currentPixel.l+sumLum[rowIdx-1][colIdx]
                                +sumLum[rowIdx][colIdx-1]
                                -sumLum[rowIdx-1][colIdx-1];

                hist[rowIdx][colIdx] = arraySum(hist[rowIdx-1][colIdx], hist[rowIdx][colIdx-1]);
                hist[rowIdx][colIdx] = arraySubtract(hist[rowIdx][colIdx], hist[rowIdx-1][colIdx-1]);

                int current_histIdx = currentPixel.h/10;
                hist[rowIdx][colIdx][current_histIdx]++;

            }

//            for (int rec_rowIdx = 0; rec_rowIdx <= rowIdx; ++rec_rowIdx) {
//                for (int rec_colIdx = 0; rec_colIdx <= colIdx; ++rec_colIdx) {

//                    HSLAPixel currentPixel = *im.getPixel(rec_rowIdx, rec_colIdx);

//                    currentsumHueX = currentsumHueX+(cos(currentPixel.h*PI/180.0));
//                    currentsumHueY = currentsumHueY+(sin(currentPixel.h*PI/180.0));

//                    currentsumSat = currentsumSat+(currentPixel.s);
//                    currentsumLum = currentsumLum+(currentPixel.l);

//                    int current_histIdx = currentPixel.h/10;
//                    hist[rowIdx][colIdx][current_histIdx]++;

//                }
//            }
//            std::cout << "currentsumHue: " << (atan2(currentsumHueY,currentsumHueX)*180.0)/PI << std::endl;
//            std::cout << "currentsumSat: " << currentsumSat << std::endl;

            // return values to corresponding positions in the 2d array
            sumHueX[rowIdx][colIdx] = currentsumHueX;
            sumHueY[rowIdx][colIdx] = currentsumHueY;
            sumSat[rowIdx][colIdx] = currentsumSat;
            sumLum[rowIdx][colIdx] = currentsumLum;

        }
    }
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

    /* YOUR CODE HERE */

    // number of pixels in the bounded rectangle can be found
    // using formula (|x1-x2|+1)*(|y1-y2|+1)

    return (abs(ul.first-lr.first)+1)*(abs(ul.second-lr.second)+1);

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

    /* YOUR CODE HERE */

    // average color info of a rectangle of pixels can be computed
    // by looking up private 2d vectors. First find lr color info,
    // since the data is accumulated values, we denote this data
    // as T. Then compute T-[lr(x)][ul(y)-1]-[ul(x)-1][lr(y)]+
    // [ul(x)-1][ul(y)-1], as the sum. Finally devide rectArea
    // handle rest of edge cases

    double currentSum_h;
    double currentSum_s;
    double currentSum_l;
    double current_a = 1.0;

    double currentSumY;
    double currentSumX;
    double currentSumS;
    double currentSumL;

    double numPixels = rectArea(ul, lr);

    if ((ul.first == 0) && (ul.second == 0)){
        // starting from [0][0]
        currentSumY = sumHueY[lr.first][lr.second];
        currentSumX = sumHueX[lr.first][lr.second];
        currentSumS = sumSat[lr.first][lr.second];
        currentSumL = sumLum[lr.first][lr.second];

    }else if ((ul.first == 0) && (ul.second != 0)){
        // rect lays on left edge
        currentSumY = sumHueY[lr.first][lr.second]-sumHueY[lr.first][ul.second-1];
        currentSumX = sumHueX[lr.first][lr.second]-sumHueX[lr.first][ul.second-1];
        currentSumS = sumSat[lr.first][lr.second]-sumSat[lr.first][ul.second-1];
        currentSumL = sumLum[lr.first][lr.second]-sumLum[lr.first][ul.second-1];

    }else if ((ul.first != 0) && (ul.second == 0)){
        // rect lays on top edge
        currentSumY = sumHueY[lr.first][lr.second]-sumHueY[ul.first-1][lr.second];
        currentSumX = sumHueX[lr.first][lr.second]-sumHueX[ul.first-1][lr.second];
        currentSumS = sumSat[lr.first][lr.second]-sumSat[ul.first-1][lr.second];
        currentSumL = sumLum[lr.first][lr.second]-sumLum[ul.first-1][lr.second];

    }else{
        // rect lays inside with no touching on left or top edges
        currentSumY = sumHueY[lr.first][lr.second]
                -sumHueY[ul.first-1][lr.second]-sumHueY[lr.first][ul.second-1]
                +sumHueY[ul.first-1][ul.second-1];
        currentSumX = sumHueX[lr.first][lr.second]
                -sumHueX[ul.first-1][lr.second]-sumHueX[lr.first][ul.second-1]
                +sumHueX[ul.first-1][ul.second-1];
        currentSumS = sumSat[lr.first][lr.second]
                -sumSat[ul.first-1][lr.second]-sumSat[lr.first][ul.second-1]
                +sumSat[ul.first-1][ul.second-1];
        currentSumL = sumLum[lr.first][lr.second]
                -sumLum[ul.first-1][lr.second]-sumLum[lr.first][ul.second-1]
                +sumLum[ul.first-1][ul.second-1];

    }

    currentSum_h = atan2(currentSumY/numPixels, currentSumX/numPixels)*180/PI;
    if (currentSum_h < 0){
        currentSum_h += 360;
    }
    currentSum_s = currentSumS;
    currentSum_l = currentSumL;

    HSLAPixel avgPixel = HSLAPixel(currentSum_h,
                                   currentSum_s/numPixels,
                                   currentSumL/numPixels,
                                   current_a);

    return avgPixel;


}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

    /* YOUR CODE HERE */
    // similar to the implementation we used for getAvg,
    // just need to do vector subtraction for the hist axis

    vector<int> resultHist;

    if ((ul.first == 0) && (ul.second == 0)){
        // starting from [0][0]
        resultHist = hist[lr.first][lr.second];

        return resultHist;

    }else if ((ul.first == 0) && (ul.second != 0)){
        // rect lays on left edge
        resultHist = arraySubtract(hist[lr.first][lr.second], hist[lr.first][ul.second-1]);

        return resultHist;

    }else if ((ul.first != 0) && (ul.second == 0)){
        // rect lays on top edge
        resultHist = arraySubtract(hist[lr.first][lr.second], hist[ul.first-1][lr.second]);

        return resultHist;

    }else{
        // rect lays inside with no touching on left or top edges
        resultHist = arraySubtract(hist[lr.first][lr.second], hist[ul.first-1][lr.second]);
        resultHist = arraySubtract(resultHist, hist[lr.first][ul.second-1]);
        resultHist = arraySum(resultHist, hist[ul.first-1][ul.second-1]);

        return resultHist;
    }

}

// takes a distribution and returns entropy
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0;
    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 )
            entropy += ((double) distn[i]/(double) area)
                                    * log2((double) distn[i]/(double) area);
    }
    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    vector<int> distn =  buildHist(ul,lr);
    int area = rectArea(ul,lr);
    return  entropy(distn,area);

}


// helper functions
vector<int> stats::arraySubtract(vector<int> v1, vector<int> v2){
    vector<int> result;
    result.resize(v1.size());       // always resize array in cpp
    for (int var = 0; var < v1.size(); ++var) {
        result[var] = v1[var]-v2[var];
    }
    return result;
}

vector<int> stats::arraySum(vector<int> v1, vector<int> v2){
    vector<int> result;
    result.resize(v1.size());       // always resize array in cpp
    for (int var = 0; var < v1.size(); ++var) {
        result[var] = v1[var]+v2[var];
    }
    return result;
}

void stats::arrayPrint(vector<int> v1){
    for (int var = 0; var < v1.size(); ++var) {
        std::cout << v1[var] << " ";
    }
}
