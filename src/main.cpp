#include "img_proccessing.h"

// Unity Build
#include "audio.cpp"
#include "raylib.h"

int main()
{
  LoadedImage img;
  img.load("image/garbage man dilbert.jpg");
  
  Pixel p = img.get_pixel(0, 0);
  std::cout << "Pixel (0,0): Hue = " << p.hue << ", Saturation = " << p.saturation << ", Brightness = " << p.brightness << "%" << std::endl;
  
  
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