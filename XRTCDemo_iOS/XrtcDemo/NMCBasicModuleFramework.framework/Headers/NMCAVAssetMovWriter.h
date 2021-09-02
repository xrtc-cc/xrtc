//
//  NMCAVAssetMovWriter.h
//  NMCBasicModule
//
//  Created by taojinliang on 2017/8/16.
//  Copyright © 2017年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>


@interface NMCAVAssetMovWriter : NSObject


@property(nonatomic,strong)AVAssetWriter* assetWriter;
@property(nonatomic,strong)AVAssetWriterInput* assetWriterVideoInput;
@property(nonatomic,strong)AVAssetWriterInput* assetWriterAudioInput;
@property(nonatomic,strong)AVAssetWriterInputPixelBufferAdaptor* assetWriterPixelBufferInput;

@property(nonatomic,copy)  void(^startHandler)(void);


@property(nonatomic, readonly)CMTime duration;
@property(nonatomic, assign)CGAffineTransform transform;
@property(nonatomic, copy)NSArray *metaData;

@property(nonatomic,assign)BOOL isRecording;



// Initialization and teardown
-(id)initWithMovURL:(NSURL *)newMovieURL;

-(BOOL)initializeVideoInputWithVideoSize:(CGSize)videoSize
                                 bitrate:(unsigned int)bitrate;

-(BOOL)initializeAudioInputWithAudioFormat:(Float64)sampleRate
                           channelPerFrame:(int)channelPerFrame
                                   bitrate:(unsigned int)bitrate;
//在录制过程中切换分辨率
-(void)changeQuality:(CGSize)videoSize;

- (void)startRecording;

- (void)startRecordingInOrientation:(CGAffineTransform)orientationTransform;

- (void)finishRecordingWithCompletionHandler:(void (^)(void))handler;

- (void)cancelRecording;


-(void)processAudioBufferList:(const AudioTimeStamp*) inTimeStamp
               inNumberFrames:(UInt32)inNumberFrames
                       ioData:(AudioBufferList*)ioData;

-(void)processVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;

-(void)processVideoSampleBuffer:(CVImageBufferRef)sampleBuffer
                      frameTime:(CMTime)frameTime;


-(unsigned int)getEncodeFps;

@property(nonatomic,assign)int encodedWidth;
@property(nonatomic,assign)int encodedHight;
@property(nonatomic,assign)int encodedBitrate;

@end

