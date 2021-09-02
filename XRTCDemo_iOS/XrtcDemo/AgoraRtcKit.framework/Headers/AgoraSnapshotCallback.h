//
//  AgoraSnapshot.h
//  AgoraRtcEngineKit
//
//  Copyright © 2020 Agora. All rights reserved.
//

#ifndef AgoraSnapshotCallback_h
#define AgoraSnapshotCallback_h

/** The definition of AgoraSnapshotCallbackDelegate

@note  Implement the callback in this protocol in the critical thread. We recommend avoiding any time-consuming operation in the critical thread.
*/
@protocol AgoraSnapshotCallbackDelegate <NSObject>
@required

- (NSInteger)onSnapshotTaken:(const NSString*)channel uid:(NSInteger)uid filePath:(const NSString*)filePath width:(NSInteger)width height:(NSInteger)height errCode:(NSInteger)errCode;

@end

#endif /* AgoraSnapshot_h */
