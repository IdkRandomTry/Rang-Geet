#include "raylib.h"

#include "common.h"
#include "img_proccessing.h"

// Unity Build
#include "audio.cpp"

Note pix_to_note(Pixel p);
Note* img_to_melody(CustomImage img);

int main()
{
  CustomImage img;
  img.load("image/Pearls-Group-blue.png");
  
  CustomImage reduced_img = img.reduce_image(8);
  reduced_img.save("image/reduced_blue.png");
  std::cout << "Image reduced" << reduced_img.width << " " << reduced_img.height << std::endl;

  Note* melody = img_to_melody(reduced_img);
  std::cout << "Melody: ";
  for (int i = 0; i < reduced_img.width*reduced_img.height; i++)
  {
    std::cout << melody[i].frequency << " ";
  }
  
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

Note pix_to_note(Pixel p)
{
  float f = 240 + p.hue/360.0f * (480-240); //linear map to 240-480 Hz
  return Note(f, Instrument::Sine);
}

Note* img_to_melody(CustomImage img)
{
  Note* melody = new Note[img.width*img.height];
  for (int i = 0; i < img.width*img.height; i++)
  {
    Pixel p = img.get_pixel(i%img.width, i/img.width);
    melody[i] = pix_to_note(p);
  }
  return melody;
}