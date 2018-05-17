#pragma once


class Point;

class JokerChange {
public:
	virtual const Point& getJokerChangePosition() const = 0;
	virtual char getJokerNewRep() const = 0;
	virtual ~JokerChange() {}
};

class JokerChangeImpl : public JokerChange {
public:
	JokerChangeImpl(Point& pos, char rep) : _pos(pos), _rep(rep) { }
	const Point& getJokerChangePosition() const override { return _pos; };
	char getJokerNewRep() const override { return _rep; };
private:
	Point& _pos;
	char _rep;
};