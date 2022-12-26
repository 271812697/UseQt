#include "clock.h"
#include<windows.h>
#include"pch.h"
//inline float Clock::time=0.0f;
//查询时间信息
LARGE_INTEGER litmp;
LONGLONG Qpart1,Qpart2;
double dfMinus,dfFreq,dfTime;

void Clock::Reset() {
    //start_time = std::chrono::system_clock::now();
    delta_time =0.0f;
    time = 0.0;
    total_time=0.0f;
    fps = ms = 0.0f;
    frame_count = 0;
    duration = 0.0f;
}

void Clock::Update() {

    static bool flag=false;
    if(!flag){
        flag=true;
        QueryPerformanceFrequency(&litmp);
        dfFreq=(double)litmp.QuadPart;
        QueryPerformanceCounter(&litmp);
        Qpart2=litmp.QuadPart;
    }
    QueryPerformanceCounter(&litmp);
    Qpart1=litmp.QuadPart;
    dfMinus=(double)(Qpart1-Qpart2);
    dfTime=dfMinus/dfFreq;
    Qpart2=Qpart1;
    delta_time = dfTime;

    CORE_INFO("time is in update delta {0}, total {1}",dfTime,time);
   // compute frames per second
    frame_count++;
    duration += delta_time;
    total_time+=delta_time;
    time =time+dfTime;
    if (duration >= 0.1f) {
        fps = frame_count / duration;
        ms = 1000.0f * duration / frame_count;
        frame_count = 0;

        duration = 0.0f;
    }
}
