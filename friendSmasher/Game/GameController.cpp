#include "GameController.h"

#include "debug.h"
#include "math_lib.h"
#include "random.h"
#include <stdlib.h>
#include <math.h>

namespace FriendSmasher
{
    namespace Game
    {
        GameController::GameController(ViewController* vc, float fScaleFactor) :
            m_fDeltaTime(0.f),
            m_kGameState(kGAMESTATE_FRONTSCREEN_LOGGEDOUT),
            m_fScaleFactor(fScaleFactor),
            m_pBackgroundSprite(NULL),
            m_pLoginButtonSprite(NULL),
            m_pPlayButtonSprite(NULL),
            m_pChallengeButtonSprite(NULL),
            m_pBragButtonSprite(NULL),
            m_pUserImageSprite(NULL),
            m_pFriendImageSprite(NULL),
            m_pLoadingSprite(NULL),
            m_pLogoSprite(NULL),
            m_pFriendTexture(NULL),
            m_nEntitiesSpawned(0),
            m_fSpawnTimer(0.f),
            m_uLivesRemaining(0),
            m_uCurrentScore(0),
            m_pUserTexture(NULL),
            m_kLossType(kUNDEFINED),
            m_pCelebLossEntity(NULL),
            m_pLogoutButtonSprite(NULL),
            m_nPlayerFBID(0),
            m_nNoSocialFriendCeleb(0)
        {
            for (int i=0; i<kMaxNumTouches; ++i) {
                m_bTouchedLastFrame[i] = false;
                m_bTouched[i] = false;
            }
            
            for (int i=0; i<10; ++i) {
                m_pNonFriendTexture[i] = NULL;
            }
                       
            for (u32 i=0; i<3; ++i) {
                m_pHeartSprite[i] = NULL;
            }
               
            m_labelName = [[UILabel alloc] initWithFrame:CGRectMake(135.0, 170.0, 220.0, 100.0)];
            m_labelName.textAlignment = UITextAlignmentLeft;
            m_labelName.textColor = [UIColor whiteColor];
            m_labelName.backgroundColor = [UIColor clearColor];
            m_labelName.font = [UIFont fontWithName:@"Futura" size:(18.0)];
            [vc.view addSubview: m_labelName];
            m_labelName.text = [NSString stringWithFormat:@"Hey: !"];
            m_labelName.hidden = YES;
            
            m_labelNameStatus = [[UILabel alloc] initWithFrame:CGRectMake(135.0, 190.0, 220.0, 100.0)];
            m_labelNameStatus.textAlignment = UITextAlignmentLeft;
            m_labelNameStatus.textColor = [UIColor whiteColor];
            m_labelNameStatus.backgroundColor = [UIColor clearColor];
            m_labelNameStatus.font = [UIFont fontWithName:@"Futura" size:(15.0)];
            [vc.view addSubview: m_labelNameStatus];
            m_labelNameStatus.text = [NSString stringWithFormat:@"Let's smash some friends!"];
            m_labelNameStatus.hidden = YES;
            
            m_labelFriendName = [[UILabel alloc] initWithFrame:CGRectMake(0.0, 2.0, 320.0, 20.0)];
            m_labelFriendName.textAlignment = UITextAlignmentCenter;
            m_labelFriendName.textColor = [UIColor whiteColor];
            m_labelFriendName.backgroundColor = [UIColor clearColor];
            m_labelFriendName.font = [UIFont fontWithName:@"Futura" size:(18.0)];
            [vc.view addSubview: m_labelFriendName];
            m_labelFriendName.text = [NSString stringWithFormat:@""];
            m_labelFriendName.hidden = YES;
            
            m_labelScore = [[UILabel alloc] initWithFrame:CGRectMake(210.0, 2.0, 100.0, 20.0)];
            m_labelScore.textAlignment = UITextAlignmentRight;
            m_labelScore.textColor = [UIColor whiteColor];
            m_labelScore.backgroundColor = [UIColor clearColor];
            m_labelScore.font = [UIFont fontWithName:@"Futura" size:(12.0)];
            [vc.view addSubview: m_labelScore];
            m_labelScore.text = [NSString stringWithFormat:@"Score: 0"];
            m_labelScore.hidden = YES;
            
        }

