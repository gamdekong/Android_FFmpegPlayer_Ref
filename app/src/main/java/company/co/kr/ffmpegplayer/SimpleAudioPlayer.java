package company.co.kr.ffmpegplayer;

import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;

import company.co.kr.videoplayer.AudioPlayer;

/**
 * Created by whkong on 2017-11-13.
 */

public class SimpleAudioPlayer extends AppCompatActivity
{
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        AudioPlayer audioPlayer = new AudioPlayer();
        audioPlayer.playMp3Audio(Environment.getExternalStorageDirectory()+"파일명");
    }
}
