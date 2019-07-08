/**
 * Author: Alan Tong
 * Date: June 22. 2019
 */

/**
 *
 * KDTree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * KDTree.cpp
 * This file will be used for grading.
 *
 */

#include "KDTree.h"

KDTree::Node::Node(pair<int,int> ul, pair<int,int> lr, HSLAPixel a)
        :upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
        {}

KDTree::~KDTree(){
        clear();
}

KDTree::KDTree(const KDTree & other) {
        copy(other);
}


KDTree & KDTree::operator=(const KDTree & rhs){
        if (this != &rhs) {
                clear();
                copy(rhs);
        }
        return *this;
}

KDTree::KDTree(PNG & imIn){

    /* YOUR CODE HERE */

    /* To clarify what is asked in the specs, the following needs to be
     * implemented.
     *
     * Root of the tree is a Node for the whole imIn from [0][0] to
     * [imIn.height()-1][imIn.width()-1]
     *
     * Root's children Nodes contain two sub-rectangles from
     * the parent rectangle based on Criteria for spliting
     *
     * Recursively, at the end there will only be single pixels which
     * form leaves of the KDTree
     *
     * Criteria for spliting: two sub-rectangles with min. weighted avg of entropy
     * Node: (pair<int,int> ul, pair<int,int> lr, HSLAPixel a)
     */

    // initialize parameters
    stats stats_imIn = stats(imIn);
    height = imIn.height();
    width = imIn.width();
    pair<int,int> origl;
    pair<int,int> origr;

    origl = std::make_pair(0,0);
    origr = std::make_pair(width-1, height-1);


    root = buildTree(stats_imIn, origl, origr);
    std::cout << std::endl << "END OF CONSTRUCTOR" << std::endl;

}

KDTree::Node * KDTree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {

    /* YOUR CODE HERE */

    /* To build tree we need to call this function recursively,
     *
     *        declare a new node for current rectangle, called n
     *
     * Base Case:       if ul==lr, we fed in 1 pixel
     *                             which means it is a leaf, so return this pixel
     *                             info
     *
     * Inductive Case:  split rectangle, denoted as R1 and R2
     *                  n->left = buildTree(s,R1)
     *                  n->right = buildTree(s,R2)
     *                  return n
     *
     */

    HSLAPixel a = s.getAvg(ul, lr);
    Node* currentNodePt = new Node(ul, lr, a);

    if (ul == lr){
        // base case
        currentNodePt->left = NULL;
        currentNodePt->right = NULL;
        return currentNodePt;

    }else {
        // split the rectangle

        vector<pair<int,int>> resultRect;
        resultRect.resize(4);

        resultRect = splitRectangle(s, ul, lr);

        pair<int,int> ul_R1 = resultRect[0];    // first sub-rectangle
        pair<int,int> lr_R1 = resultRect[1];
        pair<int,int> ul_R2 = resultRect[2];    // second sub-rectangle
        pair<int,int> lr_R2 = resultRect[3];


        currentNodePt->left = buildTree(s, ul_R1, lr_R1);
        currentNodePt->right = buildTree(s, ul_R2, lr_R2);

        return currentNodePt;
    }

}

