package com.demo.render;

import android.content.Context;
import android.os.Bundle;
import android.os.Environment;

import androidx.appcompat.app.AppCompatActivity;

import com.core.render.XPlayer;
import com.core.render.XMacView;
import com.core.render.XTimeline;
import com.core.render.XTrack;

import java.io.File;
import java.util.Timer;

// "/sdcard/Android/data/com.demo.render/files/jieqian_720x1280.yuv"

public class PlayerActivity extends AppCompatActivity {

    private XMacView mDisplayView;
    private XPlayer mPlayer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_player);
        initView();

        mPlayer = new XPlayer();
        mPlayer.setSurface(mDisplayView);
    }

    private void initView() {
        mDisplayView = findViewById(R.id.xv_display);

        findViewById(R.id.btn_once_press).setOnClickListener(v -> {
            String path = getPrivateExternalFilesDir(this, "") + "douyin_700x1240.mp4";
            XTrack track = new XTrack();
            track.setFilename(path);
            track.setDelay(0);
            track.setClipStartTime(0);
            track.setClipEndTime(10000);

            XTimeline timeline = new XTimeline();
            timeline.addTrack(track);

            mPlayer.setTimeline(timeline);
            mPlayer.prepare();
            mPlayer.start();
        });

        findViewById(R.id.btn_prepare).setOnClickListener(v -> {
        });

        findViewById(R.id.btn_start).setOnClickListener(v -> {
        });

        findViewById(R.id.btn_pause).setOnClickListener(v -> {
        });

        findViewById(R.id.btn_stop).setOnClickListener(v -> {
        });

        findViewById(R.id.btn_add_input).setOnClickListener(v -> {

        });
    }

    private static String getPrivateExternalFilesDir(Context context, String dirName) {
        try {
            String cachePath = null;
            if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
                cachePath = context.getExternalFilesDir("").getAbsolutePath();
            } else {
                File cacheDir = context.getCacheDir();
                if (cacheDir != null && cacheDir.exists()) {
                    cachePath = cacheDir.getPath();
                }
            }
            return cachePath + File.separator + dirName;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }
}
