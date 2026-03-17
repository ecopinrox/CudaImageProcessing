#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

//Image handling
typedef struct 
{
    stbi_uc* data;
    int width;
    int height;
    int channels;
} IMAGE;

IMAGE* LoadImage(const char* path)
{
    IMAGE* image = (IMAGE*)malloc(sizeof(IMAGE));
    image->data = stbi_load(path, &(image->width), &(image->height), &(image->channels), STBI_rgb_alpha);

    return image;
}

void WriteImage(const char* path, IMAGE* image)
{
    stbi_write_png(path, image->width, image->height, STBI_rgb_alpha, image->data, image->width * STBI_rgb_alpha);
}

void FreeImage(IMAGE* image)
{
    stbi_image_free(image->data);
    free(image);
}

//Pixel handling
typedef struct
{
    stbi_uc r;  //Red
    stbi_uc g;  //Green
    stbi_uc b;  //Blue
    stbi_uc a;  //Alpha (transparency)
} PIXEL;

__host__ __device__ int IsPixelInBounds(IMAGE* image, int x, int y)
{
    return 
        x >= 0              || 
        x < image->width    || 
        y >= 0              || 
        y < image->height
    ;
}

__host__ __device__ PIXEL GetPixel(IMAGE* image, int x, int y)
{
    if(!IsPixelInBounds(image, x, y)) 
    {
        return {0, 0, 0, 0};
    }

    stbi_uc* pixelAddr = image->data + STBI_rgb_alpha * (image->width * y + x);
    return 
    { 
        pixelAddr[0],       //r
        pixelAddr[1],       //g
        pixelAddr[2],       //b
        pixelAddr[3]        //a
    };
}

__host__ __device__ void SetPixel(IMAGE* image, int x, int y, PIXEL pixel)
{
    if(!IsPixelInBounds(image, x, y)) 
    {
        return;
    }

    stbi_uc* pixelAddr = image->data + STBI_rgb_alpha * (image->width * y + x);
    pixelAddr[0] = pixel.r;
    pixelAddr[1] = pixel.g;
    pixelAddr[2] = pixel.b;
    pixelAddr[3] = pixel.a;
}

