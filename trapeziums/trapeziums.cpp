#include <bits/stdc++.h>
using namespace std;
class point{
	public:
	float x;
	float y;
	point()
	{
		x=0;
		y=0;
	}
	point(const point& p)
	{
		x=p.x;
		y=p.y;
	}
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
};
class segment{
	public:
	point l;
	point r;
	segment()
	{
		l=point(0,0);
		r=point(0,0);
	}
	segment(point p1, point p2)
	{
		l=p1;
		r=p2;
	}
};
class event{
	public:
	point p;
	pair<int, int> lines;//if only first entry then starting, else if second then ending else intersection (empty==-1)
	event()
	{
		p=point(0,0);
		lines={-1,-1};
	}
	event(point p1, pair <int, int> lines1)
	{
		p=p1;
		lines=lines1;
	}
	void operator=(const event &e1)
	{
		p=e1.p;
		lines=e1.lines;
	}
};

struct CompareEvents {
    bool operator()(event const& e1, event const& e2)
	const
    {
        return e1.p.x > e2.p.x; //top is the minimum x event
    }
};

void showevents(priority_queue<event, vector<event>, CompareEvents> events)
{
	cout<<"--------------------------------------->\n";
	cout<<"showevents:\n";
	while(!events.empty())
	{
		event a = events.top();
		events.pop();
		if (a.lines.first>-1 && a.lines.second>-1)
		{
			cout<<"("<<a.p.x<<","<<a.p.y<<") "<<"intersection "<<a.lines.first<<","<<a.lines.second<<endl;
		}
		else if (a.lines.first>-1)
		{
			cout<<"("<<a.p.x<<","<<a.p.y<<") "<<"start "<<a.lines.first<<endl;
		} else if (a.lines.second>-1)
		{
			cout<<"("<<a.p.x<<","<<a.p.y<<") "<<"end "<<a.lines.second<<endl;
		}
		else{
			cout<<"err: 1";
			exit(-1);
		}
	}
	cout<<"--------------------------------------->\n";
	return;
}

class statusobj{
	public:
	int linenum;
	float y;
	statusobj()
	{
		linenum=-1;
		y=-1;
	}
	statusobj(int linenum1, float y1)
	{
		linenum=linenum1;
		y=y1;
	}
};

struct CompareStatus {
    bool operator()(statusobj const& s1, statusobj const& s2)
	const
    {
    	return (s1.y < s2.y); 
    }
};

void updateSLS(map<int, float> &vals, multiset<statusobj, CompareStatus> &lines, multiset<int> &actives, float x_new, vector<segment> &segments)
{
	lines.clear();
	for (auto it: actives)
	{
		float y_new = (x_new-segments[it].l.x)*((segments[it].r.y-segments[it].l.y)/(segments[it].r.x-segments[it].l.x)) + segments[it].l.y;
		vals[it]=y_new;
		lines.insert(statusobj(it, y_new));
	}
	return;
}

void showstatus(multiset<statusobj, CompareStatus> &lines, float x_new)
{
	cout<<"---------------------------------------> x="<<x_new<<"\n";
	for (auto it: lines)
	{
		cout<<it.linenum<<" has y="<<it.y<<endl;
	}
	cout<<"--------------------------------------->\n";
}

void update_intersection(int l1, int l2, vector<segment> &s, float x_new, priority_queue<event, vector<event>, CompareEvents> &events)
{
	//y=mx+c forms
	float m1=(s[l1].r.y-s[l1].l.y)/(s[l1].r.x-s[l1].l.x);
	float c1=(s[l1].r.y*s[l1].l.x-s[l1].l.y*s[l1].r.x)/(s[l1].l.x-s[l1].r.x);
	float m2=(s[l2].r.y-s[l2].l.y)/(s[l2].r.x-s[l2].l.x);
	float c2=(s[l2].r.y*s[l2].l.x-s[l2].l.y*s[l2].r.x)/(s[l2].l.x-s[l2].r.x);
	// if (m1==m2) {cout<<"||"<<endl; return;}
	cout<<"checking"<<l1<<","<<l2<<endl;
	float y_int = (m1*c2-m2*c1)/(m1-m2);
	float x_int = (-c2+c1)/(m2-m1);
	if (x_int>x_new && x_int<s[l1].r.x && x_int<s[l2].r.x)
	{
		// cout<<"add: "<<x_int<<","<<y_int<<endl;
		// auto it = events.find(event(point(x_int, y_int), {l1,l2}));
		// if (it==events.end()) 
		events.push(event(point(x_int, y_int), {l1,l2}));
	}
	return;
}

