#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <math.h>

using namespace std;

struct point
{
	double x;
	double y;
	double angle;
};

double min(double x, double y);
void sortByCos(point best, vector<point> pVector);

int main(int argc, char * argv[])
{
	ifstream fin;
	ofstream fout;
	int numPoints = 0;
	string fileNameExt = argv[1];

	int splitPos = fileNameExt.find(".");
	
	string fileName = fileNameExt.substr(0, splitPos);

	cout << fileName << endl;	
	
	fin.open(argv[1]);
	fileName += ".out";
	fout.open(fileName.c_str());

	while(fin >> numPoints)
	{
		cout << numPoints << endl;	

		vector<point> pVector;				

		//read in points
		for(int i = 0; i < numPoints; i++)
		{
			point temp;
			fin >> temp.x;
			cout << temp.x << " ";
			fin >> temp.y;
			cout << temp.y << endl;
			pVector.push_back(temp);
		}
		
		point best;
		best.x = INT_MAX;
		best.y = INT_MAX;

		int bestPos;

		//code for getting lower left point
		for(int i = 0; i < pVector.size(); i++)
		{	
			if(best.y == pVector[i].y)
			{
				if(pVector[i].x < best.x)
				{
					best.x = pVector[i].x;
					bestPos = i;
				}
			}

			if(pVector[i].y < best.y)
			{
				best.x = pVector[i].x;
				best.y = pVector[i].y;
				bestPos = i;
			}			
		}

		cout << best.x << " " << best.y << " Besties" << endl; 

		cout << "bestPos: " << bestPos << endl;				
		pVector.erase(pVector.begin() + bestPos);

		sortByCos(best, pVector);
	
		pVector.clear();
		cout << "CLEAR!" << endl;
	}
}

double min(double a, double b)
{
  if (a < b)
    return a;
  return b;
}

void sortByCos(point best, vector<point> pVector)
{
	point u;
	for(int i = 0; i < pVector.size(); i++)
	{
		u.x = pVector[i].x - best.x;
		u.y = pVector[i].y - best.y;
		double length = sqrt(u.x * u.x + u.y * u.y);

		pVector[i].angle = u.x / length;
	
 		cout << "u.x: " << u.x << endl;
		cout << "u.y: " << u.y << endl;
		cout << "length: " << u.y << endl;
		cout << "angle: " << pVector[i].angle << endl;
	}			
}
