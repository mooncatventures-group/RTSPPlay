
#import <FFPlayer/SDL_uikitappdelegate.h>
#import <SDL/SDL_uikitopenglview.h>
#import <SDL/SDL_events_c.h>
#import "ViewController.h"
#import <SDL/jumphack.h>

#ifdef main
#undef main
#endif
UIWindow *nativeWindow;
extern int SDL_main(int argc, char *argv[]);
static int forward_argc;
static char **forward_argv;




int main(int argc, char **argv) {
@autoreleasepool {
	int i;
		forward_argc = argc;
	forward_argv = (char **)malloc(argc * sizeof(char *));
	for (i=0; i<argc; i++) {
		forward_argv[i] = malloc( (strlen(argv[i])+1) * sizeof(char));
		strcpy(forward_argv[i], argv[i]);
	}

	/* Give over control to run loop, SDLUIKitDelegate will handle most things from here */
	return UIApplicationMain(argc, argv, nil, @"SDLUIKitDelegate");
	
}
	
}

@implementation SDLUIKitDelegate

@synthesize window;
@synthesize windowID;
@synthesize myWindow;
@synthesize navigationController;
@synthesize tabBarController;
@synthesize glInit;


/* convenience method */
+(SDLUIKitDelegate *)sharedAppDelegate {
	/* the delegate is set in UIApplicationMain(), which is garaunteed to be called before this method */
	return (SDLUIKitDelegate *)[[UIApplication sharedApplication] delegate];
}

- (id)init {
	self = [super init];
	
	window = nil;
	windowID = 0;
	return self;
}


- (void)postFinishLaunch:(NSDictionary*)parms {

    NSLog(@"ready to launch");
	
	
	
	NSString *videoPath = [parms objectForKey:@"url"];
	NSLog(@"video path %@\n",  videoPath);
	const char *cString = [videoPath UTF8String];
	////(" ","the printf value is %s\n ", cString);

	
	//NSString *glflag = [parms objectForKey:@"glflag"];
	//NSLog(@"post finish url  %@",filename);
	//NSLog(@"post finish url  %@",glflag);
	/* run the user's application, passing argc and argv */
	forward_argc=1;
	
	
	forward_argv[1] = "-fs";
	forward_argv[2] = "-skipframe";
	forward_argv[3] = "30";
	forward_argv[4] = "-fast";
	forward_argv[5] = "-sync";
	forward_argv[6] = "video";
	forward_argv[7] = "-drp";
	forward_argv[8] = "-skipidct";
	forward_argv[9] = "10";
	forward_argv[10] = "-skiploop";
	forward_argv[11] = "50";
	forward_argv[12] = "-threads";
	forward_argv[13] = "5";
	//argv[14] = "-an";
	forward_argv[14] = cString;
	
	NSLog(@"glflag %@\n ",[parms objectForKey:@"glflag"] );
	if ([parms objectForKey:@"glflag"]!=@"1") {
	   forward_argv[15]="0";
	}else {
	forward_argv[15]="1";
	}
	

	forward_argc += 15;
	
	
	int exit_status = SDL_main(forward_argc, forward_argv);
	
	/* free the memory we used to hold copies of argc and argv */
	int i;
	for (i=0; i<forward_argc; i++) {
		free(forward_argv[i]);
	}
	free(forward_argv);	
		
	/* exit, passing the return status from the user's application */
	exit(exit_status);
}


- (void)applicationDidFinishLaunching:(UIApplication *)application {
			
	myWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]]; 
	glInit=@"0";
	
	UINavigationController *localNavigationController;
	
	// set up the window
		
	// create tab bar controller and array to hold the view controllers
	tabBarController = [[UITabBarController alloc] init];
	NSMutableArray *localControllersArray = [[NSMutableArray alloc] initWithCapacity:6];
	
	
		
	
	ViewController *sdlViewController;
    sdlViewController = [[ViewController alloc] initWithNav];
    NSString *videoFilename = @"rtsp://184.72.239.149/vod/mp4:BigBuckBunny_175k.mov";
    
	[sdlViewController setUrl:videoFilename ];

	localNavigationController = [[UINavigationController alloc] initWithRootViewController:sdlViewController];
    
	[localControllersArray addObject:localNavigationController];
	[localNavigationController release];
	//[sdlViewController release];
	
	
	
	// load up our tab bar controller with the view controllers
	//tabBarController.viewControllers = localControllersArray;
	
	// release the array because the tab bar controller now has it
	//[localControllersArray release];
	
	
	// add the tabBarController as a subview in the window
	[myWindow addSubview:localNavigationController.view];
		
	//	[window addSubview:[navigationController view]];
	[myWindow makeKeyAndVisible];
	//uikitWindowToFront(myWindow);
	
	/* Set working directory to resource path */
	[[NSFileManager defaultManager] changeCurrentDirectoryPath: [[NSBundle mainBundle] resourcePath]];
//	NSLog(@"selector runs here");
//	[self performSelector:@selector(postFinishLaunch) withObject:nil
//afterDelay:0.0];
}

- (void) postProcessing:(NSDictionary*)parms {
	
	[self performSelector:@selector(postFinishLaunch:) withObject:parms
			   afterDelay:0.0];
	
}

-(void) switchTopWindow {
	NSLog(@"making uiwindw key window");
	[myWindow makeKeyAndVisible];	
}


-(void)quitSdl {
	SDL_SendQuit();
}

- (void)applicationWillTerminate:(UIApplication *)application {
	
	SDL_SendQuit();
	 /* hack to prevent automatic termination.  See SDL_uikitevents.m for details */
	//longjmp(*(jump_env()), 1);
	
}

- (void) applicationWillResignActive:(UIApplication*)application
{
//	NSLog(@"%@", NSStringFromSelector(_cmd));
	//SDL_SendWindowEvent(self.window, SDL_WINDOWEVENT_MINIMIZED, 0, 0);
}

- (void) applicationDidBecomeActive:(UIApplication*)application
{
//	NSLog(@"%@", NSStringFromSelector(_cmd));
	//SDL_SendWindowEvent(self.window, SDL_WINDOWEVENT_RESTORED, 0, 0);
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    self.tabBarController.selectedIndex = 1;
	// Handle any background procedures not related to animation here.
	SDL_SendQuit();
	/* hack to prevent automatic termination.  See SDL_uikitevents.m for details */
	//longjmp(*(jump_env()), 1);
}




-(void)dealloc {
	[window release];
	[super dealloc];
}

@end
