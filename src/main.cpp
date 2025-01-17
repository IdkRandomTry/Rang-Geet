#include "raylib.h"

static float note_time = 0.125f;
static int pixel_count = 16;

#include "common.h"
#include "img_proccessing.h"

// Unity Build
#include "audio.cpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define WINDOW_WIDTH  1080
#define WINDOW_HEIGHT 720
int main()
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Music");
  SetTargetFPS(60);
  
  int window_width = WINDOW_WIDTH;
  int window_height = WINDOW_HEIGHT;
  
  // Image to Melody
  CustomImage img;
  img.load("image/mona lisa.JPG");
  Texture2D img_to_show = LoadTexture("image/mona lisa.JPG");
  
  CustomImage reduced_img = img.reduce_image_rgbavg(pixel_count);
  reduced_img.save("image/reduced.jpg");
  std::cout << "Image reduced: (" << reduced_img.width << ", " << reduced_img.height << ")" << std::endl;
  
  Note* melody = img_to_melody(reduced_img);
  std::cout << "Melody: ";
  for (int i = 0; i < reduced_img.width*reduced_img.height; i++)
  {
    std::cout << melody[i].frequency << " ";
  }
  
  
  
  // Audio Initialization
  init_audio(melody);
  
  float tracker = 0.0f;
  
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    
    float retimed_dt = GetFrameTime();
    tracker += retimed_dt;
    if (tracker > note_time * pixel_count) tracker -= pixel_count * note_time;
    
    float track_pct = tracker * 1.0 / (pixel_count * note_time);
    
    if (IsWindowResized()) {
      window_width  = GetRenderWidth();
      window_height = GetRenderHeight();
    }
    
    
    
    // TODO(voxel): Proper sizing of images
    if (GuiButton((Rectangle) { 5, 5, ((float)(window_width/2.0f)-10),  ((float)(window_height)-10)}, "###Hello")) {
      // NOTE(voxel): Open different image
    }
    DrawTexturePro(img_to_show, Rectangle{0, 0, (float)img_to_show.width, (float)img_to_show.height},
                   Rectangle{10, 10, ((float)(window_width/2.0f)-20),  ((float)(window_height)-20)},
                   Vector2{0, 0}, 0, WHITE);
    DrawRectangle((window_width/2.0)+5, 5, ((float)(window_width/2.0f)-10),  ((float)(window_height)-10), ColorBrightness(BLUE, 0.3));
    DrawRectangle((window_width/2.0)+10, 10, ((float)(window_width/2.0f)-20),  ((float)(window_height)-20), ColorBrightness(BLUE, 0.7));
    
    for (int i = 0; i <= pixel_count; i++) {
      DrawRectangle((window_width/2.0)+10, 50 + (i/(float)pixel_count) * (window_height - 100), (window_width/2.0)-20, 3, LIGHTGRAY);
    }
    DrawRectangle((window_width/2.0)+10, 50 + track_pct * (window_height - 100), (window_width/2.0)-20, 5, RED);
    
    update_audio(retimed_dt);
    
    EndDrawing();
  }
  
  deinit_audio();
  
  
  delete[] melody;
  
  return 0;
}
