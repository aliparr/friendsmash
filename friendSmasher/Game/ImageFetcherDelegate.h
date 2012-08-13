//
//  ImageFetcherDelegate.h
//  friendSmasher
//
//  Created by Ali Parr on 19/07/2012.
//  Copyright (c) 2012 Facebook. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol ImageFetcherDelegate <NSObject>

- (void) imageFetcherDidFailToFetch:(id)sender urlUsed:(NSString *)url;
- (void) imageFetcherDidFetch:(id)sender fetchedImage:(UIImage *)image;

@end