void printskyline(vector<pair<float,float>> skyline)
{
	for (auto it : skyline)
	{
		cout<<"("<<it.first<<","<<it.second<<") ";
	}
	cout<<endl;
}

int main()
{
	//hardcoding the trapeziums for now
	vector<pair<float,float>> skyline;
	vector<segment> segments={segment(point(1,8), point(7,4)),segment(point(2,9), point(7,2)),segment(point(3,5), point(7,9))};
	//manually push the initial event points
	priority_queue<event, vector<event>, CompareEvents> events;
	for (int i=0; i<segments.size(); i++)
	{
			events.push(event(segments[i].l, {i,-1}));
			events.push(event(segments[i].r, {-1,i}));
			// cout<<segments[i].l.x<<","<<segments[i].l.y<<" "<<segments[i].r.x<<","<<segments[i].r.y<<endl;
	}
	// showevents(events);
	map<int, float> vals;//this is to get the last assumed y coordinate
	multiset<statusobj, CompareStatus> lines;//this is to get the pred and succ functionality
	multiset<int> actives;
	while (!events.empty())
	{
		event this_event=events.top();
		events.pop();
		if (this_event.lines.first>-1 && this_event.lines.second>-1)
		{
			//intersections are being counted twice
			//endpoint is the intersection
			assert(actives.size()==lines.size());
			// cout<<"this is intrsxn"<<this_event.lines.first<<" "<<this_event.lines.second<<endl;
			//switch the orders and check for neighbours
			updateSLS(vals, lines, actives, this_event.p.x, segments);
			statusobj max = *(prev(lines.end()));
			if (fabs(max.y-this_event.p.y)<0.00001)
			{
				skyline.push_back({this_event.p.x, this_event.p.y});
			}

			auto it=lines.find(statusobj(this_event.lines.first, vals[this_event.lines.first]));
			auto succ=next(it);
			auto pred=prev(it);
			// {

			// 		INCOMPLETE

			// }
			assert(actives.size()==lines.size());
		}
		else if (this_event.lines.first>-1)
		{
			//start of a segment
			assert(actives.size()==lines.size());
			actives.insert(this_event.lines.first);
			updateSLS(vals, lines, actives, this_event.p.x, segments);
			// showstatus(lines, this_event.p.x);
			auto it=lines.find(statusobj(this_event.lines.first, vals[this_event.lines.first]));
			auto pred=prev(it);
			auto succ=next(it);
			if (it!=lines.begin()) {update_intersection(this_event.lines.first, (*pred).linenum, segments, this_event.p.x, events);}
			if (succ!=lines.end()) {update_intersection(this_event.lines.first, (*succ).linenum, segments, this_event.p.x, events);}
			if (succ==lines.end())
			{
				if (actives.size()==1)
				{
					skyline.push_back({this_event.p.x,0});
					skyline.push_back({this_event.p.x,(*it).y});
				}
				else
				{
					skyline.push_back({this_event.p.x, (*pred).y});
					skyline.push_back({this_event.p.x, (*it).y});
				}
			}
			assert(actives.size()==lines.size());
		}
		else if (this_event.lines.second>-1)
		{
			//end of a segment
			assert(actives.size()==lines.size());
			updateSLS(vals, lines, actives, this_event.p.x, segments);// we will erase the line from lines and actives manually
			assert(actives.size()==lines.size());
			auto it=lines.find(statusobj(this_event.lines.second, vals[this_event.lines.second]));
			auto pred=prev(it);
			auto succ=next(it);
			if (it!=lines.begin() && succ!=lines.end()) {update_intersection((*pred).linenum, (*succ).linenum, segments, this_event.p.x, events);}
			assert(actives.size()==lines.size());
			if (succ==lines.end())
			{
				if (it==lines.begin())
				{
					skyline.push_back({this_event.p.x,(*it).y});
					skyline.push_back({this_event.p.x,0});
				}
				else
				{
					skyline.push_back({this_event.p.x, (*it).y});
					skyline.push_back({this_event.p.x, (*pred).y});
				}
			}
			actives.erase(this_event.lines.second);
			lines.erase(statusobj(this_event.lines.second, vals[this_event.lines.second]));
			assert(actives.size()==lines.size());
		}
		else
		{
			cout<<"err:2";
			exit(-1);
		}
		// cout<<actives.size()<<" "<<lines.size()<<endl;
		assert(actives.size()==lines.size());//sanity checks
	}
	printskyline(skyline);
	return 0;	
}
