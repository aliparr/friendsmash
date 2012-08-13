#include "GameController.h"

namespace FriendSmasher
{
    namespace Game
    {            
        
        // Create a Facebook session for a given set of permissions
        
        void GameController::FB_CreateNewSession()
        {
            //m_kGameState = kGAMESTATE_FRONTSCREEN_NOSOCIAL_READY;
            //return;
            
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
                
            NSArray *permissions = [[NSArray alloc] initWithObjects:
                                    @"email", @"publish_actions",
                                    nil];
            
            appDelegate.session = [[FBSession alloc] initWithPermissions:permissions];
             
        }
        
        // Attempt to open the session - perhaps tabbing over to Facebook to authorise
        void GameController::FB_Login()
        {
            //return;
         
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
            // Attempt to open the session. If the session is not open, show the user the Facebook login UX
            [appDelegate.session openWithCompletionHandler:^(FBSession *session, 
                                                             FBSessionState status, 
                                                             NSError *error) 
             {
                // Did something go wrong during login? I.e. did the user cancel?
                if (status == FBSessionStateClosedLoginFailed || status == FBSessionStateCreatedOpening) {

                    // If so, just send them round the loop again
                    [appDelegate.session closeAndClearTokenInformation];
                    appDelegate.session = nil;
                    FB_CreateNewSession();
                }
                else 
                {
                    // Great, we logged in successfully - let's make sure the active session is this one
                    [FBSession setActiveSession: session];
             
                    // Required to initialise the old SDK FB object here so we can play with Dialogs
                    appDelegate.facebook = [[Facebook alloc] initWithAppId:appDelegate.session.appID andDelegate:nil];
             
                    // Initialise the old SDK with our new credentials
                    appDelegate.facebook.accessToken = appDelegate.session.accessToken;
                    appDelegate.facebook.expirationDate = appDelegate.session.expirationDate;
             
                    // Update our game now we've logged in
                    if (m_kGameState == kGAMESTATE_FRONTSCREEN_LOGGEDOUT) {
                        UpdateView(true);
                    }
                }
                             
             }];
             
        }
        
        
        void GameController::FB_Customize()
        {
            // Provide some social context to the game by requesting the basic details of the player from facebook
            
            // Start the facebook request
            [[FBRequest requestForGraphPath:@"me"]
              startWithCompletionHandler:
              ^(FBRequestConnection *connection, NSDictionary<FBGraphUser> *result, NSError *error) 
             {
                // Did everything come back okay with no errors?
                if (!error && result)
                {
                    // If so we can extract out the player's Facebook ID and first name
                    m_nPlayerFBID = [result.id intValue];
             
                    m_nsstrUserName = [[NSString alloc] initWithString:result.first_name];
             
                    // Create a texture from the user's profile picture
                    m_pUserTexture = new System::TextureResource();
                    m_pUserTexture->CreateFromFBID(m_nPlayerFBID);
                }
             }];
             
        }
        
        
        void GameController::FB_ProcessIncomingURL()
        {
            // Process the incoming url and see if it's of value...
            
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
            NSString *urlString = [appDelegate.openedURL fragment];
            NSRange range = [urlString rangeOfString:@"notif" options:NSCaseInsensitiveSearch];
            
            
            // If the url contains 'notif', we know it's coming from a notification - let's process it
            if(urlString != nil && range.location != NSNotFound) 
            {
                // Yes the incoming URL was a notification
                FB_ProcessIncomingRequest(urlString);
            }
            
            range = [urlString rangeOfString:@"challenge%5Fbrag" options:NSCaseInsensitiveSearch];
            
            // If the url contains 'challenge_brag', we know it comes from a feed post
            if(urlString != nil && range.location != NSNotFound) 
            {
                // Yes the incoming URL was a notification
                FB_ProcessIncomingFeed(urlString);
            }
             
        }
        
