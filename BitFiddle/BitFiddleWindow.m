
#import "BitFiddleWindow.h"
#import "BitFiddleApplication.h"

@implementation BitFiddleWindow

- (void) zoom:(id)sender{
  [(BitFiddleApplication*)NSApp switchMini:sender];
}

@end



@implementation BitFiddlePanel

- (void) zoom:(id)sender{
  [(BitFiddleApplication*)NSApp switchMini:sender];
}

@end
