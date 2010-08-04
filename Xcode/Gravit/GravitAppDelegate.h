//
//  GravitAppDelegate.h
//  Gravit
//
//  Created by Gerald Kaszuba on 5/08/10.
//  Copyright 2010 Slowchop. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface GravitAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
