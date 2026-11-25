/* Link.h
A Link is one connection between two towers. It knows its own cost,
time, power use and capacity
*/
#pragma once
class Link {
public:
 // from / to = tower numbers
 // tech = 4 or 5, length = km, mb = size of the message in MB,
 // newT = how many end towers this link has to build
 Link(int f, int t, int techType, double length, double mb, int newT);
 int GetFrom();
 int GetTo();
 int GetTech();
 double GetKm();
 int GetRelays();
 int GetNewTowers();
 double GetCost();     // in k$ (thousands of dollars)
 double GetTime();     // in milliseconds
 double GetPower();    // in kWh
 double GetCapacity(); // in Mbps
 double CostIf(int t);
 double TimeIf(int t);
 double PowerIf(int t);
 double CapacityIf(int t);
 
private:
 int from;
 int to;
 int tech;
 double km;
 double messageMB;
 int newTowers;
 int relays;
 double cost;
 double timeMs;
 double power;
 double capacity;
};

