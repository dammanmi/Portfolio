/* 
Mike Damman
CIS 450
Fall 2012

CPUscheduler.cpp
Defines class and member functions for a CPU scheduler.
CPU scheduler simulates process memory management and
MLFQ CPU scheduling on CPUjob objects.
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include "CPUjob.cpp"

using namespace std;

class CPUscheduler
{
private:
	int clock;						// System clock
	vector<CPUjob*> Q1;				// Priority 1, 2, 3 Queues
	vector<CPUjob*> Q2;
	vector<CPUjob*> Q3;
	int q1Time, q2Time, q3Time;		// time spent on current process in queue
	int q1Index, q2Index, q3Index;	// index of current process in queue
	float hpSum;					// hole percentage sums
	int hpCount;					// number of hole percentages

	queue<CPUjob*> waitQ;			// wait queue
	vector<char> memory;			// memory storage
	int currentMemLoc;


public:
	CPUscheduler()					// CPU scheduler constructor
	{
		clock = 0;
		q1Time = q2Time = q3Time = 0;
		q1Index = q2Index = q3Index = 0;
		memory.assign(256, '_');
		currentMemLoc = -1;
		hpCount = 0;
		hpSum = 0;
	}

	int getClock()
		{return clock;}
	void incrementClock()
		{clock++;}

	void startNewJob(CPUjob * jptr)
	{
		waitQ.push(jptr);
	}

	int memoryScheduler(bool NF)	// performs memory scheduling
	{
		if(NF)						// if next-fit
			checkWaitQNF();
		else						// if worst-fit
			checkWaitQWF();

		if(!Q1.empty())								// Queue 1 has priority
		{
			q1Time++;
			Q1[q1Index]->incTimeProcd();			// do one time unit of work on current job
			if(Q1[q1Index]->compareTimeProcd())		// if job is done
			{
				Q1[q1Index]->setCompTime(clock);
				updateMem(Q1[q1Index]->getMemLoc(), Q1[q1Index]->getAddSize());		// remove job from memory
				Q1.erase(Q1.begin() + q1Index);		// remove job from queue
				if(q1Index >= (int)Q1.size())
					q1Index = 0;
				q1Time = 0;
				return 0;
			}
			else if(q1Time == 8)					// if queue 1 time slice is up
			{
				Q2.push_back(Q1[q1Index]);			// move job to queue 2
				Q1.erase(Q1.begin() + q1Index);
				if(q1Index >= (int)Q1.size())
					q1Index = 0;
				q1Time = 0;
				return 1;
			}
		}
		else if(!Q2.empty())						// queue 2 has priority
		{
			q2Time++;
			Q2[q2Index]->incTimeProcd();			// one time unit of work
			if(Q2[q2Index]->compareTimeProcd())		// if job is done
			{
				Q2[q2Index]->setCompTime(clock);
				updateMem(Q2[q2Index]->getMemLoc(), Q2[q2Index]->getAddSize());	// remove job from memory
				Q2.erase(Q2.begin() + q2Index);
				if(q2Index >= (int)Q2.size())
					q2Index = 0;
				q2Time = 0;
				return 0;
			}
			else if(q2Time == 16)					// if queue 2 time slice is up
			{
				Q3.push_back(Q2[q2Index]);			// move job to queue 3
				Q2.erase(Q2.begin() + q2Index);
				if(q2Index >= (int)Q2.size())
					q2Index = 0;
				q2Time = 0;
				return 1;
			}
		}
		else if(!Q3.empty())						// if queue 3 has priority
		{
			q3Time++;
			Q3[q3Index]->incTimeProcd();
			if(Q3[q3Index]->compareTimeProcd())		// if job done
			{
				Q3[q3Index]->setCompTime(clock);
				updateMem(Q3[q3Index]->getMemLoc(), Q3[q3Index]->getAddSize());		// remove from memory
				Q3.erase(Q3.begin() + q3Index);
				if(q3Index >= (int)Q3.size())
					q3Index = 0;
				q3Time = 0;
				return 0;
			}
			else if(q3Time == 32)					// if queue 3 time slice is up
			{
				q3Index++;							// select next job
				if(q3Index >= (int)Q1.size())
					q3Index = 0;
				q3Time = 0;
				return 1;
			}
		}
		return 2;
	}

	void checkWaitQNF()								// check wait queue (next-fit)
	{
		bool memSpace = true;
		while(!waitQ.empty() && memSpace)			// while queue has jobs and memory has space
		{
			memSpace = false;
			for(int i=currentMemLoc+1; i!=currentMemLoc; i++)	// loop around once from current memory location
			{
				CPUjob * jptr = waitQ.front();
				int base;
				int bound = 0;
				if(memory[i] == '_')				// find open blocks of memory
				{
					base = i;
					while(i < 256 && memory[i] == '_')
					{
						bound++;
						i++;
					}
				}
				if(i >= 255)						// reached the end of memory
					i = 0;
				if(bound >= jptr->getAddSize())		// if job fits in memory block
				{
					for(int j=base; j < base + jptr->getAddSize(); j++)
						memory[j] = jptr->getCpid();
					jptr->setMemLoc(base);
					currentMemLoc = base + jptr->getAddSize()-1;
					Q1.push_back(jptr);
					waitQ.pop();
					memSpace = true;
					cout << "New Process: " << jptr->getCpid() << endl;
					printMemAlloc();
					break;
				}
			}
		}
	}

	void checkWaitQWF()							// check wait queue (worst-fit)
	{
		while(!waitQ.empty())					// queue has jobs
		{
			CPUjob * jptr = waitQ.front();
			int highBase;
			int highBound = 0;
			for(int i=0; i<256; i++)			// loop once through memory
			{
				int base;
				int bound = 0;
				if(memory[i] == '_')			// find open memory blocks
				{
					base = i;
					while(i < 256 && memory[i] == '_')
					{
						bound++;
						i++;
					}
				}
				if(bound > highBound)			// find largest memory block
				{
					highBase = base;
					highBound = bound;
				}
			}
			if(highBound > jptr->getAddSize())	// if process fits in memory block
			{
				for(int j=highBase; j <= highBase + jptr->getAddSize(); j++)
					memory[j] = jptr->getCpid();
				jptr->setMemLoc(highBase);
				currentMemLoc = highBase + jptr->getAddSize();
				Q1.push_back(jptr);
				waitQ.pop();
				cout << "New Process: " << jptr->getCpid() << endl;
				printMemAlloc();
			}
			else
				break;
		}
	}

	void updateMem(int base, int bound)			// removes a finished job from memory
	{
		cout << "Process Finished: " << memory[base] << endl;
		for(int i=base; i<=base+bound; i++)
			memory[i] = '_';
		printMemAlloc();						// print memory map
	}
	
	float calcHolePercent()						// calculates memory hole percentage
	{
		float hp;
		int emptySlots = 0;
		for(int i=0; i<256; i++)
			if(memory[i] == '_')
				emptySlots++;
		hp = (float)emptySlots / 256;
		hpSum += hp;
		hpCount++;
		return hp * 100;
	}
	float avgHolePercent()						// calculates average hole percentage
	{
		return hpSum / hpCount * 100;
	}

	void printMemAlloc()						// prints memory map
	{
		int index = 0;
		cout << "000   ";
		for(; index<64; index++)
			cout << memory[index];
		cout << "\n064   ";
		for(; index<128; index++)
			cout << memory[index];
		cout << "\n128   ";
		for(; index<192; index++)
			cout << memory[index];
		cout << "\n256   ";
		for(; index<256; index++)
			cout << memory[index];

		cout << "\nHole Percentage: " << setprecision(3) << calcHolePercent() << "%\n\n\n";
	}
};