#include <bits/stdc++.h>
// #include "bst.h"
using namespace std;

///this is bst.h

#include <iostream>
#include <vector>
#include <algorithm>
#define EPSILON 0.000001
struct Node {
    float key;
    std::vector<int> values;
    Node *left, *right;

    Node(float key, int value) : key(key), left(nullptr), right(nullptr) {
        values.push_back(value);
    }
};

class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { clearBST(root); }
    Node* root;

    Node* insert(Node* &node, float key, int value, Node* &desgn);
    Node* search(Node* node, float key);
    Node* minValueNode(Node* node);
    Node* maxValueNode(Node* node);
    Node* deleteNode(Node* root, float key, int value, bool &deleted);
    Node* insert(float key, int value);
    Node* search(float key);
    void erase(float key, int value);
    Node* predecessor(float key);
    Node* successor(float key);
    void printBST(Node* root);
    void clearBST(Node* &root);
};

Node* BinarySearchTree::insert(float key, int value) {
    Node* desgn;
    root = insert(root, key, value, desgn);
    // return search(root, key); // save extra search
    return desgn;
}

Node* BinarySearchTree::insert(Node* &node, float key, int value, Node* &desgn) {
    if (node == nullptr)
    {
        desgn=new Node(key, value);
        return desgn;
    }
    if (key < node->key)
        node->left = insert(node->left, key, value, desgn);
    else if (key > node->key)
        node->right = insert(node->right, key, value, desgn);
    else {
        // If key already exists, just append the value to the existing node
        node->values.push_back(value);
        desgn=node;
        return desgn; // Return the existing node
    }
    return node;
}


Node* BinarySearchTree::search(float key) {
    return search(root, key);
}

Node* BinarySearchTree::search(Node* node, float key) {
    if (node == nullptr || node->key == key)
        return node;

    if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

void BinarySearchTree::erase(float key, int value) {
    // std::cout<<"before deleting "<<key<<" "<<value<<" in:"<<std::endl;
    // printBST(root);
    bool deleted=false;
    root = deleteNode(root, key, value, deleted);
    // std::cout<<"after delete "<<key<<" "<<value<<" in:"<<std::endl;
    // printBST(root);
    // std::cout<<std::endl;
        // assert(0);
}

Node* BinarySearchTree::deleteNode(Node* node, float key, int value, bool &deleted) {
    if (node == nullptr)
        return node;

    if (key < node->key)
        node->left = deleteNode(node->left, key, value, deleted);
    else if (key > node->key)
        node->right = deleteNode(node->right, key, value, deleted);
    else {
        auto it = std::find(node->values.begin(), node->values.end(), value);
        if (it != node->values.end()) {
            node->values.erase(it);
            deleted = true; // Indicate successful deletion
        }

        if (node->values.empty())
        {
            if (node->left==nullptr)
            {
                Node* temp=node->right;
                if (node==root) root=temp;
                delete node;
                return temp;
            }
            else if (node->right==nullptr)
            {
                Node* temp=node->left;
                if (node==root) root=temp;
                delete node;
                return temp;
            }
            else{
                //both the children exist
                Node* succParent = node;
                Node* succ = node->right;
                while (succ->left != NULL) {
                succParent = succ;
                succ = succ->left;
                }
                if (succParent != node)
                succParent->left = succ->right;
                else
                succParent->right = succ->right;
                node->key=succ->key;
                for (auto it:succ->values)
                {
                    node->values.push_back(it);
                }
                succ->values.clear();
                delete succ;
            }
        }
    }
    return node;
}



Node* BinarySearchTree::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node* BinarySearchTree::maxValueNode(Node* node) {
    Node* current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}


Node* BinarySearchTree::predecessor(float key) {
    Node* current = root;
    Node* pred = nullptr;
    while (current != nullptr) {
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            pred = current;
            current = current->right;
        } else {
            if (current->left != nullptr) {
                pred = maxValueNode(current->left);
            }
            break;
        }
    }
    return pred;
}

