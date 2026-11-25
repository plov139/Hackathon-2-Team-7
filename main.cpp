/*
HOW TO PLAY
 type the size of your message
 click any point, it becomes the start point yellow
 press 4 for 4G blue or 5 for 5G green
 click a gray point, a link is built from the point with the white rings,
 the new point gets the colour of the link
 keep clicking, cost, time, power and capacity add up
 U = undo last link
 I or the INFO button = open the "how this applies to the real world" screen
 ESC = on the map close the window and print a receipt
       on the info screen go back to the map
*/
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "raylib.h"
#include "Tower.h"
#include "Link.h"
using namespace std;


// creates the 16 towers at fixed positions and
// puts them in a vector, towers index in this list is how Links refer to them
vector<Tower> MakeTowers() {
 vector<Tower> towers;
 towers.push_back(Tower("A", 110, 150));
 towers.push_back(Tower("B", 300, 95));
 towers.push_back(Tower("C", 470, 170));
 towers.push_back(Tower("D", 680, 100));
 towers.push_back(Tower("E", 900, 140));
 towers.push_back(Tower("F", 1080, 230));
 towers.push_back(Tower("G", 180, 330));
 towers.push_back(Tower("H", 380, 300));
 towers.push_back(Tower("I", 560, 360));
 towers.push_back(Tower("J", 760, 290));
 towers.push_back(Tower("K", 960, 380));
 towers.push_back(Tower("L", 120, 520));
 towers.push_back(Tower("M", 330, 500));
 towers.push_back(Tower("N", 560, 540));
 towers.push_back(Tower("O", 800, 520));
 towers.push_back(Tower("P", 1040, 540));
 return towers;
}

// DistanceKm is distance between two towers in km
// How it works pythagorean theorem gives the distance in pixels then we scale it 1 pixel on the screen = 0.075 km(To got this idea we use AI)
double DistanceKm(Tower a, Tower b) {
 double dx = a.GetX() - b.GetX();
 double dy = a.GetY() - b.GetY();
 double pixels = sqrt(dx * dx + dy * dy);
 return pixels * 0.075;
}

/*
Round1 turns a number into text with one decimal for example 14.83 makes 14.8.
It adding 0.05 first makes it ROUND instead of cut off
(int) removes the decimals to get the whole part, then we pull out the
first decimal digit by hand
*/ 
string Round(double value) {
 value = value + 0.05;
 int whole = (int)value;
 int tenth = (int)((value - whole) * 10);
 return to_string(whole) + "." + to_string(tenth);
}

// 4500 ms becomes 4.5 s, 450 ms stays 450 ms
string FormatTime(double ms) {
 if (ms >= 1000) {
  return Round(ms / 1000) + " s";
 } else {
  return to_string((int)ms) + " ms";
 }
}

// 2545.0 becomes 2545 k$ (k$ = thousands of dollars)
string FormatCost(double cost) {
 return to_string((int)cost) + " k$";
}

// 123.4 becomes 123.4 kWh
string FormatPower(double kwh) {
 return Round(kwh) + " kWh";
}

// 350.0 becomes 350 Mbps
string FormatCapacity(double mbps) {
 return to_string((int)mbps) + " Mbps";
}

// 4 becomes 4G, anything else becomes 5G
string TechName(int tech) {
 if (tech == 4) {
  return "4G";
 } else {
  return "5G";
 }
}

// 4G links are blue, 5G links are green
Color TechColor(int tech) {
 if (tech == 4) {
  return BLUE;
 } else {
  return GREEN;
 }
}


struct InfoLine {
 string text;
 int size;    
 Color color;
 int gap;   
};

