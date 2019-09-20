#pragma once

class Node
{
public:
  Node(unsigned int id, double x, double y, double d) : _id(id), _x(x), _y(y), _d(d){};
  unsigned int id() { return _id; }
  double x() { return _x; }
  double y() { return _y; }
  double & d() { return _d; }

protected:
private:
  unsigned int _id;
  double _x;
  double _y;
  double _d;
};
