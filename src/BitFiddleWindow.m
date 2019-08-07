
#import "BitFiddleWindow.h"
#import "BitFiddleNSApplication.h"

@implementation BitFiddleWindow

- (void) zoom:(id)sender{
  [(BitFiddleNSApplication*)NSApp switchMini:sender];
}

@end



@implementation BitFiddlePanel

- (void) zoom:(id)sender{
  [(BitFiddleNSApplication*)NSApp switchMini:sender];
}

@end
