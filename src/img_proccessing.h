#pragma once

#include <iostream>
#include <cmath>
#include "stb_image.h"
#include "stb_image_write.h"

struct Pixel
{
  int r, g, b;
  int hue, saturation, brightness;
  
  Pixel(int r_inp=0, int g_inp = 0, int b_inp = 0)
  {
    r = r_inp;
    g = g_inp;
    b = b_inp;

    // Convert RGB to HSV and set them
    float r_ = r / 255.0f;
    float g_ = g / 255.0f;
    float b_ = b / 255.0f;
    float cmax = std::max(r_, std::max(g_, b_));
    float cmin = std::min(r_, std::min(g_, b_));
    float delta = cmax - cmin;

    if (delta == 0)
      hue = 0;
    else if (cmax == r_)
      hue = (int)(60 * (fmod(((g_ - b_) / delta), 6.0f)));
    else if (cmax == g_)
      hue = (int)(60 * (((b_ - r_) / delta) + 2.0f));
    else
      hue = (int)(60 * (((r_ - g_) / delta) + 4.0f));
    if (hue < 0)
      hue += 360;

    if (cmax == 0)
      saturation = 0;
    else
      saturation = (int)(delta / cmax * 100);

    brightness = (int)(cmax * 100);
  }

  void info()
  {
    std::cout << "RGB: (" << r << ", " << g << ", " << b << ")" << std::endl;
    std::cout << "HSV: (" << hue << ", " << saturation << ", " << brightness << ")" << std::endl;
  }
};

struct CustomImage
{
  int width=0;
  int height=0;
  Pixel* pixels; // 1D array of pixels of dynamic size
  
  CustomImage(int w=1, int h=1)
  {
    width = w;
    height = h;
  }
  
  ~CustomImage()
  {
    delete[] pixels; // Deallocate memory
  }
  
  void set_pixel(int x, int y, Pixel p)
  {
    pixels[y * width + x] = p;
  }
  
  Pixel get_pixel(int x, int y)
  {
    return pixels[y * width + x]; // Access pixel at (x, y);
  }
  
  void load(const char* filename)
  {
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);
    pixels = new Pixel[width * height];
    
    if (data == NULL)
    {
      std::cout << "Error loading image" << std::endl;
      return;
    }
    
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        int r = data[(y * width + x) * 3];
        int g = data[(y * width + x) * 3 + 1];
        int b = data[(y * width + x) * 3 + 2];
        
        Pixel p = Pixel(r, g, b);
        set_pixel(x, y, p);
      }
    }
    
    stbi_image_free(data);
  }

  void save(const char* filename)
  {
    unsigned char* data = new unsigned char[width * height * 3];
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        Pixel p = get_pixel(x, y);
        data[(y * width + x) * 3] = p.r;
        data[(y * width + x) * 3 + 1] = p.g;
        data[(y * width + x) * 3 + 2] = p.b;
      }
    }
    stbi_write_png(filename, width, height, 3, data, width * 3);
    delete[] data;
  }

  CustomImage reduce_image(int n)
  {
    CustomImage reduced_image(1, n);
    reduced_image.pixels = new Pixel[n];
    int reduced_height = height/n;
    for (int i = 0; i < n; i++)
    {
      int r = 0, g = 0, b = 0;
      for (int y = 0; y < reduced_height; y++)
      {
        for (int x = 0; x < width; x++)
        {
          Pixel p = get_pixel(x, i * reduced_height + y);
          r += p.r;
          g += p.g;
          b += p.b;
        }
      }
      r /= (width * reduced_height);
      g /= (width * reduced_height);
      b /= (width * reduced_height);
      Pixel p(r, g, b);
      reduced_image.set_pixel(0, i, p);
    }
    return reduced_image;
  }
  
};

