//
//  NMCMp4Recorder.h
//  NMCBasicModule
//
//  Created by taojinliang on 2018/1/4.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreAudio/CoreAudioTypes.h>
#import <CoreMedia/CMSampleBuffer.h>
#import <AVFoundation/AVFoundation.h>


typedef NS_ENUM(NSUInteger, NMCMp4RecorderMode) {
    NMCMp4RecorderModeAV = 0,
    NMCMp4RecorderModeAudio = 1,
    NMCMp4RecorderModeVideo = 2
};


typedef NS_ENUM(NSUInteger, NMCVideoScalingMode) {
    NMCVideoScalingModeFit = 0,
    NMCVideoScalingModeResize = 1,
    NMCVideoScalingModeResizeAspect = 2,
    NMCVideoScalingModeResizeAspectFill = 3
};

typedef NS_ENUM(NSUInteger, NMCRecordingErrorType) {
    NMCRecordingErrorNoError       = 0,
    NMCRecordingErrorNotReady      = 1,
    NMCRecordingErrorNotRecording  = 2,
    
    NMCRecordingErrorCreateFile    = 3,
    NMCRecordingErrorInitVideo     = 4,
    NMCRecordingErrorInitAudio     = 5,
    
    NMCRecordingErrorStartWriting  = 6,
    
    NMCRecordingErrorWritingInterrupted = 7,
    
    NMCRecordingErrorWillStopForLackSpace = 8,
    
    NMCRecordingErrorInvalidFilePath = 9,
    
    NMCRecordingErrorExceedMaxRecords = 10,
    
    NMCRecordingErrorSetupFailed  = 11,
    
    NMCRecordingErrorAudioIsNotSetup = 12,
};

typedef NS_ENUM(NSUInteger, NMCRecordingState) {
    NMCRecordingStateStart = 0,
    NMCRecordingStateStop = 1,
};

typedef void(^NMCMp4RecorderErrorBlock)(NSString *errString);
typedef void(^NMCMp4RecorderErrorStateBlock)(NMCRecordingErrorType error, NSURL *url);
typedef void(^NMCMp4RecorderStateBlock)(NMCRecordingState state, NMCRecordingErrorType error, NSURL *url);



@interface NMCMp4RecorderParam : NSObject
@property(nonatomic, assign) CMVideoDimensions videoDimens;
@property(nonatomic, assign) UInt32 videoBitrate;
@property(nonatomic, assign) NSInteger videoFrameInterval;
@property(nonatomic, assign) NMCVideoScalingMode mode;
@end

@interface NMCMp4Recorder : NSObject
@property(nonatomic, strong) NMCMp4RecorderErrorStateBlock errorStateCallBack;
@property(nonatomic, strong) NMCMp4RecorderStateBlock stateBlock;

- (void)startRecordMp4:(NSURL *)file
                 param:(NMCMp4RecorderParam *)param
             recordMode:(NMCMp4RecorderMode)recordMode
            errorBlock:(NMCMp4RecorderErrorBlock)errorBlock;

- (void)startRecordMp4:(NSURL *)file
                 param:(NMCMp4RecorderParam *)param
            recordMode:(NMCMp4RecorderMode)recordMode
            errorBlock:(NMCMp4RecorderErrorBlock)errorBlock
                 force:(BOOL)force;

- (void)record:(CMSampleBufferRef)sampleBuffer
       isVideo:(BOOL)isVideo;


- (BOOL)recordAudioBufferList:(AudioBufferList *)bufferList
                samplesNumber:(UInt32)samplesNumber
                  description:(AudioStreamBasicDescription)audioDescription;

- (void)stopRecording;

- (BOOL)isRecording;
@end
