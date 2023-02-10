# OpenFontRender-Demo-program

Thanks to **takkaO** for this very good library.  It is a huge amount of code and very detailed.  With a few fixes, it will be GREAT!!

First, I am SO HAPPY that this library has been created.  I have a progarm that uses 2 different fonts in various sizes in the different screens it displays.  I had to have a binary for each font and in each size.  This was a HUGE amount of souce code lines!!  Now, I only have 2 TTF binary conversions and use multiple sizes.  Saved a LOT of memory!!

### This is the status as of library version 1.0.0.

I don't yet know if this interfaces with the tft_espi button text rendering.  I am still using the old code there.  I don't know how to hook it in.

This demo program tests out all non-trivial calls for the OpenFontRender program.  I wrote this to try to overcome the lack of help in the library of the author.  He documented the API calls but not the first example of how to make the call in a real program.  So, I scratched around for several days to find out all of the right parms and how to set them up.  Some were particularly cryptic!  The alignment one took me a long time to figure out.  It is required for the drawString calls.

It also highlights some errors I found while creating and testing.  The cprintf call returns the entire length of the string added to the starting X position and returns a value that is too high.  You must calculate it yourself.  Take the starting position and add half of the developed string length to find the correct string ending position.  It still may need to be adjusted by a few pixels to be "just right".

The bonding box is calulcated wrong for both height and width.

Extra space is left at the top of some fonts and it grows with font height growth.  That forces the text out the bottom of the calculated area.

The font background color is ignored and comes out as invisible.  This is a nice feature if you want it but not so good if you want a font background color.  I hope the author can implement both versions.  Leave this version with invisible background color and implement a working background color options, too.
