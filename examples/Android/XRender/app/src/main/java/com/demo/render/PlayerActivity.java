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

        findViewById(R.id.btn_start).setOnClickListener(v -> {
            // "/sdcard/Android/data/com.demo.render/files/jieqian_720x1280.yuv"
            String path = getPrivateExternalFilesDir(this, "");
            mDisplayView.setInput(path + "jieqian_720x1280.rgba");
            mDisplayView.start();
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
