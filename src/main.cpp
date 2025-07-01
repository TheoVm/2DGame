#include <iostream>
#include <raylib.h>

#include "test.h"

typedef struct Player{
    Texture2D texture;
    Rectangle sourceRec;
    Rectangle destRec;
    Vector2 origin;
    Vector2 direction;
}Player;

void playerMove(Player* player){
    float moveSpeed = 150.0f * GetFrameTime();            // Set the moveSpeed based on the frame time
    if(IsKeyDown(KEY_W)){
        player->sourceRec.y = player->sourceRec.height*3; // Uses the correnponding sprite
        player->destRec.y -= moveSpeed;                   // Move the player based on the moveSpeed
        player->sourceRec.x += player->sourceRec.width;   // Changes to the next sprite
    }
    else if(IsKeyDown(KEY_S)){
        player->sourceRec.y = player->sourceRec.height*0;
        player->destRec.y += moveSpeed;
        player->sourceRec.x += player->sourceRec.width;
    }
    else if(IsKeyDown(KEY_A)){
        player->sourceRec.y = player->sourceRec.height*1;
        player->destRec.x -= moveSpeed;
        player->sourceRec.x += player->sourceRec.width;
    }
    else if(IsKeyDown(KEY_D)){
        player->sourceRec.y = player->sourceRec.height*2;
        player->destRec.x += moveSpeed;
        player->sourceRec.x += player->sourceRec.width;
    } else { 
        player->sourceRec.x = 0.0f;
    }
    if(player->sourceRec.x > player->sourceRec.width*3){
        player->sourceRec.x = 0.0f;
    }
}

// void checkBorders(Player* player, float width, float height){
//     if(player->destRec.x >= width - player->sourceRec.width/4){ // Checks if player center surpass the border 
//         player->destRec.x = width - player->sourceRec.width/4;  // Redefines the posion inside the limits
//     }
//     if(player->destRec.x <= 0.0f + player->sourceRec.width/4){
//         player->destRec.x = 0.0f + player->sourceRec.width/4;
//     }
//     if(player->destRec.y >= height - player->sourceRec.height/2){
//         player->destRec.y = height - player->sourceRec.height/2;
//     }
//     if(player->destRec.y <= 0.0f + player->sourceRec.height/4){
//         player->destRec.y = 0.0f + player->sourceRec.height/4;
//     }
// }

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Pokemon");
    SetTargetFPS(30);

    std::string path = "./assets/Map/TileSet/map1.json";
    Texture2D tileSet = LoadTexture("./assets/Map/TileSet/FireRed.png");

    // Sprite size inside the Sprite Sheet
    float spritewidth = 64.0f;
    float spriteHeight = 64.0f;
    
    nlohmann::json_abi_v3_12_0::basic_json<>::value_type data = generateMap(path);

    // Player definition
    Player player;

    player.texture = LoadTexture("./assets/player.png");
    if (player.texture.id == 0) {
      TraceLog(LOG_ERROR, "Erro ao carregar a textura.");
    }
    player.sourceRec = {spritewidth*0, spriteHeight*0, spritewidth, spriteHeight};
    player.destRec = {400.0f, 300.0f, spritewidth*1, spriteHeight*1};
    player.origin = {player.destRec.width/2, player.destRec.height/2};
    player.direction = {0, 0};

    // Camera definition
    Camera2D camera = {0};
    camera.target = (Vector2){player.destRec.x, player.destRec.y};
    camera.offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        // Camera update
        camera.target = (Vector2){player.destRec.x, player.destRec.y};

        // Player update
        playerMove(&player);
        // checkBorders(&player, screenWidth, screenHeight);]
        
        int tileSetColumns = tileSet.width / 16;


        // std::cout << data[4000] << std::endl;
        
        BeginDrawing();
        BeginMode2D(camera);
            ClearBackground(GRAY);

             for (int i = 0; i < (int)data.size(); i++){
                int id = (int)data[i] - 1;
                if (id < 0) continue;
                int x = i % 100;
                int y = i / 100;

                int srcX = (id % tileSetColumns) * 16;
                int srcY = (id / tileSetColumns) * 16;
                
                Rectangle mapSource = {(float)srcX, (float)srcY, 16.0f, 16.0f};
                Vector2 position = {(float)(x * 16), (float)(y * 16)};

                DrawTextureRec(tileSet, mapSource, position, WHITE);
            }

            DrawTexturePro(player.texture, player.sourceRec, player.destRec, player.origin, 0.0f, WHITE);
        EndMode2D();
        DrawFPS(1, 1);
        EndDrawing();
    }
    UnloadTexture(player.texture);
    CloseWindow();
}