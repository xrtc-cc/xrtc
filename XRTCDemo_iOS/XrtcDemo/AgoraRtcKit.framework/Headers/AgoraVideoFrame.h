//
// AgoraVideoFrame.h
// AgoraRtcEngineKit
//
// Copyright (c) 2020 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraEnumerates.h"
#import "AgoraObjects.h"

@protocol AgoraVideoDataFrameProtocol <NSObject>
@required

/** Occurs each time the SDK receives a video frame captured by the local camera.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each
 * time a video frame is received. In this callback, you can get the video data captured by the
 * local camera. You can then pre-process the data according to your scenarios.
 *
 * After pre-processing, you can send the processed video data back to the SDK by setting the
 * `videoFrame` parameter in this callback.
 *
 * @note
 * - This callback does not support sending processed RGBA video data back to the SDK.
 * - The video data that this callback gets has not been pre-processed, without the watermark, the
 * cropped content, the rotation, and the image enhancement.
 *
 * @param videoFrame Pointer to VideoFrame.
 * @return Whether or not to ignore the current video frame if the pre-processing fails:
 * - true: Do not ignore.
 * - false: Ignore the current video frame, and do not send it back to the SDK.
 */
- (BOOL)onCaptureVideoFrame:(AgoraVideoDataFrame*)videoFrame;
/** @since v3.0.0
 *
 * Occurs each time the SDK receives a video frame before encoding.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each
 * time when it receives a video frame. In this callback, you can get the video data before
 * encoding. You can then process the data according to your particular scenarios.
 *
 * After processing, you can send the processed video data back to the SDK by setting the
 * `VideoFrame` parameter in this callback.
 *
 * @note
 * - As of v3.0.1, if you want to receive this callback, you also need to set
 * `POSITION_PRE_ENCODE(1 << 2)` as a frame position in the \ref getObservedFramePosition
 * "getObservedFramePosition" callback.
 * - The video data that this callback gets has been pre-processed, with its content cropped,
 * rotated, and the image enhanced.
 * - This callback does not support sending processed RGBA video data back to the SDK.
 *
 * @param videoFrame A pointer to VideoFrame
 * @return Whether to ignore the current video frame if the processing fails:
 * - true: Do not ignore the current video frame.
 * - false: Ignore the current video frame, and do not send it back to the SDK.
 */
- (BOOL)onPreEncodeVideoFrame:(AgoraVideoDataFrame*)videoFrame;
/** Occurs each time the SDK receives a video frame sent by the remote user.
 *
 * After you successfully register the video frame observer and isMultipleChannelFrameWanted
 * return false, the SDK triggers this callback each time a video frame is received. In this
 * callback, you can get the video data sent by the remote user. You can then post-process the
 * data according to your scenarios.
 *
 * After post-processing, you can send the processed data back to the SDK by setting the
 * `videoFrame` parameter in this callback.
 *
 * @note
 * This callback does not support sending processed RGBA video data back to the SDK.
 *
 * @param uid ID of the remote user who sends the current video frame.
 * @param videoFrame Pointer to VideoFrame.
 * @return Whether or not to ignore the current video frame if the post-processing fails:
 * - true: Do not ignore.
 * - false: Ignore the current video frame, and do not send it back to the SDK.
 */
- (BOOL)onRenderVideoFrame:(AgoraVideoDataFrame*)videoFrame forUid:(unsigned int)uid;
/** Occurs each time the SDK receives a video frame and prompts you to set the video format.
 *
 * YUV420 is the default video format. If you want to receive other video formats, register this
 * callback in the IVideoFrameObserver class.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each
 * time it receives a video frame. You need to set your preferred video data in the return value
 * of this callback.
 *
 * @return Sets the video format: #VIDEO_FRAME_TYPE
 * - #FRAME_TYPE_YUV420 (0): (Default) YUV420.
 * - #FRAME_TYPE_RGBA (2): RGBA
 */
- (AgoraVideoFrameType)getVideoFormatPreference;
/** Occurs each time the SDK receives a video frame and prompts you whether or not to rotate the
 * captured video according to the rotation member in the VideoFrame class.
 *
 * The SDK does not rotate the captured video by default. If you want to rotate the captured video
 * according to the rotation member in the VideoFrame class, register this callback in the
 * IVideoFrameObserver class.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each
 * time it receives a video frame. You need to set whether or not to rotate the video frame in the
 * return value of this callback.
 *
 * @note
 * This callback applies to RGBA video data only.
 *
 * @return Sets whether or not to rotate the captured video:
 * - true: Rotate.
 * - false: （Default) Do not rotate.
 */
