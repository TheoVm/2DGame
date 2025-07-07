#include <iostream>
#include <raylib.h>

#include "map.h"

typedef struct Player{
    Texture2D texture;
    Rectangle sourceRec;
    Rectangle destRec;
    Vector2 origin;
    Vector2 direction;
    float speed;
}Player;

void playerMove(Player* player){
    float moveSpeed = player->speed;    
    if(IsKeyDown(KEY_L)){
        moveSpeed = 15.0f * GetFrameTime();        // Set the moveSpeed based on the frame time
    }
    if(IsKeyDown(KEY_W)){
        player->sourceRec.y = player->sourceRec.height*3; // Uses the correnponding sprite
        player->destRec.y -= player->speed;               // Move the player based on the moveSpeed
        player->sourceRec.x += player->sourceRec.width;;  // Changes to the next sprite
        player->direction = {0, -1};
    }
    else if(IsKeyDown(KEY_S)){
        player->sourceRec.y = player->sourceRec.height*0;
        player->destRec.y += moveSpeed;
        player->sourceRec.x += player->sourceRec.width;
        player->direction = {0, 1};
    }
    else if(IsKeyDown(KEY_A)){
        player->sourceRec.y = player->sourceRec.height*1;
        player->destRec.x -= moveSpeed;
        player->sourceRec.x += player->sourceRec.width;
        player->direction = {-1, 0};
    }
    else if(IsKeyDown(KEY_D)){
        player->sourceRec.y = player->sourceRec.height*2;
        player->destRec.x += moveSpeed;
        player->sourceRec.x += player->sourceRec.width;
        player->direction = {1, 0};
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


void checkColision(Player *player, nlohmann::json_abi_v3_12_0::basic_json<>::value_type data){
    int tileX;
    if (player->direction.x == 1 || player->direction.y == 1 || player->direction.y == -1){
        tileX = (int)((player->destRec.x + player->origin.x) / 16);
    } else {
        tileX = (int)((player->destRec.x) / 16);
    }
    int tileY = (int)((player->destRec.y + player->origin.y) / 16);
    int index = (tileY * 100) + tileX;
    int id = (int)data[index] - 1;
    if (id == 2695){
        if (player->direction.x == 1){
            player->destRec.x = player->destRec.x - player->speed;
        }
        if (player->direction.x == -1){
            player->destRec.x = player->destRec.x + player->speed;
        }
        if (player->direction.y == 1){
            player->destRec.y = player->destRec.y - player->speed;
        }
        if (player->direction.y == -1){
            player->destRec.y = player->destRec.y + player->speed;
        }
    }
}

// Rectangle checkColision(Player *player, nlohmann::json_abi_v3_12_0::basic_json<>::value_type data){
//     // Rectangle playerPosition = {player->destRec.x, player->destRec.y, player->destRec.width, player->destRec.height};
//     Rectangle positionCheck;

//     if (player->direction.x == 1){
//         positionCheck = {player->destRec.x + player->destRec.width/4, player->destRec.y - player->destRec.height/2, 1.0f, player->destRec.height};
//     } else if (player->direction.x == -1){
//         positionCheck = {player->destRec.x - player->destRec.width/4, player->destRec.y - player->destRec.height/2, 1.0f, player->destRec.height};
//     } else if (player->direction.y == 1){
//         positionCheck = {player->destRec.x - player->destRec.width/4, player->destRec.y + player->destRec.height/2, player->destRec.width/2, 1.0f};
//     } else if (player->direction.y == -1){
//         positionCheck = {player->destRec.x - player->destRec.width/4, player->destRec.y - player->destRec.height/4, player->destRec.width/2, 1.0f};
//     } else {
//         positionCheck = {player->destRec.x - player->destRec.width/2, player->destRec.y - player->destRec.height/2, 1.0f, 1.0f};
//     }   

//     int tileX = (int)((positionCheck.x + 16.0f) / 16);
//     int tileY = (int)((positionCheck.y + 16.0f) / 16);
//     int index = (tileY * 100) + tileX;
//     int id = (int)data[index] - 1;

//     std::cout << player->destRec.x << " " << player->destRec.y << std::endl;
//     std::cout << tileX << " " << tileY << std::endl;
//     std::cout << id << std::endl;

//     if (id == 2695){
//         if (player->direction.x == 1){
//             player->destRec.x = player->destRec.x - player->speed;
//         }
//         if (player->direction.x == -1){
//             player->destRec.x = player->destRec.x + player->speed;
//         }
//         if (player->direction.y == 1){
//             player->destRec.y = player->destRec.y - player->speed;
//         }
//         if (player->direction.y == -1){
//             player->destRec.y = player->destRec.y + player->speed;
//         }
//     }
//     return positionCheck;
// }


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Pokemon");

    std::string path = "./assets/Map/TileSet/map1.json";
    Texture2D tileSet = LoadTexture("./assets/Map/TileSet/FireRed.png");

    // Sprite size inside the Sprite Sheet
    float spritewidth = 64.0f;
    float spriteHeight = 64.0f;
    
    nlohmann::json_abi_v3_12_0::basic_json<>::value_type dataTexture1 = generateMap(path, 0);
    nlohmann::json_abi_v3_12_0::basic_json<>::value_type dataColision = generateMap(path, 1);

    // Player definition
    Player player;

    player.texture = LoadTexture("./assets/player.png");
    if (player.texture.id == 0) {
      TraceLog(LOG_ERROR, "Erro ao carregar a textura.");
    }
    player.sourceRec = {spritewidth*0, spriteHeight*0, spritewidth, spriteHeight};
    player.destRec = {400.0f, 300.0f, spritewidth/2, spriteHeight/2};
    player.origin = {player.destRec.width/2, player.destRec.height/2};
    player.direction = {0, 0};

    // Camera definition
    Camera2D camera = {0};
    camera.target = (Vector2){player.destRec.x, player.destRec.y};
    camera.offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    while (!WindowShouldClose())
    {
        // Camera update
        camera.target = (Vector2){player.destRec.x, player.destRec.y};

        // Player update
        player.speed = 150.0f * GetFrameTime();   

        playerMove(&player);
        checkColision(&player, dataColision);
        
        int tileSetColumns = tileSet.width / 16;
        
        BeginDrawing();
        BeginMode2D(camera);
            ClearBackground(BLACK);

             for (int i = 0; i < (int)dataTexture1.size(); i++){
                int id = (int)dataTexture1[i] - 1;
                if (id < 0) continue;
                int x = i % 100;
                int y = i / 100;

                int srcX = (id % tileSetColumns) * 16;
                int srcY = (id / tileSetColumns) * 16;
                
                Rectangle mapSource = {(float)srcX, (float)srcY, 16.0f, 16.0f};
                Rectangle position = {(float)(x * 16), (float)(y * 16), 16.0f, 16.0f};
                Vector2 origin = {16.0f /2, 16.0f /2};

                DrawTexturePro(tileSet, mapSource, position, origin, 0.0f, WHITE);
            }
            DrawTexturePro(player.texture, player.sourceRec, player.destRec, player.origin, 0.0f, WHITE);
            // DrawRectangle(check.x, check.y, check.width, check.height, RED);
        EndMode2D();
        DrawFPS(1, 1);
        EndDrawing();
    }
    UnloadTexture(player.texture);
    CloseWindow();
}