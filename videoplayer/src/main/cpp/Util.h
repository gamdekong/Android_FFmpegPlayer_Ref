//
// Created by whkong on 2017-11-10.
//

#ifndef VIDEOPLAYER_UTIL_H
#define VIDEOPLAYER_UTIL_H
#include "libavformat/avformat.h"

int findVideoStreamIndex(AVFormatContext *formatContext);

int findAudioStreamIndex(AVFormatContext *formatContext);

int findStreamIndexByType(AVFormatContext *formatContext, enum AVMediaType type);

#endif //VIDEOPLAYER_UTIL_H