// helper function
vector<pair<int,int>> KDTree::splitRectangle(stats & s, pair<int,int> ul, pair<int,int> lr) {

    int flag = 0;
    double mean_weightedEntropy = 0;
    vector<pair<int,int>> resultRect;
    resultRect.resize(4);

    int width_Rec = lr.first-ul.first+1;
    int height_Rec = lr.second-ul.second+1;

    pair<int,int> ul_R1;    // first sub-rectangle
    pair<int,int> lr_R1;

    pair<int,int> ul_R2;    // second sub-rectangle
    pair<int,int> lr_R2;

    ul_R1.first = ul.first;
    ul_R1.second = ul.second;

    lr_R2.first = lr.first;
    lr_R2.second = lr.second;


    // Case I: Horizontal Line Split
    if (height_Rec > 1){
        for (int horizLineIdx = 0; horizLineIdx < height_Rec; ++horizLineIdx) {
            lr_R1.first = lr.first;
            lr_R1.second = ul.second+horizLineIdx;

            ul_R2.first = ul.first;
            ul_R2.second = lr_R1.second+1;

//            std::cout << std::endl << "Current Size R1 is: " << s.rectArea(ul_R1,lr_R1) << std::endl;
//            std::cout << std::endl << "Current Size R2 is: " << s.rectArea(ul_R2,lr_R2) << std::endl;
//            std::cout << std::endl << "Current Size Rect is: " << s.rectArea(ul,lr) << std::endl;

//            std::cout << std::endl << "R1_ul: (" << ul_R1.first << "," << ul_R1.second << ")"
//                      << " R1_lr: (" << lr_R1.first << "," << lr_R1.second << ")" << std::endl;
//            std::cout << std::endl << "R2_ul: (" << ul_R2.first << "," << ul_R2.second << ")"
//                      << " R2_lr: (" << lr_R2.first << "," << lr_R2.second << ")" << std::endl;
//            std::cout << std::endl << "Rect_ul: (" << ul.first << "," << ul.second << ")"
//                      << " Rect_lr: (" << lr.first << "," << lr.second << ")" << std::endl;

//            std::cout << std::endl << "Current Entropy Rect is: " << s.entropy(ul,lr) << std::endl;
//            std::cout << std::endl << "Current Entropy R1 is: " << s.entropy(ul_R1,lr_R1) << std::endl;
//            std::cout << std::endl << "Current Entropy R2 is: " << s.entropy(ul_R2,lr_R2) << std::endl;


            // compute mean weighted entropy
            double curr_mean_weightedEntropy = (s.entropy(ul_R1,lr_R1)*s.rectArea(ul_R1,lr_R1)
                                                +s.entropy(ul_R2,lr_R2)*s.rectArea(ul_R2,lr_R2))
                                                /(s.rectArea(ul,lr));
            std::cout << "Current WeightedEntropy is: " << curr_mean_weightedEntropy << std::endl;

            if ((flag == 0) || (curr_mean_weightedEntropy < mean_weightedEntropy)){
                flag = 1;
                mean_weightedEntropy = curr_mean_weightedEntropy;
                resultRect[0] = ul_R1;
                resultRect[1] = lr_R1;
                resultRect[2] = ul_R2;
                resultRect[3] = lr_R2;
            }
        }
    }

    // Case II: Vertical Line Split
    if (width_Rec > 1){
        for (int vertLineIdx = 0; vertLineIdx < width_Rec; ++vertLineIdx) {
            lr_R1.first = ul.first+vertLineIdx;
            lr_R1.second = lr.second;

            ul_R2.first = lr_R1.first+1;
            ul_R2.second = ul.second;

            // compute mean weighted entropy
            double curr_mean_weightedEntropy = (s.entropy(ul_R1,lr_R1)*s.rectArea(ul_R1,lr_R1)
                                                +s.entropy(ul_R2,lr_R2)*s.rectArea(ul_R2,lr_R2))
                                                /(s.rectArea(ul,lr));

            if ((flag == 0) || (curr_mean_weightedEntropy < mean_weightedEntropy)){
                flag = 1;
                mean_weightedEntropy = curr_mean_weightedEntropy;
                resultRect[0] = ul_R1;
                resultRect[1] = lr_R1;
                resultRect[2] = ul_R2;
                resultRect[3] = lr_R2;
            }
        }
    }

    return resultRect;

}


PNG KDTree::render(){

    /* YOUR CODE HERE */

    // fill an empty image with the color stored in root

    // initialize PNG img
    PNG blankImg = PNG(width, height);
    int debug = 0;
    changePixel(blankImg, root, debug);

    return blankImg;
}

