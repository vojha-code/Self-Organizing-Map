//The Self Organizing Map
//Author: Varun Kumar Ojha, Prof. Vaclav Snasel, VSB - TUO
#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

//The global variables 
const int maxHeight = 10;//Maximum Allowable Hight of a MAP
const int maxWidth = 10;//Maximum Allowable width of a MAP
const int maxFeature = 100;//Maximum Features a MAP accept
const int maxExample = 100;//Maximum number of patterns (example) used for training
int MAX_ITERATION;//Maximum Iteration of training required

//const double decayRate = 0.96; //About 100 iterations.
const double minAlpha = 0.01;//minimum learning rate at any point of time
//const double radiusReductionPoint = 0.023;//Last 20% of iterations.
double alpha = 1.0;//learning rate to start a training


//A class containing information of SOM
class SOM
{
	int Height;//User Input height of MAP
	int Width;//User Input width of MAP
	int Feature;//User Input feature of MAP
	int Example;//User Input examples of MAP
public:
	SOM();
	~SOM();
	void setHeight(int);//Set MAP height
	int getHeight();//return MAP height
	void setWidth(int);//Set MAP width
	int getWidth();//return MAP width
	void setFeature(int);//Set MAP feature
	int getFeature();//return MAP feature
	void setExample(int);//Set MAP example
	int getExample();//return MAP example
private:
};
SOM::SOM(){}
SOM::~SOM(){}
void SOM::setHeight(int n){Height = n;}
int SOM::getHeight(){return Height;}
void SOM::setWidth(int m){Width = m;}
int SOM::getWidth(){return Width;}
void SOM::setFeature(int k){Feature = k;}
int SOM::getFeature(){	return Feature;}
void SOM::setExample(int k){Example = k;}
int SOM::getExample(){	return Example;}

//A class containing input Patterns
class PATTERN
{
public:
	double Input[maxFeature];//n elt. input vector
private:
};

//A class containing catagories i.e. SOM MAP
class MAP
{
 public:
	double Node[maxFeature];//A node of a MAP is vector n elt. vector = |Input Vector|
private:
};

//The prototypes of the functions 
void initSOM();//Initializing SOM
//Function inintializing MAP with random weight vectors
void initializeMAP(SOM som, MAP  map[maxHeight][maxWidth]);
//The procedure training of the SOM 
void training(SOM som,MAP map[maxHeight][maxWidth],PATTERN pat[maxExample]);
//Update MAP or clusters weight
void updateMAPweight(int indexROW,int indexCOL,int R,double eta, SOM som, MAP map[maxHeight][maxWidth], double I[maxFeature]);

// Function returning float value
double fRand(double fMin, double fMax);

//Function displaying the MAP values
void displayMAP(SOM som,MAP  map[maxHeight][maxWidth]);
//computing eucliean distance between map's Node and Feature vecrtor
double euclidean_dist2(SOM som, double F[maxFeature], double W[maxFeature]);

//Main
int main()
{
	cout<<"Start";
	initSOM();//Initialize SOM SOM
	cout<<"\nEnd";

	return 0;
}//End Main

//Initializing SOM
void initSOM()
{
	cout<<"\nInitilizing SOM";
	SOM som;

	int h, w, atr, ex;
	cout<<"\nInput Height of the MAP:";
	cin>>h;
	som.setHeight(h);
	//printf("\nHeight of Map :%d",som.getHeight());
    
	cout<<"\nInput Width of MAP:";
	cin>>w;
	som.setWidth(w);
	//printf("\nWidth of Map :%d",som.getWidth());

	cout<<"\nInput # Feature:";
	cin>>atr;
	som.setFeature(atr);
	//printf("\n#Feature of eamples :%d",som.getFeature());

	PATTERN pat[maxExample];

	cout<<"\nInput # Training Examples:";
	cin>>ex;
	som.setExample(ex);
	//printf("\n#Eample for Training :%d",som.getExample());


	cout<<"\nInput #Traiing Iteration:";
	cin>>MAX_ITERATION;
	//printf("\n#Traiing Iteration: %d",MAX_ITERATION);

	int i,j;
    for(i =0;i<som.getExample();i++)
	{
		printf("\n ");
		for(j=0;j<som.getFeature();j++)
	    {	 
			int r = rand() % 100 + 1;//fRand(0.0,1.0);
			pat[i].Input[j]=  r%2;
			printf("%0.1f ",pat[i].Input[j]);
		}//j
	}//i

	MAP map[maxHeight][maxWidth];
	cout<<"\nInitilizing MAP";
	initializeMAP(som,map);
	displayMAP(som,map);
	cout<<"\nTraining MAP";
	training(som,map,pat);
	//cout<<"\nTesting MAP";

}//END SOM initialization

//Function inintializing MAP with random weight vectors
void initializeMAP(SOM som, MAP  map[maxHeight][maxWidth])
{

	int i,j,k;
	for(i=0;i<som.getHeight();i++)
	{
		for(j=0;j<som.getWidth();j++)
		{
			for(k=0;k<som.getFeature();k++)
			{
				map[i][j].Node[k] =  fRand(0.0,1.0);//rand() % 100 + 1;
			}
		}
	}
}//END Function inintializing MAP

