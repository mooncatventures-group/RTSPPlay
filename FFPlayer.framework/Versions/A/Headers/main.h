
 
 #ifndef MAIN_H
#define MAIN_H

/*
 * FFplay : Simple Media Player based on the ffmpeg libraries
 * Copyright (c) 2003 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdio.h>
#include <sys/time.h>

#include <math.h>
#include <limits.h>
#include <FFmpegDecoder/libavdevice/avdevice.h>
#include <FFmpegDecoder/libavcodec/audioconvert.h>
#include <FFmpegDecoder/libavutil/internal.h>
#include "cmdutils.h"
#include <SDL/SDL_events_c.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#ifdef __MINGW32__
#undef main /* We don't want SDL to override our main() */
#endif

#undef exit

const char program_name[] = "Bouygtel TV";
const int program_birth_year = 2009;

//#define DEBUG_SYNC

#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)
#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_SUBTITLEQ_SIZE (5 * 16 * 1024)

/* SDL audio buffer size, in samples. Should be small to have precise
 A/V sync as SDL does not have hardware buffer fullness info. */
#define SDL_AUDIO_BUFFER_SIZE 1024

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
#define SAMPLE_ARRAY_SIZE (2*65536)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
//#define FRAME_X 64
//#define FRAME_Y 64
static int FRAME_X = 256;
static int  FRAME_Y = 256;
static int SCALE_X = 64;
static int SCALE_Y=64;
static int xx=500;
static AVCodecContext *enc;
static float SSCALE_X = 1.0;
static float SSCALE_Y = 1.0;
static float TSCALE_X = 1.0;
static float TSCALE_Y = 1.0;

static AVPicture tmpPict;


#define DST_PIX_FMT PIX_FMT_RGB565LE

static int sws_flags = SWS_BICUBIC;

typedef struct PacketQueue {
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	int abort_request;
	SDL_mutex *mutex;
	SDL_cond *cond;
} PacketQueue;

#define VIDEO_PICTURE_QUEUE_SIZE 1
#define SUBPICTURE_QUEUE_SIZE 4

typedef struct VideoPicture {
	double pts;                                  ///<presentation time stamp for this picture
	SDL_Surface *bmp;
	int width, height; /* source height & width */
	int allocated;
} VideoPicture;

typedef struct SubPicture {
	double pts; /* presentation time stamp for this picture */
	AVSubtitle sub;
} SubPicture;

enum {
	AV_SYNC_AUDIO_MASTER, /* default choice */
	AV_SYNC_VIDEO_MASTER,
	AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

typedef struct VideoState {
	SDL_Thread *parse_tid;
	SDL_Thread *video_tid;
	AVInputFormat *iformat;
	int no_background;
	int abort_request;
	int paused;
	int last_paused;
	int seek_req;
	int seek_flags;
	int64_t seek_pos;
	int64_t seek_rel;
	AVFormatContext *ic;
	int dtg_active_format;
	
	int audio_stream;
	
	int av_sync_type;
	double external_clock; /* external clock base */
	int64_t external_clock_time;
	
	double audio_clock;
	double audio_diff_cum; /* used for AV difference average computation */
	double audio_diff_avg_coef;
	double audio_diff_threshold;
	int audio_diff_avg_count;
	AVStream *audio_st;
	PacketQueue audioq;
	int audio_hw_buf_size;
	/* samples output by the codec. we reserve more space for avsync
	 compensation */
	DECLARE_ALIGNED(16,uint8_t,audio_buf1[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]);
	DECLARE_ALIGNED(16,uint8_t,audio_buf2[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]);
	uint8_t *audio_buf;
	unsigned int audio_buf_size; /* in bytes */
	int audio_buf_index; /* in bytes */
	AVPacket audio_pkt_temp;
	AVPacket audio_pkt;
	enum SampleFormat audio_src_fmt;
	AVAudioConvert *reformat_ctx;
	
	int show_audio; /* if true, display audio samples */
	int16_t sample_array[SAMPLE_ARRAY_SIZE];
	int sample_array_index;
	int last_i_start;
	
	SDL_Thread *subtitle_tid;
	int subtitle_stream;
	int subtitle_stream_changed;
	AVStream *subtitle_st;
	PacketQueue subtitleq;
	SubPicture subpq[SUBPICTURE_QUEUE_SIZE];
	int subpq_size, subpq_rindex, subpq_windex;
	SDL_mutex *subpq_mutex;
	SDL_cond *subpq_cond;
	
	double frame_timer;
	double frame_last_pts;
	double frame_last_delay;
	double video_clock;                          ///<pts of last decoded frame / predicted pts of next decoded frame
	int video_stream;
	AVStream *video_st;
	PacketQueue videoq;
	double video_current_pts;                    ///<current displayed pts (different from video_clock if frame fifos are used)
	int64_t video_current_pts_time;              ///<time (av_gettime) at which we updated video_current_pts - used to have running video pts
	VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
	int pictq_size, pictq_rindex, pictq_windex;
	SDL_mutex *pictq_mutex;
	SDL_cond *pictq_cond;
	
	//    QETimer *video_timer;
	char filename[1024];
	int width, height, xleft, ytop;
} VideoState;

static void show_help(void);
static int audio_write_get_buf_size(VideoState *is);

/* options specified by the user */
static AVInputFormat *file_iformat;
static const char *input_filename;
static int fs_screen_width;
static int fs_screen_height;
static int screen_width;
static int screen_height;
static int frame_width;
static int frame_height;
static enum PixelFormat frame_pix_fmt = PIX_FMT_NONE;
static int audio_disable;
static int video_disable;
static int wanted_audio_stream= 0;
static int wanted_video_stream= 0;
static int wanted_subtitle_stream= -1;
static int seek_by_bytes;
static int display_disable;
static int show_status;
static int av_sync_type = AV_SYNC_AUDIO_MASTER;
static int64_t start_time = AV_NOPTS_VALUE;
static int debug = 0;
static int debug_mv = 0;
static int step = 0;
static int thread_count = 1;
static int workaround_bugs = 1;
static int fast = 0;
static int genpts = 0;
static int lowres = 0;
static int glReady= 0;
static int idct = FF_IDCT_AUTO;
static enum AVDiscard skip_frame= AVDISCARD_DEFAULT;
static enum AVDiscard skip_idct= AVDISCARD_DEFAULT;
static enum AVDiscard skip_loop_filter= AVDISCARD_DEFAULT;
static int error_recognition = FF_ER_CAREFUL;
static int error_concealment = 3;
static int decoder_reorder_pts= 0;
static int initGl(void);
/* current context */
static int is_full_screen;
static VideoState *cur_stream;
static int64_t audio_callback_time;

static AVPacket flush_pkt;
SDL_TextureID texture;
//static GLuint tex;
GLfloat points[8];
GLfloat texturePoints[8];

#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)
SDL_Surface *screen;
SDL_WindowID window;
GLuint framebuffer;







#endif /* IMAGING_H */
