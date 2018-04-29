

#import "ASCIIWindowController.h"
#include "MandEnvironment.h"

#define SETHOVER(guielem, integer) \
  [[[self window] contentView] addTrackingArea:[[[NSTrackingArea alloc] initWithRect:[guielem frame] options:NSTrackingActiveInKeyWindow | NSTrackingMouseEnteredAndExited owner:self userInfo:[NSDictionary dictionaryWithObject:[NSNumber numberWithInteger:integer] forKey:@"ord"]] autorelease]];



const char* codes[128] = {
  "NUL",
  "SOH",
  "STX",
  "ETX",
  "EOT",
  "ENQ",
  "ACK",
  "BEL",
  "BS",
  "HT",
  "LF",
  "VT",
  "FF",
  "CR",
  "SO",
  "SI",
  
  "DLE",
  "DC1",
  "DC2",
  "DC3",
  "DC4",
  "NAK",
  "SYN",
  "ETB",
  "CAN",
  "EM",
  "SUB",
  "ESC",
  "FS",
  "GS",
  "RS",
  "US",
  
  "SPC",
  "!",
  "\\\"",
  "#",
  "$",
  "%",
  "&",
  "\\\'",
  "(",
  ")",
  "*",
  "+",
  ",",
  "-",
  ".",
  "/",

  
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7'",
  "8",
  "9",
  ":",
  ";",
  "<",
  "=",
  ">",
  "?",

  "@",
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G'",
  "H",
  "I",
  "J",
  "K",
  "L",
  "M",
  "N",
  "O",

  "P",
  "Q",
  "R",
  "S",
  "T",
  "U",
  "V",
  "W'",
  "X",
  "Y",
  "Z",
  "[",
  "\\\\",
  "]",
  "^",
  "_",

  "`",
  "a",
  "b",
  "c",
  "d",
  "e",
  "f",
  "g'",
  "h",
  "i",
  "j",
  "k",
  "l",
  "m",
  "n",
  "o",

  "p",
  "q",
  "r",
  "s",
  "t",
  "u",
  "v",
  "w'",
  "x",
  "y",
  "z",
  "{",
  "|",
  "}",
  "~",
  "DEL",
};

const char* unicodenames[128] = {
  "NULL",
  "START OF HEADING",
  "START OF TEXT",
  "END OF TEXT",
  "END OF TRANSMISSION",
  "ENQUIRY",
  "ACKNOWLEDGE",
  "BELL",
  "BACKSPACE",
  "HORIZONTAL TABULATION",
  "LINE FEED",
  "VERTICAL TABULATION",
  "FORM FEED",
  "CARRIAGE RETURN",
  "SHIFT OUT",
  "SHIFT IN",

  "DATA LINK ESCAPE",
  "DEVICE CONTROL ONE",
  "DEVICE CONTROL TWO",
  "DEVICE CONTROL THREE",
  "DEVICE CONTROL FOUR",
  "NEGATIVE ACKNOWLEDGE",
  "SYNCHRONOUS IDLE",
  "END OF TRANSMISSION BLOCK",
  "CANCEL",
  "END OF MEDIUM",
  "SUBSTITUTE",
  "ESCAPE",
  "FILE SEPARATOR",
  "GROUP SEPARATOR",
  "RECORD SEPARATOR",
  "UNIT SEPARATOR",

  "SPACE",
  "EXCLAMATION MARK",
  "QUOTATION MARK",
  "NUMBER SIGN",
  "DOLLAR SIGN",
  "PERCENT SIGN",
  "AMPERSAND",
  "APOSTROPHE",
  "LEFT PARENTHESIS",
  "RIGHT PARENTHESIS",
  "ASTERISK",
  "PLUS SIGN",
  "COMMA",
  "HYPHEN-MINUS",
  "FULL STOP",
  "SOLIDUS",

  "DIGIT ZERO",
  "DIGIT ONE",
  "DIGIT TWO",
  "DIGIT THREE",
  "DIGIT FOUR",
  "DIGIT FIVE",
  "DIGIT SIX",
  "DIGIT SEVEN",
  "DIGIT EIGHT",
  "DIGIT NINE",
  "COLON",
  "SEMICOLON",
  "LESS-THAN SIGN",
  "EQUALS SIGN",
  "GREATER-THAN SIGN",
  "QUESTION MARK",

  "COMMERCIAL AT",
  "LATIN CAPITAL LETTER A",
  "LATIN CAPITAL LETTER B",
  "LATIN CAPITAL LETTER C",
  "LATIN CAPITAL LETTER D",
  "LATIN CAPITAL LETTER E",
  "LATIN CAPITAL LETTER F",
  "LATIN CAPITAL LETTER G",
  "LATIN CAPITAL LETTER H",
  "LATIN CAPITAL LETTER I",
  "LATIN CAPITAL LETTER J",
  "LATIN CAPITAL LETTER K",
  "LATIN CAPITAL LETTER L",
  "LATIN CAPITAL LETTER M",
  "LATIN CAPITAL LETTER N",
  "LATIN CAPITAL LETTER O",

  "LATIN CAPITAL LETTER P",
  "LATIN CAPITAL LETTER Q",
  "LATIN CAPITAL LETTER R",
  "LATIN CAPITAL LETTER S",
  "LATIN CAPITAL LETTER T",
  "LATIN CAPITAL LETTER U",
  "LATIN CAPITAL LETTER V",
  "LATIN CAPITAL LETTER W",
  "LATIN CAPITAL LETTER X",
  "LATIN CAPITAL LETTER Y",
  "LATIN CAPITAL LETTER Z",
  "LEFT SQUARE BRACKET",
  "REVERSE SOLIDUS",
  "RIGHT SQUARE BRACKET",
  "CIRCUMFLEX ACCENT",
  "LOW LINE",

  "GRAVE ACCENT",
  "LATIN SMALL LETTER A",
  "LATIN SMALL LETTER B",
  "LATIN SMALL LETTER C",
  "LATIN SMALL LETTER D",
  "LATIN SMALL LETTER E",
  "LATIN SMALL LETTER F",
  "LATIN SMALL LETTER G",
  "LATIN SMALL LETTER H",
  "LATIN SMALL LETTER I",
  "LATIN SMALL LETTER J",
  "LATIN SMALL LETTER K",
  "LATIN SMALL LETTER L",
  "LATIN SMALL LETTER M",
  "LATIN SMALL LETTER N",
  "LATIN SMALL LETTER O",

  "LATIN SMALL LETTER P",
  "LATIN SMALL LETTER Q",
  "LATIN SMALL LETTER R",
  "LATIN SMALL LETTER S",
  "LATIN SMALL LETTER T",
  "LATIN SMALL LETTER U",
  "LATIN SMALL LETTER V",
  "LATIN SMALL LETTER W",
  "LATIN SMALL LETTER X",
  "LATIN SMALL LETTER Y",
  "LATIN SMALL LETTER Z",
  "LEFT CURLY BRACKET",
  "VERTICAL LINE",
  "RIGHT CURLY BRACKET",
  "TILDE",
  "DELETE",
};

