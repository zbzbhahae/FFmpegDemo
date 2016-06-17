//
//  SJMoiveObject.m
//  SJLiveVideo
//
//  Created by yuanmaochao on 16/6/16.
//  Copyright © 2016年 king. All rights reserved.
//

#import "SJMoiveObject.h"
#include "SJDecodVideo.hpp"


@implementation SJMoiveObject
{
    AVPicture   picture;
    AVFrame     *SJFrame;
}

- (void)setOutputWidth:(int)outputWidth {
    if (_outputWidth == outputWidth) return;
    _outputWidth = outputWidth;
    sj_setOutputWidth(outputWidth);
    sj_setupScaler();
}
- (void)setOutputHeight:(int)outputHeight {
    if (_outputHeight == outputHeight) return;
    _outputHeight = outputHeight;
    sj_setOutputHeight(outputHeight);
    sj_setupScaler();
}

- (double)duration {
    return sj_getDuration();
}
- (double)currentTime {
    return sj_getCurrentTime();
}
- (int)sourceWidth {
    return sj_getSourceWidth();
}
- (int)sourceHeight {
    return sj_getSourceHeight();
}
- (UIImage *)currentImage {
    if (!SJFrame->data[0]) return nil;
    picture = sj_ConvertFrameToRGB();
    return [self imageFromAVPicture:picture width:self.outputWidth height:self.outputHeight];
}
- (instancetype)initWithVideo:(NSString *)moviePath {
    
    if (!(self = [super init])) return nil;
    SJFrame = sj_DecodVideo([moviePath cStringUsingEncoding:NSUTF8StringEncoding]);
    if (SJFrame == NULL) return nil;
    self.outputWidth = sj_getSourceWidth();
    self.outputHeight = sj_getSourceHeight();
    return self;
}

- (BOOL)stepFrame {
    return sj_setupFrame();
}

- (void)seekTime:(double)seconds {
    sj_seekTime(seconds);
}


#pragma mark --------------------------
#pragma mark - 内部方法
-(UIImage *)imageFromAVPicture:(AVPicture)pict width:(int)width height:(int)height {
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CFDataRef data = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, pict.data[0], pict.linesize[0]*height,kCFAllocatorNull);
    CGDataProviderRef provider = CGDataProviderCreateWithCFData(data);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGImageRef cgImage = CGImageCreate(width,
                                       height,
                                       8,
                                       24,
                                       pict.linesize[0],
                                       colorSpace,
                                       bitmapInfo,
                                       provider,
                                       NULL,
                                       NO,
                                       kCGRenderingIntentDefault);
    CGColorSpaceRelease(colorSpace);
    UIImage *image = [UIImage imageWithCGImage:cgImage];
    CGImageRelease(cgImage);
    CGDataProviderRelease(provider);
    CFRelease(data);
    
    return image;
}

- (void)savePPMPicture:(AVPicture)pict width:(int)width height:(int)height index:(int)iFrame{
    
    NSString *feil = [NSString documentsPath:[NSString stringWithFormat:@"image%04d.ppm",iFrame]];
    FILE *file = fopen([feil cStringUsingEncoding:NSASCIIStringEncoding], "wb");
    if(file == NULL)
        return;
    
    // Write header
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    
    // Write pixel data
    for(int y = 0; y < height; y++)
        
        fwrite(pict.data[0] + y * pict.linesize[0], 1, width * 3, file);
    
    // Close file
    fclose(file);
}

#pragma mark --------------------------
#pragma mark - 释放资源
- (void)dealloc {
    sj_Deinit();
}
@end
