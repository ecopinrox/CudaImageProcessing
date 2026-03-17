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
    stbi_write_png(path, image->width, image->height, image->channels, image->data, image->width * STBI_rgb_alpha);
}

void FreeImage(IMAGE* image)
{
    stbi_image_free(image->data);
    free(image);
}

