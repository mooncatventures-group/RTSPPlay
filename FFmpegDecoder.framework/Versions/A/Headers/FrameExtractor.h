
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#define kPollingInterval 1.0/30

@protocol CGImageBufferDelegate <NSObject>
@required
-(void)didOutputCGImageBuffer:(NSTimer *)timer;
@end
@protocol PVPixelBufferDelegate <NSObject>
@optional
-(void)didOutputPixelBuffer:(NSTimer *)timer;
@end
@protocol CMSampleBufferDelegate <NSObject>
@optional
-(void)didOutputSampleBuffer:(NSTimer *)timer;
@end



@interface FrameExtractor : NSObject {
    id<CGImageBufferDelegate> cgimageDelegate;
    id<PVPixelBufferDelegate> pvpixelDelegate;
    id<CMSampleBufferDelegate> cmsampleDelegate;
	int sourceWidth, sourceHeight;
	int outputWidth, outputHeight;
    NSTimer* iTimer;
	float lastFrameTime;
	UIImage *currentImage;
	double duration;
    BOOL frameReady;	
}

- (BOOL)decodeFrame:(NSData*)frameData;
- (BOOL)isFrameReady;
- (NSData*)getDecodedFrame;
- (NSUInteger)getDecodedFrameWidth;
- (NSUInteger)getDecodedFrameHeight;
 @property (retain) id cgimageDelegate;
 @property (retain) id pvpixelDelegate;
 @property (retain) id cmsampleDelegate;

-(void)setupCgimageSession;

-(void)setupPVimageSession;

-(void)setupCmsampleSession;



-(void)setOutputWidth:(int)newValue;

-(void)setOutputHeight:(int)newValue;

-(CVPixelBufferRef)cvPixelBufferRef;

-(CMSampleBufferRef)cmSampleBufferRef;



/* Last decoded picture as UIImage */
@property (nonatomic, readonly) UIImage *currentImage;

/* Size of video frame */
@property (nonatomic, readonly) int sourceWidth, sourceHeight;

/* Output image size. Set to the source size by default. */
@property (nonatomic) int outputWidth, outputHeight;

/* Length of video in seconds */
@property (nonatomic, readonly) double duration;

/* Initialize with movie at moviePath. Output dimensions are set to source dimensions. */
-(id)initWithVideo:(NSString *)moviePath;

/* Read the next frame from the video stream. Returns false if no frame read (video over). */
-(BOOL)stepFrame;

-(void)setupScaler;

-(void)displayNextImageBuffer:(NSTimer *)timer;

-(void)displayNextPVBuffer:(NSTimer *)timer;

-(void)displayNextCMSampleBuffer:(NSTimer *)timer;



/* Seek to closest keyframe near specified time */
-(void)seekTime:(double)seconds;



@end
