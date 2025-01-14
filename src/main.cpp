#include "raylib.h"

#include "common.h"
#include "img_proccessing.h"

// Unity Build
#include "audio.cpp"

int main()
{
  CustomImage img;
  img.load("image/Pearls-Group-blue.png");
  
  CustomImage reduced_img = img.reduce_image(8);
  reduced_img.save("image/reduced_blue.png");
  std::cout << "Image reduced: (" << reduced_img.width << ", " << reduced_img.height << ")" << std::endl;
  
  Note* melody = img_to_melody(reduced_img);
  std::cout << "Melody: ";
  for (int i = 0; i < reduced_img.width*reduced_img.height; i++)
  {
    std::cout << melody[i].frequency << " ";
  }
  
  
  
  InitWindow(1080, 720, "Music");
  SetTargetFPS(60);
  
  init_audio(melody);
  
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello World!", 100, 100, 50, GREEN);
    
    //for (int i = 0; i < 8; i++) {
    //if (IsKeyPressed('1' + i)) {
    //stop_note();
    //play_note(i);
    //}
    //}
    update_audio();
    
    EndDrawing();
  }
  
  deinit_audio();
  
  
  delete[] melody;
  
  return 0;
}
