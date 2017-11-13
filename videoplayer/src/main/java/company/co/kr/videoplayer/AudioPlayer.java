package company.co.kr.videoplayer;

import android.util.Log;

/**
 * Created by whkong on 2017-11-10.
 */

public class AudioPlayer {
    static {
        System.loadLibrary("audioplayer");
    }

    public boolean playMp3Audio(final String fileName) {
        Log.d("AudioPlayer", fileName);

        new Thread(new Runnable() {
            @Override
            public void run() {
                nativePlayAudio(fileName);

            }
        }).toString();
        return true;
    }

    private native void nativePlayAudio(String fileName);
}