        GameController::~GameController()
        {
            delete m_pBackgroundSprite;
            delete m_pLoginButtonSprite;
            delete m_pPlayButtonSprite;
            delete m_pUserImageSprite;
            delete m_pFriendImageSprite;
            delete m_pLoadingSprite;
            delete m_pLogoSprite;
            delete m_pFriendTexture;
            delete m_pUserTexture;
            delete m_pChallengeButtonSprite;
            delete m_pBragButtonSprite;
            
            for (int i=0; i<10; ++i) {
                delete m_pNonFriendTexture[i];
            }

            for (u32 i=0; i<3; ++i) {
                delete m_pHeartSprite[i];
            }
        }
		
        void GameController::OnEnter()
        {
            Random::Seed();
            
            System::Graphics::Instance()->Initialise(m_fScaleFactor);
            
            System::TextureResource* pBackgroundTextureResource = new System::TextureResource();
            pBackgroundTextureResource->CreateFromFile("Art/frontscreen_background.png");
            m_pBackgroundSprite = new System::Sprite(pBackgroundTextureResource);
            
            System::TextureResource* pLogoTextureResource = new System::TextureResource();
            pLogoTextureResource->CreateFromFile("Art/logo2.tga");
            m_pLogoSprite = new System::Sprite(pLogoTextureResource);
            m_pLogoSprite->SetPosition(Math::vec2(100.f, 60.f));
            
            System::TextureResource* pLoginButtonTextureResource = new System::TextureResource();
            pLoginButtonTextureResource->CreateFromFile("Art/login_button.tga");
            m_pLoginButtonSprite = new System::Sprite(pLoginButtonTextureResource);
            m_pLoginButtonSprite->SetPosition(Math::vec2(160.f, 740.f));
            m_pLoginButtonSprite->SetLayer(10);
            
            System::TextureResource* pLogoutButtonTextureResource = new System::TextureResource();
            pLogoutButtonTextureResource->CreateFromFile("Art/logout_button.tga");
            m_pLogoutButtonSprite = new System::Sprite(pLogoutButtonTextureResource);
            m_pLogoutButtonSprite->SetPosition(Math::vec2(160.f, 780.f));
            m_pLogoutButtonSprite->SetLayer(10);
            m_pLogoutButtonSprite->SetDraw(false);
            
            
            System::TextureResource* pPlayButtonTextureResource = new System::TextureResource();
            pPlayButtonTextureResource->CreateFromFile("Art/playnow_button.tga");
            m_pPlayButtonSprite = new System::Sprite(pPlayButtonTextureResource);
            m_pPlayButtonSprite->SetDraw(false);
            m_pPlayButtonSprite->SetLayer(10);
            m_pPlayButtonSprite->SetPosition(Math::vec2(160.f, 570.f));
            
            System::TextureResource* pChallengeButtonTextureResource = new System::TextureResource();
            pChallengeButtonTextureResource->CreateFromFile("Art/challenge_button.tga");
            m_pChallengeButtonSprite = new System::Sprite(pChallengeButtonTextureResource);
            m_pChallengeButtonSprite->SetDraw(false);
            m_pChallengeButtonSprite->SetLayer(10);
            m_pChallengeButtonSprite->SetPosition(Math::vec2(160.f, 640.f));        
            
            
            System::TextureResource* pBragButtonTextureResource = new System::TextureResource();
            pBragButtonTextureResource->CreateFromFile("Art/brag_button.tga");
            m_pBragButtonSprite = new System::Sprite(pBragButtonTextureResource);
            m_pBragButtonSprite->SetDraw(false);
            m_pBragButtonSprite->SetLayer(10);
            m_pBragButtonSprite->SetPosition(Math::vec2(160.f, 710.f));   
            
            
            System::TextureResource* pLoadingTextureResource = new System::TextureResource();
            pLoadingTextureResource->CreateFromFile("Art/isloading.tga");
            m_pLoadingSprite = new System::Sprite(pLoadingTextureResource);
            m_pLoadingSprite->SetDraw(false);
            m_pLoadingSprite->SetLayer(10);
            m_pLoadingSprite->SetPosition(Math::vec2(230.f, 600.f));
            
            System::TextureResource* pHeartTextureResource = new System::TextureResource();
            pHeartTextureResource->CreateFromFile("Art/heart_red.tga");
            
            for (u32 i=0; i<3; ++i)
            {
                m_pHeartSprite[i] = new System::Sprite(pHeartTextureResource);
                m_pHeartSprite[i]->SetDraw(false);
                m_pHeartSprite[i]->SetLayer(20 + i);
                m_pHeartSprite[i]->SetScale(Math::vec2(0.6f, 0.6f));
                m_pHeartSprite[i]->SetPosition(Math::vec2(20.f + (80.f * static_cast<float>(i)), 40.f));
            }
                 
            for (int i=0; i<10; ++i)
            {
                m_pNonFriendTexture[i] = new System::TextureResource();
                char buf[128];
                sprintf(buf, "nonfriend_%d.png", i+1);
                m_pNonFriendTexture[i]->CreateFromFile(buf);
            }
            
            
            FB_CreateNewSession();
            
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
            if (appDelegate.session.state == FBSessionStateCreatedTokenLoaded) {
                FB_Login();
                
            }
            else if (m_kGameState == kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY) {
                UpdateView(false);
            }
 
        }

