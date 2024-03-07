#include <bits/stdc++.h>
using namespace std;
class point{
	public:
	double x;
	double y;
	point(float x1, float y1)
	{
		x=x1;
		y=y1;
	}
	void operator=(const point& p)
	{
		x=p.x;
		y=p.y;
	}
	bool operator<(const point &p)
	{
		return x<p.x;
	}
};
class segment{
	public:
	point l;
	point r;
	segment(point p1, point p2)
	{
		l=p1;
		r=p2;
	}
};
class event{
	point p;
	pair<int> lines;//if only first entry then starting, else if second then ending else intersection
	bool operator<(const event &e)
	{
		return p<e.p;
	}
	event(point p1, pair <int> lines1)
	{
		p=p1;
		lines=lines1;
	}
};
int main()
{
//hardcoding the trapeziums for now
vector<segment> segments={segment(point(1,8), point(8,4)),segment(point(2,9), point(6,2)),segment(point(3,5), point(7,9))};
//manually push the initial event points
priority_queue<event> events;
for (int i=0; i<segments.length; i++)
{
		events.push(event(segments[1].l, {i,0}));
		events.push(event(segments[1].r, {0,i}));
}

}
