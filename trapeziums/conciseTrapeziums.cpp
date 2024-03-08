#include <bits/stdc++.h>
#include "rbt.h"
using namespace std;

float _round(float var)// to 5 dps
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    // float value = (int)(var * 100000 + .5);
    // return (float)value / 100000;
    return var;
}

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
        if (e1.p.x > e2.p.x)
        { return true; }
        else if (fabs(e1.p.x-e2.p.x)<EPSILON)
        {
            if (e1.p.y>e2.p.y) return false;
        }
        return false;   
         //top is the minimum x event
    }
};

void showevents(priority_queue<event, vector<event>, CompareEvents> &events)
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

void updateSLS(map<int, float> &vals, RedBlackTree &lines, set<int> &actives, point &p, vector<segment> &segments, pair<int, int> &event_lines)
{
	lines.clearRBT(lines.root);
    float x_new = p.x;
	for (auto it: actives)
	{
		float y_new = (x_new-segments[it].l.x)*((segments[it].r.y-segments[it].l.y)/(segments[it].r.x-segments[it].l.x)) + segments[it].l.y;
		y_new=_round(y_new);
        if (it==event_lines.first || it==event_lines.second) y_new=p.y; //epsilon stitching off
        vals[it]=y_new;
		lines.insert(y_new, it);
	}
	return;
}

void update_intersection(vector<vector<int>> &intsxns, int l1, int l2, vector<segment> &s, float x_new, priority_queue<event, vector<event>, CompareEvents> &events)
{
    if (intsxns[l1][l2]==1) return;
    else {intsxns[l1][l2]=1; intsxns[l2][l1]=1;}
    float a = s[l1].l.x;
    float b = s[l1].l.y;
    float c = s[l1].r.x;
    float d = s[l1].r.y;
    float a1 = s[l2].l.x;
    float b1 = s[l2].l.y;
    float c1 = s[l2].r.x;
    float d1 = s[l2].r.y;
    float x_int= -((b*(c-a)+a*(b-d))*(c1-a1) - (b1*(c1-a1)+a1*(b1-d1))*(c-a))/((d-b)*(c1-a1)-(d1-b1)*(c-a));
    float y_int= ((d-b)*(b1*(c1-a1)+a1*(b1-d1)) - (d1-b1)*(b*(c-a)+a*(b-d)))/((d-b)*(c1-a1)-(d1-b1)*(c-a));
            //redid the calc to remove fp losses
    y_int=_round(y_int);
    x_int=_round(x_int);
	if (x_int>x_new && x_int<s[l1].r.x && x_int<s[l2].r.x)
	{
		events.push(event(point(x_int, y_int), {l1,l2}));
	}
	return;
}

void showintsxns(vector<vector<int>> &intsxns)
{
	for (auto it:intsxns)
	{
		for (auto jt: it)
		{
			cout<<jt<<"|";
		}
		cout<<endl;
	}
	return;
}

void printskyline(vector<pair<float,float>> &skyline)
{
	for (auto it : skyline)
	{
		cout<<"("<<it.first<<","<<it.second<<") ";
	}
	cout<<endl;
}

void printskylinef2(vector<pair<float,float>> &skyline)
{
    float lastx=-EPSILON;
    float lasty=0;
    float shadow=0;
    float area=0;
	for (auto it : skyline)
	{
        if (it.first!= lastx|| it.second!= lasty) 
        {
            cout<<it.first<<" "<<it.second<<endl;
            area = area + 0.5*(it.second+lasty)*(it.first-lastx);
            if (fabs(lasty)>EPSILON || fabs(it.second)>EPSILON) shadow = shadow + it.first-lastx;
        }
        lastx=it.first;
        lasty=it.second;    
	}
    cout<<shadow<<endl;
    cout<<area<<endl;
    return;
}