Node* BinarySearchTree::successor(float key) {
    Node* current = root;
    Node* succ = nullptr;
    while (current != nullptr) {
        if (key < current->key) {
            succ = current;
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            if (current->right != nullptr) {
                succ = minValueNode(current->right);
            }
            break;
        }
    }
    return succ;
}

void BinarySearchTree::printBST(Node* root) {

    if (root == nullptr){
            // std::cout<<"tree empt!!"<<std::endl;
        return;
    }
    std::cout << "Key: " << root->key << ", Values:";
    for (int value : root->values) {
        std::cout << " " << value;
    }
    std::cout << std::endl;

    printBST(root->left);
    printBST(root->right);
}

void BinarySearchTree::clearBST(Node *&root) {
    if (root == nullptr)
        return;
    root->values.clear();
    // root->parent=nullptr;
    clearBST(root->left);
    clearBST(root->right);
    root->left=nullptr;
    root->right=nullptr;
    delete root;
    root = nullptr;
} 


///this is the end of bst.h






void halt(){
    return;
}

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
            // cout<<it.first<<" "<<it.second<<endl;
            area = area + 0.5*(it.second+lasty)*(it.first-lastx);
            if (fabs(lasty)>EPSILON || fabs(it.second)>EPSILON) shadow = shadow + it.first-lastx;
        }
        lastx=it.first;
        lasty=it.second;    
	}
    cout<<(int)shadow<<endl;
    cout<<(int)area<<endl;
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

float updateLine(map<int, float> &vals, BinarySearchTree &lines, set<int> &actives, vector<segment> &segments, int line, event &e)
{
    int it=line;
    float x_new = e.p.x;
    if (actives.find(it) != actives.end())//line is active
    {
            float y_new = (x_new-segments[it].l.x)*((segments[it].r.y-segments[it].l.y)/(segments[it].r.x-segments[it].l.x)) + segments[it].l.y;
            y_new=_round(y_new);
            if (it==e.lines.first || it==e.lines.second) y_new=e.p.y; //epsilon stitching off
            lines.erase(vals[it],it);
            vals[it]=y_new;
            lines.insert(vals[it], it);
            return vals[it];
    }
    else//line cannot be inactive
    {
            assert(false);//sanitycheck
            lines.erase(vals[it],it);
            return -1;
    }
    return -1;// to remove the warning
}

void getPredSucc(BinarySearchTree &lines, Node* &pred, Node* &succ, event &this_event, set<int> &actives, vector<segment> &segments, map<int, float> &vals)
{
            pred = lines.predecessor(this_event.p.y);
            succ = lines.successor(this_event.p.y);
            bool settled = false;
            set<int> updated;
            while (!settled)
            {
                vector<float> points;
                if (pred!=nullptr)
                {
                    for (auto it: pred->values)
                    {
                        if (!(updated.find(it) != updated.end()))
                        {
                            updated.insert(it);
                            float u = updateLine(vals, lines, actives, segments, it, this_event);
                            points.push_back(u);
                        }
                    }
                }
                if (succ!=nullptr)
                {
                    for (auto it: succ->values)
                    {
                        if (!(updated.find(it) != updated.end()))
                        {
                            updated.insert(it);
                            float u = updateLine(vals, lines, actives, segments, it, this_event);
                            points.push_back(u);
                        }
                    }
                }
                if (points.size()>0)
                {
                    pred=lines.predecessor(this_event.p.y);
                    succ=lines.successor(this_event.p.y);
                }
                else
                {
                    //we have found pred and succ
                    settled=true;
                }
            }
            return;
}

