#include "header/img_proccessing.h"

// Example usage
int main()
{
    Image img;
    img.load("image/garbage man dilbert.jpg");

    // Retrieve and display the Hue, Saturation, and Brightness of the first pixel
    Pixel p = img.get_pixel(0, 0);
    cout << "Pixel (0,0): Hue = " << p.hue << ", Saturation = " << p.saturation << ", Brightness = " << p.brightness << "%" << endl;

    return 0;
}