#ifndef MAP_HPP
#define MAP_HPP

#include<iostream>
#include<raylib.h>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

class Walls{

    Vector2 start;
    Vector2 end;
    int textureNum;
    bool isOpening = false;
    float openAmount = 0;

    public:
        Walls(float x1, float y1, float x2, float y2, int textureNum);

        Vector2 getStart();
        Vector2 getEnd();

        int getTextureNum();

        float getOpenAmount();

        bool isOpeningCheck();

        void setOpenAmount(float amount);

        void setTexture(int num);
};

class Map{

    vector<Walls> map;

    public:
        Map();

        void update();

        void loadMap(string fileName);

        void addWall(Walls wall);

        Walls getWall(int index);
        void removeWall(int index);

        void drawMap();

        int getMapSize();

        void saveMap(string fileName);

        vector<Walls> getAllWalls();
        void setWallOpen(int index, int amountOpen);
};

#endif
