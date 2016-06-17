//
//  SJDecodVideo.hpp
//  SJLiveVideo
//
//  Created by yuanmaochao on 16/6/16.
//  Copyright © 2016年 king. All rights reserved.
//

#ifndef SJDecodVideo_hpp
#define SJDecodVideo_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include<libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
};

/**
 *  解码视频
 *
 *  @param fileName 视频路径
 */
AVFrame *sj_DecodVideo(const char *fileName);
/**
 *  转换为RGB
 *
 *  @return AVPicture 将返回的AVPicture 转为 UIImage
 */
AVPicture sj_ConvertFrameToRGB();
/**
 *  输出图像宽度,默认为源大小
 *
 *  @param newValue 宽度
 */
void sj_setOutputWidth(int newValue);
/**
 *  输出图像高度,默认为源大小
 *
 *  @param newValue 高度
 */
void sj_setOutputHeight(int newValue);
/**
 *  设置比例
 */
void sj_setupScaler();
/**
 *  寻求最近的关键帧在指定的时间
 *
 *  @param seconds 时间
 */
void sj_seekTime(double seconds);
/**
 *  设置frame
 *
 */
bool sj_setupFrame();
/**
 *  获取当前时间
 *
 *  @return
 */
double sj_getCurrentTime();
/**
 *  获取总时间
 *
 *  @return 总时间
 */
double sj_getDuration();
/**
 *  视频的frame高度
 *
 *  @param newValue 宽度
 */
int sj_getSourceWidth();
/**
 *  视频的frame高度
 *
 *  @param newValue 高度
 */
int sj_getSourceHeight();
/**
 释放所有资源
 */
void sj_Deinit();
#endif /* SJDecodVideo_hpp */
