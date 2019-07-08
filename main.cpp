
// File:        main.cpp
// Author:      cheeren
// Date:        2019-03-04
// Description: Partial test of PA3 functionality


#include "cs221util/HSLAPixel.h"
#include "cs221util/PNG.h"
#include "KDTree.h"

using namespace cs221util;
using namespace std;

int main()
{
	// read in an image
	PNG origIm1;
	origIm1.readFromFile("images/colour.png");

	// use it to build a KDtree
	KDTree t1(origIm1);

    // make some copies for pruning tests
	KDTree tCopy1(t1);
	KDTree tCopy2(t1);
	KDTree tCopy3(t1);
	KDTree tCopy4(t1);

	// prune the KDTree prune(p,t);
	//where p% of  subtree pixels are within t of mean

	tCopy1.prune(.99,0.2); 
	tCopy2.prune(.99,.1); 
	tCopy3.prune(.99,.05); 
	tCopy4.prune(.99,.025); 

	// render the KDTree
    PNG ppic1 = t1.render();
    PNG ppiccopy1 = tCopy1.render();
    PNG ppiccopy2 = tCopy2.render();
    PNG ppiccopy3 = tCopy3.render();
    PNG ppiccopy4 = tCopy4.render();

    // write the pngs to files.
	ppic1.writeToFile("images/ouput-colour.png");
	ppiccopy1.writeToFile("images/output-colour.2.png");
	ppiccopy2.writeToFile("images/output-colour.1.png");
	ppiccopy3.writeToFile("images/output-colour.05.png");
	ppiccopy4.writeToFile("images/output-colour.025.png");


  return 0;
}

