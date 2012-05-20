//
//  SDLviewController.h
//  StreamX
//
//  Created by Michelle on 2/14/10.
//  Copyright 2010 _mooncatventures.com. All rights reserved.
//  Please see specific dual licensing file for gpl/lgl/mozillia  license details

//

#import <UIKit/UIKit.h>
#import "SDL_uikitappdelegate.h"

@interface ViewController : UIViewController {
IBOutlet UITabBar *tabBar;
NSString *url;



}

@property (nonatomic, retain) UITabBar *tabBar;
@property (nonatomic, retain) NSString *url;
-(id)initWithNav;
- (void)setUrl:(NSString*)thisUrl;
@end
