#ifndef PLAYER_HPP
#define PLAYER_HPP

#include<iostream>
#include<raylib.h>
#include <cmath>
#include <vector>

#include "map.hpp"

using namespace std;

class Player{

    Vector2 position;
    float speed = 2;
    float angle = 0;

    public:
        Player(Vector2 position);
        
        double radians(double angle);

        void actions(Map& map);

        void movement(Map map);

        void drawplayer();

        Vector2 rayCast(Vector2 start, Vector2 end, float angle, int lineSize);

        float getDist(Vector2 start, Vector2 end);

       vector<vector<float>> looksie(Map map);

       Vector2 getPosition();
       
       float getAngle();
       bool checkLineIntersect(Vector2 playerPos, Vector2 start, Vector2 end);

       float clamp(float value, float min, float max);

       void setPlayerPos(Vector2 pos);

       void checkIfMove(Map map, Vector2 newPos);


        void checkforDoor(Map& map);

};

#endif
