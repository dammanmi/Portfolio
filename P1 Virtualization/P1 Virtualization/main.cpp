/* 
Mike Damman
CIS 450
Fall 2012

main.cpp
Provides a driver function for CPUscheduler
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include "CPUscheduler.cpp"

using namespace std;

void printJobStates(vector<CPUjob> myJobs, int clock);

int main()
{
	bool nextFit = false;
	CPUscheduler myCPU;
	vector<CPUjob> myJobs;
	int jobIndex, cpidIndex, completedJobs;
	jobIndex = cpidIndex = completedJobs = 0;
	int totalWait = 0;
	int fit;
	string infileName;
	char cpidArray[70] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',	// sequence of character IDs
		'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
		'A', 'B', 'C', 'D',	'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
		'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', 
		'4', '5', '6', '7', '8', '9', '@', '#', '$', '%', '&', '*', '+', '?'};

	cout << "Enter 0 for next-fit, 1 for worst-fit: ";
	cin >> fit;
	if(fit == 0)			// if next-fit argument is passed
		nextFit = true;
	cout << "\nEnter input file name: ";
	cin >> infileName;
	ifstream dataFile(infileName);
	while(dataFile.good())					// retrieve jobs from data file
	{
		int pid, at, st, as;
		string line;
		getline(dataFile, line);
		stringstream ss(line);
		ss >> pid >> at >> st >> as;

		CPUjob cj(pid, at, st, as, cpidArray[cpidIndex]);
		myJobs.push_back(cj);
		cpidIndex++;
	}
	dataFile.close();

	while(completedJobs < (int)myJobs.size())	// loop while not all jobs are finished
	{
		if(jobIndex < (int)myJobs.size() && myJobs[jobIndex].getArrTime() == myCPU.getClock())	// if new job is available
		{
			CPUjob * jptr = &(myJobs[jobIndex++]);
			myCPU.startNewJob(jptr);
		}

		int jobState = myCPU.memoryScheduler(nextFit);		// run memory scheduler
		if(jobState != 2)								// if context switch or job finished
		{
			printJobStates(myJobs, myCPU.getClock());
			if(jobState == 0)							// if job finished
				completedJobs++;
		}
		myCPU.incrementClock();							// increment time clock
	}	// end loop

	for(int i=0; i<(int)myJobs.size(); i++)				// calculate average wait time
		totalWait += myJobs[i].calcWaitTime(myCPU.getClock());

	printJobStates(myJobs, myCPU.getClock());
	cout << "Total simulated time units: " << myCPU.getClock() << endl		// print statistics
		<< "Total number of jobs: " << myJobs.size() << endl
		<< "Average hole percentage: " << myCPU.avgHolePercent() << endl
		<< "Average waiting Time: " << setprecision(0) 
		<< (float)totalWait/(float)myJobs.size() << "\n\n";

	system("Pause");
	return 0;
}

void printJobStates(vector<CPUjob> myJobs, int clock)		// prints list of jobs and their current stats
{
	cout << "The Process States at Time " << clock << endl;
	cout << "---------+-------+--------+--------+----------\n";
	cout << "Job#     |Arrival|Running |Waiting |Completion\n";
	cout << "         |Time   |Duration|Duration|Time\n";
	cout << "---------+-------+--------+--------+----------\n";
	for(int i=0; i<(int)myJobs.size(); i++)
	{
		CPUjob mj = myJobs[i];
		cout << left << setw(9) << mj.getPid() << "|" <<  setw(7) << mj.getArrTime() << "|" 
			<< setw(8) << mj.getTimeProcd() << "|" << setw(8) << mj.calcWaitTime(clock) << "|"
			<< setw(8) << mj.getCompTime() << endl;
	}
	cout << "---------+-------+--------+--------+----------\n\n\n";
	system("Pause");
}