        void GameController::OnExit()
        {
            System::Graphics::Instance()->Uninitialise();
        }
		
        void GameController::OnRender()
        {
            System::Graphics::Instance()->Update();
        }
        
        void GameController::OnUpdate()
        {   
            if (m_kGameState == kGAMESTATE_FRONTSCREEN_LOGGEDOUT)
            {
                if (!m_bTouched[0] && m_bTouchedLastFrame[0]) {
                                    
                    if (m_pLoginButtonSprite->IsPointInside(m_vEndTouchPos[0], 25.f)) 
                    {
                        AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
                        
                        if (!appDelegate.session.isOpen) {
                            
                            FB_CreateNewSession();
                            FB_Login();
                        }
                    }
                } 
            }
            else if (m_kGameState == kGAMESTATE_FRONTSCREEN_LOGGEDIN_PREPARING)
            {
                if (m_pUserTexture && m_pUserTexture->GetIsReady())
                {
                    m_pUserImageSprite = new System::Sprite(m_pUserTexture);
                    m_pUserImageSprite->SetLayer(10);
                    m_pUserImageSprite->SetPivot(Math::vec2(m_pUserImageSprite->GetWidth()*0.5f, m_pUserImageSprite->GetHeight()*0.5f));
                    m_pUserImageSprite->SetPosition(Math::vec2(180.f, 470.f));
                    
                    m_labelName.text = [NSString stringWithFormat:@"Welcome, %@!", m_nsstrUserName];
                    m_labelName.hidden = NO;
                    
                    m_labelNameStatus.hidden = NO;
                    
                    m_pPlayButtonSprite->SetDraw(true);
                    m_pLogoutButtonSprite->SetDraw(true);
                                        
                    m_kGameState = kGAMESTATE_FRONTSCREEN_LOGGEDIN_READY;
                    
                }
            }
            else if (m_kGameState == kGAMESTATE_FRONTSCREEN_LOGGEDIN_READY || m_kGameState == kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY)
            {
                if (!m_bTouched[0] && m_bTouchedLastFrame[0] && m_pPlayButtonSprite->IsPointInside(m_vEndTouchPos[0], 0.f)
                    && m_pPlayButtonSprite->GetDraw()) 
                {
                    StartGame(m_kGameState != kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY, false, nil, nil);
                }
                
                if (!m_bTouched[0] && m_bTouchedLastFrame[0] && m_pLogoutButtonSprite->IsPointInside(m_vEndTouchPos[0], 0.f)
                    && m_pLogoutButtonSprite->GetDraw()) 
                {
                    FB_Logout();
                }
                
                if (!m_bTouched[0] && m_bTouchedLastFrame[0] && m_pBragButtonSprite->IsPointInside(m_vEndTouchPos[0], 0.f)
                    && m_pBragButtonSprite->GetDraw()) 
                {
                    FB_SendBrag(m_uCurrentScore);
                }
                
                if (!m_bTouched[0] && m_bTouchedLastFrame[0] && m_pChallengeButtonSprite->IsPointInside(m_vEndTouchPos[0], 0.f)
                    && m_pChallengeButtonSprite->GetDraw()) 
                {
                    FB_SendRequest(m_uCurrentScore);
                }
            }
          
            else if (m_kGameState == kGAMESTATE_FRONTSCREEN_NOSOCIAL_LOADING || m_kGameState == kGAMESTATE_FRONTSCREEN_LOGGEDIN_LOADING)
            {
                if (m_pFriendTexture && m_pFriendTexture->GetIsReady())
                {
                    m_pLoadingSprite->SetDraw(false);
                    m_pLogoSprite->SetDraw(false);
                    
                    m_uCurrentScore = 0;
                    m_uLivesRemaining = 3;
                    
                    for (u32 i=0; i<m_uLivesRemaining; ++i) {
                        m_pHeartSprite[i]->SetDraw(true);
                    }
                    
                    m_labelScore.text = [NSString stringWithFormat:@"Score: %u", m_uCurrentScore];
                    m_labelScore.hidden = NO;
                    
                    m_labelFriendName.hidden = NO;

                    m_kLossType = kUNDEFINED;
                    m_pCelebLossEntity = NULL;
                    
                    if (m_kGameState == kGAMESTATE_FRONTSCREEN_NOSOCIAL_LOADING) {
                        m_kGameState = kGAMESTATE_PLAYING_NOSOCIAL;  
                    }
                    else {
                        m_kGameState = kGAMESTATE_PLAYING;                        
                    }

                    return;
                }
   
            }
            else if (m_kGameState == kGAMESTATE_PLAYING || m_kGameState == kGAMESTATE_PLAYING_NOSOCIAL)
            {
                m_fSpawnTimer -= 1.0f / 30.0f;
                
                if (m_fSpawnTimer < 0.f) 
                {
                    SpawnEntity();
                    m_fSpawnTimer = 0.7;
                }
                               
                std::vector<EntityInstance*>::iterator itr = m_vecEntities.begin();
                std::vector<EntityInstance*>::iterator end = m_vecEntities.end();
                for (; itr != end; ++itr)
                {
                    EntityInstance* pCurrentEntity = *itr;
                    pCurrentEntity->pSprite->SetPosition(pCurrentEntity->pSprite->GetPosition() + pCurrentEntity->vVelocity);
                    pCurrentEntity->vVelocity.y += 1.f;
                    pCurrentEntity->pSprite->SetRotation(pCurrentEntity->pSprite->GetRotation() + pCurrentEntity->fRotationVelocity);
                    
                    if (pCurrentEntity->pSprite->GetPosition().y > 1000.f && pCurrentEntity->bFriend) {
                        m_vecEntities.erase(itr);
                        pCurrentEntity->pSprite->SetDraw(false);
                        delete pCurrentEntity->pSprite;
                        delete pCurrentEntity;
                        m_uLivesRemaining--;
                        m_pHeartSprite[m_uLivesRemaining]->SetDraw(false);
                        m_kLossType = kLOSSTYPE_RANOUTOFLIVES;

                        itr = m_vecEntities.begin();
                        end = m_vecEntities.end();
                        continue;
                    }
                    
                    if (m_bTouched[0] && !m_bTouchedLastFrame[0] && pCurrentEntity->pSprite->IsPointInside(m_vBeginTouchPos[0], 25.f)) 
                    {
                        if (!pCurrentEntity->bFriend)
                        {
                            m_pCelebLossEntity = pCurrentEntity;
                            m_pCelebLossEntity->pSprite->SetLayer(m_nEntitiesSpawned + 100);
                            m_kLossType = kLOSSTYPE_HITCELEB;
                            m_uLivesRemaining = 0;
                        }
                        else
                        {
                            m_uCurrentScore++;
                            m_labelScore.text = [NSString stringWithFormat:@"Score: %u", m_uCurrentScore];
                            
                            m_vecEntities.erase(itr);
                            pCurrentEntity->pSprite->SetDraw(false);
                            delete pCurrentEntity->pSprite;
                            delete pCurrentEntity;
                            
                            
                            // If they start to score well, spawn extras
                            if ( !(m_uCurrentScore % 10) )
                            {
                                for (int i=0; i<(m_uCurrentScore/10); ++i) {
                                    SpawnEntity();
                                }
                            }
                            
                            itr = m_vecEntities.begin();
                            end = m_vecEntities.end();
                            continue;
                        }
                    }
                }
                
                // Game over
                if (m_uLivesRemaining == 0) {                    
                    m_kGameState = m_kGameState == kGAMESTATE_PLAYING ? kGAMESTATE_PLAYING_GAMEOVER : kGAMESTATE_PLAYING_GAMEOVER_NOSOCIAL;
                }
                            
            }
            else if (m_kGameState == kGAMESTATE_PLAYING_GAMEOVER || m_kGameState == kGAMESTATE_PLAYING_GAMEOVER_NOSOCIAL)
            {
                if (m_kLossType == kLOSSTYPE_HITCELEB && m_pCelebLossEntity)
                {
                    float fDistanceToUpright = fmodf(m_pCelebLossEntity->pSprite->GetRotation(),6.28318531f);
                    
                    m_pCelebLossEntity->pSprite->SetRotation(m_pCelebLossEntity->pSprite->GetRotation() + (fDistanceToUpright * 0.05f));

                    m_pCelebLossEntity->pSprite->SetScale(m_pCelebLossEntity->pSprite->GetScale() * Math::vec2(1.1f, 1.1f));
                    
                    if (m_pCelebLossEntity->pSprite->GetScale().x > 30.f)
                    {
                        EndGame();
                    }
                }
                else {
                    EndGame();
                }
                
            }
            
            for (int i=0; i<kMaxNumTouches; ++i) {
				m_bTouchedLastFrame[i] = m_bTouched[i];
            }
        }
        
