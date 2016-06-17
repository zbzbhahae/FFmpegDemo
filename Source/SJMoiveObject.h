//
//  SJMoiveObject.h
//  SJLiveVideo
//
//  Created by yuanmaochao on 16/6/16.
//  Copyright © 2016年 king. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NSString+Extions.h"

@interface SJMoiveObject : NSObject

/* 解码后的UIImage */
@property (nonatomic, readonly) UIImage *currentImage;

/* 视频的frame高度 */
@property (nonatomic, readonly) int sourceWidth, sourceHeight;

/* 输出图像大小。默认设置为源大小。 */
@property (nonatomic,assign) int outputWidth, outputHeight;

/* 视频的长度，秒为单位 */
@property (nonatomic, readonly) double duration;

/* 视频的当前秒数 */
@property (nonatomic, readonly) double currentTime;

/* 随着电影在moviepath初始化。输出维度被设置为源维度。 */
- (instancetype)initWithVideo:(NSString *)moviePath;

/* 从视频流中读取下一帧。返回假，如果没有帧读取（视频）。 */
- (BOOL)stepFrame;

/* 寻求最近的关键帧在指定的时间 */
- (void)seekTime:(double)seconds;

@end