int main()
{
    //taking the inputs
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
    BinarySearchTree lines;
    // showevents(events);
	while (!events.empty())
	{
		event this_event=events.top();
		events.pop();
        // cout<<"e "<<this_event.p.x<<" "<<this_event.p.y<<endl;
        // if (fabs(this_event.p.x-273)<1) 
        // {
        //     lines.printBST(lines.root);
        //     halt();
        // }
		if (this_event.lines.first>-1 && this_event.lines.second>-1)
		{
            // if (fabs(this_event.p.x-515.126)<0.01) 
            // {
            //     lines.printBST(lines.root);
            //     halt();
            // }
            Node* this_node = lines.search(vals[this_event.lines.second]);
            Node* this_node2 = lines.search(vals[this_event.lines.first]);
            vector<int> values1 = this_node->values;
            vector<int> values2 = this_node2->values;
            for (auto it : values1)
            {
                updateLine(vals, lines, actives, segments, it, this_event);
            }
            for (auto it : values2)
            {
                updateLine(vals, lines, actives, segments, it, this_event);
            } 
            values1.clear();
            values2.clear();
            
            Node* pred = nullptr;
            Node* succ = nullptr;
            // getPredSucc(lines, pred, succ, this_event, actives, segments, vals);   
            pred = lines.predecessor(this_event.p.y);
            succ = lines.successor(this_event.p.y);
            if (pred!=nullptr) values1=pred->values;
            if (succ!=nullptr) values2=succ->values;


            if (succ==nullptr)
            {
                skyline.push_back({this_event.p.x, this_event.p.y});
            }

            if (pred!=nullptr)
            {
                assert(values1.size()>0);
                for (auto it: values1) 
                {
                    update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
                    update_intersection(intsxns, this_event.lines.second, it, segments, this_event.p.x, events);
                }
            }
            if (succ!=nullptr)
            {
                assert(values2.size()>0);
                for (auto it: values2)
                {
                    update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
                    update_intersection(intsxns, this_event.lines.second, it, segments, this_event.p.x, events);
                }
            }
            values1.clear();
            values2.clear();
		}
		else if (this_event.lines.first>-1)
		{
            vector<int> values1;
            vector<int> values2;
            // halt();
            // lines.printBST(lines.root);
			actives.insert(this_event.lines.first);
            Node* this_node = lines.insert(this_event.p.y, this_event.lines.first);
            // lines.printBST(lines.root);
            vals[this_event.lines.first]=this_event.p.y;

            Node* pred = nullptr;
            Node* succ = nullptr;
            getPredSucc(lines, pred, succ, this_event, actives, segments, vals);
            if (pred!=nullptr) values1=pred->values;
            if (succ!=nullptr) values2=succ->values;
            // lines.printBST(lines.root); 
            if (pred!=nullptr)
            {
                assert(pred->values.size()>0);
                for (auto it: values1) update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);     
            }
            if (succ!=nullptr)
            {
                assert(succ->values.size()>0);
                for (auto it: values2) update_intersection(intsxns, this_event.lines.first, it, segments, this_event.p.x, events);
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
            vector<int> values1;
            vector<int> values2;
			//end of a segment
            lines.erase(vals[this_event.lines.second], this_event.lines.second);
            vals[this_event.lines.second]=this_event.p.y;
            Node* this_node = lines.insert(this_event.p.y, this_event.lines.second);
            // lines.printBST(lines.root);

            Node* pred = nullptr;
            Node* succ = nullptr;
            getPredSucc(lines, pred, succ, this_event, actives, segments, vals);
            if (pred!=nullptr) values1=pred->values;
            if (succ!=nullptr) values2=succ->values;        
            // lines.printBST(lines.root);


            if (pred!=nullptr && succ!=nullptr) 
            {
                for (auto it: values1){
                    for (auto jt: values2)
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
            lines.erase(vals[this_event.lines.second], this_event.lines.second);
		}
		else
		{
			cout<<"err:2";
			exit(-1);
		}
    }
    // showintsxns(intsxns);
	printskylinef2(skyline);
    return 0;
}