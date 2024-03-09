package com.demo.render;

import android.content.Context;
import android.os.Bundle;
import android.os.Environment;

import androidx.appcompat.app.AppCompatActivity;

import com.core.render.XView;

import java.io.File;

public class PlayerActivity extends AppCompatActivity {

    private XView mDisplayView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_player);
        initView();
    }

    private void initView() {
        mDisplayView = findViewById(R.id.xv_display);

        findViewById(R.id.btn_once_press).setOnClickListener(v -> {
            // "/sdcard/Android/data/com.demo.render/files/xinwenlianbo.mp4"
            String path = getPrivateExternalFilesDir(this, "");
            mDisplayView.setInput(path + "xinwenlianbo.mp4");
            mDisplayView.start();
        });

        findViewById(R.id.btn_prepare).setOnClickListener(v -> {
            mDisplayView.prepare(0);
        });

        findViewById(R.id.btn_start).setOnClickListener(v -> {
            mDisplayView.start();
        });

        findViewById(R.id.btn_pause).setOnClickListener(v -> {
            mDisplayView.pause();
        });

        findViewById(R.id.btn_stop).setOnClickListener(v -> {
            mDisplayView.stop();
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
