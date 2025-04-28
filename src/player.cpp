#include "player.hpp"

#include <iostream>
#include "raylib.h"
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

Player::Player(Vector2 position){
    this->position = position;
}

double Player::radians(double angles){
    double const pi = 3.14159265359;
    return (angles * (pi / 180));
}

void Player::actions(Map& map){
    
    if(IsKeyPressed(KEY_E)){
        checkforDoor(map);
    }
}


void Player::movement(Map map){

    Vector2 newPos = position;

    if(IsKeyDown(KEY_UP)){ //Move forwards
        newPos.x += cos(radians(angle)) * speed;
        newPos.y += sin(radians(angle)) * speed;


        checkIfMove(map, newPos);

    }
    if(IsKeyDown(KEY_DOWN)){ //Move backwards
        newPos.x -= cos(radians(angle)) * speed;
        newPos.y -= sin(radians(angle)) * speed;

        checkIfMove(map, newPos);

    }
    if(IsKeyDown(KEY_D)){ //Move Right
        newPos.x += cos(radians(angle + 90)) * speed;
        newPos.y += sin(radians(angle + 90)) * speed;

        checkIfMove(map, newPos);

    }
    if(IsKeyDown(KEY_A)){//Move Left
        newPos.x -= cos(radians(angle + 90)) * speed;
        newPos.y -= sin(radians(angle + 90)) * speed;

        checkIfMove(map, newPos);
    }


    if(IsKeyDown(KEY_LEFT))angle -= 2.5; //look to the left
    if(IsKeyDown(KEY_RIGHT))angle += 2.5; //look to the right 
}

void Player::drawplayer(){
    DrawCircleV(position, 5, WHITE);
    DrawLineV(position, {static_cast<float>(position.x + cos(radians(angle)) * 1000), static_cast<float>(position.y + sin(radians(angle)) * 1000)}, WHITE);
}

Vector2 Player::rayCast(Vector2 start, Vector2 end, float ANGLE, int lineSize){
    Vector2 rayStart = position;
    Vector2 rayEnd = {static_cast<float>(position.x + cos(radians(ANGLE)) * lineSize), static_cast<float>(position.y + sin(radians(ANGLE)) * lineSize)};

    float x1 = start.x;
    float y1 = start.y;
    float x2 = end.x;
    float y2 = end.y;

    float x3 = rayStart.x;
    float y3 = rayStart.y;
    float x4 = rayEnd.x;
    float y4 = rayEnd.y;

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom ;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom ;

    if(t > 0 && t < 1 && u > 0){
        Vector2 point = {x1 + t * (x2 - x1), y1 + t * (y2 - y1)};
        
        return point;
    }
    return {-1,-1};

}

float Player::getDist(Vector2 start, Vector2 end){
    float changeX = pow(abs(end.x - start.x),2);
    float changeY = pow(abs(end.y - start.y),2);

    return sqrt(changeX + changeY);
}

vector<vector<float>> Player::looksie(Map map){
    vector<vector<float>> allWall;
    int count = 0;
    for(float i = -30; i < 30; i += 0.25){
        vector<vector<float>> wallsOnLine;
        float look = angle + i;
    
        for(int j = 0; j < map.getMapSize(); j++){
            Walls wallPos = map.getWall(j);
            Vector2 hitPos = rayCast(wallPos.getStart(), wallPos.getEnd(), look, 1000);
            
            
            if(hitPos.x != -1){
                float dist = getDist(position, hitPos) * cos(radians(look) - radians(angle));
                float wallTextureDist = getDist(wallPos.getStart(), hitPos);
                wallsOnLine.push_back({dist, static_cast<float>(count),wallTextureDist, static_cast<float>(wallPos.getTextureNum()), wallPos.getOpenAmount(), 0});
            }

        }
        sort(wallsOnLine.begin(), wallsOnLine.end(), [](const vector<float>& a, const vector<float>& b) {
            return a[0] < b[0]; 
        });
        
        allWall.push_back(wallsOnLine.front());

        count ++;

    }
    
    return allWall;
}

Vector2 Player::getPosition(){
    return position;
}

float Player::getAngle(){
    return angle;
}

bool Player::checkLineIntersect(Vector2 playerPos, Vector2 start, Vector2 end){
    float radius = 5;
    
    float distance;

    float Cx = playerPos.x;
    float Cy = playerPos.y;

    float x1 = start.x;
    float y1 = start.y;
    float x2 = end.x;
    float y2 = end.y;

    float dx = x2 - x1;
    float dy = y2 - y1;

    if(dx == 0 && dy == 0){
        distance = sqrt(pow((Cx - x1) ,2) + pow((Cy - y1), 2));
        return distance <= radius;
    }

    float t = ((Cx -     x1) * dx + (Cy - y1) * dy) / (pow(dx, 2) + pow(dy, 2));

    t = clamp(t, 0.0, 1.0);

    float x_p = x1 + t * dx;
    float y_p = y1 + t * dy;

    distance = sqrt(pow((x_p - Cx), 2) + pow((y_p - Cy), 2));

    return distance <= radius;

}

float Player::clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void Player::setPlayerPos(Vector2 pos){
    position = pos;
}

void Player::checkIfMove(Map map, Vector2 newPos){
    bool allow = true;
    for(auto& walls : map.getAllWalls()){
        if(checkLineIntersect(newPos, walls.getStart(), walls.getEnd())){
            allow = false;
            break;
        }
    }
    if(allow)position = newPos;
}

void Player::checkforDoor(Map& map){
    for(int i = 0; i < map.getMapSize(); i++){

        Walls doors = map.getWall(i);
        Vector2 val = rayCast(doors.getStart(),doors.getEnd(), angle, 10000);
        if(getDist(position, val) < 40){
            if( val.x != -1 && val.y != -1 && doors.getTextureNum() == 4){
                if(!doors.isOpeningCheck()){
                                        
                    map.setWallOpen(i, 1);
                    cout << "TEST FOR VALID EVERYTHING" << endl;
                }
            }
        }
        
    }
}