- (BOOL)getRotationApplied;
/** Occurs each time the SDK receives a video frame and prompts you whether or not to mirror the
 * captured video.
 *
 * The SDK does not mirror the captured video by default. Register this callback in the
 * IVideoFrameObserver class if you want to mirror the captured video.
 *
 * After you successfully register the video frame observer, the SDK triggers this callback each
 * time a video frame is received. You need to set whether or not to mirror the captured video in
 * the return value of this callback.
 *
 * @note
 * This callback applies to RGBA video data only.
 *
 * @return Sets whether or not to mirror the captured video:
 * - true: Mirror.
 * - false: (Default) Do not mirror.
 */
- (BOOL)getMirrorApplied;
/**
 * Sets the frame position for the video observer.
 * @since v3.0.1
 *
 * After you successfully register the video observer, the SDK triggers this callback each time it
 * receives a video frame. You can determine which position to observe by setting the return
 * value. The SDK provides 3 positions for observer. Each position corresponds to a callback
 * function:
 * - `POSITION_POST_CAPTURER(1 << 0)`: The position after capturing the video data, which
 * corresponds to the \ref onCaptureVideoFrame "onCaptureVideoFrame" callback.
 * - `POSITION_PRE_RENDERER(1 << 1)`: The position before receiving the remote video data, which
 * corresponds to the \ref onRenderVideoFrame "onRenderVideoFrame" callback.
 * - `POSITION_PRE_ENCODER(1 << 2)`: The position before encoding the video data, which
 * corresponds to the \ref onPreEncodeVideoFrame "onPreEncodeVideoFrame" callback.
 *
 * @note
 * - Use '|' (the OR operator) to observe multiple frame positions.
 * - This callback observes `POSITION_POST_CAPTURER(1 << 0)` and `POSITION_PRE_RENDERER(1 << 1)`
 * by default.
 * - To conserve the system consumption, you can reduce the number of frame positions that you
 * want to observe.
 *
 * @return A bit mask that controls the frame position of the video observer:
 * #VIDEO_OBSERVER_POSITION.
 *
 */
- (AgoraVideoFramePosition)getObservedFramePosition;

/** Determines whether to receive video data from multiple channels.

 After you register the video frame observer, the SDK triggers this callback
 every time it captures a video frame.

 In the multi-channel scenario, if you want to get video data from multiple channels,
 set the return value of this callback as true. After that, the SDK triggers the
 \ref IVideoFrameObserver::onRenderVideoFrameEx "onRenderVideoFrameEx" callback to send you
 the video data from various channels. You can also get the channel ID of each video frame.

 @note
 - Once you set the return value of this callback as true, the SDK triggers only the
 `onRenderVideoFrameEx` callback to send the video frame. \ref
 IVideoFrameObserver::onRenderVideoFrame "onRenderVideoFrame" will not be triggered. In the
 multi-channel scenario, Agora recommends setting the return value as true.
 - If you set the return value of this callback as false, the SDK triggers only the
 `onRenderVideoFrame` callback to send the video data.
 @return
 - `true`: Receive video data from multiple channels.
 - `false`: Do not receive video data from multiple channels.
 */
- (BOOL)isMultipleChannelFrameWanted;

/** Gets the video frame from multiple channels.

 After you successfully register the video frame observer, if you set the return value of
 \ref IVideoFrameObserver::isMultipleChannelFrameWanted "isMultipleChannelFrameWanted" as true,
 the SDK triggers this callback each time it receives a video frame from any of the channel.

 You can process the video data retrieved from this callback according to your scenario, and send
 the processed data back to the SDK using the `videoFrame` parameter in this callback.

 @note This callback does not support sending RGBA video data back to the SDK.

 @param channelId The channel ID of this video frame.
 @param uid The ID of the user sending this video frame.
 @param videoFrame The pointer to VideoFrame.
 @return Whether to send this video frame to the SDK if post-processing fails:
 - `true`: Send this video frame.
 - `false`: Do not send this video frame.
 */
- (BOOL)onRenderVideoFrameEx:(AgoraVideoDataFrame*)videoFrame forUid:(unsigned int)uid inChannel:(NSString*)channelId;

@end

@protocol AgoraVideoEncodedFrameProtocol <NSObject>

@required

- (BOOL)onVideoEncodedFrame:(AgoraVideoEncodedFrame*)VideoEncodedFrame;

@end
