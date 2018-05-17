#pragma once


class Point {
public:
	virtual int getX() const = 0;
	virtual int getY() const = 0;
	virtual ~Point() = default;
};

class PointImpl : public Point {
public:
	PointImpl(int x, int y) : _x(x), _y(y) {}
	int getX() const override { return _x; }
	int getY() const override { return _y; }
private:
	int _x;
	int _y;
};