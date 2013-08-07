

#import <Cocoa/Cocoa.h>
#include "NABitArray.h"
#import "NumberOutputField.h"

typedef enum{
  COMPUTE_UNSIGNED = 0,
  COMPUTE_ONES_COMPLEMENT,
  COMPUTE_TWOS_COMPLEMENT,
} ConversionType;

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
  IBOutlet NumberOutputField* outchr8;
  IBOutlet NumberOutputField* outchr16;
  IBOutlet NumberOutputField* outchr32;
  IBOutlet NumberOutputField* outchr64;
  IBOutlet NumberOutputField* outchrn;
  IBOutlet NSSegmentedControl* segcontrol;
  IBOutlet NSButton* byteswapcheckbox;
  IBOutlet NSMenuItem* byteswapmenuitem;
  IBOutlet NSMenuItem* unsignedmenuitem;
  IBOutlet NSMenuItem* onescomplementmenuitem;
  IBOutlet NSMenuItem* twoscomplementmenuitem;
  NABitArray bitarray;
  NABitArray bitarray8;
  NABitArray bitarray16;
  NABitArray bitarray32;
  NABitArray bitarray64;
  NABitArray bitarrayn;
  NAByteArray bytearraychr8;
  NAByteArray bytearraychr16;
  NAByteArray bytearraychr32;
  NAByteArray bytearraychr64;
  NAByteArray bytearraychrn;
  NAString  chrstr8;
  NAString  chrstr16;
  NAString  chrstr32;
  NAString  chrstr64;
  NAString  chrstrn;
}

- (void)awakeFromNib;
- (void)showDialog;
//- (BOOL)windowShouldClose:(id)sender;
- (void)update;


//- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize;

- (void)controlTextDidChange:(NSNotification *)note;

- (void)valueChangeDec:(NSControl*)sender;
- (void)valueChangeHex:(NSControl*)sender;
- (void)valueChangeBin:(NSControl*)sender;
- (void)valueChangeAsc:(NSControl*)sender;

// UI elements
- (IBAction)byteswapChange:(NSControl*)sender;
- (IBAction)segmentControlChange:(NSControl*)sender;

// Menu commands
- (IBAction)switchByteSwap:(id)sender;
- (IBAction)switchToUnsigned:(id)sender;
- (IBAction)switchToOnesComplement:(id)sender;
- (IBAction)switchToTwosComplement:(id)sender;

@end
