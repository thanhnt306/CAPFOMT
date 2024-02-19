#include <iostream>
#include <cuda_runtime.h>
#include <vector>
#include <device_launch_parameters.h>
#include <crt/math_functions.hpp>
#include "pressuredata.h"

__device__ Transducer *d_transducers;
// __device__ float fData;
// __device__ int iData;


__global__ void simKernel(cudaPitchedPtr pitchPtr, Vec3D gridCenter, float gridSize, size_t numOfTransducer, PressureParameter prePara)
{

    size_t pitchZ = pitchPtr.pitch/sizeof(float);

    float* magData = (float*)pitchPtr.ptr;

    int xId = threadIdx.x;
    int yId = blockIdx.x;
    int zId = blockIdx.y;

    float xRootPos = -(blockDim.x/2.0f)*gridSize + gridCenter.x;
    float yRootPos = -(gridDim.x/2.0f)*gridSize + gridCenter.y;
    float zRootPos = -(gridDim.y/2.0f)*gridSize + gridCenter.z;

    int idx = zId*gridDim.x*pitchZ + yId*pitchZ + xId;

    Vec3D targetPt(
        (xRootPos + xId*gridSize),
        (yRootPos + yId*gridSize),
        (zRootPos + zId*gridSize)
        );
    float reAcc=0;
    float imAcc=0;

    // if(xId == 1 && yId == 1 && zId ==0) {
    //     iData = idx;
    //     fData = xRootPos;
    // }

    for(int t=0; t< numOfTransducer; t++){
        Vec3D delta = targetPt - d_transducers[t].pos();
        float r = delta.length();
        // float theta = acos(delta.y*transducers[t].normal / r);
        float dot = delta.x*d_transducers[t].normal().x + delta.y*d_transducers[t].normal().y + delta.z*d_transducers[t].normal().z;
        float lenSq1 = r;
        float lenSq2 = d_transducers[t].normal().length();
        float theta = acos(dot/(lenSq1 * lenSq2));
        float phi = d_transducers[t].getRelativeRMSPhase();

        float Df=2 * j1f(prePara.waveNumber * d_transducers[t].diameter()/2*sinf(theta)) /
                   (prePara.waveNumber*d_transducers[t].diameter()/2*sinf(theta));
        float re = prePara.Vpp * d_transducers[t].P0() * Df / r * cos(phi+prePara.waveNumber*r);
        float im = prePara.Vpp * d_transducers[t].P0() * Df / r * sin(phi+prePara.waveNumber*r);

        reAcc += re;
        imAcc += im;

    }

    float mag = sqrtf(reAcc*reAcc + imAcc*imAcc);

    magData[idx]=mag;
}

extern "C" void magCalc(float* magData, std::vector<Transducer *> *transducers, PointGridData *pointGridData, PressureParameter pressureParameter)
{

    cudaExtent extent = make_cudaExtent(pointGridData->resolutionX()*sizeof(float)
                                      , pointGridData->resolutionY()
                                      , pointGridData->resolutionZ());
    cudaPitchedPtr d_magData;
    cudaMalloc3D(&d_magData, extent);

    Transducer* temp;
    cudaMalloc((void**)&temp, transducers->size()*sizeof(Transducer));

    for (int i = 0; i < transducers->size(); ++i) {
        cudaError_t trets = cudaMemcpy(&temp[i], transducers->at(i), sizeof(Transducer), cudaMemcpyHostToDevice);
        if(trets!= cudaSuccess)
            std::cout << "error t: " << i << "--"<< cudaGetErrorString(trets) <<"\n";
    }

    cudaError_t rets = cudaMemcpyToSymbol(d_transducers, &temp, sizeof(temp));
    if(rets!= cudaSuccess)
        std::cout << "error: " << "--"<< cudaGetErrorString(rets) <<"\n";

    dim3 blockSize(pointGridData->resolutionX(), 1, 1);
    dim3 numblocks(pointGridData->resolutionY(), pointGridData->resolutionZ(), 1);

    simKernel<<<numblocks, blockSize>>>(d_magData,
                                        Vec3D(pointGridData->gridCenter().x,pointGridData->gridCenter().y, pointGridData->gridCenter().z),
                                        pointGridData->gridSize(),
                                        transducers->size(),
                                        pressureParameter);

    cudaDeviceSynchronize();

    cudaMemcpy3DParms memcpyParms = {0};
    memcpyParms.srcPtr=d_magData;
    memcpyParms.dstPtr.ptr=magData;
    memcpyParms.dstPtr.pitch=pointGridData->resolutionX() *sizeof(float);
    memcpyParms.dstPtr.xsize=pointGridData->resolutionX();
    memcpyParms.dstPtr.ysize=pointGridData->resolutionY();
    memcpyParms.extent.width=pointGridData->resolutionX() *sizeof(float);
    memcpyParms.extent.height=pointGridData->resolutionY();
    memcpyParms.extent.depth=pointGridData->resolutionZ();
    memcpyParms.kind=cudaMemcpyDeviceToHost;

    // int idata = -1;
    // float fdata = -1;
    // cudaMemcpyFromSymbol(&idata, iData, sizeof(int));
    // cudaMemcpyFromSymbol(&fdata, fData, sizeof(float));
    // std::cout << "idata: " << idata <<"\n";
    // std::cout << "fdata: " << fdata <<"\n";

    cudaError_t ret=cudaMemcpy3D(&memcpyParms);
    if(ret!= cudaSuccess)
        std::cout << "error " << cudaGetErrorString(ret) <<"\n";

    cudaFree(d_magData.ptr);
    cudaFree(d_transducers);
    return;
}
