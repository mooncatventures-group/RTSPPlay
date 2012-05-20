#import <UIKit/UIKit.h>
#import <SDL/SDL_uikitopenglview.h>
#import <SDL/SDL_video.h>

/* *INDENT-OFF* */
@interface SDLUIKitDelegate:NSObject<UIApplicationDelegate> {
      UIWindow *window;
	  SDL_WindowID windowID;
	  UIWindow *myWindow;
	  UITabBarController *tabBarController;
      UINavigationController *navigationController;
	  NSString *glInit;

}


@property (readwrite, retain) UIWindow *window;
@property (readwrite, assign) SDL_WindowID windowID;
@property (readwrite, retain) UIWindow *myWindow;
@property (nonatomic, retain) UITabBarController *tabBarController;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;
@property (nonatomic, retain) NSString *glInit;

- (void)postFinishLaunch:(NSDictionary*)parms;
-(void)postProcessing:(NSDictionary*) parms;
-(void) quitSdl;
-(void) switchTopWindow;

+(SDLUIKitDelegate *)sharedAppDelegate;

@end
/* *INDENT-ON* */
