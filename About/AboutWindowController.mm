
#import "AboutWindowController.h"


@implementation AboutWindowController

- (void)awakeFromNib{
  NSString* versionstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
  [version setStringValue:[NSString stringWithFormat:@"%@%@", @"Version ", versionstring]];

  // both are needed, otherwise hyperlink won't accept mousedown
  [websitelink setAllowsEditingTextAttributes: YES];
  [websitelink setSelectable: YES];

  NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString: @"http://manderim.ch"];
  NSRange range = NSMakeRange(0, [attrString length]);

  NSURL* url = [NSURL URLWithString:@"http://manderim.ch"];
  [attrString beginEditing];
  [attrString addAttribute:NSLinkAttributeName value:[url absoluteString] range:range];
  [attrString addAttribute:NSUnderlineStyleAttributeName value:[NSNumber numberWithInt:NSSingleUnderlineStyle] range:range];
  [attrString addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:range];
  [attrString endEditing];

  [websitelink setAttributedStringValue: attrString];
  [attrString release];


  // both are needed, otherwise hyperlink won't accept mousedown
  [manderclink setAllowsEditingTextAttributes: YES];
  [manderclink setSelectable: YES];

  attrString = [[NSMutableAttributedString alloc] initWithString: @"Inspired by ManderC"];
  range = NSMakeRange(12, 7);

  url = [NSURL URLWithString:@"http://manderc.manderby.com/concepts/umrechner/index.php"];
  [attrString beginEditing];
  [attrString addAttribute:NSLinkAttributeName value:[url absoluteString] range:range];
  [attrString addAttribute:NSUnderlineStyleAttributeName value:[NSNumber numberWithInt:NSSingleUnderlineStyle] range:range];
  [attrString addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:range];
  [attrString endEditing];

  [manderclink setAttributedStringValue: attrString];
  [attrString release];
}

- (void)showDialog{
  [[self window] makeKeyAndOrderFront:self];
}

//- (IBAction)pressDone:(NSButton*)sender{
//  [[self window] performClose:sender];
//}

- (IBAction)goToWebsite:(id)sender{
//  [websitebutton setTransparent:NO];
//  [[self window] performClose:sender];
  if(sender == websitelink){
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://manderim.ch"]];
  }else if(sender == manderclink){
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://manderc.manderby.com/concepts/umrechner/index.php"]];
  }
}

@end




