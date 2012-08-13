//
//  ImageFetcher.h
//  friendSmasher
//
//  Created by Ali Parr on 19/07/2012.
//  Copyright (c) 2012 Facebook. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ImageFetcherDelegate.h"

@interface ImageFetcher : NSObject
{
    NSOperationQueue *queue;
    
    id<ImageFetcherDelegate> delegate;
}

@property (nonatomic, assign)  id<ImageFetcherDelegate> delegate;

- (void) fetchImageWithUrl:(NSString *)url;
- (void) fetchImageWithUrl:(NSString *)url andCompletionBlock:(bool (^)(UIImage*))block ;


@end
