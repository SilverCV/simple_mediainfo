#include <iostream>
#define __STDC_FORMAT_MACROS
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavfilter/avfilter.h>
#include <libavutil/avutil.h>
#include <libavutil/timestamp.h>
#include <libavdevice/avdevice.h>
#ifdef __cplusplus
}
#endif
void show_audio_info(AVFormatContext *fmt_ctx,int audio_stream_index);
void show_video_info(AVFormatContext *fmt_ctx,int video_stream_index);
int main(int argc,char **argv)
{
  if (argc != 2){
    std::cout << "Usage : mediaInfo <media file>" << std::endl;
    return -1;
  }
  const char *input = argv[1];
  av_register_all();
  avformat_network_init();
  AVFormatContext *_ifmt_ctx = avformat_alloc_context();
  if (avformat_open_input(&_ifmt_ctx,input,NULL,NULL) < 0){
    av_log(NULL,AV_LOG_ERROR,"fail to open input");
    return -1;
  }
  if (avformat_find_stream_info(_ifmt_ctx,NULL) < 0){
    av_log(NULL,AV_LOG_ERROR,"fail to find stream");
    return -1;
  }
 // av_dump_format(_ifmt_ctx,0,input,0);
  int _audio_stream_index = -1;
  int _video_stream_index = -1;
  _audio_stream_index = av_find_best_stream(_ifmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
  _video_stream_index = av_find_best_stream(_ifmt_ctx,AVMEDIA_TYPE_VIDEO,-1,-1,NULL,0);
  if (_audio_stream_index == -1){
    std::cout << "no audio info" <<std::endl;
  }
  if (_video_stream_index == -1){
    std::cout << "no video info" << std::endl;
  }
  if (_audio_stream_index == _video_stream_index){
    std::cout << "no audio info either video info or could not decode this file" << std::endl;
    return -1;
  }
  std::cout << "filename : " << _ifmt_ctx->url << std::endl;
  std::cout << "input container : " << _ifmt_ctx->iformat->name << std::endl;
  std::cout << "title : " << _ifmt_ctx-> <<std::endl;
  if(_audio_stream_index != -1) {
    std::cout << "----------------audio info---------------" << std::endl;
    show_audio_info(_ifmt_ctx, _audio_stream_index);
  }
  if (_video_stream_index != -1) {
    std::cout << "----------------video info---------------" << std::endl;
    show_video_info(_ifmt_ctx, _video_stream_index);
  }
  avformat_close_input(&_ifmt_ctx);
  return 0;
}
//音频要展示的有 编码格式 时长，比特率。采样率 ，声道数目，音频流所在的位置
void show_audio_info(AVFormatContext *fmt_ctx,int audio_stream_index){
  AVStream* audio_steam =fmt_ctx->streams[audio_stream_index];
    AVCodecContext *pCodecCtx = audio_steam->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
      av_log(NULL,AV_LOG_ERROR,"fail to find codec");
      exit(-1);
    }
    if (avcodec_open2(pCodecCtx,pCodec,NULL) < 0){
      av_log(NULL,AV_LOG_ERROR,"fail to open codec");
      exit(-1);
    }
    //输出信息
  std::cout << "ID : " << audio_stream_index << std::endl;
  std::cout << "codec name: " << pCodec->name << std::endl;
    std::cout << " channel : " << pCodecCtx->channels << std::endl;
    std::cout << "sample rate is : " << pCodecCtx->sample_rate /1000.0  << "Hz"<< std::endl;
    int seconds =  av_q2d(audio_steam->time_base) * audio_steam->duration;
    int minutes = seconds / 60;
    seconds = seconds %60;
    std::cout << "duration : " << minutes << "m " << seconds << "s" << std::endl;
    std::cout << "bit rate: " << pCodecCtx->bit_rate / 1000 << "Hz" << std::endl;
}
//视频要展示的有 宽 高 ，编码格式，时长，比特率 音频流所在的位置
void show_video_info(AVFormatContext *fmt_ctx,int video_stream_index){
  AVStream* video_stream =fmt_ctx->streams[video_stream_index];
  AVCodecContext *pCodecCtx = fmt_ctx->streams[video_stream_index]->codec;
  AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL){
    av_log(NULL,AV_LOG_ERROR,"fail to find codec");
    exit(-1);
  }
  if (avcodec_open2(pCodecCtx,pCodec,NULL) < 0){
    av_log(NULL,AV_LOG_ERROR,"fail to open codec");
    exit(-1);
  }
  std::cout << "ID : " << video_stream_index << std::endl;
  std::cout << "codec name: " << pCodec->name << std::endl;
  std::cout << "pix fmt : " << pCodecCtx->pix_fmt << std::endl;
  int seconds =  av_q2d(video_stream->time_base) * video_stream->duration;
  int minutes = seconds / 60;
  seconds = seconds %60;
  std::cout << "duration : " << minutes << "m " << seconds << "s" << std::endl;
  std::cout << "bit rate: " << pCodecCtx->bit_rate / 1000 << "Hz" << std::endl;
  std::cout << "width X height: " << pCodecCtx->width << "X" << pCodecCtx->height << std::endl;
  std::cout << "frame count:" << video_stream->nb_frames << std::endl;
}