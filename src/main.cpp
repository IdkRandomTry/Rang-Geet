#include "raylib.h"

static float note_time   = 0.25f;
static int   pixel_count = 16;

#include "common.h"
#include "img_proccessing.h"

// Unity Build
#include "audio.cpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define WINDOW_WIDTH  1080
#define WINDOW_HEIGHT 720

std::string OpenFileDialog(const char* filter);


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
  
  float img_aspect = (float)img.width / (float)img.height;
  Rectangle image_fit = Rectangle{
    (((window_width/2.0f)-20.0f) - ((window_height-20.0f) * img_aspect)) / 2.0f,
    10,
    (window_height-20.0f) * img_aspect,
    window_height-20.0f,
  };
  
  // Audio Initialization
  init_audio(melody);
  
  float tracker = 0.0f;
  float time_scale = 1.0f;
  bool refreshed_melody = false;
  
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    
    float retimed_dt = GetFrameTime() * time_scale;
    tracker += retimed_dt;
    if (tracker > note_time * pixel_count) tracker -= pixel_count * note_time;
    
    float track_pct = tracker * 1.0 / (pixel_count * note_time);
    
    if (IsWindowResized()) {
      window_width  = GetRenderWidth();
      window_height = GetRenderHeight();
    }
    
    // TODO(voxel): Proper sizing of images
    if (GuiButton((Rectangle) { 5, 5, ((float)(window_width/2.0f)-10),  ((float)(window_height)-10)}, "###Hello")) {
      stop_note();
      time_scale = 0.0f;
      std::string new_file = OpenFileDialog("*.JPG\0");
      if (new_file != "") {
        delete[] img.pixels;
        delete[] reduced_img.pixels;
        
        img.load(new_file.c_str());
        reduced_img = img.reduce_image_rgbavg(pixel_count);
        img_to_show = LoadTexture(new_file.c_str());
        
        delete[] melody;
        melody = img_to_melody(reduced_img);
        refreshed_melody = true;
        
        img_aspect = (float)img.width / (float)img.height;
        image_fit = Rectangle{
          (((window_width/2.0f)-20.0f) - ((window_height-20.0f) * img_aspect)) / 2.0f,
          10,
          (window_height-20.0f) * img_aspect,
          window_height-20.0f,
        };
      }
    } else {
      if (refreshed_melody) {
        tracker = 0;
        time_scale = 1.0f;
        stop_note();
        refresh_melody(melody);
        refreshed_melody = false;
      }
      update_audio(retimed_dt);
    }
    
    DrawTexturePro(img_to_show, Rectangle{0, 0, (float)img_to_show.width, (float)img_to_show.height},
                   image_fit,
                   Vector2{0, 0}, 0, WHITE);
    
    DrawRectangle(5*(window_width/10.0)+5, 5, ((float)(5*window_width/10.0f)-10),  ((float)(window_height)-10), ColorBrightness(BLUE, 0.3));
    DrawRectangle(5*(window_width/10.0)+10, 10, ((float)(5*window_width/10.0f)-20),  ((float)(window_height)-20), ColorBrightness(BLUE, 0.7));
    
    for (int i = 0; i <= pixel_count; i++) {
      if (i != pixel_count) {
        DrawRectangle(5*(window_width/10.0)+10,
                      lmap(0, 1, 10, window_height-20, i/(float)pixel_count),
                      (window_height-10-5)/(float)(pixel_count),
                      (window_height-10-5)/(float)(pixel_count),
                      Color{(unsigned char)reduced_img.pixels[i].r,
                        (unsigned char)reduced_img.pixels[i].g,
                        (unsigned char)reduced_img.pixels[i].b,
                        255});
      }
      
      DrawRectangle(5*(window_width/10.0)+10, lmap(0, 1, 10, window_height-20, i/(float)pixel_count),
                    5*(window_width/10.0)-20, 3, ColorLerp(LIGHTGRAY, BLUE, 0.3f));
      
      if (i != pixel_count) {
        DrawRectanglePro(Rectangle{
                           lmap(120, 480,
                                5*(window_width/10.0)+10 + (window_height-10-5)/(float)(pixel_count) + 40,
                                window_width - 20 - 40, melody[i].frequency),
                           lmap(0, 1, 10, window_height-20, i/(float)pixel_count) + ((window_height-30)/(pixel_count*2.0f)),
                           25, 25
                         }, Vector2{10,10}, 45, BLUE);
        DrawRectanglePro(Rectangle{
                           lmap(120, 480,
                                5*(window_width/10.0)+10 + (window_height-10-5)/(float)(pixel_count) + 40,
                                window_width - 20 - 40, melody[i].frequency),
                           lmap(0, 1, 10, window_height-20, i/(float)pixel_count) + ((window_height-30)/(pixel_count*2.0f)),
                           20, 20
                         }, Vector2{10,10}, 45, ColorLerp(LIGHTGRAY, BLUE, 0.5f));
      }
    }
    
    DrawRectangle(5*(window_width/10.0)+10, lmap(0, 1, 10, window_height-20, track_pct), 5*(window_width/10.0)-20, 5, RED);
    
    
    EndDrawing();
  }
  
  deinit_audio();
  
  
  delete[] melody;
  
  return 0;
}
