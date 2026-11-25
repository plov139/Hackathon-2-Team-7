// Drawing functions 
#include "Tower.h"
using namespace std;

Tower::Tower(string n, float px, float py) {
 name = n;
 x = px;
 y = py;
 state = 0; // starts gray
 built = false;
 time = 0;
}

string Tower::GetName() { return name; }
float Tower::GetX() { return x; }
float Tower::GetY() { return y; }
int Tower::GetState() { return state; }
void Tower::SetState(int newState) { state = newState; }
bool Tower::IsBuilt() { return built; }
void Tower::SetBuilt(bool value) { built = value; }
double Tower::GetTime() { return time; }
void Tower::SetTime(double ms) { time = ms; }

bool Tower::IsClicked(float mouseX, float mouseY) {
 // distance between the mouse and the centre of the tower
 float dx = mouseX - x;
 float dy = mouseY - y;
 if (dx * dx + dy * dy <= 20 * 20) {
  return true;
 } else {
  return false;
 }
}

void Tower::Draw(bool isCurrent, bool isHovered) {
 // pick the colour from the state
 Color c = GRAY; 
 if (state == 1) {
  c = YELLOW;
 } else if (state == 4) {
  c = BLUE;
 } else if (state == 5) {
  c = GREEN;
 }
 DrawCircle((int)x, (int)y, 13, c);
 DrawCircle((int)x, (int)y, 4, BLACK);
 if (isHovered) {
  DrawCircleLines((int)x, (int)y, 19, LIGHTGRAY);
 }
 if (isCurrent) {
  DrawCircleLines((int)x, (int)y, 21, WHITE);
  DrawCircleLines((int)x, (int)y, 22, WHITE);
 }
 DrawText(name.c_str(), (int)x - 20, (int)y + 20, 14, LIGHTGRAY);
 if (state == 1) {
  DrawText("START", (int)x - 20, (int)y - 34, 14, YELLOW);
 }

}
