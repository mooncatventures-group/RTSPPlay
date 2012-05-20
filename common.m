/*
  */
#import <UIKit/UIKit.h>
#include "common.h"
#include <SDL/SDL.h>
#include <stdlib.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include <FFPlayer/SDL_uikitappdelegate.h>
#include <SDL/SDL_uikitopenglview.h>



/*
	Produces a random int x, min <= x <= max 
	following a uniform distribution
*/
int
randomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/*
	Produces a random float x, min <= x <= max 
	following a uniform distribution
 */
float
randomFloat(float min, float max)
{
    return rand() / (float) RAND_MAX *(max - min) + min;
}


void iPhonePumpMessages()
{
	
 ////av_log(" ","runloop return %u\n",CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, TRUE));
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, TRUE);
	}






void
fatalError(const char *string)
{
    ////(" ","%s: %s\n", string, SDL_GetError());
    exit(1);
}

void switchWindows() {
	
	SDLUIKitDelegate *appDelegate = [SDLUIKitDelegate sharedAppDelegate];
	NSLog(@"switch windows");
    [appDelegate switchTopWindow];
}

int eaglSetContext(void) {
    EAGLContext *context
 = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

if (![EAGLContext setCurrentContext:context]) {
	NSLog(@"Error: Failed to set current openGL context in [Player initWithFrame:]");
	[context release];
	return -1;
}
	return 0;
}
