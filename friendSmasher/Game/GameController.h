#ifndef FRIENDSMASHER_GAMECONTROLLER
#define FRIENDSMASHER_GAMECONTROLLER

#include <vector>

#include "math_lib.h"

#include "Graphics.h"
#include "Sprite.h"
#include "TextureResource.h"

#import "ViewController.h"
#import "AppDelegate.h"

namespace FriendSmasher
{
    namespace Game
    { 
        class GameController
        {
            
        public:

            GameController(ViewController* vc, float fScaleFactor);
            virtual ~GameController();

            void OnEnter();
            void OnExit();
            void OnUpdate();
            void OnRender();
            
            void BeginTouch(int nIndex, float fX, float fY);
            void ContinueTouch(int nIndex, float fX, float fY);
            void EndTouch(int nIndex, float fX, float fY);
                

            // Facebook Integration
            void FB_CreateNewSession();
            void FB_Login();
            void FB_Logout();
            void FB_ProcessIncomingURL();
            void FB_ProcessIncomingRequest(NSString* urlString);
            void FB_ProcessIncomingFeed(NSString* urlString);
            void FB_Customize();
            void FB_SendRequest(const int nScore);
            void FB_SendFilteredRequest(const int nScore);
            void FB_SendBrag(const int nScore);
            void FB_SendScore(const int nScore);
            
        protected:    
            
            void SpawnEntity();
            void UpdateView(bool bSocial);
            void EndGame();
            void StartGame(bool bSocial, bool bChallenge, NSString *challengeFriendName,  NSString *challengeFriendId);
            
            System::Sprite* m_pBackgroundSprite;
            System::Sprite* m_pLoginButtonSprite;
            System::Sprite* m_pPlayButtonSprite;
            System::Sprite* m_pLogoutButtonSprite;
            System::Sprite* m_pChallengeButtonSprite;
            System::Sprite* m_pBragButtonSprite;
            System::Sprite* m_pUserImageSprite;
            System::Sprite* m_pFriendImageSprite;
            System::Sprite* m_pLoadingSprite;
            System::Sprite* m_pLogoSprite;
            System::Sprite* m_pHeartSprite[3];
            
                
            //////
            
            System::TextureResource* m_pUserTexture;
            System::TextureResource* m_pFriendTexture;
            System::TextureResource* m_pNonFriendTexture[10];
            
            ///////
            UILabel *m_labelName;
            UILabel *m_labelNameStatus;
            UILabel *m_labelFriendName;
            UILabel *m_labelScore;
            NSString* m_nsstrUserName;
            
            NSArray* fetchedFriendData;
            
            struct EntityInstance
            {
                System::Sprite* pSprite;
                Math::vec2 vVelocity;
                float fRotationVelocity;
                bool bFriend;
            };
            
            std::vector<EntityInstance*> m_vecEntities;
            
            int m_nEntitiesSpawned;
            
            int m_nNoSocialFriendCeleb;
            
            float m_fSpawnTimer;
            
            ///////
                
            
            enum eGameState
            {
                kGAMESTATE_FRONTSCREEN_LOGGEDOUT,
                kGAMESTATE_FRONTSCREEN_LOGGEDIN_PREPARING,
                kGAMESTATE_FRONTSCREEN_LOGGEDIN_READY,
                kGAMESTATE_FRONTSCREEN_LOGGEDIN_LOADING,
                kGAMESTATE_FRONTSCREEN_NOSOCIAL_LOADING,
                kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY,
                kGAMESTATE_PLAYING,
                kGAMESTATE_PLAYING_NOSOCIAL,
                kGAMESTATE_PLAYING_GAMEOVER,
                kGAMESTATE_PLAYING_GAMEOVER_NOSOCIAL,
            };
            
            eGameState m_kGameState;
            float m_fDeltaTime;
            
            static const int kMaxNumTouches = 5;
            Math::vec2 m_vBeginTouchPos[kMaxNumTouches];
            Math::vec2 m_vCurrentTouchPos[kMaxNumTouches];
            Math::vec2 m_vEndTouchPos[kMaxNumTouches];
            bool m_bTouched[kMaxNumTouches];
            bool m_bTouchedLastFrame[kMaxNumTouches];
            
            float m_fScaleFactor;
            
            u32 m_uLivesRemaining;
            u32 m_uCurrentScore;
            
            enum eLossType
            {
                kUNDEFINED,
                kLOSSTYPE_RANOUTOFLIVES,
                kLOSSTYPE_HITCELEB,
            };
            
            int m_nPlayerFBID;
            
            eLossType m_kLossType;
            
            EntityInstance* m_pCelebLossEntity;

        };
    }
}

#endif