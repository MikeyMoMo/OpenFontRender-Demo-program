#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#include "TTF_Binaries.h"  // Includes LiquidCrystalTTF, BritanicBoldTTF & Comic_Sans_Italic.
#include "OpenFontRender.h" // Include after M5Stack.h / M5Core2.h

OpenFontRender render;

// Setting PWM properties, do not change this unless you are sure what you are doing!
const int pwmFreq = 5000;        // High frequency.  No noticable flickering
const int pwmResolution = 8;     // 8 bit resolution on the brightness
const int pwmLedChannelTFT = 4;  // Backlight PWM channel number 4

int  myTW, myTH;                           // For getting text width and height
int  x, y;                                 // Get/set cursor position
int  font_size;                            // Setting and fetching font size in pixels
char str_buf [32];                         // Utility
char txtSample[100] = "A1MY";              // For font size testing and utility use
char vers[render.FT_VERSION_STRING_SIZE];  // Area to return version information (from library)
char cred[render.CREDIT_STRING_SIZE];      // Areas to return credits information (from library)

/***************************************************************************/
void setup()
/***************************************************************************/
{
  Serial.begin(115200); delay(1000);
  Serial.println("Running from:"); Serial.println(__FILE__);
  render.setSerial(Serial); delay(1000);  // Needed for render library to print messages.
  Serial.println("\r\n--------------------------------\r\nStarting");
  render.showFreeTypeVersion();      // print FreeType version
  render.showCredit();               // print FTL credit
  render.getFreeTypeVersion(vers);   // A different way where you get the text in hand
  Serial.print("Fetched version: "); Serial.print(vers); // Tell the world (library adds \n)
  render.getCredit(cred);            // This way, you get the text in hand
  Serial.print("Fetched credit : "); Serial.println(cred); // Tell the world + blank line.

  tft.begin();  // Initialize the screen driver

  // ILI9486 likes it this way.  Yours may not
  tft.setSwapBytes(true); tft.invertDisplay(false); tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);  // 1 is a landscape rotation in ILI9486
#if defined TFT_BL     // Only if you have a backlight pin defined
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT); // TFT_BL, 0 - 15
  ledcWrite(pwmLedChannelTFT, 200);        // Close to max brightness (255 for 8 bit)
