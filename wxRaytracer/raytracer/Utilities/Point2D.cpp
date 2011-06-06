
#include "Point2D.h"

Point2D::Point2D(void)
	: x(0.0), y(0.0)
{}

Point2D::Point2D(const float arg)
	: x(arg), y(arg)
{}

Point2D::Point2D(const float x1, const float y1)
	: x(x1), y(y1)
{}

Point2D::Point2D(const Point2D& p)
	: x(p.x), y(p.y)
{}

Point2D& 							
Point2D::operator= (const Point2D& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x;
	y = rhs.y;

	return (*this);
}