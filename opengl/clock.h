#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <string>


    class Clock {
      private:



        static inline int frame_count = 0;
        static inline float duration = 0.0f;

      public:
        static inline float delta_time = 0.0f;
        static inline float time = 0.0f;

        static inline float fps = 0.0f;  // frames per second (sampled every 0.1 second)
        static inline float ms = 0.0f;   // milliseconds per frame


        static void Reset();
        static void Update();
    };


#endif // CLOCK_H