//The procedure training of the SOM 
void training(SOM som,MAP map[maxHeight][maxWidth],PATTERN pat[maxExample])
{
    //find the best match unit
	double disMAP[maxHeight][maxWidth];
	double  F[maxFeature];//A vector feteching Input
	double  W[maxFeature];//Vector fetching Weight from a node
	int minROWindex = 0;//index in ROW in MAP of BMU
	int minCOLindex= 0;//index of COLUMN in MAP of BMU
	double minDIST= 100000.0;//Initialial minidistance taken and/or temp variable for computing BMU

	int R,R0;// R0 is the Base radius of the MAP
	double lamda;//MAX_Iteration/Base Radius 

	//Computing Base Radius
	if(som.getHeight()==som.getWidth())
	{	R0 = som.getHeight()-1;	}
	else if(som.getHeight()<som.getWidth())
	{	R0 = som.getHeight();	}
	else
	{	R0 = som.getWidth();	}
	lamda = (double)MAX_ITERATION/(double)R0;
	//printf("\n%d,%f",R0,lamda);

	int i,j,k,l;
	int iteration=0;//index of training iteration

	while (iteration<MAX_ITERATION)
	{
		iteration ++;//increamenting traiong iteration
        	
		for(i=0;i<som.getExample();i++)// run for each input 
		{
			for(j=0;j<som.getFeature();j++)
				F[j] = pat[i].Input[j];//copy the ith example to vector F

			//compute best match unit It is the node having minROWindex and minCOLindex
			for (j=0;j<som.getHeight();j++)
			{
				//printf("\n ");
				for(k=0;k<som.getWidth();k++)
				{
					for(l=0;l<som.getFeature();l++)
					{	W[l]=map[j][k].Node[l];}

					//Computing distance between clusters (MAP) and input pattern i
					disMAP[j][k] = euclidean_dist2(som,F,W);
					//printf("%0.2f :",disMAP[j][k]);
					if(disMAP[j][k]<minDIST)
					{	
						minDIST = disMAP[j][k];
						minROWindex = j;//index in ROW in MAP of BMU
						minCOLindex = k;//index of COLUMN in MAP of BMU
					}//if
				}//k
			}//j
			//Computing Radius of the neighbourhoodr =
			R = (int)((double)R0*exp(-(iteration/lamda)));//Updating radius for current itration 
			alpha = (double)alpha*exp(-(iteration/lamda));//Updating learning rate alpha

			if(alpha < 0.01)
			{  alpha = minAlpha;	}
				

			printf("\n Minimum Dist Row:%d, Col:%d> %0.2f with Radius %d, alpha %0.2f",minROWindex,minCOLindex,minDIST,R,alpha);

			updateMAPweight(minROWindex,minCOLindex,R,alpha,som,map,F);//updating winnier node and its neighbour
			displayMAP(som,map);
		}//i
	}//iteartion

	//displayMAP(som,map);
}// END training

//computing eucliean distance between map's Node and Feature vecrtor
double euclidean_dist2(SOM som, double F[maxFeature], double W[maxFeature])
{
	//printf("I M Euclidean");
    double sum=0.0;
    int i;
				
    for (i=0;i<som.getFeature();i++) 
    {           
		sum += (W[i]-F[i])*(W[i]-F[i]);
    }
    return sum;
}// END computing eucliean distance


//Update MAP or clusters weight
void updateMAPweight(int indexROW,int indexCOL,int R,double eta, SOM som, MAP map[maxHeight][maxWidth], double I[maxFeature])
{
	//winner node is situated at MAP position MAP[indexROW][indexCOL]

	//compute radious  for updation of neighbours as well
	//A ractangular neighbour hood is considered/ we may choose any kind of neighbourhood

	int Radius = R;// Radius of the MAP at the present iteration

	int ROWminusR, ROWplusR;
	int COLminusR, COLplusR;

	ROWminusR = indexROW - Radius;
	if(ROWminusR < 0)
		ROWminusR = 0;//set lowest height of the MAP
	
    ROWplusR = indexROW + Radius;
	if(ROWplusR > som.getHeight())
		ROWplusR = som.getHeight();//set greatest height of the MAP

	COLminusR = indexCOL - Radius;
	if(COLminusR < 0)
		COLminusR = 0;//set lowest width of the MAP
	
    COLplusR = indexCOL + Radius;
	if(COLplusR > som.getWidth())
		COLplusR = som.getWidth();//set greatest width of the MAP
	double theta;//The neighbourhood function of a Best match unit
	double distBUM2;//Distanse from BMU to a its neighbout node

	int i,j,k;
	for(i=ROWminusR;i<=ROWplusR;i++)
	{
		for(j=COLminusR;j<=COLplusR;j++)
		{
			distBUM2 = (indexROW - i)*(indexROW - i) + (indexCOL - j)*(indexCOL - j);
			for(k=0;k<som.getFeature();k++)
			{
				if(Radius>0)
				   theta = exp(-( distBUM2/(2*Radius*Radius)));
				else
					theta = 1.0;
				printf("(Theta %0.2f)  ",theta);
				map[i][j].Node[k] = map[i][j].Node[k] + theta*eta*(I[k] - map[i][j].Node[k]);//updating weight
			}//k
		}//j
	}//i
}//End of Weight Updates

//Function returning random float value
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}//END Function returning random float


//Function displaying the MAP values
void displayMAP(SOM som,MAP  map[maxHeight][maxWidth])
{
	int i,j,k;
	for(i=0;i<som.getHeight();i++)
	{
		cout<<"\n ";
		for(j=0;j<som.getWidth();j++)
		{
			cout<<" [";
			for(k=0;k<som.getFeature();k++)
			{
				cout<<" ";
				printf("%0.2f",map[i][j].Node[k]);
			}
			cout<<"]";
		}
	}
}//END Function displaying the MAP