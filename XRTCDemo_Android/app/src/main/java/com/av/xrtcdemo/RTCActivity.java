package com.av.xrtcdemo;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.av.xrtc.XrtcEngine;
import com.av.xrtc.IEngineEventHandler;
import com.av.xrtc.params.InitParams;
import com.av.xrtc.params.LocalAudioStats;
import com.av.xrtc.params.LocalVideoStats;
import com.av.xrtc.params.RemoteAudioStats;
import com.av.xrtc.params.RemoteVideoStats;
import com.av.xrtc.params.RtcStats;
import com.av.xrtc.params.TipInfo;
import com.av.xrtc.params.UserParam;
import com.av.xrtc.type.ConnStateType;
import com.av.xrtc.type.RenderMode;
import com.av.xrtc.type.RoleType;
import com.av.xrtc.type.RtcType;
import com.av.xrtc.type.VideoEncodeType;

import org.json.JSONException;

import java.util.ArrayList;
import java.util.HashMap;

public class RTCActivity extends BaseActivity {

    public static final String USER_ID = "userId";
    public static final String ROOM_ID = "roomId";
    public static final String RTC_TYPE = "rtcType";

    private static final String[] PERMISSIONS = {
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.CAMERA,
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
    };
    private static final int PERMISSION_REQ_CODE = 0x1;
    private static final String TAG = "RTCActivity";

    private TextView tvTitle;
    private Button btStopAudio;
    private Button btStopVideo;
    private Button btLeave;
    private FrameLayout flLocal;
    private FrameLayout flRemote1;
    private FrameLayout flRemote2;
    private FrameLayout flRemote3;

    private String roomId;
    private int userId;

    private int rtcType = 0; //RTC?????????0.?????????1.?????? 2.?????? 3.??????
    private XrtcEngine mRtcEngine;

    private boolean isGranted = false;//??????????????????
    private boolean isJoinedRoom = false;
    private boolean isPushStream = false;
    boolean mLocalMuteAudio = false;
    boolean mLocalMuteVideo = false;