        void GameController::FB_ProcessIncomingRequest(NSString* urlString)
        {

            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
            // Extract the notification id
            
            NSArray *pairs = [urlString componentsSeparatedByString:@"&"];
            NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
            for (NSString *pair in pairs) 
            {
                NSArray *kv = [pair componentsSeparatedByString:@"="];
                NSString *val = [[kv objectAtIndex:1]
                                 stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                
                [params setObject:val forKey:[kv objectAtIndex:0]];
            }
            
            NSString *targetURLString = [params valueForKey:@"target_url"];
            
            if (targetURLString) 
            {
                NSArray *pairs = [targetURLString componentsSeparatedByString:@"&"];
                NSMutableDictionary *targetParams = [[NSMutableDictionary alloc] init];
                
                for (NSString *pair in pairs) 
                {
                    NSArray *kv = [pair componentsSeparatedByString:@"="];
                    NSString *val = [[kv objectAtIndex:1]
                                     stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                    
                    [targetParams setObject:val forKey:[kv objectAtIndex:0]];
                }
                
                NSString *requestIDParam = [targetParams objectForKey:@"request_ids"];
                NSArray *requestIDs = [requestIDParam  componentsSeparatedByString:@","];
                
                FBRequest *req = [[FBRequest alloc] initWithSession:appDelegate.session graphPath:[requestIDs objectAtIndex:0]];
                
                [req startWithCompletionHandler:^(FBRequestConnection *connection, id result, NSError *error) 
                 {
                 if (!error) 
                 {
                    NSString *from;
                    NSString *id;
                 
                    if ([result objectForKey:@"data"]) 
                    {
                        from = [[result objectForKey:@"from"] objectForKey:@"name"];
                        id = [[result objectForKey:@"from"] objectForKey:@"id"];
                 
                        StartGame(true, true, from, id);
                    } 
                 
                 }
                 
                 }];
            }
        }
        
        void GameController::FB_ProcessIncomingFeed(NSString* urlString)
        {
            // Here we process an incoming link that has launched the app via a feed post
            
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
            // Here is extract out the FBID component at the end of the brag, so 'challenge_brag_123456' becomes just 123456
            NSString* val = [[urlString componentsSeparatedByString:@"challenge%5Fbrag%5F"] lastObject];
            
            FBRequest *req = [[FBRequest alloc] initWithSession:appDelegate.session graphPath:val];
            
            // With the FBID extracted, we have enough information to go ahead and request the user's profile picture
            // But we also need their name, so here we make a request to http://graph.facebook.com/USER_ID to get their basic information
            [req startWithCompletionHandler:^(FBRequestConnection *connection, id result, NSError *error) 
            {
                // If the result came back okay with no errors...
                if (result && !error) 
                {
                    NSString *from;
             
                    from = [result objectForKey:@"first_name"];
             
                    // We can start the game, 
                    StartGame(true, true, from, val);

                }
             
             }];
            
        }
        
        void GameController::FB_SendScore(const int nScore)
        {
            // Here we send a score to our server component along with our user ID, it is the responsibility of the server to get the APP access token, and pass that on with the score to
            // the Facebook Graph API for Scores
            
            NSMutableURLRequest *request = [NSMutableURLRequest 
                                            requestWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"http://www.freeeel.com/friendsmash/og.php?score=%d&uid=%d", nScore, m_nPlayerFBID]]];
            
            [request setHTTPMethod:@"POST"];
            [[NSURLConnection alloc] initWithRequest:request delegate:nil];
             
        }
        
        void GameController::FB_SendRequest(const int nScore)
        {
            FB_SendFilteredRequest(nScore);
            return;
            
            
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
            // Enable this to turn on Frictionless Requests
            // [[appDelegate facebook] enableFrictionlessRequests];
            
     
            // Normally this won't be hardcoded but will be context specific, i.e. players you are in a match with, or players who recently played the game etc
            //NSArray *suggestedFriends = [[NSArray alloc] initWithObjects:
                                         //@"695755709", @"685145706", @"569496010", @"623111",
                                         //nil];
            
            //SBJSON *jsonWriter = [SBJSON new];
            //NSDictionary *challenge =  [NSDictionary dictionaryWithObjectsAndKeys: [NSString stringWithFormat:@"%d", nScore], @"challenge_score", nil];
            //NSString *challengeStr = [jsonWriter stringWithObject:challenge];
            
            // Create a dictionary of key/value pairs which are the parameters of the dialog
            
            // 1. No additional parameters provided - enables generic Multi-friend selector
            NSMutableDictionary* params =   [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                            [NSString stringWithFormat:@"I just smashed %d friends! Can you beat it?", nScore], @"message",
            // 2. Optionally provide a 'to' param to direct the request at a specific user                               
                                            //@"685145706", @"to", // Gareth
            // 3. Suggest friends the user may want to request, could be game context specific?                               
                                            //[suggestedFriends componentsJoinedByString:@","], @"suggestions",
            // 4. Provide context through the data param
                                            //challengeStr, @"data",
                                           nil];
            
            // Actually invoke the dialog
            [appDelegate.facebook dialog:@"apprequests" andParams:params andDelegate:nil];
             
        }
        
