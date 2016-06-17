//
//  SJDecodVideo.cpp
//  SJLiveVideo
//
//  Created by yuanmaochao on 16/6/16.
//  Copyright © 2016年 king. All rights reserved.
//

#include "SJDecodVideo.hpp"

AVFormatContext     *SJFormatCtx;
AVCodecContext      *SJCodecCtx;
AVFrame             *SJFrame;
AVPacket            packet;
AVPicture           picture;
int                 videoStream;
struct SwsContext   *img_convert_ctx;
int sourceWidth, sourceHeight;
int outputWidth, outputHeight;
double duration;
double currentTime;

AVFrame *sj_DecodVideo(const char *fileName) {
    
    AVCodec *pCodec;
    // 注册所有解码器
    avcodec_register_all();
    av_register_all();
    avformat_network_init();
    // 打开视频文件
    if (avformat_open_input(&SJFormatCtx, fileName, NULL, NULL) != 0) {
        av_log(NULL, AV_LOG_ERROR, "打开文件失败\n");
        goto initError;
    }
    // 检查数据流
    if (avformat_find_stream_info(SJFormatCtx, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "检查数据流失败\n");
        goto initError;
    }
    // 根据数据流,找到第一个视频流
    if ((videoStream =  av_find_best_stream(SJFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &pCodec, 0)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "没有找到第一个视频流\n");
        goto initError;
    }
    // 获取视频流的编解码上下文的指针
    SJCodecCtx = SJFormatCtx->streams[videoStream]->codec;
    // 查找解码器
    pCodec = avcodec_find_decoder(SJCodecCtx->codec_id);
    if (pCodec == NULL) {
        av_log(NULL, AV_LOG_ERROR, "没有找到解码器\n");
        goto initError;
    }
    // 打开解码器
    if(avcodec_open2(SJCodecCtx, pCodec, NULL) < 0) {
        av_log(NULL, AV_LOG_ERROR, "打开解码器失败\n");
        goto initError;
    }
    // 分配视频帧
    SJFrame = av_frame_alloc();
    outputWidth = SJCodecCtx->width;
    outputHeight = SJCodecCtx->height;
    return SJFrame;
initError:
    return NULL;
}
void sj_setupScaler() {
    
    // 先释放就的图片和计数器
    avpicture_free(&picture);
    sws_freeContext(img_convert_ctx);
    // 分配RGB图片
    avpicture_alloc(&picture, AV_PIX_FMT_RGB24, outputWidth, outputHeight);
    // 设置比例
    img_convert_ctx = sws_getContext(SJCodecCtx->width,
                                     SJCodecCtx->height,
                                     SJCodecCtx->pix_fmt,
                                     outputWidth,
                                     outputHeight,
                                     AV_PIX_FMT_RGB24,
                                     SWS_FAST_BILINEAR,
                                     NULL,
                                     NULL,
                                     NULL);
}
void sj_setOutputWidth(int newValue) {
    
    if (outputWidth == newValue) return;
    outputWidth = newValue;
    sj_setupScaler();
}
void sj_setOutputHeight(int newValue) {
    
    if (outputHeight == newValue) return;
    outputHeight = newValue;
    sj_setupScaler();
}
int sj_getSourceWidth() {
    return SJCodecCtx->width;
}
int sj_getSourceHeight() {
    return SJCodecCtx->height;
}
double sj_getCurrentTime() {
    
    AVRational timeBase = SJFormatCtx->streams[videoStream]->time_base;
    return packet.pts * (double)timeBase.num / timeBase.den;
}
double sj_getDuration() {
    return (double)SJFormatCtx->duration / AV_TIME_BASE;
}
void sj_seekTime(double seconds) {
    
    AVRational timeBase = SJFormatCtx->streams[videoStream]->time_base;
    int64_t targetFrame = (int64_t)((double)timeBase.den / timeBase.num * seconds);
    avformat_seek_file(SJFormatCtx,
                       videoStream,
                       0,
                       targetFrame,
                       targetFrame,
                       AVSEEK_FLAG_FRAME);
    avcodec_flush_buffers(SJCodecCtx);
}
bool sj_setupFrame() {
    
    int frameFinished = 0;
    while (!frameFinished && av_read_frame(SJFormatCtx, &packet) >= 0) {
        
        if (packet.stream_index == videoStream) {
            avcodec_decode_video2(SJCodecCtx,
                                  SJFrame,
                                  &frameFinished,
                                  &packet);
        }
    }
    return frameFinished != 0;
}
AVPicture sj_ConvertFrameToRGB() {
    
    sws_scale(img_convert_ctx,
              SJFrame->data,
              SJFrame->linesize,
              0,
              SJCodecCtx->height,
              picture.data,
              picture.linesize);
    return picture;
}
void sj_Deinit() {
    
    // 释放比例
    sws_freeContext(img_convert_ctx);
    // 释放RGB
    avpicture_free(&picture);
    // 释放frame
    av_packet_unref(&packet);
    // 释放YUV frame
    av_free(SJFrame);
    // 关闭解码器
    if (SJCodecCtx) { avcodec_close(SJCodecCtx); }
    // 关闭文件
    if (SJFormatCtx) { avformat_close_input(&SJFormatCtx); }
}