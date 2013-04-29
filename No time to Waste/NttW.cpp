/*
	NttW.cpp
	Written by Mike Damman
	Last updated 11/15/11
	Purpose:	To read input of a group of intersections, each with a set of routes 
				leading to another intersection and average delay times to proceed along that route.
				Given this information, to use Dijkstra's Algorithm and the optimum data structure 
				for both time and space usage to find the shortest route and average delay time 
				travelling from one	given intersection to another.
	Input:		For each group: number of intersections, number of routes from each intersection,
				destination of each route, start and end intersections.
	Output:		Route of ordered intersections to pass through, minimum delay time.
	Known Bugs:	Given data where an alternate path from the start to the end can be obtained
				before the optimum path, program ends with initial path.  This is because of 
				the way the program chooses the next intersection to count as visited.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

struct Route		// struct to define routes
{
	Route(int x = 0, int y = 0, int dl = 0)
		:from(x), to(y), delay(dl){}
	int from;
	int to;
	int delay;
};
// struct to define comparison operator for priority queue comparison of routes
struct myGreater : binary_function <Route, Route, bool> {
	bool operator() (const Route& lhs, const Route& rhs) const
	{return lhs.delay > rhs.delay;}
};
struct Answer		// struct to define answer information
{
	stack<int> path;	// series of routes
	int time;			// minimum delay
};

// struct to define Intersections
struct Intersection
{
	Intersection(int t = 0, int p = 0, bool vis = false)
		:time(t), prev(p), visited(vis){}
	int time;				// time from starting intersection
	int prev;				// index of previous intersection
	bool visited;
	vector<Route> routeV;	//vector to hold all of intersection's routes
};
Answer findPath(vector<Intersection> *interV, int start, int end);
void printAnswer(queue<Answer>);

int main()
{
	int numInter;			// number of intersections
	queue<Answer> answerQ;	// hold answers for output at the end
	cin >> numInter;

	while(numInter != 0)	// while still groups to process
	{
		int start, end;		// start and end intersections
		vector<Intersection> interV(numInter+1);	// vector to hold intersections
		vector<Intersection> *interPtr = &interV;
		for(int i=1; i<numInter+1; i++)				// read in intersection data
		{
			int numRoute;	// number of routes for an intersection
			Route inRoute;
			cin >> numRoute;
			for(int j=0; j<numRoute; j++)			// read in route data
			{
				cin >> inRoute.to >> inRoute.delay;
				inRoute.from = i;
				interV[i].routeV.push_back(inRoute);
			}
		}
		cin >> start >> end;
		answerQ.push(findPath(interPtr, start, end));	// run findPath function and insert answer into queue
		cin >> numInter;								// read in next number of intersections
	}
	
	printAnswer(answerQ);	// print answers from queue
	system("Pause");
	return 0;
}

/* Name:		findPath
   Purpose:		To take a group of intersections and routes and utilize Dijkstra's algorithm
				to map out the shortest path from the starting intersection to every other intersection
				until the designated ending intersection is found.
   Pre:			Pointer to a vector of all input intersections and routes for a given group, 
				indexes of starting and ending intersections.
   Post:		An answer struct containing the optimum path and minimum delay time.
   Called by:	main
   Calls:		None
*/
Answer findPath(vector<Intersection> *interV, int start, int end)
{
	Intersection *current;			// used to process Intersections
	priority_queue<Route, vector<Route>, myGreater> routeQ;		// priority queue to hold routes
	Answer ans;
	current = &(*interV)[start];	// begin with current as starting intersection
	current->time = 0;
	current->visited = true;

	while(!(*interV)[end].visited)	// while ending intersection not found
	{
		for(int i=0; i<(int)current->routeV.size(); i++)	// push all routes leading from current into priority queue
			routeQ.push(current->routeV[i]);
		while((*interV)[routeQ.top().to].visited)			// if route's destination is already visited
		{
			// if route time + prev intersection time < next intersection time
			if(routeQ.top().delay +  (*interV)[routeQ.top().from].time < (*interV)[routeQ.top().to].time)
			{
				(*interV)[routeQ.top().to].time = routeQ.top().delay +  (*interV)[routeQ.top().from].time;	// set new travel time
				(*interV)[routeQ.top().to].prev = routeQ.top().from;		// set previous intersection
			}
			routeQ.pop();			// remove used route from queue
		}
		current = &(*interV)[routeQ.top().to];	// set next intersection as current and visited
		current->visited = true;
		current->prev = routeQ.top().from;		// set previous
		current->time = routeQ.top().delay + (*interV)[routeQ.top().from].time;	// set time
		routeQ.pop();							// pop route
	}

	current = &(*interV)[end];
	ans.time = current->time;					// answer time
	ans.path.push(end);							// push ending intersection into stack
	while(current->time != 0)					// push preceeding intersections into stack
	{
		ans.path.push(current->prev);
		current = &(*interV)[current->prev];
	}
	return ans;
}

/* Name:		printAnswer
   Purpose:		To take a group of intersections and routes and utilize Dijkstra's algorithm
				to map out the shortest path from the starting intersection to every other intersection
				until the designated ending intersection is found.
   Pre:			Queue containing completed answers for each group of intersections processed
   Post:		Formatted answer output to the screen
   Called by:	main
   Calls:		None
*/
void printAnswer(queue<Answer> ans1)
{
	int count = 1;
	while(!ans1.empty())				// while still answers
	{
		Answer current = ans1.front();
		ans1.pop();
		cout << "Case " << count++ << ":   ";
		while(!current.path.empty())	// while still routes
		{
			cout << current.path.top() << " ";
			current.path.pop();
		}
		cout << "; " << current.time << endl;
	}
}