#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include "SystemFun.h"

#ifndef GEOMETRY
#define GEOMETRY

#define al_rgb al_map_rgb
#define MAX(a, b) (((a) > (b)) ? (a): (b))
#define MIN(a, b) (((a) < (b)) ? (a): (b))
#define FROMRADTOGRAD (180.0/ALLEGRO_PI)


double koren2 = sqrt(2.0);
double koren2on2 = 1 / koren2;

class QPoint;
class QVector;
class QLine;

bool CollideDetect(double x1_1, double y1_1, double x1_2, double y1_2, double x2_1, double y2_1, double x2_2, double y2_2);
bool CollideDetect(double x1_1, double y1_1, double x1_2, double y1_2, double x2, double y2, double r2);
bool CollideDetect(double x1, double y1, double r1, double x2_1, double y2_1, double x2_2, double y2_2);
bool CollideDetect(double x1, double y1, double r1, double x2, double y2, double r2);

double LenFrom1To2(double x1, double y1, double x2, double y2);
double AngleToTarget(double x1, double y1, double x2, double y2);



class QPoint
{
public:


	double x, y;

	double dist(QPoint other) {
		return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
	}
	double dist2step(QPoint other) {
		return pow(x - other.x, 2) + pow(y - other.y, 2);
	}

	void init(double x1, double y1);

	QPoint operator = (const QPoint &p1) {
		x = p1.x, y = p1.y;
		return *this;
	}
	QPoint operator + (const QPoint &p1) {
		return QPoint(x + p1.x, y + p1.y);
	}
	QPoint operator - (const QPoint &p1) {
		return QPoint(x - p1.x, y - p1.y);
	}

	QPoint(double x1, double y1);
};
QPoint::QPoint(double x1 = 0, double y1 = 0)
{
	x = x1;
	y = y1;
}
void QPoint::init(double x1, double y1) {
	x = x1;
	y = y1;
	return;
}
class QLine
{
public:
	QPoint p1, p2;
	void init(double p1x, double p1y, double p2x, double p2y);
	void init(QPoint P1, QPoint P2);
};
void QLine::init(double p1x, double p1y, double p2x, double p2y)
{
	p1.x = p1x;
	p1.y = p1y;
	p2.x = p2x;
	p2.y = p2y;
}
void QLine::init(QPoint P1, QPoint P2)
{
	p1 = P1;
	p2 = P2;
}

class QVector
{
public:
	double x, y;

	QVector(double ax, double ay);
	QVector(QPoint a, QPoint b);

	void init(double x1, double y1) {
		x = x1;
		y = y1;
	}
	
	QVector operator = (const QVector &v1) {
		x = v1.x, y = v1.y;
		return *this;
	}

	double dist() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}
	double dist2step() {
		return pow(x, 2) + pow(y, 2);
	}

	QVector operator + (const QVector &v1) {
		return QVector(x + v1.x, y + v1.y);
	}
	QVector operator - (const QVector &v1) {
		return QVector(x - v1.x, y - v1.y);
	}
	QVector operator += (const QVector &v1) {
		return QVector(x = x + v1.x, y = y + v1.y);
	}
	QVector operator -= (const QVector &v1) {
		return QVector(x = x - v1.x, y = y - v1.y);
	}
	double operator * (const QVector &v1) { // скал€рное
		return x * v1.x + y * v1.y;
	}
	double operator % (const QVector &v1) { // векторное (псевдо скал€рное)
		return x * v1.y - v1.x * y;
	}

	QVector operator + (double a) {
		return QVector(x + a, y + a);
	}
	QVector operator - (double a) {
		return QVector(x - a, y - a);
	}
	QVector operator * (double a) {
		return QVector(x * a, y * a);
	}
	QVector operator / (double a) {
		return QVector(x / a, y / a);
	}
	QVector operator += (double a) {
		return QVector(x = x + a, y = y + a);
	}
	QVector operator -= (double a) {
		return QVector(x = x - a, y = y - a);
	}
	QVector operator *= (double a) {
		return QVector(x = x * a, y = y * a);
	}
	QVector operator /= (double a) {
		return QVector(x = x / a, y = y / a);
	}


};
QVector::QVector(double ax = 0, double ay = 0)
{
	x = ax;
	y = ay;
}
QVector::QVector(QPoint a, QPoint b)
{
	x = a.x - b.x;
	y = a.y - b.y;
}

bool CollideDetect(double x1_1, double y1_1, double x1_2, double y1_2, double x2, double y2, double r2){
	
	if (x2 > x1_2) {
		if (y2 > y1_2) return CollideDetect(x2, y2, r2, x1_2, y1_2, 0);
		if (y2 < y1_1) return CollideDetect(x2, y2, r2, x1_2, y1_1, 0);
		return x2 - r2 <= x1_2;	
	}
	if (x2 < x1_1) {
		if (y2 > y1_2) return CollideDetect(x2, y2, r2, x1_1, y1_2, 0);
		if (y2 < y1_1) return CollideDetect(x2, y2, r2, x1_1, y1_1, 0);
		return x2 + r2 >= x1_1;
	}
	return (y2 - r2 <= y1_2) && (y2 + r2 >= y1_1);

}
bool CollideDetect(double x1_1, double y1_1, double x1_2, double y1_2, double x2_1, double y2_1, double x2_2, double y2_2) {
	double xmin, xmax, ymin, ymax;
	xmin = MAX(x1_1, x2_1);
	ymin = MAX(y1_1, y2_1);
	xmax = MIN(x1_2, x2_2);
	ymax = MIN(y1_2, y2_2);
	if (xmin >= xmax || ymin >= ymax)return false;
	return true;
}
bool CollideDetect(double x1, double y1, double r1, double x2, double y2, double r2) {
	return LenFrom1To2(x1, y1, x2, y2) < (r1 + r2);
}
double LenFrom1To2(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
double AngleToTarget(double x1, double y1, double x2, double y2) {

	return atan2(y2 - y1, x2 - x1)*FROMRADTOGRAD;
}


#endif