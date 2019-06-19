/*
Programmer: Dmitri Korin, Hanzhang Wang, Bintao Wang
Program:    final_project.cpp ( part 1 )

Purpose:    Generate a 2d Model of a tree given the grammar rule and output and STL file with the coordinates of the tree.

Input:      User inputs file location which points to an in file containing the grammar rule. The file is inputed and the user inputs
            enter to continue growing the tree or presses the q key to quit growing the tree

Output:     The result of each enter key press is a new string representing the tree generation as well as the
            STL file containing the instructions for the tree.

*/
using namespace std;
#include <iostream>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <stack>
#include <time.h>
struct node
{
    int angleN = 0; //when theres a + or - the current rectangles angleNEXT is changed (angle for next rectangle)
    int angle = 90; //actual angle for current rectangle        4----3
    float width = 2;//rectangle width                           |    |
    float height= 10;//rectangle height                         |    |
    float pointOne[2]= {0}; //Bottom left point of rectangle      |    |
    float pointTwo[2]= {2}; //Bottom right point of rectangle     |    |
    float pointThree[2];//top right point of rectangle          |    |
    float pointFour[2];//top left point of rectangle        ex: 1----2
};
class plant
{
private:
    void rotateL(node* cur);//rotates the next rectangle to the left
    void rotateR(node* cur);//rotates the next rectangle to the right
    void adderToStl(float one[],float two[],float three[],float four[]);//converts rect->triangle and adds to STL file
    node *branch(node* cur);//branches and returns the new branch
    struct node* root = new node;//starting branch
    string result = "BE";//result (E is used as the last char in the string to stop whileloop
    void print(node *cur);//sends all four points from a given node to the adderToStl function
    void buildGrammar();
    ofstream onFP;
public:
    void load();
    void run();
    void draw ();
    plant();
    string rule = " ";

};
void plant::adderToStl(float one[],float two[],float three[],float four[])
{
//Purpose: Convert a given rectangle to two triangles and export it to an STL file for printing
//Input: four points representing the corners of a rectangle
//Output: a new facet entry on the STL file
    onFP << "facet normal 0 0 0" << endl;
    onFP << "   outer loop " << endl;
    onFP << "   vertex  " << one[0] << " " << one[1] << " " << "1" << endl;
    onFP << "   vertex  " << two[0] << " " << two[1] << " " << "1" << endl;
    onFP << "   vertex  " << three[0] << " " << three[1] << " " << "1" << endl;
    onFP << "   endloop " << endl;
    onFP << "endfacet" << endl;
    onFP << "facet normal 0 0 0" << endl;
    onFP << "   outer loop " << endl;
    onFP << "   vertex  " << three[0] << " " << three[1] << " " << "1" << endl;
    onFP << "   vertex  " << four[0] << " " << four[1] << " " << "1" << endl;
    onFP << "   vertex  " << one[0] << " " << one[1] << " " << "1" << endl;
    onFP << "   endloop " << endl;
    onFP << "endfacet" << endl;
}
plant::plant()  //Class built for describing pixars tree
{
//Purpose: Constructor, initialize root
//Input: root object
//Output: initialized root
    root->pointOne[0] = 50;//sets up the root node with all points hard-coded
    root->pointOne[1] = 50;
    root->pointTwo[0] = 52;
    root->pointTwo[1] = 50;
    root->pointThree[0] = 52;
    root->pointThree[1] =60;
    root->pointFour[0] = 50;
    root->pointFour[1] = 60;
    onFP.open("tree.stl");
}
void plant::print(node *cur)
{
//Purpose: To call the adderToStl by taking a given node and extracting its points
//Input: pointer to branch node
//Output: Four points from current node that are sent to adderToStl method
    adderToStl(cur->pointOne, cur->pointTwo, cur->pointThree, cur->pointFour);
}
void plant::draw()
{
//Purpose: To translate the given string into a tree
//Input:final result (as a string) of replacements following the grammar rule
//Output: All rectangles full built and added to STL
    int i = 0;
    stack<node*> s;//pointer stack
    node* cur = root;//starts off at the root
    /*print(cur);//prints the root
    cur = cur=branch(cur);
    print(cur);//prints the root*/
    while(result[i]!='E')
    {
        if(result[i]=='B')
        {
            cur=branch(cur);//calls branch function on current branch
            print(cur);
        }
        else if(result[i]=='+')
        {
            rotateR(cur);//rotate future branch right
        }
        else if(result[i]=='-')
        {
            rotateL(cur);//rotate future branch left
        }
        else if(result[i]=='[')
        {
            s.push(cur);
        }
        else if(result[i]==']')
        {
            cur=s.top();
            s.pop();
            cur->angleN=0;
        }
        i++;
    }
    onFP.close();
}
void plant::run()
{
//Purpose: To generate the string that will build our tree
//Input: Grammar rule
//Output: Resulting string that will be used to generate the string
    for(int i = 0; i<result.length(); i++)
    {
        if(result[i]=='B')
        {
            result.insert(i+1, rule);
            result.erase(i,1);
            i += rule.length()-1;
        }
    }
    cout<<"|||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
    cout<<result<<endl;
    cout<<"|||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
}
void plant::load()
{
//Purpose: Store grammar rule from user inputed file
//Input: Grammar file name as well as the actual file containing the rule
//Output: rule string initialized with rule from file
    char filename[50];
    ifstream inFP;
    cout << "Grammar file location: ";
    cin.getline(filename, 50);
    inFP.open(filename);
    if(!inFP.is_open())
    {
        exit(EXIT_FAILURE);
    }
    getline(inFP, rule);
    // Truncate the B-> at front of the rule.
    rule = rule.substr(3, rule.length());
    inFP.close();

}
void plant::rotateL(node* cur)
{
//Purpose: Make rectangle rotate anti-clockwise
    srand (time(NULL));
    cur->angleN+= -1*(rand()%40+20);

}
void plant::rotateR(node* cur)
{
//Purpose: Make rectangle rotate clockwise
    cur->angleN+=rand()%40+20;
}
node *plant::branch(node* cur)
{
//Purpose: Creates a new branch based on the angleN
//Input: the previous branch that we are branching off of
//Output: Pointer to the new branch
    node* newB = new node;
    float conv = 3.14159265/180;
    if(cur->angleN<0)  //left branch
    {
        newB->width=(cur->width)*0.95;//sets the width of newB to a smaller width than its root thus the branches get thinner
        newB->height=(cur->height)*0.95;//sets the height of newB to a smaller height than its root thus the branches get shorter
        float twoX = cur->pointThree[0];//Sets rotation axis point (x-value) to the same point as point 4 in the previous branch
        float twoY = cur->pointThree[1];//Sets rotation axis point (y-value) to the same point as point 4 in the previous branch
        float oneY = twoY;//on an unrotated rectangle points one and two have the same y value
        float oneX = twoX-(newB->width);//point two has the same x value as point one but shifted right by the width
        float threeX=twoX;//point two and three share the same x values
        float threeY=twoY+(newB->height);//point three has the same Y value as point 2 but shifted up by the height
        float fourX= oneX;//point 4 and 1 have the same x -value
        float fourY= threeY;//point four and three have the same y value
        newB->pointTwo[0]=twoX;//since this is the axis of rotation the point will not be rotated
        newB->pointTwo[1]=twoY;//GOUSIE HELP QUESTION:I hard coded all the points to the ones generated before but the rectangle
        //just isn't touching the previous rectangle which make me wonder how
        cout<<"angle is: " <<cur->angle-cur->angleN-90<<endl;
        newB->pointOne[0] = ((oneX-twoX)*cos((-1)*(cur->angle-cur->angleN-90)*conv)+(oneY-twoY)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+twoX;
        newB->pointOne[1] = ((oneY-twoY)*cos((-1)*(cur->angle-cur->angleN-90)*conv)-(oneX-twoX)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+twoY;
        newB->pointThree[0]=((threeX-twoX)*cos((-1)*(cur->angle-cur->angleN-90)*conv)+(threeY-twoY)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+twoX;
        newB->pointThree[1]=((threeY-twoY)*cos((-1)*(cur->angle-cur->angleN-90)*conv)-(threeX-twoX)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+twoY;
        newB->pointFour[0] =((fourX-twoX)*cos((-1)*(cur->angle-cur->angleN-90)*conv)+(fourY-twoY)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+twoX;
        newB->pointFour[1] =((fourY-twoY)*cos((-1)*(cur->angle-cur->angleN-90)*conv)-(fourX-twoX)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+twoY;
        newB->angle=cur->angle-cur->angleN;
        newB->angleN=0;
    }
    else   //right branch: WE FIRST DRAW A NON ROTATED RECTANGLE STARTING WITH THE BOTTOM LEFT POINT (AXIS OF ROTATION)
    {
        cout << "<<<" << endl;
        newB->width=(cur->width)*0.95;//sets the width of newB to a smaller width than its root thus the branches get thinner
        newB->height=(cur->height)*0.95;//sets the height of newB to a smaller height than its root thus the branches get shorter
        float oneX = cur->pointFour[0];//Sets rotation axis point (x-value) to the same point as point 4 in the previous branch
        float oneY = cur->pointFour[1];//Sets rotation axis point (y-value) to the same point as point 4 in the previous branch
        float twoY = oneY;//on an unrotated rectangle points one and two have the same y value
        float twoX = oneX+(newB->width);//point two has the same x value as point one but shifted right by the width
        float threeX=twoX;//point two and three share the same x values
        float threeY=twoY+(newB->height);//point three has the same Y value as point 2 but shifted up by the height
        float fourX= oneX;//point 4 and 1 have the same x -value
        float fourY= threeY;//point four and three have the same y value
        newB->pointOne[0]=oneX;//since this is the axis of rotation the point will not be rotated
        newB->pointOne[1]=oneY;//GOUSIE HELP QUESTION:I hard coded all the points to the ones generated before but the rectangle
        //just isn't touching the previous rectangle which make me wonder how
        newB->pointTwo[0] = ((twoX-oneX)*cos((-1)*(cur->angle-cur->angleN-90)*conv)+(twoY-oneY)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+oneX;
        newB->pointTwo[1] = ((twoY-oneY)*cos((-1)*(cur->angle-cur->angleN-90)*conv)-(twoX-oneX)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+oneY;
        newB->pointThree[0]=((threeX-oneX)*cos((-1)*(cur->angle-cur->angleN-90)*conv)+(threeY-oneY)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+oneX;
        newB->pointThree[1]=((threeY-oneY)*cos((-1)*(cur->angle-cur->angleN-90)*conv)-(threeX-oneX)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+oneY;
        newB->pointFour[0] =((fourX-oneX)*cos((-1)*(cur->angle-cur->angleN-90)*conv)+(fourY-oneY)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+oneX;
        newB->pointFour[1] =((fourY-oneY)*cos((-1)*(cur->angle-cur->angleN-90)*conv)-(fourX-oneX)*sin((-1)*(cur->angle-cur->angleN-90)*conv))+oneY;
        //cur->angleN=0;
        newB->angle=cur->angle-cur->angleN;
        newB->angleN=0;
    }
    return newB;
}
int main ()
{
    plant myPlant;
    srand (time(NULL));
    myPlant.load();
    cout<<"The production rule is B->"<<myPlant.rule<<endl;
W://Documents//School//input.in
    cout<<"Press enter to continoue building the plant's string, or press q to quit";
    char result;
    result=cin.get();
    while(result != 'q')                //Once you enter 'q', the loop should be ended
    {
        if(result=='\n')
        {
            myPlant.run();
        }
        else
        {
            cout<<"invalid input entered, please enter either q or enter"<<endl;
        }
        result=cin.get();
    }
    myPlant.draw();
    return 0;
}
