#pragma once

#include "Point.h"


class JokerChange {
public:
	virtual const Point& getJokerChangePosition() const = 0;
	virtual char getJokerNewRep() const = 0;
	virtual ~JokerChange() {}
};

class JokerChangeImpl : public JokerChange {
public:
	JokerChangeImpl(PointImpl& pos, char rep) :
		_pos(std::make_unique<PointImpl>(pos)),
		_rep(rep) { }
	const Point& getJokerChangePosition() const override { return *_pos; };
	char getJokerNewRep() const override { return _rep; };
private:
	std::unique_ptr<Point> _pos;
	char _rep;
};