#endif

  // The single setDrawer call is the same as using the four calls under it. Take your pick!
  //render.setDrawer(tft);             // Link renderer to base display.
  render.setDrawPixel(tft.drawPixel);
  render.setDrawFastHLine(tft.drawFastHLine); // optional
  render.setStartWrite(tft.startWrite);       // optional
  render.setEndWrite(tft.endWrite);           // optional

  // Currently, in the library, these are all set to 128.  I assume this is KB.
  // Setting them to 0 causes the cache size to be unlimited and to overflow
  //  and the program to crash.  Not good, yes?!?!  If you need more, issue this:
  //void setCacheSize(unsigned int max_faces,
  //                  unsigned int max_sizes,
  //                  unsigned long max_bytes)
}
/***************************************************************************/
void loop()
/***************************************************************************/
{
  drawStrings();  // Demo 1 - lots of strings all over the place
  drawChars();    // Demo 2 - Just single characters printed on screen
}
/***************************************************************************/
void drawStrings()
/***************************************************************************/
{
  // First, we do it with printf, cprintf and rprintf then with three
  //  drawstring statements.
  //  Should look the same except for color change.
  // You can select text alignment! use printf, cprintf or rprintf functions.

  //-----Comic Sans Italic-----------------------------------------------------

  Serial.println(">----------Comic Sans Italic");
  tft.fillScreen(TFT_BLACK);
  render.unloadFont();  // Be sure you have the fix or this will blow up on you!
  if (render.loadFont(Comic_Sans_Italic, sizeof(Comic_Sans_Italic)))  // Error code. 0 is success.
  {
    Serial.println("Render loadFont error for Comic_Sans_Italic");
    Serial.print("In routine "); Serial.println(__func__);  // Tell which loadFont blew up.
    Serial.print(", line "); Serial.println(__LINE__);      // Tell which loadFont blew up.
    while (1);
  }
  font_size = 30;  // Fits nicely on 480x320
  render.setFontSize(font_size);
  render.setLineSpaceRatio(0.7);  // This only affects the spacing if you have a \n in your string.
  myTW = render.getTextWidth(txtSample);
  myTH = render.getTextHeight(txtSample);

  // Alignment Left
  // If the second font color is missing, it will be written invisible and the background color
  //  already on the screen will not be changed.  To change it, use a second color like this:
  render.setFontColor(TFT_WHITE, TFT_RED);  // Foreground, Background (optional)
  // Note, it can also be set by 8 bit hex values.  Foreground only or foreground and background, as above.
  //  render.setFontColor(0x50, 0x70, 0x150); // Mostly blue, a little green and less red. FG only set.
  //  ...or...
  //  render.setFontColor(0x50, 0x70, 0x150, 0x150, 0x70, 0x50);  // Set FG and BG to custom colors.
  //
  // You can also set the background color, only, thus:
  render.setBackgroundColor(TFT_RED);  // Change BG color only, leave font color as already set.
  // So far, the background color setting does nothing.  Maybe this is by design.

  strcpy(txtSample, "Comic Sans Italic size:");

  // Alignment left
  render.setCursor(0, 0);
  render.printf("Hello\nWorld size ");
  // If you want to chain text together, use the cursor position after printer to add on.
  // Note: As of library version 1.0.0, this fails on cprintf returning an
  //        X value to far to the right. It is too long by half of the text string length.
  //        He forgot to cut it in half for cprintf. There is a workaround, herewith.  Search
  //        for "cprintf x value fix".
  x = render.getCursorX(); y = render.getCursorY();
  itoa (font_size, str_buf, 10);
  Serial.print("Cursor starting at 0,0 - ");
  Serial.printf("iota result %s, cursor ending location %i/%i\r\n", str_buf, x, y);
  render.setCursor(x, y); render.printf(str_buf);  // Append font size digits

  // Alignment Center
  render.setCursor(tft.width() / 2, tft.height() / 3); render.setFontColor(TFT_GREEN);
  render.cprintf(txtSample);
  // If you want to chain text together, use the cursor position after printer to add on.
  x = render.getCursorX(); y = render.getCursorY();
  itoa (font_size, str_buf, 10);
  Serial.printf("Text is \"%s\", width is %i\r\n", txtSample, render.getTextWidth(txtSample));
  Serial.printf("Cursor starting at %i,%i - ", tft.width() / 2, tft.height() / 3);
  Serial.printf("iota result %s, cursor ending location %i/%i\r\n", str_buf, x, y);
  // Append font size digits
  myTW = render.getTextWidth(txtSample);
  // Cheating because of error in library (maybe).
  // The getCursorX routine should tell the right number but is half a string too long.
  // cprintf x value fix
  render.setCursor(tft.width() / 2 + myTW / 2, y); render.printf(str_buf);

  // Alignment Right
  render.setCursor(tft.width(), tft.height() / 3 * 2); render.setFontColor(TFT_ORANGE);
  render.rprintf("> right justified");
  delay(2000); tft.fillScreen(TFT_BLACK);
  /* You can select text alignment! use drawString function */
  // Alignment Left
  render.drawString("Hello\nWorld", 0, 0, TFT_MAGENTA, TFT_BLACK);
  // Alignment Center
  render.cdrawString("Comic_Sans_Italic", tft.width() / 2, tft.height() / 3, TFT_YELLOW, TFT_BLACK);
  // Alignment Right
  render.rdrawString("> right justified", tft.width(), tft.height() / 3 * 2, TFT_CYAN, TFT_BLACK);
  delay(2000); tft.fillScreen(TFT_BLACK);

  //-----Liquid Crystal-----------------------------------------------------

  Serial.println(">----------Liquid Crystal");

  render.unloadFont();
  if (render.loadFont(LiquidCrystalTTF, sizeof(LiquidCrystalTTF)))  // Error code. 0 is success.
  {
    Serial.println("Render loadFont error for LiquidCrystalTTF");
    Serial.print("In routine "); Serial.println(__func__);  // Tell which loadFont blew up.
    Serial.print(", line "); Serial.println(__LINE__);      // Tell which loadFont blew up.
    while (1);
  }
  font_size = 30;  // in pixels
  strcpy(txtSample, "LiquidCrystalTTF size:");
  render.setFontSize(font_size);
  render.setLineSpaceRatio(0.7);  // This affects the spacing if you have a \n in your string.
  myTW = render.getTextWidth(txtSample);
  myTH = render.getTextHeight(txtSample);

  // Alignment Left
  render.setCursor(0, 0); render.setFontColor(TFT_WHITE);
  render.printf("Hello\nWorld");

  // Alignment Center
  render.setCursor(tft.width() / 2, tft.height() / 3); render.setFontColor(TFT_GREEN);
  render.cprintf(txtSample);
  x = render.getCursorX(); y = render.getCursorY();
  itoa (font_size, str_buf, 10);
  Serial.printf("Text string width is %i\r\n", render.getTextWidth(txtSample));
  Serial.printf("Cursor starting at %i,%i - ", tft.width() / 2, tft.height() / 3);
  Serial.printf("iota result %s, cursor ending location %i/%i\r\n", str_buf, x, y);
  // Append font size
  myTW = render.getTextWidth(txtSample);
  // Cheating because of error in library (maybe).
  // The getCursorX routine should tell the right number but is half a string too long.
  // And, the + 20 should not be necessary.
  render.setCursor(tft.width() / 2 + myTW / 2 + 20, y); render.cprintf(str_buf);

  // Alignment Right
  render.setCursor(tft.width(), tft.height() / 3 * 2); render.setFontColor(TFT_ORANGE);
  render.rprintf("> right justified");

  delay(2000); tft.fillScreen(TFT_BLACK);

  // Alignment Left
  render.drawString("Hello\nWorld", 0, 0, TFT_MAGENTA, TFT_BLACK);

  // Alignment Center
  render.cdrawString("LiquidCrystalTTF", tft.width() / 2, tft.height() / 3, TFT_YELLOW, TFT_BLACK);

  // Alignment Right
  render.rdrawString("> right justified", tft.width(), tft.height() / 3 * 2, TFT_CYAN, TFT_BLACK);
  delay(2000);

  //-----BritanicBold-----------------------------------------------------

  Serial.println(">----------BritanicBoldTTF");
  tft.fillScreen(TFT_BLACK);
  render.unloadFont();
  if (render.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF)))  // Error code. 0 is success.
  {
    Serial.println("Render loadFont error for BritanicBoldTTF");
    Serial.print("In routine "); Serial.println(__func__);  // Tell which loadFont blew up.
    Serial.print(", line "); Serial.println(__LINE__);      // Tell which loadFont blew up.
    while (1);
  }
  font_size = 40;
  render.setFontSize(font_size);
  render.setLineSpaceRatio(0.9);  // This affects the spacing if you have a \n in your string.
  myTW = render.getTextWidth(txtSample);
  myTH = render.getTextHeight(txtSample);

  // Alignment Left
  render.setCursor(0, 0); render.setFontColor(TFT_WHITE);
  render.printf("Hello\nWorld");

  // Alignment Center
  render.setCursor(tft.width() / 2, tft.height() / 3); render.setFontColor(TFT_GREEN);
  render.cprintf("BritanicBoldTTF");

  // Alignment Right
  render.setCursor(tft.width(), tft.height() / 3 * 2); render.setFontColor(TFT_ORANGE);
  render.rprintf("> right justified");
  delay(2000);

  tft.fillScreen(TFT_BLACK);
  /* You can select text alignment! use drawString function */

  // Alignment Left
  render.drawString("Hello\nWorld", 0, 0, TFT_MAGENTA, TFT_BLACK);

  // Alignment Center
  render.cdrawString("BritanicBoldTTF", tft.width() / 2, tft.height() / 3, TFT_YELLOW, TFT_BLACK);

  // Alignment Right
  render.rdrawString("> right justified", tft.width(), tft.height() / 3 * 2, TFT_CYAN, TFT_BLACK);
  delay(2000);

  //-----FinalFrontier-----------------------------------------------------

  Serial.println(">----------FinalFrontier");
  tft.fillScreen(TFT_BLACK);
  render.unloadFont();
  if (render.loadFont(FinalFrontier, sizeof(FinalFrontier)))  // Error code. 0 is success.
  {
    Serial.println("Render loadFont error for FinalFrontier");
    Serial.print("In routine "); Serial.println(__func__);  // Tell which loadFont blew up.
    Serial.print(", line "); Serial.println(__LINE__);      // Tell which loadFont blew up.
    while (1);
  }
  font_size = 60;
  render.setFontSize(font_size);
  render.setLineSpaceRatio(0.9);  // This affects the spacing if you have a \n in your string.
  myTW = render.getTextWidth(txtSample);
  myTH = render.getTextHeight(txtSample);

  // Alignment Left
  render.setCursor(0, 0); render.setFontColor(TFT_WHITE);
  render.printf("Hello\nWorld");

  // Alignment Center
  render.setCursor(tft.width() / 2, (tft.height() / 3) + 30);
  render.setFontColor(TFT_GREEN); render.cprintf("FinalFrontier");

  // Alignment Right
  render.setCursor(tft.width(), tft.height() / 3 * 2);
  render.setFontColor(TFT_ORANGE); render.rprintf("> right justified");
  delay(2000);

  tft.fillScreen(TFT_BLACK);
  /* You can select text alignment! use drawString function */

  //FT_Pos
  //typedef signed long FT_Pos;

  FT_BBox BBOX;                       // This is an structure
  //  typedef struct  FT_BBox_
  //  {
  //    FT_Pos  xMin, yMin;
  //    FT_Pos  xMax, yMax;
  //
  //  } FT_BBox;

  Drawing doDraw = Drawing::Execute;  // This is an emum
  //  enum class Drawing {
  //    Execute,
  //    Skip
  //  };

  Align myAlign = Align::Left;        // This is an emum
  //  enum class Align {
  //    Left,
  //    Center,
  //    Right
  //  };

  //  typedef int FT_Error;
  FT_Error FTERR;  // This is just an integer.

  // Alignment Left
  render.drawHString("Hello\nWorld", tft.width() / 2, 0, TFT_MAGENTA, TFT_BLACK, myAlign, doDraw, BBOX, FTERR);
  delay(1000);
  myAlign = Align::Center;
  render.drawHString("Hello\nWorld", tft.width() / 2, tft.height() / 3, TFT_MAGENTA, TFT_BLACK, myAlign, doDraw, BBOX, FTERR);
  delay(1000);
  myAlign = Align::Right;
  render.drawHString("Hello\nWorld", tft.width() / 2, tft.height() / 3 * 2, TFT_MAGENTA, TFT_BLACK, myAlign, doDraw, BBOX, FTERR);
  delay(2000);

  //-----Courier New-----------------------------------------------------

  Serial.println(">----------Courier New");
  strcpy(txtSample, "Courier New size:");
  tft.fillScreen(TFT_BLACK);
  render.unloadFont();
  if (render.loadFont(CourierNewTTF, sizeof(CourierNewTTF)))  // Error code. 0 is success.
  {
    Serial.println("Render loadFont error for CourierNewTTF");
    Serial.print("In routine "); Serial.println(__func__);  // Tell which loadFont blew up.
    Serial.print(", line "); Serial.println(__LINE__);      // Tell which loadFont blew up.
    while (1);
  }
  font_size = 35;
  render.setFontSize(font_size);
  render.setLineSpaceRatio(0.5);  // This affects the spacing if you have a \n in your string.
  myTW = render.getTextWidth(txtSample);
  myTH = render.getTextHeight(txtSample);

  // Alignment Left
  render.setCursor(0, 0); render.setFontColor(TFT_WHITE);
  render.printf("Hello\nWorld");

  // Alignment Center
  render.setCursor(tft.width() / 2, (tft.height() / 3));
  render.setFontColor(TFT_GREEN); render.cprintf(txtSample);
  // If you want to chain text together, use the cursor position after printer to add on.
  x = render.getCursorX(); y = render.getCursorY();
  itoa (font_size, str_buf, 10);
  Serial.printf("Text is \"%s\", width is %i\r\n", txtSample, render.getTextWidth(txtSample));
  //  Serial.printf("Text string width is %i\r\n", render.getTextWidth(txtSample));
  Serial.printf("Cursor starting at %i,%i - ", tft.width() / 2, tft.height() / 3);
  Serial.printf("iota result %s, cursor ending location %i/%i\r\n", str_buf, x, y);
  // Append font size
  myTW = render.getTextWidth(txtSample);
  // Cheating because of error in library (maybe).
  // The getCursorX routine should tell the right number but is half a string too long.
  // And, the + 4 should not be necessary.
  render.setCursor(tft.width() / 2 + myTW / 2 + 4, y); render.printf(str_buf);

  // Alignment Right
  render.setCursor(tft.width(), tft.height() / 3 * 2);
  render.setFontColor(TFT_ORANGE); render.rprintf("> right justified");
  delay(2000);

  // From here on, I go to Serial.print'ing because the numbers are quick and need to be compared, later.
  // That's hard to do with screen output.

  strcpy(txtSample, "A1MY");  // To use for all sizing tests
  font_size = 40;
  myAlign = Align::Left;
  Layout myLayout = Layout::Horizontal;  // One way to do it
  render.setLayout(Layout::Horizontal);  // Another way to do it

  BBOX = render.calculateBoundingBoxFmt(0, 0, font_size, myAlign, myLayout, txtSample);
  Serial.printf("Bounding Box Fmt - Font size %i, test string \"%s\" creates bounding box:\r\n", font_size, txtSample);
  Serial.printf("xMin = %i, xMax = %i, yMin = %i, yMax = %i\r\n", BBOX.xMin, BBOX.xMax, BBOX.yMin, BBOX.yMax);

  BBOX = render.calculateBoundingBox(0, 0, font_size, myAlign, myLayout, txtSample);
  Serial.printf("Bounding Box - Font size %i, test string \"%s\" creates bounding box:\r\n", font_size, txtSample);
  Serial.printf("xMin = %i, xMax = %i, yMin = %i, yMax = %i\r\n", BBOX.xMin, BBOX.xMax, BBOX.yMin, BBOX.yMax);
  Serial.printf("Text width for same string is %i pixels\r\n", render.getTextWidth(txtSample));

  font_size = 18;
  BBOX = render.calculateBoundingBox(0, 0, font_size, myAlign, myLayout, txtSample);
  Serial.printf("Bounding Box - Font size %i, test string \"%s\" creates bounding box:\r\n", font_size, txtSample);
  Serial.printf("xMin = %i, xMax = %i, yMin = %i, yMax = %i\r\n", BBOX.xMin, BBOX.xMax, BBOX.yMin, BBOX.yMax);
  Serial.printf("Text width for same string is %i pixels\r\n", render.getTextWidth(txtSample));
  Serial.printf("Text height for same string is %i pixels\r\n", render.getTextHeight(txtSample));

  myTW = 50; myTH = 50;
  font_size = render.calculateFitFontSizeFmt(myTW, myTH, myLayout, txtSample);
  Serial.printf("Font size fit for FontSizeFmt %ix%i pixels is %i\r\n", myTW, myTH, font_size);
  myTW = 100; myTH = 100;
  font_size = render.calculateFitFontSizeFmt(myTW, myTH, myLayout, txtSample);
  Serial.printf("Font size fit for FontSizeFmt %ix%i pixels is %i\r\n", myTW, myTH, font_size);
  myTW = 150; myTH = 150;
  font_size = render.calculateFitFontSizeFmt(myTW, myTH, myLayout, txtSample);
  Serial.printf("Font size fit for FontSizeFmt %ix%i pixels is %i\r\n", myTW, myTH, font_size);

  myTW = 50; myTH = 50;
  font_size = render.calculateFitFontSize(myTW, myTH, myLayout, txtSample);
  Serial.printf("Font size fit for FontSize %ix%i pixels is %i\r\n", myTW, myTH, font_size);
  myTW = 100; myTH = 100;
  font_size = render.calculateFitFontSize(myTW, myTH, myLayout, txtSample);
  Serial.printf("Font size fit for FontSize %ix%i pixels is %i\r\n", myTW, myTH, font_size);
  myTW = 150; myTH = 150;
  font_size = render.calculateFitFontSize(myTW, myTH, myLayout, txtSample);
  Serial.printf("Font size fit for FontSize %ix%i pixels is %i\r\n", myTW, myTH, font_size);
  Serial.println("--------------------------------------------------------------");
}
/***************************************************************************/
void drawChars()
{
  int xpos = 0, ypos = 0;
  char sChar;

  tft.fillScreen(TFT_BLACK);
  render.unloadFont();
  if (render.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF)))  // Error code. 0 is success.
  {
    Serial.println("Render loadFont error for BritanicBoldTTF");
    Serial.print("In routine "); Serial.println(__func__);  // Tell which loadFont blew up.
    Serial.print(", line "); Serial.println(__LINE__);      // Tell which loadFont blew up.
    while (1);
  }
  font_size = 60;
  render.setFontSize(font_size);
  strcpy(txtSample, "W");
  myTW = render.getTextWidth(txtSample);
  myTH = render.getTextHeight(txtSample);

  sChar = 'C'; render.drawChar(sChar, xpos, ypos, TFT_YELLOW, TFT_BLACK); xpos += myTW; ypos += myTH;
  sChar = 'h'; render.drawChar(sChar, xpos, ypos, TFT_GREEN, TFT_BLACK); xpos += myTW; ypos += myTH;
  sChar = 'a'; render.drawChar(sChar, xpos, ypos, TFT_RED, TFT_BLACK); xpos += myTW; ypos += myTH;
  sChar = 'r'; render.drawChar(sChar, xpos, ypos, TFT_WHITE, TFT_BLACK); xpos += myTW; ypos += myTH;
  sChar = 's'; render.drawChar(sChar, xpos, ypos, TFT_BLUE, TFT_BLACK); xpos += myTW; ypos += myTH;
  delay(2000);
}