        void GameController::StartGame(bool bSocial, bool bChallenge, NSString *challengeFriendName,  NSString *challengeFriendId)
        {
            m_pLoadingSprite->SetDraw(true);
            
            m_pPlayButtonSprite->SetDraw(false);
            m_pLogoutButtonSprite->SetDraw(false);
            m_pChallengeButtonSprite->SetDraw(false);
            m_pBragButtonSprite->SetDraw(false);
            
            m_labelName.hidden = YES;
            m_labelNameStatus.hidden = YES;
            
            if (bSocial)
            {
                m_pUserImageSprite->SetDraw(false);   
                
                if (bChallenge) {
                    
                    m_labelFriendName.text = [NSString stringWithFormat:@"Smash %@ !", [[challengeFriendName componentsSeparatedByString:@" "] objectAtIndex:0]];
                    
                    if (m_pFriendTexture) { 
                        delete m_pFriendTexture;
                    }
                    
                    m_pFriendTexture = new System::TextureResource();
                    m_pFriendTexture->CreateFromFBID([challengeFriendId intValue]);
                    
                    m_kGameState = kGAMESTATE_FRONTSCREEN_LOGGEDIN_LOADING;
                    
                }
                else {
                    [[FBRequest requestForGraphPath:@"me/friends"]
                     startWithCompletionHandler:
                     ^(FBRequestConnection *connection, 
                       NSDictionary *result,
                       NSError *error) 
                     {
                        if (!error && result) 
                        {
                            fetchedFriendData = [[NSArray alloc] initWithArray:[result objectForKey:@"data"]];
                            
                            NSDictionary *friendData = [fetchedFriendData objectAtIndex: arc4random() % fetchedFriendData.count];
                             
                            NSString *friendId = [friendData objectForKey:@"id"];
                            NSString *friendName = [friendData objectForKey:@"name"];
                             
                            m_labelFriendName.text = [NSString stringWithFormat:@"Smash %@ !", [[friendName componentsSeparatedByString:@" "] objectAtIndex:0]];
                             
                            if (m_pFriendTexture) { 
                                delete m_pFriendTexture;
                            }
                             
                            m_pFriendTexture = new System::TextureResource();
                            m_pFriendTexture->CreateFromFBID([friendId intValue]);
                             
                            m_kGameState = kGAMESTATE_FRONTSCREEN_LOGGEDIN_LOADING;
                     
                        }
                     }];  
                }
                
                
            }
            else 
            {
                if (m_pFriendTexture) { 
                    delete m_pFriendTexture;
                }
                
                m_nNoSocialFriendCeleb = Random::GetRandom(1, 10);
                
                char buf[128];
                sprintf(buf, "nonfriend_%d.png", m_nNoSocialFriendCeleb);
                
                m_pFriendTexture = new System::TextureResource();
                m_pFriendTexture->CreateFromFile(buf);
                
                
                NSArray *celebArray =  [NSArray arrayWithObjects:
                                        [NSString stringWithFormat:@"Einstein"],
                                        [NSString stringWithFormat:@"Xzibit"],
                                        [NSString stringWithFormat:@"Goldsmith"],
                                        [NSString stringWithFormat:@"Sinatra"],
                                        [NSString stringWithFormat:@"George"],
                                        [NSString stringWithFormat:@"Jacko"],
                                        [NSString stringWithFormat:@"Rick"],
                                        [NSString stringWithFormat:@"Keanu"],
                                        [NSString stringWithFormat:@"Arnie"],
                                        [NSString stringWithFormat:@"Jean-Luc"],
                                        nil];
                
                m_labelFriendName.text = [NSString stringWithFormat:@"Smash %@ !", [celebArray objectAtIndex:m_nNoSocialFriendCeleb-1]];
                
                m_kGameState = kGAMESTATE_FRONTSCREEN_NOSOCIAL_LOADING;
            }
            
            
        }
        
