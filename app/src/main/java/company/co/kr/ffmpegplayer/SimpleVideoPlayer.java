package company.co.kr.ffmpegplayer;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import company.co.kr.videoplayer.VideoPlayer;

/**
 * Created by whkong on 2017-11-13.
 */

public class SimpleVideoPlayer extends AppCompatActivity implements SurfaceHolder.Callback
{
    private VideoPlayer mPlayer;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_simple_video_player);
        mPlayer = new VideoPlayer();
        mSurfaceView = (SurfaceView)findViewById(R.id.surface);
        mSurfaceHolder = mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);

    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder)
    {
        mPlayer.play(surfaceHolder.getSurface(), "/storage/9016-4EF8/aaa.mkv");
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder)
    {
        mPlayer.stop();
    }
}
