Yet another bootloader for Atmel's XMEGA series of microcontrollers. The main point of interest here is the stripped down ASF Code so that the whole thing will fit inside the 4K boot section of an XMEGA32A4U (and others).

Note: since my Blusb firmware needs to be able to write to flash, the bootloader includes a jump table stored at the end of the flash section in order to expose shared functions to the main application. Most people will not need it.

See my Blusb CLI for the complementary host application code to upload firmware binary data.

This code is tested and proven to work, but it is not universal, it was specifically made for the XMEGA32A4U. Yet, since the ASF code is the same for almost all XMEGA variants, it should be possible to use it with any other XMEGA as well, give or take a few minor adjustments.
