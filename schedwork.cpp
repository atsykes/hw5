#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
void makeAll(const AvailabilityMatrix& avail, const size_t dailyNeed, DailySchedule& sched,map<size_t, vector<vector<Worker_T>>>& m, size_t day); // Creates a map of all possible workers for each day
void scheduleOnDay(const AvailabilityMatrix& avail, const size_t dailyNeed, vector<Worker_T> sched, vector<vector<Worker_T>>& possibleSchedules,int day); // Creates all possible workers for a given day
bool alreadyScheduled(int day, size_t nurse, vector<Worker_T> sched, size_t i); // Checks if a worker has already been scheduled on a day
bool joinPossibleDays(map<size_t, vector<vector<Worker_T>>>& m, vector<size_t>& numShifts, const size_t& maxShifts, DailySchedule& sched, size_t day); // Given the possible schedules of each day, generates a possible schedule given the restrictions
bool checkAndUpdateNumShifts(vector<Worker_T> nurses, vector<size_t>& numShifts, const size_t& maxShifts, size_t i); // Updates the amount of shifts that the nurses have


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();

    // Add your code below
    vector<size_t> numShifts;
    for (size_t i=0; i < avail[0].size(); ++i)
        numShifts.push_back(0);
    map<size_t, vector<vector<Worker_T>>> m;
    makeAll(avail, dailyNeed, sched, m, 0);

    return joinPossibleDays(m, numShifts, maxShifts, sched, 0);
}

void makeAll(const AvailabilityMatrix& avail, const size_t dailyNeed, DailySchedule& sched,map<size_t, vector<vector<Worker_T>>>& m, size_t day)
{
    if (day == avail.size()) return;

    vector<Worker_T> dummy;
    vector<vector<Worker_T>> possibleSchedules;
    scheduleOnDay(avail, dailyNeed, dummy, possibleSchedules, day);

    // Inserts the possible workers of each day into a map
    pair<size_t, vector<vector<Worker_T>>> p(day, possibleSchedules);
    m.insert(p);

    makeAll(avail, dailyNeed, sched, m, day+1);
    return;
}


void scheduleOnDay(const AvailabilityMatrix& avail, const size_t dailyNeed, vector<Worker_T> sched, vector<vector<Worker_T>>& possibleSchedules,int day)
{
    if (sched.size() == dailyNeed) 
    {
        possibleSchedules.push_back(sched);
        return;
    }

    // Gets all possible combinations of nurses for each day
    for (size_t nurse=0; nurse < avail[day].size(); ++nurse)
    {
        if (avail[day][nurse] && !alreadyScheduled(day, nurse, sched, 0) && sched.size() < dailyNeed)
        {
            sched.push_back(nurse);
            scheduleOnDay(avail, dailyNeed, sched, possibleSchedules, day);
            sched.pop_back();
        }
    }

    // If it gets here, all possible combinations for 1 day have been found
    return;
}

bool alreadyScheduled(int day, size_t nurse, vector<Worker_T> sched, size_t i)
{
    if (i >= sched.size()) return false;

    if (nurse == sched[i]) return true;
    else return alreadyScheduled(day, nurse, sched, i+1);
}

bool joinPossibleDays(map<size_t, vector<vector<Worker_T>>>& m, vector<size_t>& numShifts, const size_t& maxShifts, DailySchedule& sched, size_t day)
{
    map<size_t, vector<vector<Worker_T>>>::iterator it = m.find(day);
    if (it == m.end()) return true;

    for (size_t i=0; i < it->second.size(); ++i)
    {
        vector<Worker_T> nurses = it->second[i];
        vector<size_t> alteredNumShifts = numShifts;
        sched.push_back(nurses);
        DailySchedule alteredSched = sched;

        // Joins all possible workers of each day together, making sure that they are meeting the conditions
        if (checkAndUpdateNumShifts(nurses, alteredNumShifts, maxShifts, 0))
        {
            if (joinPossibleDays(m, alteredNumShifts, maxShifts, alteredSched, day+1)) 
            {
                sched = alteredSched;
                return true;
            }
        }
        sched.pop_back();
    }

    return false;
}

bool checkAndUpdateNumShifts(vector<Worker_T> nurses, vector<size_t>& numShifts, const size_t& maxShifts, size_t i)
{
    if (i == nurses.size()) return true;

    Worker_T current = nurses[i];
    if (numShifts[current] + 1 > maxShifts) return false;
    numShifts[current]++;
    return checkAndUpdateNumShifts(nurses, numShifts, maxShifts, i+1);
}