        void GameController::FB_SendFilteredRequest(const int nScore)
        {
            // Okay, we're going to filter our friends by their device, we're looking for friends with an iPhone or iPad
            
            // We're going to place these friends into this container
            NSMutableArray *deviceFilteredFriends = [[NSMutableArray alloc] init];
            
            // We request a list of our friends' names and devices
            [[FBRequest requestForGraphPath:@"me/friends?fields=name,devices"]
             startWithCompletionHandler:
             ^(FBRequestConnection *connection, 
               NSDictionary *result,
               NSError *error) 
             {
                // If we received a result with no errors...
                if (!error && result) 
                {
                    // Get the result
                    NSArray *resultData = [result objectForKey:@"data"];
                    
                    // Check we have some friends. If the player doesn't have any friends, they probably need to put down the demo app anyway, and go outside...
                    if ([resultData count] > 0) 
                    {
                        // Loop through the friends returned
                        for (NSDictionary *friendObject in resultData) 
                        {
                            // Check if devices info available
                            if ([friendObject objectForKey:@"devices"]) 
                            {
                                // Yep, we know what devices this friend has.. let's extract them
                                NSArray *deviceData = [friendObject objectForKey:@"devices"];
                                  
                                // Loop through the list of devices this friend has...
                                for (NSDictionary *deviceObject in deviceData) 
                                {
                                    // Check if there is a device match, in this case we're looking for iOS
                                    if ([@"iOS" isEqualToString: [deviceObject objectForKey:@"os"]]) 
                                    {
                                        // If there is a match, add it to the list - this friend has an iPhone or iPad. Hurrah!
                                        [deviceFilteredFriends addObject: [friendObject objectForKey:@"id"]];
                                        break;
                                    }
                                }
                            }
                        }
                    }
             
                    // Now we have a list of friends with an iOS device, we can send requests to them
                    AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
                    // We create our parameter dictionary as we did before
                    NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                   [NSString stringWithFormat:@"I just smashed %d friends! Can you beat it?", nScore], @"message",
                                                    nil];
            
                    // We have the same list of suggested friends
                    NSArray *suggestedFriends = [[NSArray alloc] initWithObjects:
                                                 @"695755709", @"685145706", @"569496010", @"623111", 
                                                 nil];
             
             
                    // Of course, not all of our suggested friends will have iPhones or iPads - we need to filter them down
                    NSMutableArray *validSuggestedFriends = [[NSMutableArray alloc] init];
             
                    // So, we loop through each suggested friend
                    for (NSString* suggestedFriend in suggestedFriends)
                    {
                        // If they are on our device filtered list, we know they have an iOS device
                        if ([deviceFilteredFriends containsObject:suggestedFriend])
                        {
                            // So we can call them valid
                            [validSuggestedFriends addObject:suggestedFriend];
                        }
                    }
                
                    // If at least one of our suggested friends had an iOS device...
                    if ([deviceFilteredFriends count] > 0) 
                    {
                        // We add them to the suggest friend param of the dialog
                        NSString *selectIDsStr = [validSuggestedFriends componentsJoinedByString:@","];
                        [params setObject:selectIDsStr forKey:@"suggestions"];
                    }
            
                    // Finally, we're ready to invoke the dialog
                    [appDelegate.facebook dialog:@"apprequests" andParams:params andDelegate:nil];
            
                 }
             }]; 
        }
        
        void GameController::FB_SendBrag(const int nScore)
        {
            // This function will invoke the Feed Dialog to post to a user's Timeline and News Feed
            
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
 
            // Put together the dialog parameters
            NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                           @"Checkout my Friend Smash greatness!", @"name",
                                           @"Come smash me back!", @"caption",
                                           [NSString stringWithFormat:@"I just smashed %d friends! Can you beat my score?", nScore], @"description",
                                           @"https://young-atoll-3559.herokuapp.com/images/friendsmashlogo_small.jpg", @"picture",
                                           
                                           // Add the link param for Deep Linking
                                           //[NSString stringWithFormat:@"https://www.mydomain.com/challenge_brag_%d", m_nPlayerFBID], @"link",
                                           nil];
            
            // Invoke the dialog
            [appDelegate.facebook dialog:@"feed" andParams:params andDelegate:nil];
            
             
        }
        
        
        void GameController::FB_Logout()
        {
            // Log out of Facebook and reset our session
            
            AppDelegate *appDelegate = [[UIApplication sharedApplication]delegate];
            
            [appDelegate.facebook logout:nil];
            [appDelegate.session closeAndClearTokenInformation];
            appDelegate.session = nil;
            
            m_pLoginButtonSprite->SetDraw(true);
            
            m_pPlayButtonSprite->SetDraw(false);
            m_pLogoutButtonSprite->SetDraw(false);
            m_pChallengeButtonSprite->SetDraw(false);
            m_pBragButtonSprite->SetDraw(false);
            m_pUserImageSprite->SetDraw(false);
            
            m_labelName.hidden = YES;
            m_labelNameStatus.hidden = YES;
            m_labelFriendName.hidden = YES;
            m_labelScore.hidden = YES;
            
            m_kGameState = kGAMESTATE_FRONTSCREEN_LOGGEDOUT;
        }
    
    }
		
}