//
//  ViewController.m
//  SJLiveVideo
//
//  Created by yuanmaochao on 16/6/14.
//  Copyright © 2016年 king. All rights reserved.
//

#import "ViewController.h"
#import "SJMoiveObject.h"

#define LERP(A,B,C) ((A)*(1.0-C)+(B)*C)

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UIImageView *ImageView;
@property (weak, nonatomic) IBOutlet UILabel *fps;
@property (weak, nonatomic) IBOutlet UIButton *playBtn;
@property (weak, nonatomic) IBOutlet UIButton *TimerBtn;
@property (weak, nonatomic) IBOutlet UILabel *TimerLabel;
@property (nonatomic, strong) SJMoiveObject *video;
@property (nonatomic, assign) float lastFrameTime;
@end

@implementation ViewController

@synthesize ImageView, fps, playBtn, video;

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    self.video = [[SJMoiveObject alloc] initWithVideo:[NSString bundlePath:@"Dalshabet.mp4"]];
    self.video = [[SJMoiveObject alloc] initWithVideo:@"http://wvideo.spriteapp.cn/video/2016/0328/56f8ec01d9bfe_wpd.mp4"];
//    video.outputWidth = 1920;
//    video.outputHeight = 1080;
    
    NSLog(@"视频总时长>>>video duration: %f",video.duration);
    NSLog(@"源尺寸>>>video size: %d x %d", video.sourceWidth, video.sourceHeight);
    NSLog(@"输出尺寸>>>video size: %d x %d", video.outputWidth, video.outputHeight);
    
//        [ImageView setTransform:CGAffineTransformMakeRotation(M_PI)];
    NSLog(@"%@",NSStringFromCGRect(self.view.frame));
}

- (IBAction)PlayClick:(UIButton *)sender {
    
    [playBtn setEnabled:NO];
    _lastFrameTime = -1;
    
    // seek to 0.0 seconds
    [video seekTime:0.0];
    
    
    [NSTimer scheduledTimerWithTimeInterval: 1 / 30
                                     target:self
                                   selector:@selector(displayNextFrame:)
                                   userInfo:nil
                                    repeats:YES];
}

- (IBAction)TimerCilick:(id)sender {
    
    NSLog(@"current time: %f s",video.currentTime);
}

-(void)displayNextFrame:(NSTimer *)timer {
    NSTimeInterval startTime = [NSDate timeIntervalSinceReferenceDate];
    self.TimerLabel.text = [NSString stringWithFormat:@"%f s",video.currentTime];
    if (![video stepFrame]) {
        [timer invalidate];
        [playBtn setEnabled:YES];
        return;
    }
    ImageView.image = video.currentImage;
    float frameTime = 1.0 / ([NSDate timeIntervalSinceReferenceDate] - startTime);
    if (_lastFrameTime < 0) {
        _lastFrameTime = frameTime;
    } else {
        _lastFrameTime = LERP(frameTime, _lastFrameTime, 0.8);
    }
    [fps setText:[NSString stringWithFormat:@"fps %.0f",_lastFrameTime]];
}

@end
