//
//  ImageFetcher.m
//  friendSmasher
//
//  Created by Ali Parr on 19/07/2012.
//  Copyright (c) 2012 Facebook. All rights reserved.
//

#import "ImageFetcher.h"

@implementation ImageFetcher

@synthesize delegate;


- (id) init{

    if (self = [super init]){
    
        queue = [[NSOperationQueue alloc] init];
        queue.maxConcurrentOperationCount = 5;
        
    }
    
    return self;
}

- (void) reportSuccess:(UIImage*)image{
    

    [self.delegate imageFetcherDidFetch:self fetchedImage:image];
}

- (void) reportError:(NSString*)url{
    [self.delegate imageFetcherDidFailToFetch:self urlUsed:url];
}

- (void) fetchImageWithUrl:(NSString *)url andCompletionBlock:(bool (^)(UIImage*))block {
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    
    NSHTTPURLResponse *response  =nil;
    NSError *error = nil;
    
    NSData *imageData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    
    //[request release];
    
    if (error || response.statusCode != 200 || !imageData || imageData.length == 0){
        
        [self performSelectorOnMainThread:@selector(reportError:) withObject:url waitUntilDone:NO];
    }else{
        
        UIImage *image = [[UIImage alloc]initWithData:imageData];
        block(image);
        //[image release];
    }
}


- (void) fetchImageSyncWithString:(NSString *)url{

    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    
    NSHTTPURLResponse *response  =nil;
    NSError *error = nil;
    
    NSData *imageData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    
    if (error || response.statusCode != 200 || !imageData || imageData.length == 0){
        
        [self performSelectorOnMainThread:@selector(reportError:) withObject:url waitUntilDone:NO];
    }
    else{
        UIImage *image = [[UIImage alloc]initWithData:imageData];
        [self performSelectorOnMainThread:@selector(reportSuccess:) withObject:image waitUntilDone:NO];
    }
    
}

- (void) fetchImageWithUrl:(NSString *)url{
    
    NSInvocationOperation *op = [[NSInvocationOperation alloc]initWithTarget:self selector:@selector(fetchImageSyncWithString:) object:url];
    [queue addOperation:op];
}


@end
