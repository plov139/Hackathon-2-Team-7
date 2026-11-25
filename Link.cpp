#include "Link.h"
#include <cmath>

/*
The idea that 5G is faster but reaches less far is a design choice of ours
Data transfer pricing idea Amazon AWS pricing pages
https://aws.amazon.com/ec2/pricing/on-demand/
*/

// cost in k$
// 4G cost = 450 + (45 * km)      5G cost = 1200 + (90 * km)
static double BaseCostOf(int tech) {
 if (tech == 4) { 
  return 450.0; 
} else { 
  return 1200.0; }
}
static double KmCostOf(int tech) {
 if (tech == 4) { 
  return 45.0;
} else { return 90.0; }
}

// power in kWh 
// 4G kWh = 12 + (1.5 * km)       5G kWh = 35 + (3.5 * km)
static double BasePowerOf(int tech) {
 if (tech == 4) { 
  return 12.0; 
} else { 
  return 35.0; }
}
static double KmPowerOf(int tech) {
 if (tech == 4) { 
  return 1.5; 
} else { 
  return 3.5; }
}

// capacity in Mbps
// 4G Mbps = 50 + (5 * km)        5G Mbps = 300 + (20 * km)
// Capacity is how much data the link can carry at once
static double BaseCapacityOf(int tech) {
 if (tech == 4) { 
  return 50.0; 
} else { 
  return 300.0; }
}
static double KmCapacityOf(int tech) {
 if (tech == 4) { 
  return 5.0; 
} else { 
  return 20.0; }
}

// range how far one segment can go before a relay tower is needed
// 5G reaches less far than 4G, so it needs more relays on the same distance
static double RangeOf(int tech) {
 if (tech == 4) { 
  return 8.0; 
} else { 
    return 3.0; }
}

// speed used for the time calculation in megabits per second
static double SpeedOf(int tech) {
 if (tech == 4) { 
  return 40.0; 
} else { 
  return 400.0; }
}

// latency delay added by every segment in milliseconds
static double LatencyOf(int tech) {
 if (tech == 4) { 
  return 40.0; 
} else { 
  return 10.0; }
}

// RELAYS: number of relay towers needed for a link of this length.
// How it works: split the link into segments the size of the range,
// the towers BETWEEN the segments are the relays.
// Example: 14 km with range 3 km = ceil(14/3) = 5 segments, so 4 relays.
static int RelaysFor(double km, int tech) {
 int segments = (int)ceil(km / RangeOf(tech));
 int r = segments - 1;
 if (r < 0) { // a link of length 0 would give -1, so keep it at 0
  r = 0;
 }
 return r;
}

// -----------------------------------------------------------------------
// CONSTRUCTOR
// "Link::" means "this function belongs to the Link class".
// Copies the inputs into the variables, then calculates every result ONCE
// and stores it, so GetCost() etc. just hand back the saved number.
Link::Link(int f, int t, int techType, double length, double mb, int newT) {
 from = f;
 to = t;
 tech = techType;
 km = length;
 messageMB = mb;
 newTowers = newT;
 relays = RelaysFor(km, tech);
 cost = CostIf(tech);         // uses the cost formula for THIS link's tech
 timeMs = TimeIf(tech);
 power = PowerIf(tech);
 capacity = CapacityIf(tech);
}

int Link::GetFrom() { return from; }
int Link::GetTo() { return to; }
int Link::GetTech() { return tech; }
double Link::GetKm() { return km; }
int Link::GetRelays() { return relays; }
int Link::GetNewTowers() { return newTowers; }
double Link::GetCost() { return cost; }
double Link::GetTime() { return timeMs; }
double Link::GetPower() { return power; }
double Link::GetCapacity() { return capacity; }

// base price + price per km times the length of this link
double Link::CostIf(int t) {
 return BaseCostOf(t) + KmCostOf(t) * km;
}

// time how long the message takes to cross this link in milliseconds
// the message is in MB, speed is in megabits per second, so we convert MB * 8 / speed * 1000 = ms
// we add a small delay for every segment 
double Link::TimeIf(int t) {
 int r = RelaysFor(km, t);
 double sendTime = messageMB * 8.0 / SpeedOf(t) * 1000.0;
 return sendTime + LatencyOf(t) * (r + 1);
}

// base energy + energy per km times the length of this link
double Link::PowerIf(int t) {
 return BasePowerOf(t) + KmPowerOf(t) * km;
}

// base capacity + capacity per km times the length of this link
double Link::CapacityIf(int t) {
 return BaseCapacityOf(t) + KmCapacityOf(t) * km;
}
