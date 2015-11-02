#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include <float.h>
#include <math.h>
#include <iomanip>

using namespace std;

struct point
{
	double x;
	double y;

	bool operator==(const point& p)
	{
		if (this->x != p.x)
			return false;
		if (this->y != p.y)
			return false;
		return true;
	}
};

struct line
{
	point p1;
	point p2;
	double slope;
};

bool isConvex(vector<point> pVector);
double direction(point a, point b, point c);
double cross(point a, point b);
double area(vector<point> pVector);
bool intersect(point p1, point p2, point p3, point p4);
double bisection(vector<point> &pVector, double &leftOver, double &rightOver, double oldMid);
bool on(point a, point b, point c);
bool topBotLines(vector<point> pVector, line &botLine, line &topLine);

int main(int argc, char * argv[])
{
	ifstream fin;
	ofstream fout;
	double leftOver = 0;
	double rightOver = 0;
	double cut = 0;
	int numPoints = 0;
	string fileNameExt = argv[1];
	int caseCount = 0;

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
			fout << "Case " << caseCount << ": " << "No solution" << endl;
			continue;
		}

		cout << "Area: " << area(pVector) << endl;

		bool once = false;

		while(true)
		{
			cut = bisection(pVector, leftOver, rightOver, cut);

			double test = abs(leftOver - rightOver);
			if (test < 0.0000001)
			{
				if (once == true)
					break;
				once = true;
			}
		}

		cout << "cut: " << fixed << setprecision(5) << cut << endl;
		pVector.clear();
		cout << "CLEAR!" << endl;

		fout << "Case " << caseCount << ": " << cut << endl;

		caseCount++;
	}
	cin >> leftOver;
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
			if (positive == true)
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
	if (fabs(result) < 1.0e-9)
		result = 0.0;
	return result;
}


//Corwin
double cross(point a, point b)
{
	return a.x * b.y - a.y * b.x;
}

double bisection(vector<point> &pVector, double &leftOver, double &rightOver, double oldMid)
{
	int n = pVector.size();
	double maxX = 0;
	double minX = FLT_MAX;
	double maxY = 0;
	double minY = FLT_MAX;
	double leftArea = 0;
	double rightArea = 0;
	double tempLeft = 0;
	double tempRight = 0;
	bool shortCircuit = false;

	point botPoint;
	point topPoint;	
	line botLine;
	line topLine;
	vector<point> leftPoints;
	vector<point> rightPoints;

	shortCircuit = topBotLines(pVector, botLine, topLine);

	if (shortCircuit == true)
	{
		leftOver = rightOver;
		return oldMid;
	}

	for (int i = 0; i < n; i++)
	{
		minX = min(minX, pVector[i].x);
		maxX = max(maxX, pVector[i].x);
		minY = min(minY, pVector[i].y);
		maxY = max(maxY, pVector[i].y);
	}

	double midX = ((maxX - minX) / 2 + minX);

	topPoint.x = midX;
	topPoint.y = ((topLine.p2.y - topLine.p1.y) / (topLine.p2.x - topLine.p1.x)) * (midX - topLine.p1.x) + topLine.p1.y;
	botPoint.x = midX;
	botPoint.y = ((botLine.p2.y - botLine.p1.y) / (botLine.p2.x - botLine.p1.x)) * (midX - botLine.p1.x) + botLine.p1.y;

	for (int i = 0; i < n; i++)
	{
		if (pVector[i].x < midX)
		{
			leftPoints.push_back(pVector[i]);
		}

		else
		{
			rightPoints.push_back(pVector[i]);
		}

		if (pVector[i] == botLine.p1 && pVector[(i + 1) % n] == botLine.p2 ||
			pVector[i] == botLine.p2 && pVector[(i + 1) % n] == botLine.p1)
		{
			leftPoints.push_back(botPoint);
			rightPoints.push_back(botPoint);
		}

		if (pVector[i] == topLine.p1 && pVector[(i + 1) % n] == topLine.p2 ||
			pVector[i] == topLine.p2 && pVector[(i + 1) % n] == topLine.p1)
		{
			leftPoints.push_back(topPoint);
			rightPoints.push_back(topPoint);
		}
	}

	tempLeft = area(leftPoints);
	tempRight = area(rightPoints);

	leftArea = tempLeft + leftOver;
	rightArea = tempRight + rightOver;

	//cout << "leftArea: " << leftArea << endl;
	//cout << "rightArea: " << rightArea << endl;

	if( leftArea > rightArea)
	{
		rightOver += tempRight;
		pVector.clear();
		for(int i = 0; i < leftPoints.size(); i++)
			pVector.push_back(leftPoints[i]);
	}
	else if (rightArea > leftArea)
	{
		leftOver += tempLeft;
		pVector.clear();
		for(int i = 0; i < rightPoints.size(); i++)
			pVector.push_back(rightPoints[i]);
	}
	else
	{
		if (rightOver > leftOver)
		{
			leftOver += leftArea;
			for (int i = 0; i < leftPoints.size(); i++)
				pVector.push_back(leftPoints[i]);
		}
		else
		{
			rightOver += rightArea;
			for (int i = 0; i < rightPoints.size(); i++)
				pVector.push_back(rightPoints[i]);
		}
	}

	return midX;
}

bool topBotLines(vector<point> pVector, line &botLine, line &topLine)
{
	int n = pVector.size();
	double maxX = 0;
	double minX = FLT_MAX;
	double midX = 0;
	double maxY = 0;
	double minY = FLT_MAX;
	point midBot;
	point midTop;
	line temp;
	vector<point> topBotPoints;
	vector<double> pMax;

	for(int i = 0; i < n; i++)
	{
		minX = min(minX, pVector[i].x);
		maxX = max(maxX, pVector[i].x);
		minY = min(minY, pVector[i].y);
		maxY = max(maxY, pVector[i].y);	
	}

	midX = ((maxX - minX)/2 + minX);
	
	midTop.x = midX;
	midTop.y = maxY + 1;
	midBot.x = midX;
	midBot.y = minY;	

	for(int i = 0; i < n; i++)
	{
		bool intersection;	

		intersection = intersect(midBot, midTop, pVector[i], pVector[(i+1) % n]);

		if(intersection == true)
		{
			topBotPoints.push_back( pVector[i]);
			topBotPoints.push_back( pVector[(i+1) % n]);
			pMax.push_back(max(pVector[i % n].y, pVector[(i + 1) % n].y));			
		}
	}

	if (pMax.size() != 2)
	{
		//cout << "ShortCircuit" << endl;
		return true;
	}

	if(pMax[0] > pMax[1])
	{
		topLine.p1 = topBotPoints[0];
		topLine.p2 = topBotPoints[1];
		botLine.p1 = topBotPoints[2];
		botLine.p2 = topBotPoints[3];	
	}
	else
	{
		botLine.p1 = topBotPoints[0];
		botLine.p2 = topBotPoints[1];
		topLine.p1 = topBotPoints[2];
		topLine.p2 = topBotPoints[3];
	}

	topLine.slope = (fabs(topLine.p1.y - topLine.p2.y) / fabs(topLine.p1.x - topLine.p2.x));
	botLine.slope = (fabs(botLine.p1.y - botLine.p2.y) / fabs(botLine.p1.x - botLine.p2.x));
}

//Corwin
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

//Corwin
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