void printskylineset(set<pair<int, int>> &skylineset)
{
	for (auto it : skylineset)
	{
        cout<<it.first<<" "<<it.second<<endl; 
	}
    return;
}
int main()
{
    //taking the inputs
	// vector<segment> segments={segment(point(1,8), point(7,4)),segment(point(2,9), point(7,2)),segment(point(3,5), point(7,9))};
	int n;
	cin >> n;
	float a,b,c,d;
    vector<segment> segments;
	priority_queue<event, vector<event>, CompareEvents> events;
	for(int i=0; i<n; i++){
		cin >> a;
		cin >> b;
		cin >> c;
		cin >> d;
        a=_round(a);
        b=_round(b);
        c=_round(c);
        d=_round(d);
        segments.push_back(segment(point(a,b), point(c,d)));
        events.push(event(point(a,b), {i,-1}));
        events.push(event(point(c,d), {-1,i}));	
	}
	vector<pair<float,float>> skyline;
	vector<vector<int>> intsxns(n, vector<int>(n, 0));
	map<int, float> vals;//this is to get the last assumed y coordinate
	set<int> actives;
    RedBlackTree lines;
	while (!events.empty())
	{
		event this_event=events.top();
		events.pop();
		if (this_event.lines.first>-1 && this_event.lines.second>-1)
		{
            updateSLS(vals, lines, actives, this_event.p, segments, this_event.lines);
            Node* maxValNode = lines.maxValueNode(lines.root);
            if (fabs(maxValNode->key-this_event.p.y)<EPSILON)
            {
                skyline.push_back({this_event.p.x, this_event.p.y});
            }
            Node* pred = lines.predecessor(vals[this_event.lines.first]);
            Node* succ = lines.successor(vals[this_event.lines.first]);
            Node* this_node = lines.search(vals[this_event.lines.first]);
            if (pred!=nullptr)
            {
                assert(pred->values.size()>0);
                for (auto it: pred->values) 
                {
                    update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
                    update_intersection(intsxns, this_event.lines.second, it, segments, this_event.p.x, events);
                }
                // for (auto it: this_node->values) if (it!=this_event.lines.first) update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
            }
            if (succ!=nullptr)
            {
                assert(succ->values.size()>0);
                for (auto it: succ->values)
                {
                    update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
                    update_intersection(intsxns, this_event.lines.second, it, segments, this_event.p.x, events);
                }
                // for (auto it: this_node->values) if (it!=this_event.lines.first) update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
            }
		}
		else if (this_event.lines.first>-1)
		{
			//start of a segment
			actives.insert(this_event.lines.first);
			updateSLS(vals, lines, actives, this_event.p, segments, this_event.lines);
			// showstatus(lines, this_event.p.x);
            Node* pred = lines.predecessor(vals[this_event.lines.first]);
            Node* succ = lines.successor(vals[this_event.lines.first]);
            Node* this_node = lines.search(vals[this_event.lines.first]);
            // cout<<"--------------------->"<<endl;
            // lines.printRBT(lines.root);
            // cout<<actives.size()<<endl;
            // cout<<"--------------------->"<<endl;
            if (pred!=nullptr)
            {
                assert(pred->values.size()>0);
                for (auto it: pred->values) update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
            
            }
            if (succ!=nullptr)
            {
                assert(succ->values.size()>0);
                for (auto it: succ->values) update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
            }
            else
            {
                if (pred==nullptr)
                {
                    if (this_node->values.size()>1)
                    {
                        skyline.push_back({this_event.p.x,this_event.p.y});
                    }
                    else
                    {
                        skyline.push_back({this_event.p.x,0});
                        skyline.push_back({this_event.p.x,this_event.p.y});        

                    }
                }
                else
                {
                    if (this_node->values.size()>1)
                    {
                        skyline.push_back({this_event.p.x,this_event.p.y});
                    }
                    else
                    {
                        skyline.push_back({this_event.p.x,pred->key});
                        skyline.push_back({this_event.p.x,this_event.p.y});
                    }
                }
            }
        }
        else if (this_event.lines.second>-1)
		{
			//end of a segment
			updateSLS(vals, lines, actives, this_event.p, segments, this_event.lines);// we will erase the line from lines and actives manually
            Node* pred = lines.predecessor(vals[this_event.lines.second]);
            Node* succ = lines.successor(vals[this_event.lines.second]);
            Node* this_node = lines.search(vals[this_event.lines.second]);
            if (pred!=nullptr && succ!=nullptr) 
            {
                for (auto it: pred->values){
                    for (auto jt: pred->values)
                    {
                        update_intersection(intsxns, it, jt, segments, this_event.p.x, events);
                    }
                }
            }
            if (succ==nullptr)
            {
                if (this_node->values.size()>1)
                {
                    skyline.push_back({this_event.p.x,this_event.p.y});

                }else if (pred==nullptr)
                {
                    skyline.push_back({this_event.p.x,this_event.p.y});
                    if (actives.size()==1) {
                        skyline.push_back({this_event.p.x,0});
                    }
                }
                else if (pred!=nullptr)
                {
                    skyline.push_back({this_event.p.x, this_event.p.y});
                    skyline.push_back({this_event.p.x, pred->key});
                }
                else
                {
                    cout<<"err:4"<<endl;
                    exit(-1);
                }
            }
            actives.erase(this_event.lines.second);
			updateSLS(vals, lines, actives, this_event.p, segments, this_event.lines);
		}
		else
		{
			cout<<"err:2";
			exit(-1);
		}
    }
	// printskyline(skyline);
	printskylinef2(skyline);
    // printskylineset(skylineset);
    // showintsxns(intsxns);
    return 0;
}