//
// Created by whkong on 2017-11-10.
//
#include <string>

#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>


extern "C" {

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"

}

#include "VideoPlayer.h"

#define TAG "VideoPlayer"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print( ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

JNIEXPORT jint JNICALL
Java_company_co_kr_videoplayer_VideoPlayer_nativePlay(JNIEnv *env, jobject instance,
                                                      jobject surface, jstring fileName) {
    LOGV("file_name = %s", fileName);
    //const char *file_name = "파일 경로 이름";
    const char *file_name = env->GetStringUTFChars( fileName, NULL);
    LOGV("file_name = %s", file_name);
    int ret = 0;
    ANativeWindow *nativeWindow = NULL;
    AVCodec *codec = NULL;

    //Must call av_register_all() first,register libavformat and register all the muxers, demuxers and
    //protocols. If you need network ,you should call avformat_network_init()
    av_register_all();
    LOGV("av_register_all()");
    //AVFormatContext is Format I/O context ,use for deal with different type of media file
    //AVFormatContext should be created by avformat_alloc_context(),you should call
    //avformat_free_context(formatContext) to avoid leak.

    AVFormatContext *formatContext = avformat_alloc_context();

    //Open an input stream and read the header. The codecs are not opened.
    //The stream must be closed with avformat_close_input().
    if ((ret = avformat_open_input(&formatContext, file_name, NULL, NULL)) != 0) {
        LOGV("avformat_open_input() ret = %d", ret);
        return JNI_FALSE;
    }

    //Read packets of a media file to get stream information. This
    //is useful for file formats with no headers such as MPEG. This
    //function also computes the real framerate in case of MPEG-2 repeat
    //frame mode.
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        LOGV("Couldn't find stream information.");
        return JNI_FALSE;
    }

    // many stream ,we just find the video stream
    int videoStreamIndex = -1;
    int i = 0;
    for (i; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
            videoStreamIndex < 0) {
            videoStreamIndex = i;
            LOGV("find videoStreamIndex = %d", i);
            break;
        }
    }
    if (videoStreamIndex < 0) {
        LOGE("can not find videoStreamIndex ");
        return JNI_FALSE;
    }

    //If you find the stream ,you can get it's AVCodecContext
    //In old FFmpeg version ,just use AVCodecContext *codecContext = formatContext->streams[videoStreamIndex]->codec;
    //But in newer version ,you should get codec first ,then create AVCodecContext
    codec = avcodec_find_decoder(formatContext->streams[videoStreamIndex]->codecpar->codec_id);
    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar);

    // get native window
    nativeWindow = ANativeWindow_fromSurface(env, surface);

    // get video width, height
    int videoWidth = codecContext->width;
    int videoHeight = codecContext->height;

    // set native window's buffer size, type
    ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                     WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer windowBuffer;


    //얻은 코덱은 avcodec_open2로 열어야 합니다
    avcodec_open2(codecContext, codec, NULL);
    if (!codec) {
        return JNI_FALSE;
    }

    //디코딩 된 이미지를 받아들이도록 AVFrame을 만듭니다.
    AVFrame *pFrame = av_frame_alloc();

    AVFrame *pRGBFrame = av_frame_alloc();
    int numRGBFrameByte = av_image_get_buffer_size(AV_PIX_FMT_RGBA, codecContext->width,
                                                   codecContext->height, 1);
    uint8_t *buffer = (uint8_t *) av_malloc(numRGBFrameByte * sizeof(uint8_t));
    av_image_fill_arrays(pRGBFrame->data, pRGBFrame->linesize, buffer, AV_PIX_FMT_RGBA,
                         codecContext->width, codecContext->height, 1);

    // 디코딩 된 프레임 형식이 RGBA가 아니기 때문에 렌더링 하기 전에 형식 변환이 필요합니다.
    struct SwsContext *sws_ctx = sws_getContext(codecContext->width,
                                                codecContext->height,
                                                codecContext->pix_fmt,
                                                codecContext->width,
                                                codecContext->height,
                                                AV_PIX_FMT_RGBA,
                                                SWS_BILINEAR,
                                                NULL,
                                                NULL,
                                                NULL);
    AVPacket packet;
    while(1)
    {
        if(( ret = av_read_frame(formatContext, &packet)) < 0)
            break;

        if(packet.stream_index == videoStreamIndex)
        {
            ret = avcodec_send_packet(codecContext, &packet);
            if(ret < 0)
            {
                av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
                break;
            }

            while(ret >= 0)
            {
                ret = avcodec_receive_frame(codecContext, pFrame);
                if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                {
                    break;
                }else if( ret < 0)
                {
                    av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
                    return JNI_FALSE;
                }

                if (ret >= 0) {
                    LOGV("got_picture");
                    // lock native window buffer
                    ANativeWindow_lock(nativeWindow, &windowBuffer, 0);

                    // 格式转换
                    sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                              pFrame->linesize, 0, codecContext->height,
                              pRGBFrame->data, pRGBFrame->linesize);

                    // 获取stride
                    uint8_t *dst = (uint8_t *) windowBuffer.bits;
                    int dstStride = windowBuffer.stride * 4;
                    uint8_t *src = (pRGBFrame->data[0]);
                    int srcStride = pRGBFrame->linesize[0];

                    // 由于window的stride和帧的stride不同,因此需要逐行复制
                    int h;
                    for (h = 0; h < videoHeight; h++) {
                        memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                    }

                    ANativeWindow_unlockAndPost(nativeWindow);
                }
            }
        }
        av_packet_unref(&packet);
    }


    ANativeWindow_release(nativeWindow);
    av_free(buffer);
    av_free(pRGBFrame);
    // Free the YUV frame
    av_free(pFrame);

    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);

    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);

    sws_freeContext(sws_ctx);
    env->ReleaseStringUTFChars(fileName, file_name);
    return JNI_TRUE;
}

JNIEXPORT  jboolean JNICALL
Java_company_co_kr_videoplayer_VideoPlayer_nativeStop(JNIEnv *env, jobject instance) {
    return JNI_TRUE;
}


