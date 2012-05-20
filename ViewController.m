//
//  SDLviewController.m
//  StreamX
//
//  Created by Michelle on 2/14/10.
//  Copyright 2010 _mooncatventures.com. All rights reserved.
//  Please see specific dual licensing file for gpl/lgl/mozillia  license details
//

#import "ViewController.h"
#import <SDL/SDL.h>



@implementation ViewController

@synthesize tabBar;
@synthesize url;
 
 -(id) initWithNav {
 if ([self init]) {
 //this is the label on the tab button itself
 // set the long name shown in the navigation bar
 //self.navigationItem.title=@"Player";
 }
 return self;
 
 }
 
- (void)viewDidLoad {
    [super viewDidLoad];

	SDLUIKitDelegate *appDelegate = [SDLUIKitDelegate sharedAppDelegate];
	NSString *glString = appDelegate.glInit;
	appDelegate.glInit =@"1";
	NSLog(@"pass stage 1");
	NSMutableDictionary *parms = [[NSMutableDictionary alloc] init];
	[parms setObject: url forKey: @"url"];
	NSLog(@"set url");
    [parms setObject: glString forKey: @"glflag" ];
	NSLog(@"all objects set");
	[appDelegate postProcessing:parms];
		 
}


- (void)setUrl:(NSString*)thisUrl {
	url=thisUrl;
	
}


-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}


- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
		NSLog(@"exit sdviewControler unload");
	
	
}
-(void) viewDidDisappear:(BOOL)animated {
	[super viewDidDisappear:NO];
	NSLog(@"exit sdviewControler");
	
	}


- (void)dealloc {
    [super dealloc];
}


@end
