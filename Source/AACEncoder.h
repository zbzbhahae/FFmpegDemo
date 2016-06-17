//
//  AACEncoder.h
//  SJLiveVideo
//
//  Created by yuanmaochao on 16/6/15.
//  Copyright © 2016年 king. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface AACEncoder : NSObject

- (void)encodeSampleBuffer:(CMSampleBufferRef)sampleBuffer completionBlock:(void (^)(NSData *encodedData, NSError* error))completionBlock;


@end