#include "img_proccessing.h"

// Unity Build
#include "audio.cpp"
#include "raylib.h"

int main()
{
  CustomImage img;
  img.load("image/Pearls-Group-blue.png");

  CustomImage reduced_img = img.reduce_image(16);
  reduced_img.save("image/reduced_blue.png");
  
  InitWindow(1080, 720, "Music");
  SetTargetFPS(60);
  
  init_audio();
  
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello World!", 100, 100, 50, GREEN);
    
    for (int i = 0; i < 8; i++) {
      if (IsKeyPressed('1' + i)) {
        stop_note();
        play_note(i);
      }
    }
    
    EndDrawing();
  }
  
  deinit_audio();
  
  
  return 0;
}