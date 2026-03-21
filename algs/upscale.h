#include "../utils/interp.h"
#include "../utils/image.h"

__global__ void UpscaleKernel(IMAGE* in, IMAGE* out)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if(x >= out->width)  return;
    if(y >= out->height) return;

    //Floating-point coordinates in input image
    float xs = ((float)x / (out->width - 1)) * (in->width - 1);
    float ys = ((float)y / (out->height - 1)) * (in->height - 1);

    SetPixel(out, x, y, ImageInterp(in, xs, ys));
}

//Upscales the image using bilinear interpolation
IMAGE* Upscale(IMAGE* input, int factor)
{
    IMAGE* d_in = CudaImageMalloc(input->width, input->height);
    CudaImageCopy(d_in, input, cudaMemcpyHostToDevice);

    int upscaledWidth = input->width * factor - factor + 1;
    int upscaledHeight = input->height * factor - factor + 1;

    IMAGE* d_out = CudaImageMalloc(upscaledWidth, upscaledHeight);

    IMAGE* output = MallocImage(upscaledWidth, upscaledHeight);

    UpscaleKernel<<< GetGridDim(output) , imgBlockDim >>>(d_in, d_out);
    cudaDeviceSynchronize();

    CudaImageCopy(output, d_out, cudaMemcpyDeviceToHost);

    CudaImageFree(d_in);
    CudaImageFree(d_out);
    return output;
}
