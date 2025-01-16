#include <raylib.h>
#include <iostream>
#include <cmath>
#include "player.hpp"
#include "map.hpp"

int projPlaneDist = 300;
int sliceHeights = 128;

bool editor = true;

int textureNum = 2;

bool placingLine = false;
bool delLine = false;
Vector2 startPos;

double radians(double angles){
    double const pi = 3.14159265359;
    return (angles * (pi / 180));
}

int main()
{

    Map map;
    const int screenWidth = 1200;
    const int screenHeight = 1000;

    Player player({screenWidth / 2, screenHeight / 3});

    InitWindow(screenWidth, screenHeight, "Raycasting (again)");
    SetTargetFPS(60);
    
    string fileName = "map1.txt";

    map.loadMap(fileName);

    Texture2D mossyBrick = LoadTexture("images/139.png");
    Texture2D brickWall = LoadTexture("images/078.png");
    Texture2D concreteWall = LoadTexture("images/522.png");

    while (!WindowShouldClose()){

        DrawFPS(10, 10);
        //Main raycasting
        if(!editor){
            player.movement(map);
            //player.actions(map);
            map.update();

            vector<vector<float>> allWalls = player.looksie(map);

            //cout << allWalls.size() << endl;

            BeginDrawing();
            //////////////////////////////////
            DrawRectangle(0,500,1200,500,Color{50,50,50,255});

            int prevCol = -1;
            for(auto& x : allWalls){
                if(x[1] != prevCol){
                    float projWallHeight =  (sliceHeights / x[0]) * projPlaneDist;

                    float height = projWallHeight + x[4];
                    if(height < 0){height = 0;}

                    Rectangle wallRect = { x[1] * 5 , (screenHeight / 2) - (projWallHeight / 2), 5, height};

                    int slide = static_cast<int>(x[2]) % 256;

                    int gray = 255 - (x[0] / 3);

                    if(gray > 255) gray = 255;
                    if(gray < 0) gray = 0;
                    Color shade = {gray, gray, gray, 255};

                    Texture2D texture;

                    switch(static_cast<int>(x[3])){
                        case 1:
                            texture = mossyBrick;
                            break;
                        case 2:
                            texture = brickWall;
                            break;
                        case 3:
                            texture = concreteWall;
                            break;

                    }  

                    DrawTexturePro(texture,{static_cast<float>(slide),0, 1, static_cast<float>(texture.height)},wallRect, {0,0}, 0.0f, shade);                  
      
                }

                prevCol = x[1];
            }

            //////////////////////////////////
            ClearBackground(BLACK);
            EndDrawing();
        }
        //EDITOR
        else{
            BeginDrawing();
            // DRAWING BACKGROUND AND GRID
            for(int i = 0; i < 1200; i += 10){
                DrawLine(i, 0, i, 1000, BLACK);
                DrawLine(0, i, 1200, i, BLACK);
            }
            //Drawing the map
            for(int i = 0; i < map.getMapSize(); i++){
                Walls wall = map.getWall(i);
                Color lineCol;

                switch(wall.getTextureNum()){
                    case 1:
                        lineCol = DARKGREEN; 
                        break;
                    case 2:
                        lineCol = BROWN; 
                        break;
                    case 3:
                        lineCol = GRAY; 
                        break;

                }
                DrawLineV(wall.getStart(), wall.getEnd(), lineCol);
            }

            string name;
            Color drawLineCol;

            switch(textureNum){
                case 1:
                    name = "Mossy Brick";
                    drawLineCol = DARKGREEN; 
                    break;
                case 2:
                    name = "Brick Wall";
                    drawLineCol = BROWN; 
                    break;
                case 3:
                    name = "Concrete Wall";
                    drawLineCol = GRAY; 
                    break;
            }

            Vector2 mousePos1 = GetMousePosition();
            Vector2 mousePos;
            mousePos.x = 10 * (floor(mousePos1.x / 10));
            mousePos.y = 10 * (floor(mousePos1.y / 10));
            //Drawing a new wall
            if(!placingLine && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                placingLine = true;
                startPos = mousePos;
            }
            if(placingLine){
                DrawLineV(startPos, mousePos, drawLineCol);
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && startPos.x == mousePos.x && startPos.y == mousePos.y){
                    placingLine = false;
                }

                else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                    placingLine = false;
                    Walls newWall(startPos.x, startPos.y, mousePos.x, mousePos.y, textureNum);
                    map.addWall(newWall);
                }
            }
            //Deleting wall
            if(!delLine && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                delLine = true;
                startPos = mousePos;
            }
            if(delLine){
                DrawRectangleV(startPos, {mousePos.x - startPos.x, mousePos.y - startPos.y}, Color{100,100,150,100});
                if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)){
                    delLine = false;
                    for(int i=0; i<map.getMapSize(); i++){
                        Walls wall = map.getWall(i);
                        Vector2 start = wall.getStart();
                        Vector2 end = wall.getEnd();
                        if(start.x > startPos.x && start.y > startPos.y && end.x < mousePos.x && end.y < mousePos.y){
                            map.removeWall(i);
                        }
                    }
                }
            }
            //TP player
            if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
                player.setPlayerPos(mousePos1);
            }
            //Draw player and look direction
            DrawCircleV(mousePos, 1, RED);
            Vector2 playerPos = player.getPosition();
            DrawLineV(playerPos, {(playerPos.x) + (cos(radians(player.getAngle())) * 10), (playerPos.y) + (sin(radians(player.getAngle())) * 10)}, YELLOW);
            DrawCircleV(playerPos, 3, GREEN);

            DrawText(name.c_str(), 10, 980, 20, WHITE);

            ClearBackground(Color{50,50,100,255});
            EndDrawing();
        
            
            if(IsKeyPressed(KEY_S)){
                map.saveMap(fileName);
            }
            for(int i = KEY_ZERO; i <= KEY_NINE; i++){
                if(IsKeyPressed(i)){
                    textureNum = i - KEY_ZERO;
                    break;
                }
            }    

        }

        if(IsKeyPressed(KEY_ENTER)) editor = !editor; 
    }

    CloseWindow();
    return 0;
}