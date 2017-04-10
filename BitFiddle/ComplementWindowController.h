

#import <Cocoa/Cocoa.h>
#include "BitArray.h"
#import "NumberOutputField.h"


@interface ComplementWindowController : NSWindowController <NSWindowDelegate>{
  IBOutlet NSTextField* indec;
  IBOutlet NSTextField* inhex;
  IBOutlet NSTextField* inbin;
  IBOutlet NSTextField* inasc;
  IBOutlet NumberOutputField* outdec8;
  IBOutlet NumberOutputField* outdec16;
  IBOutlet NumberOutputField* outdec32;
  IBOutlet NumberOutputField* outdec64;
  IBOutlet NumberOutputField* outdecn;
  IBOutlet NumberOutputField* outhex8;
  IBOutlet NumberOutputField* outhex16;
  IBOutlet NumberOutputField* outhex32;
  IBOutlet NumberOutputField* outhex64;
  IBOutlet NumberOutputField* outhexn;
  IBOutlet NumberOutputField* outbin8;
  IBOutlet NumberOutputField* outbin16;
  IBOutlet NumberOutputField* outbin32;
  IBOutlet NumberOutputField* outbin64;
  IBOutlet NumberOutputField* outbinn;
  IBOutlet NumberOutputField* outasc8;
  IBOutlet NumberOutputField* outasc16;
  IBOutlet NumberOutputField* outasc32;
  IBOutlet NumberOutputField* outasc64;
  IBOutlet NumberOutputField* outascn;
  IBOutlet NSSegmentedControl* segcontrol;
  IBOutlet NSButton* byteswapcheckbox;
  NABuffer* bitarray;
  
  NABool isMini;
}

- (void)awakeFromNib;
- (void)setMini:(NABool)ismini;
- (void)showDialog;
- (void)hideDialog;
- (void)update;


//- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize;

- (void)controlTextDidChange:(NSNotification *)note;

- (void)valueChangeDec:(NSControl*)sender;
- (void)valueChangeHex:(NSControl*)sender;
- (void)valueChangeBin:(NSControl*)sender;
- (void)valueChangeAsc:(NSControl*)sender;

- (void)resetValue;


// UI elements
//- (IBAction)byteswapChange:(NSControl*)sender;
//- (IBAction)segmentControlChange:(NSControl*)sender;

// Menu commands
//- (IBAction)switchByteSwap:(id)sender;
//- (IBAction)switchToUnsigned:(id)sender;
//- (IBAction)switchToOnesComplement:(id)sender;
//- (IBAction)switchToTwosComplement:(id)sender;

@end