/* 
DrawText does not wrap long text 
walk through the text one character at a time, collecting letters into a word 
When we hit a space we try adding the word to the current line 
MeasureText tells us how wide that line would be in pixels
if it is wider than maxWidth we save the current line and start a new one with the word 
returns the list of finished lines
*/
vector<InfoLine> WrapText(string text, int size, Color color, int gap, int maxWidth) {
 vector<InfoLine> lines;
 string current = "";   // the line we are building
 string word = "";      // the word we are building
 bool first = true;     // only the first line of a paragraph gets the gap above
 for (int i = 0; i <= (int)text.size(); i++) {
  // a word ends at a space or at the end of the text
  if (i == (int)text.size() || text[i] == ' ') {
   if (word != "") {
    string attempt = current;
    if (attempt != "") {
     attempt = attempt + " ";
    }
    attempt = attempt + word;
    if (MeasureText(attempt.c_str(), size) > maxWidth && current != "") {
     int lineGap = 0;
     if (first == true) {
      lineGap = gap;
     }
     lines.push_back({current, size, color, lineGap});
     first = false;
     current = word;
    } else {
     current = attempt; 
    }
    word = "";
   }
  } else {
   word = word + text[i]; 
  }
 }
 if (current != "") {
  int lineGap = 0;
  if (first == true) {
   lineGap = gap; 
  }
  lines.push_back({current, size, color, lineGap});
 }
 return lines;
}

/*
builds the whole info page as a list of InfoLines
we list every heading and paragraph with a kind
0 = big heading, 1 = sub heading, 2 = body text, then we wrap each one
with the right size and color and join all the lines into one big list
*/ 
vector<InfoLine> MakeInfoLines(int maxWidth) {
 vector<InfoLine> all;
 Color head = {255, 210, 90, 255};   // yellow
 Color sub = {130, 190, 255, 255};   // blue
 Color body = {225, 225, 225, 255};  // gray
 vector<InfoLine> part;

 struct Item { int kind; string text; };
 vector<Item> items = {
  {0, "Overview"},
  {2, "The internet is something billions of people use everyday but most don't understand the complexity behind the system that we use to communicate. Behind this whole simple interface of sending a message and communicating is a really complex system of physical servers and connections."},
  {1, "1. Abstraction"},
  {2, "Most internet users only see the applications and the websites they interact with and not the actual physical system behind it. Our project shows that there is a hidden layer of devices, servers and connections with the circles and lines. It shows how data moves between the digital world."},
  {1, "2. Efficiency"},
  {2, "Not all the network connections function at the same speed and have the same resources. Our project shows that the number of connections and speed can affect the cost."},
  {1, "3. Control"},
  {2, "The internet isn't owned just by one company. Instead a lot of different governments, organizations, companies and individuals work together to help operate it. This means that where our data goes is sometimes unknown to us and the route's information can depend on the organization controlling the networks involved. The project shows that there is vast infrastructure out there."},
  {0, "The Physical Infrastructure Behind the Internet"},
  {2, "Despite being described as a wireless / digital system, the internet relies heavily on its physical infrastructure."},
  {1, "Underwater Cables"},
  {2, "Submarine fiber-optic cables carry a large portion of the internet traffic and connect across continents."},
  {1, "Servers and Data Centers"},
  {2, "Applications and the web services rely on physical computers that process and distribute the information. These systems require their own facilities that use a lot of electricity, cooling and need a lot of maintenance. (One of the facilities can cost from hundreds of millions to 10 billion dollars)"},
  {1, "Vulnerability"},
  {2, "The physical infrastructure can be damaged sometimes so there need to be alternative routes to deliver information."},
  {1, "Unequal Access"},
  {2, "The Internet isn't available for everyone, there is different infrastructure in different parts of earth and for a lot of people their financial situation determines their access to good internet."}
 };

 // wrap every item with the style that matches its kind
 for (int i = 0; i < (int)items.size(); i++) {
  if (items[i].kind == 0) {
   part = WrapText(items[i].text, 28, head, 34, maxWidth);
  } else if (items[i].kind == 1) {
   part = WrapText(items[i].text, 22, sub, 22, maxWidth);
  } else {
   part = WrapText(items[i].text, 18, body, 8, maxWidth);
  }
  for (int j = 0; j < (int)part.size(); j++) {
   all.push_back(part[j]);
  }
 }
 return all;
}

