#pragma once
#include "Szczur/Modules/Cinematics/MovieSound.hpp"
#include "Szczur/Application.hpp"


/*
 Main class of playing movies. 
 It's a prototype and have a lot of bugs like desynchronized video and audio (rarely). 
 The biggest problem is video stream which drops some frames. 

 For this moment class loads .mp4 correctly
*/
class MoviePlayer
{
public:
    MoviePlayer() = default;
    bool loadFromFile(const char* filename);
    void play();

private:
    AVFormatContext *pFormatCtx = nullptr;
    AVCodecContext  *pCodecCtx = nullptr;
    AVCodecContext  *paCodecCtx = nullptr;
    AVCodec         *pCodec = nullptr;
    AVCodec         *paCodec = nullptr;
    AVFrame         *pFrame = nullptr;
    AVFrame         *pFrameRGB = nullptr;
    int             frameFinished;
    int             numBytes;
    uint8_t         *buffer = nullptr;
    int64_t         m_lastDecodedTimeStamp = 0;
    
    AVDictionary    *optionsDict = nullptr;
    AVDictionary    *optionsDictA = nullptr;
    SwsContext      *sws_ctx = nullptr;
    sf::Texture im_video;
    bool syncAV = false;
    int64_t blockPts = 0;
    std::vector<AVPacket*> audioSyncBuffer;

    int videoStream;
    int audioStream;

};