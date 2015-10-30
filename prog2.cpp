#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include <stack>

using namespace std;

struct point
{
	double x;
	double y;
};

//double min(double x, double y);
//void sortByCos(point best, vector<point> &pVector);
bool isConvex(vector<point> pVector);
double direction(point a, point b, point c);
double cross(point a, point b);
double area(vector<point> pVector);

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
	
		bool convex = isConvex(pVector);

		if(convex)
			cout << "CONVEX" << endl;
		else
		{
			cout << "NOT CONVEX" << endl;
			continue;
		}

		cout << "Area: " << area(pVector) << endl;

		pVector.clear();
		cout << "CLEAR!" << endl;
	}
}

bool isConvex(vector<point> pVector)
{
	bool negative = false;
	int n = pVector.size();

	for(int i = 0; i < n; i++)
	{
		if(direction(pVector[i%n], pVector[(i+1)%n], pVector[(i+2)%n]) > 0)
		{
			if ( negative == true)
				return false;	
		}			
		else
			negative = true;	
	}		
	
	return true;
}

//Corwin
double direction(point a, point b, point c)
{
	point ab;
	point bc;
	double result;

	ab.x = b.x - a.x;
	ab.y = b.y - a.y;
	bc.x = c.x - b.x;
	bc.y = c.y - b.y;
	result =  cross(ab, bc);
	if (fabs(result) < 1.0e-6)
		result = 0.0;
	return result;
}


//Corwin
double cross(point a, point b)
{
	return a.x * b.y - a.y * b.x;
}

int bisection(vector<point> pVector, float lArea, float rArea)
{
	double cut = 0;
	double maxX = 0;
	double minX = FLT_MAX;
	double midX = 0;

	for(int i = 0; i < pVector.size; i++)
	{
		minX = min(minX, pVector[i]);
		maxX = max(maxX, pVector[i]);
	}

	midX = (maxX - minX)/2;

	if(lArea == rArea)
		return cut;
	if(lArea > rArea)

	if(lArea < rArea) 

	
}

//Corwin
//edited by me
double area(vector<point> pVector)
{
	int i;
	int j;
	double result;
	int n = pVector.size();

	result = 0;

	for (i = 0; i < n; i++)
	{
		j = (i + 1) % n;
		result += pVector[i].x * pVector[j].y;
		result -= pVector[i].y * pVector[j].x;
	}
	return fabs(result / 2);
}

//Corwin
double min(double a, double b)
{
  if (a < b)
    return a;
  return b;
}

double max(double a, double b)
{
	if(a > b)
		return a;
	return b;
}

/*
void sortByCos(point best, vector<point> &pVector)
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

	sort(pVector.begin(), pVector.end(), angleDesc);
}*/
