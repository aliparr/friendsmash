#ifndef FRIENDSMASH_RANDOM
#define FRIENDSMASH_RANDOM

#include "type_defs.h"

namespace FriendSmasher
{
    namespace Random
    {
        void Seed(u32 uSeed);
        void Seed();

        int GetRandom();	
        int GetRandom(int iMax);
        int GetRandom(int iMin, int iMax);
        bool GetRandomBool();	
            
        float GetRandom(float fMax);
        float GetRandom(float fMin, float fMax);
    }
}

#endif 