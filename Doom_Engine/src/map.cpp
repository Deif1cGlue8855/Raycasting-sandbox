#include<iostream>
#include<raylib.h>
#include <cmath>
#include <vector>
#include <fstream>

#include "map.hpp"

using namespace std;

Walls::Walls(float x1, float y1, float x2, float y2, int textureNum){
    start = {x1, y1};
    end = {x2, y2};
    this->textureNum = textureNum;
    openAmount = 0;
}

Vector2 Walls::getStart(){
    return start;
}

Vector2 Walls::getEnd(){
    return end;
}

int Walls::getTextureNum(){
    return textureNum;
}

float Walls::getOpenAmount(){
    return openAmount;
}

bool Walls::isOpeningCheck(){
    return this->isOpening;
}

void Walls::setOpenAmount(float OpenAmount){
    openAmount = OpenAmount;
    isOpening = true;
}

void Walls::setTexture(int num){
    textureNum = num;
}

Map::Map(){
}

void Map::update(){
    for(int i = 0; i < map.size(); i++){
        Walls& wall = map[i];
        if(wall.getTextureNum() == 4){
            if(wall.isOpeningCheck()){
                wall.setOpenAmount(0);
            }
        }
    }
}

void Map::loadMap(string fileName){
    ifstream MyFile("Maps/" + fileName);

    if(!MyFile){
        cout << "ERROR WHEN OPENING TXT FILE" << endl;
    }

    string line;

    while(getline (MyFile, line)){
        string numString;
        vector<int> nums;
        for(auto& x: line){
            if(x == ',' || x == '\n'){
                nums.push_back(stoi(numString));
                numString = "";
            }
            else{
                numString += x;
            }
        }

        Walls newWall(nums[0], nums[1], nums[2], nums[3], nums[4]);
        addWall(newWall);

        numString = "";
        nums.clear();
    }

    MyFile.close();
}

void Map::addWall(Walls wall){
    map.push_back(wall);
}

void Map::removeWall(int index){
    map.erase(map.begin() + index);
}

Walls Map::getWall(int index){
    return map[index];
}

void Map::drawMap(){
    for(auto& wall : map){
        DrawLineV(wall.getStart(), wall.getEnd(), WHITE);
    }
}

int Map::getMapSize(){
    return map.size();
}

void Map::saveMap(string fileName){
    ofstream file("Maps/" + fileName);

    file.clear();
    
    for(auto& walls : map){
        string line;
        line = to_string(int(walls.getStart().x)) + "," + to_string(int(walls.getStart().y)) + "," + to_string(int(walls.getEnd().x)) + "," + to_string(int(walls.getEnd().y)) + "," + to_string(walls.getTextureNum()) + ",\n";

        file << line;
    }

    file.close();

    cout << "Map saved successfully!" << endl;
}

vector<Walls> Map::getAllWalls(){
    return map;
}

void Map::setWallOpen(int index, int amountOpen){
    map[index].setOpenAmount(amountOpen); 
}