const char* addinfo[128] = {
  "C-escape: \\0",
  "",
  "",
  "Terminal ctrl-C: Abort",
  "",
  "",
  "",
  "Beeps on some systems",
  "C-escape: \\b",
  "C-escape: \\t",
  "C-escape: \\n\nLine ending Unix LF Windows CR-LF",
  "C-escape: \\v",
  "C-escape: \\f",
  "C-escape: \\r\nLine ending Windows CR-LF old MacOS CR",
  "",
  "",

  "",
  "Code sometimes also XON",
  "",
  "Code sometimes also XOFF",
  "",
  "Terminal ctrl-U: Delete line",
  "",
  "Terminal ctrl-W: Delete Word",
  "",
  "",
  "Terminal ctrl-Z: Suspend",
  "C-escape on some compilers: \\e",
  "",
  "",
  "",
  "",

  "Code sometimes also SP",
  "",
  "C-escape: \\\"\nHTML-escape: &quot;",
  "Latex-escape: \\#\nCasually called \"Hash\"",
  "Latex-escape: \\$",
  "Latex-escape: \\%",
  "HTML-escape: &amp;\nLatex-escape: \\&",
  "C-escape:\\\'\nHTML-escape:&#39;",
  "",
  "",
  "Casually called \"Multiplication\"",
  "",
  "",
  "",
  "Casually called \"Dot\"",
  "Casually called \"Slash\" or \"Division\"",

  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "HTML-escape: &lt;",
  "",
  "HTML-escape: &gt;",
  "C-escape: \\\? (when using trigraphs)",

  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "The authors favourite letter",
  "",
  "",

  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "C-escape: \\\\\nCasually called \"Backslash\"",
  "",
  "Latex-escape: \\^\nCasually called \"Caret\" or \"Power\"",
  "Latex-escape: \\_\nCasually called \"Underscore\"",

  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",

  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "Latex-escape: \\{",
  "Casually called \"Bar\" or \"Pipe\"\nSometimes with or without gap (broken bar)",
  "Latex-escape: \\}",
  "",
  "",
};




@implementation ASCIIWindowController

