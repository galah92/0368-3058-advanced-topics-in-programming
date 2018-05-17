#pragma once


class Point;

class Move {
public:
	virtual const Point& getFrom() const = 0;
	virtual const Point& getTo() const = 0;
	virtual ~Move() {}
};

class MoveImpl : public Move {
public:
	MoveImpl(Point& from, Point& to) : _from(from), _to(to) {}
	const Point& getFrom() const override { return _from; };
	const Point& getTo() const override { return _to; };
private:
	Point& _from;
	Point& _to;
};