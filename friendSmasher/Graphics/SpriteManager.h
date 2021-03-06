#ifndef FRIENDSMASHER_SPRITEMANAGER
#define FRIENDSMASHER_SPRITEMANAGER

#include "vec2.h"
#include "vec4.h"
#include "mtx44.h"
#include "type_defs.h"

namespace FriendSmasher
{
	namespace System
	{
		class Sprite;
		class ShaderProgram;
		
		class SpriteManager
		{
		public:
			SpriteManager();
			~SpriteManager();
            
            enum eLocationStream
			{
				kVERTEX_ARRAY = 0,
				kTEXCOORD_ARRAY,
				kCOLOR_ARRAY
			};
			
            void					AddSprite( Sprite* pSprite );
			void					RemoveSprite( Sprite* pSprite );
            
			virtual void			Open();
			virtual void			Close(const Math::mtx44& projection);
			
		private:
            
            void SortList();
			
			struct Vertex
			{
				float x, y;
				float u, v;
				float r, g, b, a;
			};
			
			Vertex* m_vertices;
			
			unsigned int m_bufferId[2];
			unsigned int m_indexBufferId;
			unsigned int m_currentBuffer;
			
			u16* m_uIndices;
			
            static const int kMaxNumSprites = 1024;
			
			Sprite*					m_apSprites[kMaxNumSprites];					/// The array of sprites 
			unsigned int			m_uNumSprites;									/// The number of sprites that need to be drawn this frame.
			
			
			ShaderProgram* m_pSpriteShader;		
            int            m_mvpUniformLocation;
		};
		
	}
}


#endif // FRIENDSMASHER_SPRITEMANAGER_IPHONE