    HashMap<String, HashMap> remoteUserMap = new HashMap<>();//????????????

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rtc);

        tvTitle = findViewById(R.id.tv_title);
        btStopAudio = findViewById(R.id.bt_stop_audio);
        btStopVideo = findViewById(R.id.bt_stop_video);
        btLeave = findViewById(R.id.bt_leave);
        flLocal= findViewById(R.id.fl_local);
        flRemote1= findViewById(R.id.fl_remote1);
        flRemote2= findViewById(R.id.fl_remote2);
        flRemote3= findViewById(R.id.fl_remote3);

        flRemote1.setTag(0);
        flRemote2.setTag(0);
        flRemote3.setTag(0);

        initData();

        tvTitle.setText("???????????????...");

        isGranted = checkPermission();

        if (!isGranted) {
            ActivityCompat.requestPermissions(this, PERMISSIONS, PERMISSION_REQ_CODE);
        } else {
            initRtc();
        }
    }

    private void initData() {
        userId = getIntent().getIntExtra(USER_ID, 0);
        roomId = getIntent().getStringExtra(ROOM_ID);
        rtcType = getIntent().getIntExtra(RTC_TYPE, 0);

        if (userId == 0 || TextUtils.isEmpty(roomId)) {
            Toast.makeText(this, getString(R.string.room_id_nonnull), Toast.LENGTH_SHORT).show();
            finish();
        }
    }

    public UserParam getUserParam() {
        if (userParam == null) {
            userParam = new UserParam(userId);
        }
        return userParam;
    }

    private UserParam userParam;

    private void initRtc() {

        tvTitle.setText("???????????????...");

        XrtcEngine.getAuthorization("xrtctest01", "", new XrtcEngine.AuthorizationCallBack() {
            @Override
            public void onResult(int code, String msg) {
                Log.i(TAG, "???????????????" + "code=" + code + ",msg=" + msg);
                if (code == 0) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mRtcEngine = XrtcEngine.createRtc(initParams);

                            mRtcEngine.configVideoEncoder(VideoEncodeType.VIDEO_ENCODE_240P);
                            mRtcEngine.addEventHandler(engineEventHandler);

                            tvTitle.setText("???????????????...");

                            mRtcEngine.joinChannel(getUserParam(), "", roomId);
                        }
                    });
                }
                else
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            tvTitle.setText("???????????????");
                        }
                    });
                }
            }
        });
    }

    private final IEngineEventHandler engineEventHandler = new IEngineEventHandler() {
        @Override
        public void onWarning(TipInfo tipInfo) {

        }

        @Override
        public void onError(TipInfo tipInfo) {

        }

        @Override
        public void onJoinChannelSuccess(String channel, UserParam.UUID uid) {
            Log.i(TAG, "onJoinChannelSuccess");
            isJoinedRoom=true;

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    tvTitle.setText("???????????????" + roomId);
                }
            });
        }

        @Override
        public void onLeaveChannel() {
            Log.i(TAG, "onLeaveChannel");
        }

        @Override
        public void onUserJoined(UserParam.UUID uid) {
            Log.i(TAG, "onUserJoined:" + uid.getUid());

            HashMap<String, Integer> map = new HashMap<>();
            map.put("uid", uid.getUid());
            map.put("isRemoteMuteAudio", 0);
            map.put("isRemoteMuteVideo", 0);

            remoteUserMap.put(uid.getUserid(), map);
        }

        @Override
        public void onUserOffline(UserParam.UUID uid, int reason) {
            Log.i(TAG, "onUserOffline:" + uid.getUid());
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    int userId = uid.getUid();

                    remoteUserMap.remove(uid.getUserid());

                    unbindVideoView(userId,true);
                }
            });
        }

        @Override
        public void onConnStateChange(ConnStateType state) {
            Log.i(TAG, "onConnStateChange,state=" + state);
        }

        @Override
        public void onFirstRemoteAudioDecoded(UserParam.UUID uid) {
            Log.i(TAG, "onFirstRemoteAudioDecoded???" + uid.getUid());
        }

        @Override
        public void onFirstRemoteVideoDecoded(UserParam.UUID uid) {
            Log.i(TAG, "onFirstRemoteVideoDecoded???" + uid.getUid());
            runOnUiThread(() -> {
                int userId = uid.getUid();
                FrameLayout videoView = getBlankVideoView(userId);

                if (videoView != null) {
                    UserParam remoteUserParam = new UserParam(userId);
                    mRtcEngine.setRemoteVideo(videoView, true, RenderMode.HIDDEN, remoteUserParam);
                }
                else {
                    mRtcEngine.muteRemoteAudioStream(uid, true);
                    mRtcEngine.muteRemoteVideoStream(uid, true);
                    Toast.makeText(getBaseContext(), "????????????????????????4??????", Toast.LENGTH_SHORT).show();
                }
            });
        }

        @Override
        public void onUserMuteAudio(UserParam.UUID uid, boolean muted) {
            Log.i(TAG, "onUserMuteAudio???" + uid.getUid() + ",is muted=" + muted);

            HashMap<String, Integer> map=remoteUserMap.get(uid.getUserid());
            map.put("isRemoteMuteAudio", muted ? 1 : 0);

            int isRemoteMuteAudio=map.get("isRemoteMuteAudio");
            int isRemoteMuteVideo=map.get("isRemoteMuteVideo");

            //???????????????????????????
            runOnUiThread(() -> {
                int userId = uid.getUid();
                FrameLayout bindVideoView = getUseVideoView(userId);
                if (isRemoteMuteVideo==1 && isRemoteMuteAudio==0) {
                    Drawable img = getResources().getDrawable(R.drawable.audio_play);
                    if (bindVideoView!=null) {
                        bindVideoView.setForeground(img);
                    }
                }
                else {
                    if (bindVideoView!=null && bindVideoView.getForeground() !=null) {
                        bindVideoView.setForeground(null);
                    }
                }
            });
        }

        @Override
        public void onUserMuteVideo(UserParam.UUID uid, boolean muted) {
            Log.i(TAG, "onUserMuteVideo???" + uid.getUid() + ",is muted=" + muted);

            HashMap<String, Integer> map=remoteUserMap.get(uid.getUserid());
            map.put("isRemoteMuteVideo", muted ? 1 : 0);

            int isRemoteMuteAudio=map.get("isRemoteMuteAudio");
            int isRemoteMuteVideo=map.get("isRemoteMuteVideo");

            //???????????????????????????
            runOnUiThread(() -> {
                int userId = uid.getUid();
                FrameLayout bindVideoView = getUseVideoView(userId);
                if (isRemoteMuteVideo==1 && isRemoteMuteAudio==0) {
                    Drawable img = getResources().getDrawable(R.drawable.audio_play);
                    if (bindVideoView!=null) {
                        bindVideoView.setForeground(img);
                    }
                }
                else {
                    if (bindVideoView!=null && bindVideoView.getForeground() !=null) {
                        bindVideoView.setForeground(null);
                    }
                }
            });
        }

        @Override
        public void onVideoStats(@Nullable LocalVideoStats localVideoStats, @Nullable RemoteVideoStats remoteVideoStats) {

        }

        @Override
        public void onAudioStats(@Nullable LocalAudioStats localAudioStats, @Nullable RemoteAudioStats remoteAudioStats) {

        }
    };

    /**
     * ??????uid?????????????????????????????????
     * @param uid ??????ID
     * @param isRemoteView ?????????????????????
     */
    private void unbindVideoView(int uid, boolean isRemoteView) {
        UserParam userParam = new UserParam(uid);
        if (isRemoteView){
            FrameLayout bindVideoView = getUseVideoView(uid);
            if (bindVideoView!=null) {
                bindVideoView.setTag(0);
                mRtcEngine.setRemoteVideo(bindVideoView, false, RenderMode.HIDDEN, userParam);
            }
        }else {
            mRtcEngine.setLocalVideo(flLocal, false, RenderMode.HIDDEN, userParam);
        }
    }

    /**
     * ??????XRTC?????????????????????
     */
    InitParams initParams = new InitParams() {

        @Override
        public Context appContext() {
            return getApplicationContext();
        }

        @Override
        public RtcType rtcType() {
            return getSDKType(rtcType);
        }

        @Override
        public String appId() {
            return getRTCAppId(rtcType);
        }

        /**
         * ??????
         * ?????????domain??????????????????RTC???????????????
         * @return ??????Domain
         */
        @Override
        public String domain() {
            return getDomain();
        }

        /**
         * ??????
         * ?????????secretKey?????????????????????RTC???????????????
         * @return secretKey
         */
        @Override
        public String secretKey() {
            return getSecretKey();
        }
    };

    private String getSecretKey() {
        return getString(R.string.tencent_secretKey);
    }

    private String getDomain() {
        return getString(R.string.hw_Domain);
    }

    /**
     * ???????????????????????????RTC??????????????????RTC????????????APPid
     *
     * @param rtcType RTC?????????
     * @return ??????RTC????????????APPid
     */
    private String getRTCAppId(int rtcType) {
        switch (rtcType) {
            case 1:
                return getString(R.string.hw_appid);
            case 2:
                return getString(R.string.tencent_appid);
            case 3:
                return getString(R.string.yunxin_appid);
            default:
                return getString(R.string.agora_appid);
        }
    }

    /**
     * ???????????????????????????RTC??????????????????XRTC?????????Type
     *
     * @param rtcType RTC?????????
     * @return XRTC?????????Type
     */
    private RtcType getSDKType(int rtcType) {
        RtcType hRtc;
        switch (rtcType) {
            case 1:
                hRtc = RtcType.HRtc;
                break;
            case 2:
                hRtc = RtcType.TRtc;
                break;
            case 3:
                hRtc = RtcType.WRtc;
                break;
            default:
                hRtc = RtcType.Agora;
                break;
        }
        Log.i(TAG, "RtcType:" + hRtc.name());
        return hRtc;
    }

    private boolean checkPermission() {
        boolean granted = true;
        for (String per : RTCActivity.PERMISSIONS) {
            if (!permissionGranted(per)) {
                granted = false;
                break;
            }
        }
        return granted;
    }

    private boolean permissionGranted(String per) {
        return ContextCompat.checkSelfPermission(this, per) == PackageManager.PERMISSION_GRANTED;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_REQ_CODE) {
            boolean granted = true;
            for (int grant : grantResults) {
                if (PackageManager.PERMISSION_GRANTED != grant) {
                    granted = false;
                    break;
                }
            }

            if (!granted) {
                Toast.makeText(this, R.string.permission_denied, Toast.LENGTH_SHORT).show();
                isGranted = false;
            } else {
                isGranted = true;
                initRtc();
            }
        }
    }

    /**
     * ??????RTCDemo??????
     *
     * @param activity ????????????RTCDemo?????????
     * @param userId       ??????ID
     * @param roomId  ?????????
     * @param rtcType  rtc??????
     */
    public static void startActivity(Activity activity, int userId, String roomId, int rtcType) {
        if (userId == 0 || TextUtils.isEmpty(roomId)) {
            Toast.makeText(activity.getBaseContext(), activity.getString(R.string.room_id_nonnull), Toast.LENGTH_SHORT).show();
        } else {
            Intent intent = new Intent(activity, RTCActivity.class);
            intent.putExtra(RTCActivity.USER_ID, userId);
            intent.putExtra(RTCActivity.ROOM_ID, roomId);
            intent.putExtra(RTCActivity.RTC_TYPE, rtcType);
            activity.startActivity(intent);
        }
    }

    /**
     * ?????????????????????
     *
     * @param view view
     *
     */
    public void onLeaveClick(View view) {

        if (isJoinedRoom==true) {
            if (isPushStream){
                int uid = getUserParam().getUser().getUid();
                unbindVideoView(uid,false);
                isPushStream = false;
            }

            mRtcEngine.removeEventHandler(engineEventHandler);
            mRtcEngine.leaveChannel();
            mRtcEngine.destroy();
        }

        finish();
    }

    @Override
    protected void onDestroy() {
        onLeaveClick(btLeave);
        super.onDestroy();
    }

    /**
     * ?????????????????????
     */
    public void onStopAudioClick(View view) {
        if (isPushStream) {
            Log.i(TAG, "onStopAudioClick");
            mLocalMuteAudio = !mLocalMuteAudio;
            muteLocalAudioStream(mLocalMuteAudio);
        } else {
            Toast.makeText(this, getString(R.string.please_push_stream), Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * ?????????????????????
     */
    public void onStopVideoClick(View view) {
        if (isPushStream) {
            Log.i(TAG, "onStopVideoClick");
            mLocalMuteVideo = !mLocalMuteVideo;
            muteLocalVideoStream(mLocalMuteVideo);

            //???????????????????????????
            if (mLocalMuteVideo==true) {
                Drawable img = getResources().getDrawable(R.drawable.audio_play);
                flLocal.setForeground(img);
            }
            else {
                if (flLocal.getForeground() !=null) {
                    flLocal.setForeground(null);
                }
            }

        } else {
            Toast.makeText(this, getString(R.string.please_push_stream), Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * ??????????????????
     */
    public void onStartPushStreamClick(View view) {
        Log.i(TAG, "onStartPushStreamClick");

        //?????????????????????
        if (!isJoinedRoom) {
            Toast.makeText(this, getString(R.string.joined_room), Toast.LENGTH_SHORT).show();

            return;
        }

        //????????????
        if (!isGranted) {
            Toast.makeText(this, getString(R.string.need_necessary_permissions), Toast.LENGTH_SHORT).show();

            return;
        }

        //?????????????????????
        if (isPushStream) {
            Toast.makeText(this, getString(R.string.alrady_push_stream), Toast.LENGTH_SHORT).show();

            return;
        }

        //???????????????????????????
        mRtcEngine.switchRole(RoleType.ANCHOR);

        //??????????????????
        UserParam userParam = getUserParam();
        int uid = userParam.getUser().getUid();

        //?????????????????????
        mRtcEngine.setLocalVideo(flLocal, true, RenderMode.HIDDEN, getUserParam());
        //???????????????
        muteLocalVideoStream(false);

        muteLocalAudioStream(false);

        isPushStream = true;
    }

    /**
     * ??????????????????
     */
    public void onStopPushStreamClick(View view) {
        if (isPushStream) {
            Log.i(TAG, "onStopPushStreamClick");
            //????????????????????????????????????
            mRtcEngine.switchRole(RoleType.AUDIENCE);
            //????????????
            muteLocalVideoStream(true);
            muteLocalAudioStream(true);

            int uid = userParam.getUser().getUid();
            unbindVideoView(uid,false);

            if (flLocal.getForeground() !=null) {
                flLocal.setForeground(null);
            }

            mLocalMuteAudio = false;
            mLocalMuteVideo = false;

            isPushStream = false;
        } else {
            Toast.makeText(this, getString(R.string.please_push_stream), Toast.LENGTH_SHORT).show();
        }
    }

    private void muteLocalVideoStream(boolean mute) {
        mRtcEngine.muteLocalVideoStream(mute);
        mLocalMuteVideo = mute;
        if (mute) {
            btStopVideo.setText(R.string.open_video);
        } else {
            btStopVideo.setText(R.string.stop_video);
        }
    }

    /**
     * ??????/?????? ????????????
     * @param mute ????????????
     */
    private void muteLocalAudioStream(boolean mute) {
        mLocalMuteAudio = mute;
        mRtcEngine.muteLocalAudioStream(mute);
        if (mute) {
            btStopAudio.setText(R.string.open_audio);
        } else {
            btStopAudio.setText(R.string.stop_audio);
        }
    }

    /**
     * ??????????????????????????????
     * @param uid
     * @return
     */
    private FrameLayout getBlankVideoView(int uid) {

        FrameLayout flUse = null;
        if ((int)flRemote1.getTag() == 0) {
            flUse = flRemote1;
        }
        else if ((int)flRemote2.getTag() == 0) {
            flUse = flRemote2;
        }
        else if ((int)flRemote3.getTag() == 0) {
            flUse = flRemote3;
        }

        if (flUse != null) {
            flUse.setTag(uid);
        }

        return flUse;
    }

    /**
     * ??????????????????????????????
     * @param uid
     * @return
     */
    private FrameLayout getUseVideoView(int uid) {

        FrameLayout flUse = null;
        if ((int)flRemote1.getTag() == uid) {
            flUse = flRemote1;
        }
        else if ((int)flRemote2.getTag() == uid) {
            flUse = flRemote2;
        }
        else if ((int)flRemote3.getTag() == uid) {
            flUse = flRemote3;
        }

        return flUse;
    }

}