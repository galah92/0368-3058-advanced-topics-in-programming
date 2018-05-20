#pragma once

#include "Point.h"


class Move {
public:
	virtual const Point& getFrom() const = 0;
	virtual const Point& getTo() const = 0;
	virtual ~Move() {}
};

class MoveImpl : public Move {
public:
	MoveImpl(const PointImpl& from, const PointImpl& to) :
		_from(from),
		_to(to) {}
	const Point& getFrom() const override { return _from; };
	const Point& getTo() const override { return _to; };
private:
	PointImpl _from;
	PointImpl _to;
};