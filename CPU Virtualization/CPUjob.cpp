/*
Mike Damman
CIS 450
Fall 2012

CPUjob.cpp
Definitions and member-functions for CPUjob class
Defines aspects of a single process being utilized by the CPU
*/
#include <string>
using namespace std;

class CPUjob
{
private:
	int pid, arrivalTime, serviceTime, addressSize, timeProcessed,
		completionTime, memoryLocation;
	char cpid;

public:
	CPUjob(int pd, int at, int st, int ms, char cp)		// constructor
	{
		pid = pd;
		arrivalTime = at;
		serviceTime = st;
		addressSize = ms;
		cpid = cp;
		timeProcessed = completionTime = 0;
	}

	int getPid()								// access/mutator functions
		{return pid;}
	int getArrTime()
		{return arrivalTime;}
	int getServTime()
		{return serviceTime;}
	int getAddSize()
		{return addressSize;}
	
	int getMemLoc()
		{return memoryLocation;}
	int getTimeProcd()
		{return timeProcessed;}
	int getCompTime()
		{return completionTime;}
	char getCpid()
		{return cpid;}

	void setMemLoc(int ml)
		{memoryLocation = ml;}
	void incTimeProcd()
		{timeProcessed++;}
	void setCompTime(int ct)
		{completionTime = ct;}
	void setCpid(char cp)
		{cpid = cp;}

	bool compareTimeProcd()						// compare time processed to service time
	{return timeProcessed == serviceTime;}

	int calcWaitTime(int clock)					// returns process wait time
	{
		if(arrivalTime > clock)					// job not ready
			return 0;
		else if(completionTime == 0)			// job ready but not finished
			return (clock - arrivalTime - timeProcessed + 1);
		return (completionTime - arrivalTime - timeProcessed + 1);	// job finished
	}
};