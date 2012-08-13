//
//  ViewController.m
//  friendSmasher
//
//  Created by Ali Parr on 17/07/2012.
//  Copyright (c) 2012 Facebook. All rights reserved.
//

#import "ViewController.h"
#import "AppDelegate.h"
#import "GameController.h"

static FriendSmasher::Game::GameController* pGameController = NULL;
static float fScaleFactor = 1.f;


@interface ViewController () {
    }
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation ViewController

@synthesize context = _context;


- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [_context release];
    [super dealloc];
}

- (void)handledURL{
    if (pGameController) {
        pGameController->FB_ProcessIncomingURL();
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
    
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] &&
        ([UIScreen mainScreen].scale == 2.0)) {
        fScaleFactor = 2.f;
    } else {
        fScaleFactor = 1.f;
    }
    

    
    for (int i=0; i<5; ++i) {
        touchArray[i] = NULL;
    } 
    
    pGameController = new FriendSmasher::Game::GameController(self, fScaleFactor);
    pGameController->OnEnter();
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handledURL) name:APP_HANDLED_URL object:nil];
}



- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
        
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)update
{
    pGameController->OnUpdate();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{    
    pGameController->OnRender();
}



- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
    NSArray* arrayNewTouches = [touches allObjects];
    
	int nNewTouchCount = [arrayNewTouches count];
	
	for (int i=0; i<nNewTouchCount; ++i)
	{
		for (int j=0; j<5; ++j)
		{
			if ( touchArray[j] == NULL)
			{
				touchArray[j] = [arrayNewTouches objectAtIndex:i];
				
				CGPoint touchPoint = [[arrayNewTouches objectAtIndex:i] locationInView:self.view];
                
                // Normalize everything into retina space - so fixing at double the pixels here.
                pGameController->BeginTouch(j, touchPoint.x * 2.f, touchPoint.y * 2.f);
                
				break;
			}
		}
	}
}

- (void)touchesCancelled:(NSSet *)setEndTouches withEvent:(UIEvent *)event
{
	[self touchesEnded:setEndTouches withEvent:event];
}

- (void)touchesEnded:(NSSet *)setEndTouches withEvent:(UIEvent *)event
{
	NSArray* arrayEndTouches = [setEndTouches allObjects];
	
	int nEndTouchCount = [arrayEndTouches count];
	
	for (int i=0; i<nEndTouchCount; ++i)
	{
		CGPoint touchPoint = [[arrayEndTouches objectAtIndex:i] locationInView:self.view];
        
		for (int j=0; j<5; ++j)
		{
			if ( touchArray[j] == [arrayEndTouches objectAtIndex:i])
			{
				touchArray[j] = NULL;
				
                pGameController->EndTouch(j, touchPoint.x * 2.f, touchPoint.y * 2.f);
			
				break;
			}
		}
	}
}



@end
