#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include <ctime>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "stats.h"
#include "KDTree.h"

using namespace std;
using namespace cs221util;


//TEST_CASE("stats::basic rectArea","[weight=1][part=stats]"){


//    PNG data; data.resize(2,2);

//    stats s(data);
//    pair<int,int> ul(0,0);
//    pair<int,int> lr(1,1);
//    long result = s.rectArea(ul,lr);

//    REQUIRE(result == 4);

//}

//TEST_CASE("stats::basic getAvg","[weight=1][part=stats]"){
//    PNG data; data.resize(2,2);
//    for (int i = 0; i < 2; i ++){
//        for (int j = 0; j < 2; j++){
//            HSLAPixel * p = data.getPixel(i,j);
//            p->h = 135*j + i * 90;
//            p->s = 1.0;
//            p->l = 0.5;
//            p->a = 1.0;
//        }
//    }
//    stats s(data);
//    pair<int,int> ul(0,0);
//    pair<int,int> lr(1,1);
//    HSLAPixel result = s.getAvg(ul,lr);
//    HSLAPixel expected(112.5,1.0, 0.5);

//    REQUIRE(result == expected);
//}


//TEST_CASE("stats::basic entropy","[weight=1][part=stats]"){
//    PNG data; data.resize(2,2);
//    for (int i = 0; i < 2; i ++){
//        for (int j = 0; j < 2; j++){
//            HSLAPixel * p = data.getPixel(i,j);
//            p->h = 135*j + i * 90;
//            p->s = 1.0;
//            p->l = 0.5;
//            p->a = 1.0;
//        }
//    }
//    stats s(data);
//    pair<int,int> ul(0,0);
//    pair<int,int> lr(1,1);
//    long result = s.entropy(ul,lr);

//    REQUIRE(result == 2);
//}

//TEST_CASE("KDTree::basic ctor render","[weight=1][part=KDTree]"){
//    PNG img;
//    img.readFromFile("images/geo.png");

//    KDTree t1(img);

//    PNG out = t1.render();
//    out.convert();

//    out.writeToFile("images/geo_out.png");
//    REQUIRE(out==img);
//}


//TEST_CASE("KDTree::basic copy","[weight=1][part=KDTree]"){
//    PNG img;
//    img.readFromFile("images/geo.png");

//    KDTree t1(img);
//    KDTree t1copy(t1);

//    PNG out = t1copy.render();
//    out.convert();

//    REQUIRE(out==img);
//}

//TEST_CASE("KDTree::basic prune 1","[weight=1][part=KDTree]"){
//    time_t tstart, tend;
//    tstart = time(0);

//    PNG img;
//    img.readFromFile("images/Canada-visa-service.png");

//    KDTree t1(img);
//    t1.prune(0.95,0.03);
//    PNG result = t1.render();

//    result.convert();
//    result.writeToFile("images/Result_Canada-visa-service.png");

//    tend = time(0);
//    cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;

//    REQUIRE(1==1);
//}

//TEST_CASE("KDTree::basic prune 2","[weight=1][part=KDTree]"){
//    time_t tstart, tend;
//    tstart = time(0);

//    PNG img;
//    img.readFromFile("images/canadaPlace.png");

//    KDTree t1(img);
//    t1.prune(0.95,0.03);
//    PNG result = t1.render();

//    result.convert();
//    result.writeToFile("images/Result_Travel-to-Shanghai-China-night-river-skyscrapers-tower-lights_iphone_320x480.png");

//    tend = time(0);
//    cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;

//    REQUIRE(1==1);
//}

//TEST_CASE("KDTree::basic prune 3","[weight=1][part=KDTree]"){
//    time_t tstart, tend;
//    tstart = time(0);

//    PNG img;
//    img.readFromFile("images/temple.png");

//    KDTree t1(img);
//    t1.prune(0.95,0.03);
//    PNG result = t1.render();

//    result.convert();
//    result.writeToFile("images/Result_temple.png");

//    tend = time(0);
//    cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;

//    REQUIRE(1==1);
//}

TEST_CASE("KDTree::basic prune 4","[weight=1][part=KDTree]"){
    time_t tstart, tend;
    tstart = time(0);

    PNG img;
    img.readFromFile("images/sakula.png");

    KDTree t1(img);
    t1.prune(0.95,0.03);
    PNG result = t1.render();

    result.convert();
    result.writeToFile("images/Result_sakula.png");

    tend = time(0);
    cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;

    REQUIRE(1==1);
}


