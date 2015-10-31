#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include <float.h>

using namespace std;

struct point
{
	double x;
	double y;
};


bool isConvex(vector<point> pVector);
double direction(point a, point b, point c);
double cross(point a, point b);
double area(vector<point> pVector);
bool intersect(point p1, point p2, point p3, point p4);
int bisection(vector<point> pVector);
bool on(point a, point b, point c);
vector<point> topBotPoints(vector<point> pVector);

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

		bisection(pVector);

		pVector.clear();
		cout << "CLEAR!" << endl;
	}
}

bool isConvex(vector<point> pVector)
{
	bool negative = false;
	bool positive = false;
	int n = pVector.size();

	for(int i = 0; i < n; i++)
	{
		if(direction(pVector[i%n], pVector[(i+1)%n], pVector[(i+2)%n]) > 0)
		{
			if ( negative == true)
				return false;
			else
				positive = true;	
		}			
		else
		{
			if (positive = true)
				return false;
			else	
				negative = true;	
		}
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

int bisection(vector<point> pVector)
{
	int n = pVector.size();
	double cut = 0;
	double maxX = 0;
	double minX = FLT_MAX;
	double midX = 0;
	double maxY = 0;
	double minY = FLT_MAX;
	point botPoint;
	point topPoint;	
	vector<point> topBot;
	
	topBot = topBotPoints(pVector);

	if(topBot.size() != 4)
		cout << "Something went wrong" << endl;

	for(int i = 0; i < topBot.size(); i++)
	{
		cout << "P" << i << ".x: " << topBot[i].x << endl;
		cout << "P" << i << ".y: " << topBot[i].y << endl;
	}
}

vector<point> topBotPoints(vector<point> pVector)
{
	int n = pVector.size();
	double maxX = 0;
	double minX = FLT_MAX;
	double midX = 0;
	double maxY = 0;
	double minY = FLT_MAX;
	point botPoint;
	point topPoint;
	vector<point> topBot;

	for(int i = 0; i < n; i++)
	{
		minX = min(minX, pVector[i].x);
		maxX = max(maxX, pVector[i].x);
		minY = min(minY, pVector[i].y);
		maxY = max(maxY, pVector[i].y);	
	}

	midX = ((maxX - minX)/2 + minX);
	
	topPoint.x = midX;
	topPoint.y = maxY + 1;
	botPoint.x = midX;
	botPoint.y = minY;	

	cout << "topPoint.x: " << topPoint.x << endl;
	cout << "topPoint.y: " << topPoint.y << endl;
	cout << "botPoint.x: " << botPoint.x << endl;
	cout << "botPoint.y: " << botPoint.y << endl;

	for(int i = 0; i < n; i++)
	{
		bool test;	
		cout << "pVector[i].x: " << pVector[i].x << endl;
		cout << "pVector[i].y: " << pVector[i].y << endl;
		cout << "pVector[i+1].x: " << pVector[(i+1) % n].x << endl;
		cout << "PVector[i+1].y: " << pVector[(i+1) % n].y << endl;
		test = intersect(botPoint, topPoint, pVector[i], pVector[(i+1) % n]);
		if(test == true)
		{
			topBot.push_back(pVector[i]);
			topBot.push_back(pVector[(i+1) % n]);
			cout << "Intersect" << endl;
		}
		else
			cout << "No Intersect" << endl;
	}

	return topBot;
}

bool intersect(point p1, point p2, point p3, point p4)
{
	double d1;
	double d2;
	double d3;
	double d4;

	d1 = direction(p3, p4, p1);
	d2 = direction(p3, p4, p2);
	d3 = direction(p1, p2, p3);
	d4 = direction(p1, p2, p4);

	cout << "d1: " << d1 << endl;
	cout << "d2: " << d2 << endl;
	cout << "d3: " << d3 << endl;
	cout << "d4: " << d4 << endl;

	if ((((d1 > 0) && (d2 < 0)) || ((d1 < 0) && (d2 > 0))) &&
	(((d3 > 0) && (d4 < 0)) || ((d3 < 0) && (d4 > 0))))
		return true;

	if ((d1 == 0) && on(p3, p4, p1))
		return true;

	if ((d2 == 0) && on(p3, p4, p2))
		return true;

	if ((d3 == 0) && on(p1, p2, p3))
		return true;

	if ((d4 == 0) && on(p1, p2, p4))
		return true;

	return false;
}

bool on(point a, point b, point c)
{
	if (((min(a.x, b.x) <= c.x) && (c.x <= max(a.x, b.x))) &&
	((min(a.y, b.y) <= c.y) && (c.y <= max(a.y, b.y))))
		return true;

	return false;
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
