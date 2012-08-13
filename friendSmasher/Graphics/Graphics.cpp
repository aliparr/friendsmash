#include "Graphics.h"
#include "mtx44_op.h"

namespace FriendSmasher
{
	namespace System
	{
		Graphics* Graphics::ms_Instance = NULL;

        Graphics* Graphics::Instance()
		{
			if( !ms_Instance ) {
				ms_Instance = new Graphics;
			}
			return ms_Instance;
		}
        
		Graphics::Graphics() :
			m_pSpriteManager(NULL),
            m_fScaleFactor(1.f)
		{
		}
		
		Graphics::~Graphics()
		{
		}
        
		bool Graphics::Initialise(float fScaleFactor)
		{
            m_fScaleFactor = fScaleFactor;
            
			FBAssert(ms_Instance);
            m_pSpriteManager = new SpriteManager();
			ResetAll2DViewMatrices();
            
			return true;
		}
		
		unsigned int Graphics::GetFrameBuffer() const
		{
            return 0;
		}
                
        int Graphics::GetBackbufferWidth() const
        {
            return 320 * m_fScaleFactor;
        }
        
        int Graphics::GetBackbufferHeight() const
        {
            return 480 * m_fScaleFactor;
        }
		
		void Graphics::Update()
		{	
            BeginRender();
			Render3D();
            Begin2DRender();
            Render2D();
			End2DRender();
            PostRender3D();
            EndRender();
        }
		
		bool Graphics::Uninitialise()
		{
			delete m_pSpriteManager;
			
			return true;
		}
		
		void Graphics::BeginRender()
		{
            glClearColor(0.25f, 0.15f, 0.65f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, GetBackbufferWidth(), GetBackbufferHeight());
		}
		
		void Graphics::EndRender()
		{
			
		}
		
		void Graphics::Begin2DRender()
		{
            
			float w = GetBackbufferWidth();
			float h = GetBackbufferHeight();
			
            if (Math::approxf(m_fScaleFactor, 1.f, Math::kEPSILON)) {
                w *= 2.f;
                h *= 2.f;
            }
 
            
            Math::mtx44MakeOrthographicOffCenterLH(m_projMatrix, 
											 0.0f, w, 
											 0.0f, h, 
											 0.0f, 1.0f);
		}
		
		void Graphics::Render2D()
		{
			m_pSpriteManager->Open();
			m_pSpriteManager->Close(m_projMatrix);
		}
		
		void Graphics::End2DRender()
		{
			return;
		}
		
		void Graphics::Render3D()
		{

		}
		
		void Graphics::PostRender3D()
		{

		}
		
		void Graphics::SetSpriteDrawingMode()
		{
            glDisable(GL_SCISSOR_TEST);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);			
		}
		
		void Graphics::SetSpriteMaskWriteMode()
		{
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		}
		
		void Graphics::SetSpriteMaskEnabledMode()
		{
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
		}
		
		void Graphics::SetScissorRegion(const Math::vec2& tl, const Math::vec2& br)
		{
			glEnable(GL_SCISSOR_TEST);
			
			float w = 0.f;
			float h = 0.f;
                
			w = static_cast<float>(GetBackbufferWidth());
			h = static_cast<float>(GetBackbufferHeight());
			            
			// Note the transpose of x/y on iPhone hardware	
			float tlx = (1.0f - br.y) * h;
			float tly = (1.0f - br.x) * w;
			
			float ox = (br.y - tl.y) * h;
			float oy = (br.x - tl.x) * w;
            			
			if (ox < 0.f) { ox = 0.f; }
			if (oy < 0.f) { oy = 0.f; }
			
			glScissor( static_cast<GLint>(tlx),
                      static_cast<GLint>(tly),
                      static_cast<GLsizei>(ox),
                      static_cast<GLsizei>(oy) );	
		}
        
		SpriteManager* Graphics::GetSpriteManager() const
		{
			return m_pSpriteManager;
		}

		void Graphics::Set2DViewMatrix( const Math::mtx33& mView, int index /* = 0 */ )
		{
			m_2DViewMatrix[index] = mView;
		}
        
		void Graphics::Get2DViewMatrix( Math::mtx33& mOutput, int index /* = 0 */ ) const
		{
			mOutput = m_2DViewMatrix[index];
		}

		void Graphics::Reset2DViewMatrix(int index /* = 0 */)
		{
			mtx33MakeIdentity(m_2DViewMatrix[index]);
		}
		
		void Graphics::ResetAll2DViewMatrices()
		{
			for( unsigned int i=0; i<kMaxNumViewMatrices; ++i ) {
				Reset2DViewMatrix(i);
			}
		}
        
        void Graphics::SetScaleFactor(const float fScaleFactor)
        {
            m_fScaleFactor = fScaleFactor;
        }
	}
}


