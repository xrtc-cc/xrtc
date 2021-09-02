//
// AgoraAudioFrame.h
// AgoraRtcEngineKit
//
// Copyright (c) 2020 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

/** The delegate of the raw audio data.
 */
@protocol AgoraAudioDataFrameProtocol <NSObject>
@required

/** Gets the recorded raw audio data.

 @since v3.3.0

 @note To ensure that the captured audio frame has the expected format, Agora recommends that you call [setRecordingAudioFrameParametersWithSampleRate]([AgoraRtcEngineKit setRecordingAudioFrameParametersWithSampleRate:channel:mode:samplesPerCall:]) to set the audio capturing format.

 @param frame The raw audio data. For details, see `AgoraAudioFrame`.

 @return - `YES`: The audio data is valid, and will be sent to the SDK.
- `NO`: The audio data is invalid, and will not be sent to the SDK.
 */
- (BOOL)onRecordAudioFrame:(AgoraAudioFrame* _Nonnull)frame;

/** Gets the playback raw audio data.

 @since v3.3.0

 @note To ensure that the audio playback frame has the expected format, Agora
 recommends that you call [setPlaybackAudioFrameParametersWithSampleRate]([AgoraRtcEngineKit setPlaybackAudioFrameParametersWithSampleRate:channel:mode:samplesPerCall:]) to set the audio playback format.

 @param frame The raw audio data. For details, see `AgoraAudioFrame`.

 @return - `YES`: The audio data is valid, and will be sent to the SDK.
- `NO`: The audio data is invalid, and will not be sent to the SDK.
 */
- (BOOL)onPlaybackAudioFrame:(AgoraAudioFrame* _Nonnull)frame;

/** Receives the mixed raw audio data of the local user and all remote users.

 @since v3.3.0

 The SDK periodically triggers this callback according to the sample interval
 set by the [setMixedAudioFrameParametersWithSampleRate]([AgoraRtcEngineKit setMixedAudioFrameParametersWithSampleRate:samplesPerCall:])
 method. You can retrieve the mixed audio data of the local and remote users
 from this callback.

 @note To ensure that the mixed captured and playback audio frame has the expected format, Agora
 recommends that you call [setMixedAudioFrameParametersWithSampleRate]([AgoraRtcEngineKit setMixedAudioFrameParametersWithSampleRate:samplesPerCall:]) to set the mixed audio format.

 @param frame The raw audio data. For details, see `AgoraAudioFrame`.

 @return - `YES`: The audio data is valid, and will be sent to the SDK.
- `NO`: The audio data is invalid, and will not be sent to the SDK.
 */
- (BOOL)onMixedAudioFrame:(AgoraAudioFrame* _Nonnull)frame;

/** Gets the raw audio data of a remote user before mixing.

 @since v3.3.0

 After you register the audio frame observer, the SDK triggers this callback
 every time it captures an audio frame.

 @note To ensure that the audio playback frame has the expected format, Agora
 recommends that you call [setPlaybackAudioFrameParametersWithSampleRate]([AgoraRtcEngineKit setPlaybackAudioFrameParametersWithSampleRate:channel:mode:samplesPerCall:]) to set the audio playback format.

 @param frame The raw audio data. For details, see `AgoraAudioFrame`.
 @param uid The user ID.

 @return - `YES`: The audio data is valid, and will be sent to the SDK.
- `NO`: The audio data is invalid, and will not be sent to the SDK.
 */
- (BOOL)onPlaybackAudioFrameBeforeMixing:(AgoraAudioFrame* _Nonnull)frame uid:(NSUInteger)uid;

/**
 * Sets the frame position for the audio observer.
 *
 * @return A bit mask that controls the frame position of the audio observer.
 * @note - Use '|' (the OR operator) to observe multiple frame positions.
 * <p>
 * After you successfully register the audio observer, the SDK triggers this callback each time it receives a audio frame. You can determine which position to observe by setting the return value.
 * The SDK provides 3 positions for observer. Each position corresponds to a callback function:
 * - `AgoraAudioFramePositionPlayback (1 << 0)`: The position for playback audio frame is received, which corresponds to the \ref onPlaybackFrame "onPlaybackFrame" callback.
 * - `AgoraAudioFramePositionRecord (1 << 1)`: The position for record audio frame is received, which corresponds to the \ref onRecordFrame "onRecordFrame" callback.
 * - `AgoraAudioFramePositionMixed (1 << 2)`: The position for mixed audio frame is received, which corresponds to the \ref onMixedFrame "onMixedFrame" callback.
 * - `AgoraAudioFramePositionBeforeMixing (1 << 3)`: The position for playback audio frame before mixing is received, which corresponds to the \ref onPlaybackFrameBeforeMixing "onPlaybackFrameBeforeMixing" callback.
 */
- (AgoraAudioFramePosition)getObservedAudioFramePosition;

/**
 * Sets the audio sampling format for the \ref onMixedFrame "onMixedFrame" callback.
 */
- (AgoraAudioParam* _Nonnull)getMixedAudioParams;

/**
 * Sets the audio sampling format for the \ref onRecordFrame "onRecordFrame" callback.
 */
- (AgoraAudioParam* _Nonnull)getRecordAudioParams;

/**
 * Sets the audio sampling format for the \ref onPlaybackFrame "onPlaybackFrame" callback.
 */
- (AgoraAudioParam* _Nonnull)getPlaybackAudioParams;

@end
