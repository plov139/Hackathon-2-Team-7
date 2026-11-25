/* 
 A Tower is point on the map
 The class only knows about its name, where it is, what state it is in
 and how to draw itself

 state:  0  not connected (gray)
         1  START point   (yellow)
         4  reached by a 4G link (blue)
         5  reached by a 5G link (green)
*/
#pragma once

#include <string>
#include "raylib.h"

class Tower {
public:
    Tower(std::string n, float px, float py);

    std::string GetName();
    float GetX();
    float GetY();

    int GetState();
    void SetState(int newState);

    bool IsBuilt();              
    void SetBuilt(bool value);

    double GetTime();            // milliseconds for a message to get here from start
    void SetTime(double ms);

    bool IsClicked(float mouseX, float mouseY);   // is the mouse on top of this tower
    void Draw(bool isCurrent, bool isHovered);    // draw the tower with raylib

private:
    std::string name;
    float x;
    float y;
    int state;
    bool built;
    double time;

};
