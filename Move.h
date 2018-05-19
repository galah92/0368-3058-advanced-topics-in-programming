#pragma once

#include <memory>
#include "Point.h"


class Move {
public:
	virtual const Point& getFrom() const = 0;
	virtual const Point& getTo() const = 0;
	virtual ~Move() {}
};

class MoveImpl : public Move {
public:
	MoveImpl(PointImpl& from, PointImpl& to) :
		_from(std::make_unique<PointImpl>(from)),
		_to(std::make_unique<PointImpl>(to)) {}
	const Point& getFrom() const override { return *_from; };
	const Point& getTo() const override { return *_to; };
private:
	std::unique_ptr<Point> _from;
	std::unique_ptr<Point> _to;
};