package company.co.kr.videoplayer;

import android.util.Log;
import android.view.Surface;

import java.io.File;

/**
 * Created by whkong on 2017-11-10.
 */

public class VideoPlayer {
    static {
        System.loadLibrary("videoplayer");
    }

    public boolean play(final Surface surface, final String fileName) {

        new Thread(new Runnable () {
            @Override
            public void run() {
                File file = new File(fileName);
                //File file = new File("/storage/9016-4EF8/aaa.mkv");
                Log.d("VideoPlayer", "file.exists()" + file.exists());
                nativePlay(surface, fileName);

            }
        }).start();
        return true;

    }

    public boolean stop() {
        return nativeStop();
    }

    native private int nativePlay(Surface surface, String fileName);
    native private boolean nativeStop();
}