// helper function
void KDTree::changePixel(PNG & blankImg, Node* node, int & debug) {
    // traverse the tree in a PreOrder manner

    if (node->left == NULL && node->right == NULL){

    // check the node itself
    pair<int,int> curr_ul = node->upLeft;
    pair<int,int> curr_lr = node->lowRight;
    HSLAPixel avgPixel = node->avg;

    int rowIncrement = curr_lr.second-curr_ul.second;
    int colIncrement = curr_lr.first-curr_ul.first;

    // replace all pixels in blankImg with avgPixel
    for (int colIdx = 0; colIdx <= colIncrement; ++colIdx) {
        for (int rowIdx = 0; rowIdx <= rowIncrement; ++rowIdx) {
            *blankImg.getPixel((curr_ul.first+colIdx),(curr_ul.second+rowIdx)) = avgPixel;
        }
    }
    }

//    string name = "images/test/test_"+std::to_string(debug++)+".png";   // for debugging
//    blankImg.writeToFile(name);
    std::cout << "Currently rendering Node #: " << debug++ << std::endl;

    if (node->left != NULL){
        // check the node left
        changePixel(blankImg, node->left, debug);
    }

    if (node->right != NULL){
        // check the node right
        changePixel(blankImg, node->right, debug);
    }

}

void KDTree::prune(double pct, double tol){

    /* YOUR CODE HERE */
    std::cout << std::endl << "Starting Prune Function" << std::endl;

    Trim(root, pct, tol);

}

// helper function
void KDTree::Trim(Node* subroot, double pct, double tol){

    if (subroot == NULL){
        // do nothing
    }else {
        // check current one and trim is possible
        HSLAPixel ref = subroot->avg;
        int total = 0;
        double trimScore = check2Trim(subroot, tol, ref, total);

        std::cout << std::endl << std::endl;
        std::cout << "Trim Score is: " << trimScore << std::endl;

        if ((trimScore/total) >= pct) {
            clearNode(subroot->left);
            clearNode(subroot->right);

        }else {
            Trim(subroot->left, pct, tol);
            Trim(subroot->right, pct, tol);
        }

    }

}

double KDTree::check2Trim(Node* const subroot, double tol, HSLAPixel ref, int & total){
    // traverse subroot all the way to leaf and check
    // if it meets the criteria to be trimmed
    if (subroot ->left == NULL && subroot ->right == NULL){
        // at a leaf
        total++;
        HSLAPixel currentPixel = subroot->avg;
        if (currentPixel.dist(ref) <= tol){
            return 1;
        }else {
            return 0;
        }
    }else {
        // not a leaf
        return check2Trim(subroot->left, tol, ref, total)
                +check2Trim(subroot->right, tol, ref, total);
    }

}


void KDTree::clear() {

    /* YOUR CODE HERE */
    clearNode(root);
}

// helper function
void KDTree::clearNode(Node* & root2delete){
    if (root2delete == NULL){
        return;
    }else {
        // have to do it LR then root
        // otherwise pointer lost
        clearNode(root2delete->left);
        clearNode(root2delete->right);
        delete root2delete;
        root2delete = NULL;
    }
}

void KDTree::copy(const KDTree & orig){

    /* YOUR CODE HERE */

    // we feed root into copyNode helper function
    // that recursively assign Root, Left, Right
    height = orig.height;
    width = orig.width;
    copyNode(root, orig.root);
    std::cout << "COPY COMPLETE" << std::endl;
}

// helper function
void KDTree::copyNode(Node* &blank, Node* const & orig_root){
    if (orig_root == NULL){
        std::cout << "reached here in NULL" << std::endl;

        // we hit the bottom of the tree
        blank = NULL;

    }else {
        std::cout << "reached here in not NULL" << std::endl;

        // create new Node and copy orig_root to this Node
        blank = new Node(orig_root->upLeft, orig_root->lowRight, orig_root->avg);

        copyNode(blank->left, orig_root->left);
        copyNode(blank->right, orig_root->right);
    }
}