        void GameController::EndGame()
        {
            std::vector<EntityInstance*>::iterator itr = m_vecEntities.begin();
            std::vector<EntityInstance*>::iterator end = m_vecEntities.end();
            for (; itr != end; ++itr)
            {
                EntityInstance* pCurrentEntity = *itr;
                delete pCurrentEntity->pSprite;
                delete pCurrentEntity;
            }
            
            m_vecEntities.clear();
            
            m_uLivesRemaining = 3;
            for (u32 i=0; i<m_uLivesRemaining; ++i) {
                m_pHeartSprite[i]->SetDraw(false);
            }
            
            m_pPlayButtonSprite->SetDraw(true);
            m_pLogoSprite->SetDraw(true);
            
            delete m_pFriendTexture;
            m_pFriendTexture = NULL;
            
            m_labelScore.hidden = YES;
            m_labelFriendName.hidden = YES;
            
            m_labelName.text = [NSString stringWithFormat:@"Game Over!"];
            m_labelNameStatus.text = [NSString stringWithFormat:@"You scored %d!", m_uCurrentScore];
            
            m_labelName.hidden = NO;
            m_labelNameStatus.hidden = NO;
            
            if (m_kGameState == kGAMESTATE_PLAYING_GAMEOVER)
            {
                m_pUserImageSprite->SetDraw(true);
                m_pLogoutButtonSprite->SetDraw(true);
                m_pChallengeButtonSprite->SetDraw(true);
                m_pBragButtonSprite->SetDraw(true);
                
                FB_SendScore(m_uCurrentScore);
                
                m_kGameState = kGAMESTATE_FRONTSCREEN_LOGGEDIN_READY;
            }
            else {
                m_kGameState = kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY;
            }
        }
        
        
        void GameController::SpawnEntity()
        {
            EntityInstance* pEntity = new EntityInstance;
            m_vecEntities.push_back(pEntity);
            
            float fEntityType = Random::GetRandom(0.f, 100.f);

            
            if (fEntityType < 80.f) {
                pEntity->pSprite = new System::Sprite(m_pFriendTexture);
                pEntity->bFriend = true;
            }
            else {
                
                int nCelebToSpawn = Random::GetRandom(0, 9);
                
                if (m_kGameState == kGAMESTATE_PLAYING_NOSOCIAL) 
                {
                    while (nCelebToSpawn == (m_nNoSocialFriendCeleb-1))
                    {
                        nCelebToSpawn = Random::GetRandom(0, 9);    
                    }
                }
                
                pEntity->pSprite = new System::Sprite(m_pNonFriendTexture[nCelebToSpawn]);
                pEntity->bFriend = false;
            }
            
            
            pEntity->pSprite->SetLayer(10 + m_nEntitiesSpawned);
            pEntity->pSprite->SetPivot(Math::vec2(pEntity->pSprite->GetWidth()*0.5f, pEntity->pSprite->GetHeight()*0.5f));
            pEntity->pSprite->SetPosition(Math::vec2(Random::GetRandom(-200.f, 840.f), 1000.f));
            
            pEntity->fRotationVelocity = Random::GetRandom(-0.5f, 0.5f);

            float fDistanceToMiddle = Random::GetRandom(200.f, 440.f) - pEntity->pSprite->GetPosition().x;
            
            pEntity->vVelocity = Math::vec2(fDistanceToMiddle * Random::GetRandom(0.02f, 0.03f), Random::GetRandom(-42.f, -34.f));
            
            m_nEntitiesSpawned++;
        }
        

        
        void GameController::UpdateView(bool bSocial)
        {
            if (bSocial)
            {
                AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
                
                if (appDelegate.session.isOpen) {        
                    m_kGameState = kGAMESTATE_FRONTSCREEN_LOGGEDIN_PREPARING;
                    
                    m_pLoginButtonSprite->SetDraw(false);
                    
                    FB_Customize();
                    
                } else {        
                    m_kGameState = kGAMESTATE_FRONTSCREEN_LOGGEDOUT;
                }
            }
            else {
                m_pLoginButtonSprite->SetDraw(false);
                
                m_pPlayButtonSprite->SetDraw(true);
                
                m_kGameState = kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY;
            }
        }
          
        void GameController::BeginTouch(int nIndex, float fX, float fY)
        {
            m_bTouched[nIndex] = true;
            m_vBeginTouchPos[nIndex] = Math::vec2(fX, fY);
            m_vCurrentTouchPos[nIndex] = Math::vec2(fX, fY);
        }
        
        void GameController::ContinueTouch(int nIndex, float fX, float fY)
        {
            m_vCurrentTouchPos[nIndex] = Math::vec2(fX, fY);
        }
        
        void GameController::EndTouch(int nIndex, float fX, float fY)
        {
            m_bTouched[nIndex] = false;
            m_vEndTouchPos[nIndex] = Math::vec2(fX, fY);	
            m_vCurrentTouchPos[nIndex] = Math::vec2::allzero;
        }
    }
		
}