/*
prints the final summary after the window is closed
loop over every link and add up the real totals, and add up
what the same links would have cost if all were 4G or all were 5G
that gives the 4G vs 5G comparison, which is our take on efficiency
*/
void PrintReceipt(vector<Tower> towers, vector<Link> links, double messageMB) {

 // if nothing built
 if (links.size() == 0) {
  cout << " You built no links. A network that does not exist costs 0 k$." << endl;
  return;
 }

 double total = 0;
 double totalTime = 0;
 double totalPower = 0;
 double totalCapacity = 0;
 int relays = 0;
 double cost4 = 0;  
 double cost5 = 0; 
 double time4 = 0;
 double time5 = 0;
 double power4 = 0;
 double power5 = 0;
 double cap4 = 0;
 double cap5 = 0;
 for (int i = 0; i < (int)links.size(); i++) {
  total = total + links[i].GetCost();
  totalTime = totalTime + links[i].GetTime();
  totalPower = totalPower + links[i].GetPower();
  totalCapacity = totalCapacity + links[i].GetCapacity();
  relays = relays + links[i].GetRelays();
  cost4 = cost4 + links[i].CostIf(4);
  cost5 = cost5 + links[i].CostIf(5);
  time4 = time4 + links[i].TimeIf(4);
  time5 = time5 + links[i].TimeIf(5);
  power4 = power4 + links[i].PowerIf(4);
  power5 = power5 + links[i].PowerIf(5);
  cap4 = cap4 + links[i].CapacityIf(4);
  cap5 = cap5 + links[i].CapacityIf(5);
 }

 cout << " Message size: " << Round(messageMB) << " MB" << endl;
 cout << " Links built: " << links.size() << endl;
 cout << " Relay towers the map never shows: " << relays << endl;
 cout << " Total cost: " << FormatCost(total) << endl;
 cout << " Total power: " << FormatPower(totalPower) << endl;
 cout << " Total capacity: " << FormatCapacity(totalCapacity) << endl;
 cout << " Time to reach the last point: " << FormatTime(totalTime) << endl;
 cout << endl;
 cout << "If every link was 4G: " << FormatCost(cost4) << ", " << FormatTime(time4)
      << ", " << FormatPower(power4) << ", " << FormatCapacity(cap4) << endl;
 cout << "If every link was 5G: " << FormatCost(cost5) << ", " << FormatTime(time5)
      << ", " << FormatPower(power5) << ", " << FormatCapacity(cap5) << endl;
 cout << endl;

 // towers still gray were never connected 
 cout << "Points that were left out: ";
 for (int i = 0; i < (int)towers.size(); i++) {
  if (towers[i].GetState() == 0) {
   cout << " " << towers[i].GetName();
  }
 }
 cout << endl;
}


