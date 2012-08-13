#include "random.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "math_op.h"

namespace FriendSmasher
{
    namespace Random
    {
            
        static float rcp = 1.0f / static_cast<float>(RAND_MAX);

        void Seed()
        {
            Seed(time(NULL));	
        }
            
        void Seed(u32 uSeed)
        {
            srand(uSeed);
        }
            
        int GetRandom()
        {
            return GetRandom(INT_MAX);
        }

        int GetRandom(int iMax)
        {
            iMax = Math::Max(iMax, 2);
            float fRand = (float)rand() * rcp;
            return static_cast<int>(fRand * static_cast<float>(iMax));
        }

        int GetRandom(int iMin, int iMax)
        {
            float t = static_cast<float>(rand()) * rcp;
            return static_cast<int>( 0.5f + static_cast<float>(iMin) + t * static_cast<float>(iMax - iMin) );
        }

        float GetRandom(float fMax)
        {
            float fRand = (float)rand() * rcp;
            return fRand * fMax;
        }

        float GetRandom(float fMin, float fMax)
        {
            float fRand = (float)rand() * rcp;
            float fRange = fMax - fMin;
            return fRand * fRange + fMin;
        }

        bool GetRandomBool()
        {
            return (rand()%2);
        }
        
    }
}