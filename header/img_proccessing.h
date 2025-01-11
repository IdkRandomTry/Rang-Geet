#pragma once

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

struct Pixel
{
    int hue;
    int saturation;
    int brightness;

    Pixel(int h=0, int s = 0, int b = 0)
    {
        hue = h;
        saturation = s;
        brightness = b;
    }
};

struct Image
{
    int width=0;
    int height=0;
    Pixel* pixels; // 1D array of pixels of dynamic size
    
    Image(int w=1, int h=1)
    {
        width = w;
        height = h;
    }

    ~Image()
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

    Pixel rgb_to_hsv(int r, int g, int b)
    {
        Pixel p;
        float r_ = r / 255.0f;
        float g_ = g / 255.0f;
        float b_ = b / 255.0f;
        float cmax = max(r_, max(g_, b_));
        float cmin = min(r_, min(g_, b_));
        float delta = cmax - cmin;

        if (delta == 0)
            p.hue = 0;
        else if (cmax == r_)
            p.hue = (int)(60 * (fmod(((g_ - b_) / delta), 6.0f)));
        else if (cmax == g_)
            p.hue = (int)(60 * (((b_ - r_) / delta) + 2.0f));
        else
            p.hue = (int)(60 * (((r_ - g_) / delta) + 4.0f));
        if (p.hue < 0)
            p.hue += 360;

        if (cmax == 0)
            p.saturation = 0;
        else
            p.saturation = (int)(delta / cmax * 100);

        p.brightness = (int)(cmax * 100);
        

        return p;
    }

    void load(const char* filename)
    {
        int channels;
        unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);
        pixels = new Pixel[width * height];

        if (data == NULL)
        {
            cout << "Error loading image" << endl;
            return;
        }

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int r = data[(y * width + x) * 3];
                int g = data[(y * width + x) * 3 + 1];
                int b = data[(y * width + x) * 3 + 2];

                Pixel p = rgb_to_hsv(r, g, b);
                set_pixel(x, y, p);
            }
        }

        stbi_image_free(data);
    }

};

