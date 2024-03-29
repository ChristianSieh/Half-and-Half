//Auther: Christian Sieh
//Date: 11/1/2015
//Class: Analysis of Algorithms, CSC 372
//Professor: Dr. Logar
//TA: Dr. Corwin
//Usage: prog2.exe sample.txt
//
//Overview: Given a text file that starts with the number of following points
//and then each pair of points (seperated by a space) this program will 
//compute the vertical line that is needed to split the area of the polygon 
//in half. This program first checks that the shapes is convex, if not then 
//no solution is outputted. Then since the shape is convex it will split 
//the polygon in half and "recursively" split the larger polygon until it 
//finds the x value where a vertical cut will give each polygon half the area.
//
//TODO: 1) This program has a while(true) that needs to be removed.
//	2) The is a shortcircuit it the code to work around an exception that
//	   needs to be fixed.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include <float.h>
#include <iomanip>
#include <cmath>

using namespace std;

//Original Author: Dr. Corwin
//Modified: Christian Sieh
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
double bisection(vector<point> &pVector, double &leftOver, 
	double &rightOver, double oldMid);
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
	int caseCount = 1;

	int splitPos = fileNameExt.find(".");

	//This will get the file name without the extension	
	string fileName = fileNameExt.substr(0, splitPos);
	
	fin.open(argv[1]);
	fileName += ".out";
	fout.open(fileName.c_str());

	while(fin >> numPoints)
	{
		vector<point> pVector;				

		//read in points
		for(int i = 0; i < numPoints; i++)
		{
			point temp;
			fin >> temp.x;
			fin >> temp.y;
			pVector.push_back(temp);
		}
	
		//Make sure the shape is convex
		bool convex = isConvex(pVector);

		if(!convex)
		{
			fout << "Case " << caseCount << ": " << "No solution" << endl;
			continue;
		}

		//This is used to fix an edge case where a polygon needs to be
		//bisected one more time. Should probably get fixed some day
		bool once = false;

		//Need to switch this because it's stupid but I'm going to bed
		while(true)
		{
			cut = bisection(pVector, leftOver, rightOver, cut);
			double test = abs(leftOver - rightOver);
			//If the precision is good enough then we can quit
			if (test < 0.0000001)
			{
				if (once == true)
					break;
				once = true;
			}
		}

		pVector.clear();

		fout << "Case " << caseCount << ": " << fixed << setprecision(5) << cut << endl;

		caseCount++;
	}
}

//This function will check if a set of points makes a convex shape
//as long as the points are in order (either clockwise or counterclockwise)
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

//Author: Dr. Corwin
//Compute cross product of vector from a to b and vector from b to c
//Set to zero if close to zero
//Note: Precision modified
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


//Author: Dr. Corwin
//Cross product of vectors a and b
double cross(point a, point b)
{
	return a.x * b.y - a.y * b.x;
}

//This function will take a set of points and bisect the polygon that comes from those
//points. Should probably be reworked cause I don't like it
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

	//Needed to short circuit the code, bad
	if (shortCircuit == true)
	{
		leftOver = rightOver;
		return oldMid;
	}

	//Get min and max
	for (int i = 0; i < n; i++)
	{
		minX = min(minX, pVector[i].x);
		maxX = max(maxX, pVector[i].x);
		minY = min(minY, pVector[i].y);
		maxY = max(maxY, pVector[i].y);
	}

	//Get the x midpoint
	double midX = ((maxX - minX) / 2 + minX);

	//Get the top and bottom intersection points
	topPoint.x = midX;
	topPoint.y = ((topLine.p2.y - topLine.p1.y) / (topLine.p2.x - topLine.p1.x))
			 * (midX - topLine.p1.x) + topLine.p1.y;
	botPoint.x = midX;
	botPoint.y = ((botLine.p2.y - botLine.p1.y) / (botLine.p2.x - botLine.p1.x))
			 * (midX - botLine.p1.x) + botLine.p1.y;

	//This for loop will create two new vectors, one for the right half
	//of the shape and one for the left
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

		//This makes sure we insert the botPoint into the vector at the correct
		//time so the points stay in order
		if (pVector[i] == botLine.p1 && pVector[(i + 1) % n] == botLine.p2 ||
			pVector[i] == botLine.p2 && pVector[(i + 1) % n] == botLine.p1)
		{
			leftPoints.push_back(botPoint);
			rightPoints.push_back(botPoint);
		}
		
		//This maes sure we insert the topPoint into the vector at the correct
		//time so the points stay in order
		if (pVector[i] == topLine.p1 && pVector[(i + 1) % n] == topLine.p2 ||
			pVector[i] == topLine.p2 && pVector[(i + 1) % n] == topLine.p1)
		{
			leftPoints.push_back(topPoint);
			rightPoints.push_back(topPoint);
		}
	}

	//Get the left and right area for each half of the polygon
	tempLeft = area(leftPoints);
	tempRight = area(rightPoints);

	leftArea = tempLeft + leftOver;
	rightArea = tempRight + rightOver;

	//If left is bigger then we need to bisect the left shape
	//and we need to keep track of the right portion (rightOver)
	//that was already computed and vice versa for right
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
		//This is here in case the left and right area are the
		//same. Works the same as above
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


//This funciton will get the line about and below the x midpoint value
//where we are intersecting. We need these lines to compute the midpoint
//y values
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

	//Get the min and max x and y values
	for(int i = 0; i < n; i++)
	{
		minX = min(minX, pVector[i].x);
		maxX = max(maxX, pVector[i].x);
		minY = min(minY, pVector[i].y);
		maxY = max(maxY, pVector[i].y);	
	}

	//Calculate the x midpoint
	midX = ((maxX - minX)/2 + minX);
	
	//Create a point about the polygon (midTop)
	//and a point at the bottom of the polygon (midBot)
	//will use this for intersection
	midTop.x = midX;
	midTop.y = maxY + 1;
	midBot.x = midX;
	midBot.y = minY;	

	//Find which two lines the midTop/midBot line intersects
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

	//This is a work around that short circuits the code since I 
	//was running into trouble with the intersect function. Needs to 
	//be fixed and most likely has something to do with precision in
	//small numbers
	if (pMax.size() != 2)
	{
		return true;
	}

	//This if/else is just for reordering the two lines so
	//we know which is top and which is bottom
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
	
	//Didn't need to short-circuit code, yay
	return false;
}

//Author: Dr. Corwin
//From tesxt, page 937 ( second edition)
//Does line segment from p1 to p2 intersect line segment from p3 to p4?
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

//Author: Dr. Corwin
//Is c on the line segment from a to b?
//Assumes c is on line from a to b
bool on(point a, point b, point c)
{
	if (((min(a.x, b.x) <= c.x) && (c.x <= max(a.x, b.x))) &&
	((min(a.y, b.y) <= c.y) && (c.y <= max(a.y, b.y))))
		return true;

	return false;
}

//Author: Dr. Corwin
//Modified: Christian Sieh (changed to vector)
//Area of polygon by adding/subtracting trapezoids
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

//Author: Dr. Corwin
double min(double a, double b)
{
  if (a < b)
    return a;
  return b;
}

//Author: Dr. Corwin
double max(double a, double b)
{
	if(a > b)
		return a;
	return b;
}