-(void)awakeFromNib{

  [[self window] setDelegate:self];

  NSInteger segmentindex;
  segmentindex = mandGetUserDefaultInt("asciinumbersystem");
  [numbercontrol setSelectedSegment:segmentindex];
  segmentindex = mandGetUserDefaultInt("asciicodeswitch");
  [codecontrol setSelectedSegment:segmentindex];


  currenthighlight = -1;
  [comment1 setStringValue:@""];
  [comment2 setStringValue:@""];

  SETHOVER(chr00, 0x00);
  SETHOVER(chr01, 0x01);
  SETHOVER(chr02, 0x02);
  SETHOVER(chr03, 0x03);
  SETHOVER(chr04, 0x04);
  SETHOVER(chr05, 0x05);
  SETHOVER(chr06, 0x06);
  SETHOVER(chr07, 0x07);
  SETHOVER(chr08, 0x08);
  SETHOVER(chr09, 0x09);
  SETHOVER(chr0a, 0x0a);
  SETHOVER(chr0b, 0x0b);
  SETHOVER(chr0c, 0x0c);
  SETHOVER(chr0d, 0x0d);
  SETHOVER(chr0e, 0x0e);
  SETHOVER(chr0f, 0x0f);

  SETHOVER(chr10, 0x10);
  SETHOVER(chr11, 0x11);
  SETHOVER(chr12, 0x12);
  SETHOVER(chr13, 0x13);
  SETHOVER(chr14, 0x14);
  SETHOVER(chr15, 0x15);
  SETHOVER(chr16, 0x16);
  SETHOVER(chr17, 0x17);
  SETHOVER(chr18, 0x18);
  SETHOVER(chr19, 0x19);
  SETHOVER(chr1a, 0x1a);
  SETHOVER(chr1b, 0x1b);
  SETHOVER(chr1c, 0x1c);
  SETHOVER(chr1d, 0x1d);
  SETHOVER(chr1e, 0x1e);
  SETHOVER(chr1f, 0x1f);

  SETHOVER(chr20, 0x20);
  SETHOVER(chr21, 0x21);
  SETHOVER(chr22, 0x22);
  SETHOVER(chr23, 0x23);
  SETHOVER(chr24, 0x24);
  SETHOVER(chr25, 0x25);
  SETHOVER(chr26, 0x26);
  SETHOVER(chr27, 0x27);
  SETHOVER(chr28, 0x28);
  SETHOVER(chr29, 0x29);
  SETHOVER(chr2a, 0x2a);
  SETHOVER(chr2b, 0x2b);
  SETHOVER(chr2c, 0x2c);
  SETHOVER(chr2d, 0x2d);
  SETHOVER(chr2e, 0x2e);
  SETHOVER(chr2f, 0x2f);

  SETHOVER(chr30, 0x30);
  SETHOVER(chr31, 0x31);
  SETHOVER(chr32, 0x32);
  SETHOVER(chr33, 0x33);
  SETHOVER(chr34, 0x34);
  SETHOVER(chr35, 0x35);
  SETHOVER(chr36, 0x36);
  SETHOVER(chr37, 0x37);
  SETHOVER(chr38, 0x38);
  SETHOVER(chr39, 0x39);
  SETHOVER(chr3a, 0x3a);
  SETHOVER(chr3b, 0x3b);
  SETHOVER(chr3c, 0x3c);
  SETHOVER(chr3d, 0x3d);
  SETHOVER(chr3e, 0x3e);
  SETHOVER(chr3f, 0x3f);

  SETHOVER(chr40, 0x40);
  SETHOVER(chr41, 0x41);
  SETHOVER(chr42, 0x42);
  SETHOVER(chr43, 0x43);
  SETHOVER(chr44, 0x44);
  SETHOVER(chr45, 0x45);
  SETHOVER(chr46, 0x46);
  SETHOVER(chr47, 0x47);
  SETHOVER(chr48, 0x48);
  SETHOVER(chr49, 0x49);
  SETHOVER(chr4a, 0x4a);
  SETHOVER(chr4b, 0x4b);
  SETHOVER(chr4c, 0x4c);
  SETHOVER(chr4d, 0x4d);
  SETHOVER(chr4e, 0x4e);
  SETHOVER(chr4f, 0x4f);

  SETHOVER(chr50, 0x50);
  SETHOVER(chr51, 0x51);
  SETHOVER(chr52, 0x52);
  SETHOVER(chr53, 0x53);
  SETHOVER(chr54, 0x54);
  SETHOVER(chr55, 0x55);
  SETHOVER(chr56, 0x56);
  SETHOVER(chr57, 0x57);
  SETHOVER(chr58, 0x58);
  SETHOVER(chr59, 0x59);
  SETHOVER(chr5a, 0x5a);
  SETHOVER(chr5b, 0x5b);
  SETHOVER(chr5c, 0x5c);
  SETHOVER(chr5d, 0x5d);
  SETHOVER(chr5e, 0x5e);
  SETHOVER(chr5f, 0x5f);

  SETHOVER(chr60, 0x60);
  SETHOVER(chr61, 0x61);
  SETHOVER(chr62, 0x62);
  SETHOVER(chr63, 0x63);
  SETHOVER(chr64, 0x64);
  SETHOVER(chr65, 0x65);
  SETHOVER(chr66, 0x66);
  SETHOVER(chr67, 0x67);
  SETHOVER(chr68, 0x68);
  SETHOVER(chr69, 0x69);
  SETHOVER(chr6a, 0x6a);
  SETHOVER(chr6b, 0x6b);
  SETHOVER(chr6c, 0x6c);
  SETHOVER(chr6d, 0x6d);
  SETHOVER(chr6e, 0x6e);
  SETHOVER(chr6f, 0x6f);

  SETHOVER(chr70, 0x70);
  SETHOVER(chr71, 0x71);
  SETHOVER(chr72, 0x72);
  SETHOVER(chr73, 0x73);
  SETHOVER(chr74, 0x74);
  SETHOVER(chr75, 0x75);
  SETHOVER(chr76, 0x76);
  SETHOVER(chr77, 0x77);
  SETHOVER(chr78, 0x78);
  SETHOVER(chr79, 0x79);
  SETHOVER(chr7a, 0x7a);
  SETHOVER(chr7b, 0x7b);
  SETHOVER(chr7c, 0x7c);
  SETHOVER(chr7d, 0x7d);
  SETHOVER(chr7e, 0x7e);
  SETHOVER(chr7f, 0x7f);

  SETHOVER(asc00, 0x00);
  SETHOVER(asc01, 0x01);
  SETHOVER(asc02, 0x02);
  SETHOVER(asc03, 0x03);
  SETHOVER(asc04, 0x04);
  SETHOVER(asc05, 0x05);
  SETHOVER(asc06, 0x06);
  SETHOVER(asc07, 0x07);
  SETHOVER(asc08, 0x08);
  SETHOVER(asc09, 0x09);
  SETHOVER(asc0a, 0x0a);
  SETHOVER(asc0b, 0x0b);
  SETHOVER(asc0c, 0x0c);
  SETHOVER(asc0d, 0x0d);
  SETHOVER(asc0e, 0x0e);
  SETHOVER(asc0f, 0x0f);

  SETHOVER(asc10, 0x10);
  SETHOVER(asc11, 0x11);
  SETHOVER(asc12, 0x12);
  SETHOVER(asc13, 0x13);
  SETHOVER(asc14, 0x14);
  SETHOVER(asc15, 0x15);
  SETHOVER(asc16, 0x16);
  SETHOVER(asc17, 0x17);
  SETHOVER(asc18, 0x18);
  SETHOVER(asc19, 0x19);
  SETHOVER(asc1a, 0x1a);
  SETHOVER(asc1b, 0x1b);
  SETHOVER(asc1c, 0x1c);
  SETHOVER(asc1d, 0x1d);
  SETHOVER(asc1e, 0x1e);
  SETHOVER(asc1f, 0x1f);

  SETHOVER(asc20, 0x20);
  SETHOVER(asc21, 0x21);
  SETHOVER(asc22, 0x22);
  SETHOVER(asc23, 0x23);
  SETHOVER(asc24, 0x24);
  SETHOVER(asc25, 0x25);
  SETHOVER(asc26, 0x26);
  SETHOVER(asc27, 0x27);
  SETHOVER(asc28, 0x28);
  SETHOVER(asc29, 0x29);
  SETHOVER(asc2a, 0x2a);
  SETHOVER(asc2b, 0x2b);
  SETHOVER(asc2c, 0x2c);
  SETHOVER(asc2d, 0x2d);
  SETHOVER(asc2e, 0x2e);
  SETHOVER(asc2f, 0x2f);

  SETHOVER(asc30, 0x30);
  SETHOVER(asc31, 0x31);
  SETHOVER(asc32, 0x32);
  SETHOVER(asc33, 0x33);
  SETHOVER(asc34, 0x34);
  SETHOVER(asc35, 0x35);
  SETHOVER(asc36, 0x36);
  SETHOVER(asc37, 0x37);
  SETHOVER(asc38, 0x38);
  SETHOVER(asc39, 0x39);
  SETHOVER(asc3a, 0x3a);
  SETHOVER(asc3b, 0x3b);
  SETHOVER(asc3c, 0x3c);
  SETHOVER(asc3d, 0x3d);
  SETHOVER(asc3e, 0x3e);
  SETHOVER(asc3f, 0x3f);

  SETHOVER(asc40, 0x40);
  SETHOVER(asc41, 0x41);
  SETHOVER(asc42, 0x42);
  SETHOVER(asc43, 0x43);
  SETHOVER(asc44, 0x44);
  SETHOVER(asc45, 0x45);
  SETHOVER(asc46, 0x46);
  SETHOVER(asc47, 0x47);
  SETHOVER(asc48, 0x48);
  SETHOVER(asc49, 0x49);
  SETHOVER(asc4a, 0x4a);
  SETHOVER(asc4b, 0x4b);
  SETHOVER(asc4c, 0x4c);
  SETHOVER(asc4d, 0x4d);
  SETHOVER(asc4e, 0x4e);
  SETHOVER(asc4f, 0x4f);

  SETHOVER(asc50, 0x50);
  SETHOVER(asc51, 0x51);
  SETHOVER(asc52, 0x52);
  SETHOVER(asc53, 0x53);
  SETHOVER(asc54, 0x54);
  SETHOVER(asc55, 0x55);
  SETHOVER(asc56, 0x56);
  SETHOVER(asc57, 0x57);
  SETHOVER(asc58, 0x58);
  SETHOVER(asc59, 0x59);
  SETHOVER(asc5a, 0x5a);
  SETHOVER(asc5b, 0x5b);
  SETHOVER(asc5c, 0x5c);
  SETHOVER(asc5d, 0x5d);
  SETHOVER(asc5e, 0x5e);
  SETHOVER(asc5f, 0x5f);

  SETHOVER(asc60, 0x60);
  SETHOVER(asc61, 0x61);
  SETHOVER(asc62, 0x62);
  SETHOVER(asc63, 0x63);
  SETHOVER(asc64, 0x64);
  SETHOVER(asc65, 0x65);
  SETHOVER(asc66, 0x66);
  SETHOVER(asc67, 0x67);
  SETHOVER(asc68, 0x68);
  SETHOVER(asc69, 0x69);
  SETHOVER(asc6a, 0x6a);
  SETHOVER(asc6b, 0x6b);
  SETHOVER(asc6c, 0x6c);
  SETHOVER(asc6d, 0x6d);
  SETHOVER(asc6e, 0x6e);
  SETHOVER(asc6f, 0x6f);

  SETHOVER(asc70, 0x70);
  SETHOVER(asc71, 0x71);
  SETHOVER(asc72, 0x72);
  SETHOVER(asc73, 0x73);
  SETHOVER(asc74, 0x74);
  SETHOVER(asc75, 0x75);
  SETHOVER(asc76, 0x76);
  SETHOVER(asc77, 0x77);
  SETHOVER(asc78, 0x78);
  SETHOVER(asc79, 0x79);
  SETHOVER(asc7a, 0x7a);
  SETHOVER(asc7b, 0x7b);
  SETHOVER(asc7c, 0x7c);
  SETHOVER(asc7d, 0x7d);
  SETHOVER(asc7e, 0x7e);
  SETHOVER(asc7f, 0x7f);
  
  [self update];
}

