package company.co.kr.ffmpegplayer;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener
{
    private Button mSimpleVideoPlay;
    private Button mSImpleAudioPlay;
    private Button mAudioTest;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        permissionAccess();

        mSimpleVideoPlay = (Button) findViewById(R.id.simple_video_player);
        mSImpleAudioPlay = (Button) findViewById(R.id.simple_audio_player);
        mAudioTest = (Button) findViewById(R.id.simple_audio_test);
        mSimpleVideoPlay.setOnClickListener(this);
        mSImpleAudioPlay.setOnClickListener(this);

    }


    @Override
    public void onClick(View view)
    {
        switch (view.getId())
        {
            case R.id.simple_video_player:
                gotoSimpleVideoPlayer();
                break;
            case R.id.simple_audio_player:
                gotoSimpleAudioPlayer();
                break;
//            case R.id.simple_audio_test:
//                gotoAudioTestPlayer();
//                break;
        }
    }

//    private void gotoAudioTestPlayer()
//    {
//        Intent intent = new Intent(this, AudioTestActivity.class);
//        startActivity(intent);
//    }

    private void gotoSimpleAudioPlayer()
    {
        Intent intent = new Intent(this, SimpleAudioPlayer.class);
        startActivity(intent);
    }

    private void gotoSimpleVideoPlayer()
    {
        Intent intent = new Intent(this, SimpleVideoPlayer.class);
        startActivity(intent);
    }

    private void permissionAccess() {
        // Here, thisActivity is the current activity
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {

            // Should we show an explanation?
            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.READ_EXTERNAL_STORAGE)) {

                // Show an explanation to the user *asynchronously* -- don't block
                // this thread waiting for the user's response! After the user
                // sees the explanation, try again to request the permission.

            } else {
                // No explanation needed, we can request the permission.
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                        1);

                // MY_PERMISSIONS_REQUEST_READ_CONTACTS is an
                // app-defined int constant. The callback method gets the
                // result of the request.
            }
        }

    }
}
