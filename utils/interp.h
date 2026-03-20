#pragma once

#include "image.h"

__device__ int Floor(float x)
{
    return (int)x;
}

__device__ int Ceil(float x)
{
    int floor = (int)x;
    return (x - floor > 0) ? floor + 1 : floor;
}

__device__ int Round(float x)
{
    int floor = (int)x;
    return (x - floor > 0.5) ? floor + 1 : floor;
}

//Linear interpolation between pixels
__device__ PIXEL Lerp(PIXEL p1, PIXEL p2, float p)
{
    return
    {
        (stbi_uc)Round(p1.r * (1 - p) + p2.r * p),
        (stbi_uc)Round(p1.g * (1 - p) + p2.g * p),
        (stbi_uc)Round(p1.b * (1 - p) + p2.b * p),
        (stbi_uc)Round(p1.a * (1 - p) + p2.a * p)
    };
}

//Bilinear interpolation
__device__ PIXEL ImageInterp(IMAGE* image, float x, float y)
{
    int x1 = Floor(x);
    int x2 = Ceil(x);

    int y1 = Floor(y);
    int y2 = Ceil(y);

    if(x1 == x2 && y1 == y2)
    {
        return GetPixel(image, x1, y1);
    }

    if(x1 == x2)
    {
        PIXEL p1 = GetPixel(image, x1, y1);
        PIXEL p2 = GetPixel(image, x1, y2);
        return Lerp(p1, p2, y - y1);
    }

    if(y1 == y2)
    {
        PIXEL p1 = GetPixel(image, x1, y1);
        PIXEL p2 = GetPixel(image, x2, y1);
        return Lerp(p1, p2, x - x1);
    }

    //Interpolate along X, and then along Y
    PIXEL p1 = ImageInterp(image, x, y1);
    PIXEL p2 = ImageInterp(image, x, y2);
    return Lerp(p1, p2, y - y1);
}