int main() {
 // Ask for the message size if the user types 0 or a negative number
 // we use 20 MB as a default
 double messageMB;
 cout << "Money is in k$ (thousands of dollars)" << endl;
 cout << "How big is the message you want to send, in MB? ";
 cin >> messageMB;
 if (messageMB <= 0) {
  messageMB = 20;
 }

 vector<Tower> towers = MakeTowers();
 vector<Link> links;
 int startIndex = -1;
 int current = -1;
 int tech = 4;
 string message = "";
 int mapHeight = 600;  
 int windowW = 1200;
 int windowH = 780;
 bool showInfo = false; 
 bool quit = false;
 float scroll = 0; 

 InitWindow(windowW, windowH, "map");
 SetTargetFPS(60);
 // By default the window closes on ESC
 // We turn that off so that ESC can mean go back on the info screen
 SetExitKey(KEY_NULL);

 Color background = {15, 18, 26, 255}; 
 Rectangle infoButton = {600, (float)mapHeight + 64, 130, 30};
 Rectangle backButton = {1040, 16, 130, 36};

 vector<InfoLine> infoLines = MakeInfoLines(1100);
 int infoTop = 70; 
 int infoViewHeight = windowH - infoTop - 10; 
 int infoTotalHeight = 20;
 for (int i = 0; i < (int)infoLines.size(); i++) {
  infoTotalHeight = infoTotalHeight + infoLines[i].gap + infoLines[i].size + 6;
 }
 // maxScroll is how far we may scroll, all text height minus visible height
 float maxScroll = (float)(infoTotalHeight - infoViewHeight);
 if (maxScroll < 0) {
  maxScroll = 0; 
 }

 while (!WindowShouldClose() && !quit) {
  Vector2 mouse = GetMousePosition();
  bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT); 

  if (showInfo) {
   scroll = scroll - GetMouseWheelMove() * 40;
   if (IsKeyDown(KEY_DOWN)) { scroll = scroll + 6; }
   if (IsKeyDown(KEY_UP)) { scroll = scroll - 6; }
   if (scroll < 0) { scroll = 0; }
   if (scroll > maxScroll) { scroll = maxScroll; }
   if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_I) ||
       (clicked && CheckCollisionPointRec(mouse, backButton))) {
    showInfo = false;
   }
  } else {

   // which tower is the mouse on
   // hovered the tower number under the mouse or -1 for none
   // we only look when the mouse is above the info panel
   int hovered = -1;
   if (mouse.y < mapHeight) {
    for (int i = 0; i < (int)towers.size(); i++) {
     if (towers[i].IsClicked(mouse.x, mouse.y)) {
      hovered = i;
     }
    }
   }

   if (IsKeyPressed(KEY_ESCAPE)) {
    quit = true;
   }
   if (IsKeyPressed(KEY_I) || (clicked && CheckCollisionPointRec(mouse, infoButton))) {
    showInfo = true;
    scroll = 0; 
    clicked = false;
   }
   if (IsKeyPressed(KEY_FOUR)) {
    tech = 4;
   }
   if (IsKeyPressed(KEY_FIVE)) {
    tech = 5;
   }
   // take the last link turn the tower it reached back to gray
   // and unbuilt give the white rings back to the tower it started from
   // then delete the link from the list
   if (IsKeyPressed(KEY_U)) {
    if (links.size() > 0) {
     Link last = links[links.size() - 1];
     towers[last.GetTo()].SetState(0);
     towers[last.GetTo()].SetBuilt(false);
     towers[last.GetTo()].SetTime(0);
     if (last.GetNewTowers() == 2) {
      towers[last.GetFrom()].SetBuilt(false);
     }
     current = last.GetFrom();
     links.pop_back();
     message = "Last link removed.";
     cout << endl << "UNDO last link removed." << endl;
    } else {
     message = "Nothing to undo.";
    }
   }
   if (clicked && hovered != -1) {
    if (startIndex == -1) {
     startIndex = hovered;
     current = hovered;
     towers[hovered].SetState(1);
     message = "START is " + towers[hovered].GetName() + ". Press 4 or 5, then click another point.";
     cout << endl << "[START] " << towers[hovered].GetName() << endl;
    } else if (towers[hovered].GetState() != 0) {
     message = towers[hovered].GetName() + " is already used.";
    } else {
     double km = DistanceKm(towers[current], towers[hovered]);
     int newTowers = 1;
     if (towers[current].IsBuilt() == false) {
      newTowers = 2;
     }
     Link link(current, hovered, tech, km, messageMB, newTowers);
     links.push_back(link);
     towers[current].SetBuilt(true);
     towers[hovered].SetBuilt(true);
     towers[hovered].SetState(tech); 
     towers[hovered].SetTime(towers[current].GetTime() + link.GetTime());

     double total = 0;
     for (int i = 0; i < (int)links.size(); i++) {
      total = total + links[i].GetCost();
     }

     message = "Built " + TechName(tech) + " link to " + towers[hovered].GetName()
               + " for " + FormatCost(link.GetCost());
     current = hovered;
    }
   }
   // add up every link built 
   double totalCost = 0;
   double totalTime = 0;
   double totalPower = 0;
   double totalCapacity = 0;
   int relayCount = 0;
   for (int i = 0; i < (int)links.size(); i++) {
    totalCost = totalCost + links[i].GetCost();
    totalTime = totalTime + links[i].GetTime();
    totalPower = totalPower + links[i].GetPower();
    totalCapacity = totalCapacity + links[i].GetCapacity();
    relayCount = relayCount + links[i].GetRelays();
   }

   BeginDrawing();
   ClearBackground(background);

   for (int i = 0; i < (int)links.size(); i++) {
    Tower a = towers[links[i].GetFrom()];
    Tower b = towers[links[i].GetTo()];
    Color c = TechColor(links[i].GetTech());
    Vector2 p1 = {a.GetX(), a.GetY()};
    Vector2 p2 = {b.GetX(), b.GetY()};
    DrawLineEx(p1, p2, 4, c); 
    string label = TechName(links[i].GetTech()) + " " + to_string((int)links[i].GetKm()) + " km";
    DrawText(label.c_str(), (int)((a.GetX() + b.GetX()) / 2) - 25,
             (int)((a.GetY() + b.GetY()) / 2) - 22, 12, WHITE);
   }

   for (int i = 0; i < (int)towers.size(); i++) {
    towers[i].Draw(i == current, i == hovered);
   }
   DrawRectangle(0, mapHeight, windowW, windowH - mapHeight, {24, 28, 40, 255});
   if (startIndex == -1) {
    DrawText("Click a point to choose the START (yellow).", 20, mapHeight + 14, 18, WHITE);
   } else {
    DrawText("Press 4 or 5, then click a gray point.", 20, mapHeight + 14, 18, WHITE);
   }
   string techText = "Next link: " + TechName(tech) + " (key 4 = 4G blue, key 5 = 5G green)";
   DrawText(techText.c_str(), 20, mapHeight + 42, 16, TechColor(tech));
   DrawText("U = undo last link    ESC = finish", 20, mapHeight + 68, 14, WHITE);
   DrawText(message.c_str(), 20, mapHeight + 116, 15, WHITE);

   Color buttonColor = {50, 90, 160, 255};
   if (CheckCollisionPointRec(mouse, infoButton)) {
    buttonColor = {80, 130, 210, 255};
   }
   DrawRectangleRec(infoButton, buttonColor);
   DrawText("INFO (I)", (int)infoButton.x + 28, (int)infoButton.y + 7, 16, WHITE);

   DrawText("TOTAL COST", 760, mapHeight + 14, 14, LIGHTGRAY);
   DrawText(FormatCost(totalCost).c_str(), 760, mapHeight + 34, 30, WHITE);
   string stats = to_string((int)links.size()) + " link(s), " + to_string(relayCount) + " hidden relay tower(s)";
   DrawText(stats.c_str(), 760, mapHeight + 76, 14, LIGHTGRAY);

   DrawText("TIME TO LAST POINT", 980, mapHeight + 14, 14, LIGHTGRAY);
   DrawText(FormatTime(totalTime).c_str(), 980, mapHeight + 34, 30, WHITE);

   DrawText("TOTAL POWER", 760, mapHeight + 104, 14, LIGHTGRAY);
   DrawText(FormatPower(totalPower).c_str(), 760, mapHeight + 124, 24, WHITE);

   DrawText("TOTAL CAPACITY", 980, mapHeight + 104, 14, LIGHTGRAY);
   DrawText(FormatCapacity(totalCapacity).c_str(), 980, mapHeight + 124, 24, WHITE);

   DrawCircle(30, mapHeight + 160, 7, YELLOW);
   DrawText("start", 42, mapHeight + 152, 14, WHITE);
   DrawCircle(120, mapHeight + 160, 7, GRAY);
   DrawText("not linked", 132, mapHeight + 152, 14, WHITE);
   DrawCircle(250, mapHeight + 160, 7, BLUE);
   DrawText("4G", 262, mapHeight + 152, 14, WHITE);
   DrawCircle(320, mapHeight + 160, 7, GREEN);
   DrawText("5G", 332, mapHeight + 152, 14, WHITE);

   EndDrawing();
   continue; 
  }

  BeginDrawing();
  ClearBackground(background);
  DrawText("How this applies to the real world", 40, 18, 30, WHITE);

  Color buttonColor = {50, 90, 160, 255};
  if (CheckCollisionPointRec(mouse, backButton)) {
   buttonColor = {80, 130, 210, 255};
  }
  DrawRectangleRec(backButton, buttonColor);
  DrawText("< BACK", (int)backButton.x + 28, (int)backButton.y + 9, 18, WHITE);
  DrawText("scroll: mouse wheel or UP / DOWN keys    ESC = back", 40, 52, 12, GRAY);

  /*
  BeginScissorMode only draw inside a rectangle, so text that
  is scrolled above the top or below the bottom gets cut off instead of drawing over the title and button
  y starts at the top of the text area minus the scroll amount
  then each line is drawn under the previous one
  */
  BeginScissorMode(0, infoTop, windowW, infoViewHeight);
  float y = (float)infoTop - scroll;
  for (int i = 0; i < (int)infoLines.size(); i++) {
   y = y + infoLines[i].gap;
   DrawText(infoLines[i].text.c_str(), 50, (int)y, infoLines[i].size, infoLines[i].color);
   y = y + infoLines[i].size + 6;
  }
  EndScissorMode();
  EndDrawing();
 }
 
 CloseWindow(); 
 // the final receipt prints in the terminal
 PrintReceipt(towers, links, messageMB);
 return 0;
}
