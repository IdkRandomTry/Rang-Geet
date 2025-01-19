# Rang-Geet

This was made for an installation at Curiosity Carnival 2025, IIT Gandhinagar. 'Rang' means color and 'Geet' means song in Hindi. 

## Objective

Correlate color composition with musical compositions. Visually appealing color compositions should result in enjoyable musical compositions. This is a creative 'What if..?' project. However, future practical applications may include making a device that enables the color blind to enjoy color compositions and paintings.

## Motivation

The ideas used in this project are inspired by 
- [this talk by Adam Neely](https://www.youtube.com/watch?v=JiNKlhspdKg])
- [Neil Harbisson](https://www.ted.com/talks/neil_harbisson_i_listen_to_color)

## Description
The project generates rhythms corresponding to the color composition of an image. First, the image is vertically split into 16 bands, and the average RGB value of each band is found. The image is now reduced to 16 colors. The hue of each color is closely related to its frequency. Hence, the hue is mapped to a frequency in the hearing range. In music, the frequency is doubled when we go up an octave. Hence, hue is exponentially mapped to sound frequencies. The 16 colors give us 16 frequencies, which when played sequentially, give us the rhythm corresponding to the image!

## Screenshot
![image](https://github.com/user-attachments/assets/1e0eee26-7913-42fd-b035-a14db736ab88)