- (void)showDialog{
  [[self window] makeKeyAndOrderFront:self];
}

-(void)update{

  mandSetUserDefaultInt([numbercontrol selectedSegment], "asciinumbersystem");

  if([numbercontrol selectedSegment] == 0){
    [asc00 setStringValue:@"00"];
    [asc01 setStringValue:@"01"];
    [asc02 setStringValue:@"02"];
    [asc03 setStringValue:@"03"];
    [asc04 setStringValue:@"04"];
    [asc05 setStringValue:@"05"];
    [asc06 setStringValue:@"06"];
    [asc07 setStringValue:@"07"];
    [asc08 setStringValue:@"08"];
    [asc09 setStringValue:@"09"];
    [asc0a setStringValue:@"0a"];
    [asc0b setStringValue:@"0b"];
    [asc0c setStringValue:@"0c"];
    [asc0d setStringValue:@"0d"];
    [asc0e setStringValue:@"0e"];
    [asc0f setStringValue:@"0f"];
    [asc10 setStringValue:@"10"];
    [asc11 setStringValue:@"11"];
    [asc12 setStringValue:@"12"];
    [asc13 setStringValue:@"13"];
    [asc14 setStringValue:@"14"];
    [asc15 setStringValue:@"15"];
    [asc16 setStringValue:@"16"];
    [asc17 setStringValue:@"17"];
    [asc18 setStringValue:@"18"];
    [asc19 setStringValue:@"19"];
    [asc1a setStringValue:@"1a"];
    [asc1b setStringValue:@"1b"];
    [asc1c setStringValue:@"1c"];
    [asc1d setStringValue:@"1d"];
    [asc1e setStringValue:@"1e"];
    [asc1f setStringValue:@"1f"];
    [asc20 setStringValue:@"20"];
    [asc21 setStringValue:@"21"];
    [asc22 setStringValue:@"22"];
    [asc23 setStringValue:@"23"];
    [asc24 setStringValue:@"24"];
    [asc25 setStringValue:@"25"];
    [asc26 setStringValue:@"26"];
    [asc27 setStringValue:@"27"];
    [asc28 setStringValue:@"28"];
    [asc29 setStringValue:@"29"];
    [asc2a setStringValue:@"2a"];
    [asc2b setStringValue:@"2b"];
    [asc2c setStringValue:@"2c"];
    [asc2d setStringValue:@"2d"];
    [asc2e setStringValue:@"2e"];
    [asc2f setStringValue:@"2f"];
    [asc30 setStringValue:@"30"];
    [asc31 setStringValue:@"31"];
    [asc32 setStringValue:@"32"];
    [asc33 setStringValue:@"33"];
    [asc34 setStringValue:@"34"];
    [asc35 setStringValue:@"35"];
    [asc36 setStringValue:@"36"];
    [asc37 setStringValue:@"37"];
    [asc38 setStringValue:@"38"];
    [asc39 setStringValue:@"39"];
    [asc3a setStringValue:@"3a"];
    [asc3b setStringValue:@"3b"];
    [asc3c setStringValue:@"3c"];
    [asc3d setStringValue:@"3d"];
    [asc3e setStringValue:@"3e"];
    [asc3f setStringValue:@"3f"];
    [asc40 setStringValue:@"40"];
    [asc41 setStringValue:@"41"];
    [asc42 setStringValue:@"42"];
    [asc43 setStringValue:@"43"];
    [asc44 setStringValue:@"44"];
    [asc45 setStringValue:@"45"];
    [asc46 setStringValue:@"46"];
    [asc47 setStringValue:@"47"];
    [asc48 setStringValue:@"48"];
    [asc49 setStringValue:@"49"];
    [asc4a setStringValue:@"4a"];
    [asc4b setStringValue:@"4b"];
    [asc4c setStringValue:@"4c"];
    [asc4d setStringValue:@"4d"];
    [asc4e setStringValue:@"4e"];
    [asc4f setStringValue:@"4f"];
    [asc50 setStringValue:@"50"];
    [asc51 setStringValue:@"51"];
    [asc52 setStringValue:@"52"];
    [asc53 setStringValue:@"53"];
    [asc54 setStringValue:@"54"];
    [asc55 setStringValue:@"55"];
    [asc56 setStringValue:@"56"];
    [asc57 setStringValue:@"57"];
    [asc58 setStringValue:@"58"];
    [asc59 setStringValue:@"59"];
    [asc5a setStringValue:@"5a"];
    [asc5b setStringValue:@"5b"];
    [asc5c setStringValue:@"5c"];
    [asc5d setStringValue:@"5d"];
    [asc5e setStringValue:@"5e"];
    [asc5f setStringValue:@"5f"];
    [asc60 setStringValue:@"60"];
    [asc61 setStringValue:@"61"];
    [asc62 setStringValue:@"62"];
    [asc63 setStringValue:@"63"];
    [asc64 setStringValue:@"64"];
    [asc65 setStringValue:@"65"];
    [asc66 setStringValue:@"66"];
    [asc67 setStringValue:@"67"];
    [asc68 setStringValue:@"68"];
    [asc69 setStringValue:@"69"];
    [asc6a setStringValue:@"6a"];
    [asc6b setStringValue:@"6b"];
    [asc6c setStringValue:@"6c"];
    [asc6d setStringValue:@"6d"];
    [asc6e setStringValue:@"6e"];
    [asc6f setStringValue:@"6f"];
    [asc70 setStringValue:@"70"];
    [asc71 setStringValue:@"71"];
    [asc72 setStringValue:@"72"];
    [asc73 setStringValue:@"73"];
    [asc74 setStringValue:@"74"];
    [asc75 setStringValue:@"75"];
    [asc76 setStringValue:@"76"];
    [asc77 setStringValue:@"77"];
    [asc78 setStringValue:@"78"];
    [asc79 setStringValue:@"79"];
    [asc7a setStringValue:@"7a"];
    [asc7b setStringValue:@"7b"];
    [asc7c setStringValue:@"7c"];
    [asc7d setStringValue:@"7d"];
    [asc7e setStringValue:@"7e"];
    [asc7f setStringValue:@"7f"];
  }else{
    [asc00 setStringValue:@"0"];
    [asc01 setStringValue:@"1"];
    [asc02 setStringValue:@"2"];
    [asc03 setStringValue:@"3"];
    [asc04 setStringValue:@"4"];
    [asc05 setStringValue:@"5"];
    [asc06 setStringValue:@"6"];
    [asc07 setStringValue:@"7"];
    [asc08 setStringValue:@"8"];
    [asc09 setStringValue:@"9"];
    [asc0a setStringValue:@"10"];
    [asc0b setStringValue:@"11"];
    [asc0c setStringValue:@"12"];
    [asc0d setStringValue:@"13"];
    [asc0e setStringValue:@"14"];
    [asc0f setStringValue:@"15"];
    [asc10 setStringValue:@"16"];
    [asc11 setStringValue:@"17"];
    [asc12 setStringValue:@"18"];
    [asc13 setStringValue:@"19"];
    [asc14 setStringValue:@"20"];
    [asc15 setStringValue:@"21"];
    [asc16 setStringValue:@"22"];
    [asc17 setStringValue:@"23"];
    [asc18 setStringValue:@"24"];
    [asc19 setStringValue:@"25"];
    [asc1a setStringValue:@"26"];
    [asc1b setStringValue:@"27"];
    [asc1c setStringValue:@"28"];
    [asc1d setStringValue:@"29"];
    [asc1e setStringValue:@"30"];
    [asc1f setStringValue:@"31"];
    [asc20 setStringValue:@"32"];
    [asc21 setStringValue:@"33"];
    [asc22 setStringValue:@"34"];
    [asc23 setStringValue:@"35"];
    [asc24 setStringValue:@"36"];
    [asc25 setStringValue:@"37"];
    [asc26 setStringValue:@"38"];
    [asc27 setStringValue:@"39"];
    [asc28 setStringValue:@"40"];
    [asc29 setStringValue:@"41"];
    [asc2a setStringValue:@"42"];
    [asc2b setStringValue:@"43"];
    [asc2c setStringValue:@"44"];
    [asc2d setStringValue:@"45"];
    [asc2e setStringValue:@"46"];
    [asc2f setStringValue:@"47"];
    [asc30 setStringValue:@"48"];
    [asc31 setStringValue:@"49"];
    [asc32 setStringValue:@"50"];
    [asc33 setStringValue:@"51"];
    [asc34 setStringValue:@"52"];
    [asc35 setStringValue:@"53"];
    [asc36 setStringValue:@"54"];
    [asc37 setStringValue:@"55"];
    [asc38 setStringValue:@"56"];
    [asc39 setStringValue:@"57"];
    [asc3a setStringValue:@"58"];
    [asc3b setStringValue:@"59"];
    [asc3c setStringValue:@"60"];
    [asc3d setStringValue:@"61"];
    [asc3e setStringValue:@"62"];
    [asc3f setStringValue:@"63"];
    [asc40 setStringValue:@"64"];
    [asc41 setStringValue:@"65"];
    [asc42 setStringValue:@"66"];
    [asc43 setStringValue:@"67"];
    [asc44 setStringValue:@"68"];
    [asc45 setStringValue:@"69"];
    [asc46 setStringValue:@"70"];
    [asc47 setStringValue:@"71"];
    [asc48 setStringValue:@"72"];
    [asc49 setStringValue:@"73"];
    [asc4a setStringValue:@"74"];
    [asc4b setStringValue:@"75"];
    [asc4c setStringValue:@"76"];
    [asc4d setStringValue:@"77"];
    [asc4e setStringValue:@"78"];
    [asc4f setStringValue:@"79"];
    [asc50 setStringValue:@"80"];
    [asc51 setStringValue:@"81"];
    [asc52 setStringValue:@"82"];
    [asc53 setStringValue:@"83"];
    [asc54 setStringValue:@"84"];
    [asc55 setStringValue:@"85"];
    [asc56 setStringValue:@"86"];
    [asc57 setStringValue:@"87"];
    [asc58 setStringValue:@"88"];
    [asc59 setStringValue:@"89"];
    [asc5a setStringValue:@"90"];
    [asc5b setStringValue:@"91"];
    [asc5c setStringValue:@"92"];
    [asc5d setStringValue:@"93"];
    [asc5e setStringValue:@"94"];
    [asc5f setStringValue:@"95"];
    [asc60 setStringValue:@"96"];
    [asc61 setStringValue:@"97"];
    [asc62 setStringValue:@"98"];
    [asc63 setStringValue:@"99"];
    [asc64 setStringValue:@"100"];
    [asc65 setStringValue:@"101"];
    [asc66 setStringValue:@"102"];
    [asc67 setStringValue:@"103"];
    [asc68 setStringValue:@"104"];
    [asc69 setStringValue:@"105"];
    [asc6a setStringValue:@"106"];
    [asc6b setStringValue:@"107"];
    [asc6c setStringValue:@"108"];
    [asc6d setStringValue:@"109"];
    [asc6e setStringValue:@"110"];
    [asc6f setStringValue:@"111"];
    [asc70 setStringValue:@"112"];
    [asc71 setStringValue:@"113"];
    [asc72 setStringValue:@"114"];
    [asc73 setStringValue:@"115"];
    [asc74 setStringValue:@"116"];
    [asc75 setStringValue:@"117"];
    [asc76 setStringValue:@"118"];
    [asc77 setStringValue:@"119"];
    [asc78 setStringValue:@"120"];
    [asc79 setStringValue:@"121"];
    [asc7a setStringValue:@"122"];
    [asc7b setStringValue:@"123"];
    [asc7c setStringValue:@"124"];
    [asc7d setStringValue:@"125"];
    [asc7e setStringValue:@"126"];
    [asc7f setStringValue:@"127"];
  }


  mandSetUserDefaultInt([codecontrol selectedSegment], "asciicodeswitch");

  if([codecontrol selectedSegment] == 0){
    [chr00 setStringValue:@"\\0"];
    [chr01 setStringValue:@""];
    [chr02 setStringValue:@""];
    [chr03 setStringValue:@""];
    [chr04 setStringValue:@""];
    [chr05 setStringValue:@""];
    [chr06 setStringValue:@""];
    [chr07 setStringValue:@""];
    [chr08 setStringValue:@"\\b"];
    [chr09 setStringValue:@"\\t"];
    [chr0a setStringValue:@"\\n"];
    [chr0b setStringValue:@"\\v"];
    [chr0c setStringValue:@"\\f"];
    [chr0d setStringValue:@"\\r"];
    [chr0e setStringValue:@""];
    [chr0f setStringValue:@""];
    [chr10 setStringValue:@""];
    [chr11 setStringValue:@""];
    [chr12 setStringValue:@""];
    [chr13 setStringValue:@""];
    [chr14 setStringValue:@""];
    [chr15 setStringValue:@""];
    [chr16 setStringValue:@""];
    [chr17 setStringValue:@""];
    [chr18 setStringValue:@""];
    [chr19 setStringValue:@""];
    [chr1a setStringValue:@""];
    [chr1b setStringValue:@"(\\e)"];
    [chr1c setStringValue:@""];
    [chr1d setStringValue:@""];
    [chr1e setStringValue:@""];
    [chr1f setStringValue:@""];
    [chr20 setStringValue:@"space"];
    [chr7f setStringValue:@""];
  }else{
    [chr00 setStringValue:[NSString stringWithCString:codes[0x00] encoding:NSUTF8StringEncoding]];
    [chr01 setStringValue:[NSString stringWithCString:codes[0x01] encoding:NSUTF8StringEncoding]];
    [chr02 setStringValue:[NSString stringWithCString:codes[0x02] encoding:NSUTF8StringEncoding]];
    [chr03 setStringValue:[NSString stringWithCString:codes[0x03] encoding:NSUTF8StringEncoding]];
    [chr04 setStringValue:[NSString stringWithCString:codes[0x04] encoding:NSUTF8StringEncoding]];
    [chr05 setStringValue:[NSString stringWithCString:codes[0x05] encoding:NSUTF8StringEncoding]];
    [chr06 setStringValue:[NSString stringWithCString:codes[0x06] encoding:NSUTF8StringEncoding]];
    [chr07 setStringValue:[NSString stringWithCString:codes[0x07] encoding:NSUTF8StringEncoding]];
    [chr08 setStringValue:[NSString stringWithCString:codes[0x08] encoding:NSUTF8StringEncoding]];
    [chr09 setStringValue:[NSString stringWithCString:codes[0x09] encoding:NSUTF8StringEncoding]];
    [chr0a setStringValue:[NSString stringWithCString:codes[0x0a] encoding:NSUTF8StringEncoding]];
    [chr0b setStringValue:[NSString stringWithCString:codes[0x0b] encoding:NSUTF8StringEncoding]];
    [chr0c setStringValue:[NSString stringWithCString:codes[0x0c] encoding:NSUTF8StringEncoding]];
    [chr0d setStringValue:[NSString stringWithCString:codes[0x0d] encoding:NSUTF8StringEncoding]];
    [chr0e setStringValue:[NSString stringWithCString:codes[0x0e] encoding:NSUTF8StringEncoding]];
    [chr0f setStringValue:[NSString stringWithCString:codes[0x0f] encoding:NSUTF8StringEncoding]];
    [chr10 setStringValue:[NSString stringWithCString:codes[0x10] encoding:NSUTF8StringEncoding]];
    [chr11 setStringValue:[NSString stringWithCString:codes[0x11] encoding:NSUTF8StringEncoding]];
    [chr12 setStringValue:[NSString stringWithCString:codes[0x12] encoding:NSUTF8StringEncoding]];
    [chr13 setStringValue:[NSString stringWithCString:codes[0x13] encoding:NSUTF8StringEncoding]];
    [chr14 setStringValue:[NSString stringWithCString:codes[0x14] encoding:NSUTF8StringEncoding]];
    [chr15 setStringValue:[NSString stringWithCString:codes[0x15] encoding:NSUTF8StringEncoding]];
    [chr16 setStringValue:[NSString stringWithCString:codes[0x16] encoding:NSUTF8StringEncoding]];
    [chr17 setStringValue:[NSString stringWithCString:codes[0x17] encoding:NSUTF8StringEncoding]];
    [chr18 setStringValue:[NSString stringWithCString:codes[0x18] encoding:NSUTF8StringEncoding]];
    [chr19 setStringValue:[NSString stringWithCString:codes[0x19] encoding:NSUTF8StringEncoding]];
    [chr1a setStringValue:[NSString stringWithCString:codes[0x1a] encoding:NSUTF8StringEncoding]];
    [chr1b setStringValue:[NSString stringWithCString:codes[0x1b] encoding:NSUTF8StringEncoding]];
    [chr1c setStringValue:[NSString stringWithCString:codes[0x1c] encoding:NSUTF8StringEncoding]];
    [chr1d setStringValue:[NSString stringWithCString:codes[0x1d] encoding:NSUTF8StringEncoding]];
    [chr1e setStringValue:[NSString stringWithCString:codes[0x1e] encoding:NSUTF8StringEncoding]];
    [chr1f setStringValue:[NSString stringWithCString:codes[0x1f] encoding:NSUTF8StringEncoding]];
    [chr20 setStringValue:[NSString stringWithCString:codes[0x20] encoding:NSUTF8StringEncoding]];
    [chr7f setStringValue:[NSString stringWithCString:codes[0x7f] encoding:NSUTF8StringEncoding]];
  }
}

- (IBAction)segmentControlChange:(NSControl*)sender{
  [self update];
}




- (void)mouseEntered:(NSEvent *)theEvent{
  int ord = [[(NSDictionary*)[theEvent userData] objectForKey:@"ord"] intValue];
  if((ord <= 32) || (ord == 127)){
    [comment1 setStringValue:[NSString stringWithFormat:@"%i   0x%02x   %s\n%s", ord, ord, codes[ord], unicodenames[ord]]];
  }else{
    [comment1 setStringValue:[NSString stringWithFormat:@"%i   0x%02x   \'%s\'\n%s", ord, ord, codes[ord], unicodenames[ord]]];
  }
  NSString* addinfostring = [[NSBundle mainBundle] localizedStringForKey:[NSString stringWithFormat:@"BitFiddleAdditionalInfo%02x", ord] value:nil table:nil];
  [comment2 setStringValue:addinfostring];
  currenthighlight = ord;
}

- (void)mouseExited:(NSEvent *)theEvent{
  currenthighlight = -1;
}

@end
