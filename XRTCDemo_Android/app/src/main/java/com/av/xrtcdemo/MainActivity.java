package com.av.xrtcdemo;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    final String[] items = {"声网", "华为", "腾讯", "云信"};
    //默认选中第一个
    int rtcType = 0;

    private TextView tvSelectRtc;
    private EditText etRoomId;
    private EditText etUserId;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvSelectRtc = findViewById(R.id.tv_select_rtc);
        etRoomId = findViewById(R.id.et_room_id);
        etUserId = findViewById(R.id.et_user_id);
    }

    /**
     * 选择RTC被点击
     */
    public void onRtcSelectClick(View view) {
        showRTCSelectDialog();
    }

    int mSelect = 0;

    private void showRTCSelectDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this).setIcon(R.mipmap.ic_launcher).setTitle("选择RTC")
                .setSingleChoiceItems(items, mSelect, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        mSelect = i;
                    }
                })

                .setPositiveButton(R.string.confirm, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        if (rtcType != mSelect) {
                            rtcType = mSelect;
                            setRtcName(rtcType);
                        }

                    }
                });
        builder.create().show();
    }

    private void setRtcName(int rtcType) {
        tvSelectRtc.setText(String.format("选择RTC：%s", items[rtcType]));
    }

    /**
     * 进入频道被点击
     *
     * @param view
     */
    public void onJoinClick(View view) {
        String userId = etUserId.getText().toString();
        String roomId = etRoomId.getText().toString();
        if (TextUtils.isEmpty(userId) || TextUtils.isEmpty(roomId)) {
            Toast.makeText(this, getString(R.string.room_id_nonnull), Toast.LENGTH_SHORT).show();
        } else {
            RTCActivity.startActivity(this, Integer.parseInt(userId), roomId, rtcType);
        }
    }

}