#ifndef FRIENDSMASHER_TEXTURERESOURCE
#define FRIENDSMASHER_TEXTURERESOURCE

#include "type_defs.h"


namespace FriendSmasher
{
	namespace System
	{
        class TextureResource
        {
        public:
            TextureResource();
            ~TextureResource();
    	
            bool CreateFromFile(const char* filename);
            bool CreateFromFBID(int nID);
            
            u32 GetTextureID() const {return m_uTextureID[0];}
            
            u32 GetWidth() const;
			u32 GetHeight() const;
            bool GetIsReady() const { return m_bReady; }
            
        private:
            
            bool    m_bReady;
            u32     m_uWidth;
            u32     m_uHeight;
            u32		m_uTextureID[1];  
        };
        
	}
}

#endif // FRIENDSMASHER_SPRITE