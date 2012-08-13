#include "GenericShaders.h"

namespace FriendSmasher
{
    namespace GenericShaders
    {
        // Generic sprite vertex shader
        static const char* g_SpriteVertexShader = 
        "	varying vec4 oColor;													\n"
        "	varying vec2 oTexcoord;													\n"
        "	attribute vec4 vColor;													\n"
        "	attribute vec2 vTexcoord;												\n"
        "	attribute vec4 vPosition;												\n"
        "	uniform mat4 g_matWorldViewProj;										\n"
        "	void main ()															\n"
        "	{																		\n"
        "		gl_Position = (vPosition * g_matWorldViewProj);						\n"
        "		oColor = vColor;													\n"
        "		oTexcoord = vTexcoord;												\n"
        "	}																		\n"
        "																			\n";
                
        // Generic sprite pixel shader
        static const char* g_SpriteFragmentShader = 
        "	precision lowp float;													\n"	
        "	varying vec4 oColor;													\n"
        "	varying vec2 oTexcoord;													\n"
        "	uniform sampler2D texture;												\n"
        "																			\n"
        "	void main ()															\n"
        "	{																		\n"
        "		gl_FragColor = (texture2D (texture, oTexcoord) * oColor);			\n"
        "	}																		\n"
        "																			\n";

        const char* GetSpriteVertexShader()
        {
            return g_SpriteVertexShader;
        }

        const char* GetSpriteFragmentShader()
        {
            return g_SpriteFragmentShader;
        }
        
    }
}


