/*
 * xio.c - Xmega IO devices - common file
 * Part of TinyG project
 * Copyright (c) 2010 Alden S. Hart, Jr.
 *
 * Xmega IO devices made compatible with avr-gcc stdio
 *
 * ---- To add a device ----
 *
 * Provide a xio_DEVICE.c and xio_DEVICE.h file (see xio_usb.c /.h for a model)
 *
 *	Include the following lines in this xio.c module:
 *		- add the include file, e.g.:#include "xio_usb.h"
 *		- add its init to xio_init(), e.g. xio_usb_init(XIO_BAUD_115200);
 *
 *	For further notes see the end of xio.h
 *
 */

#include <stdio.h>
#include <avr/pgmspace.h>

#include "xio.h"				// put includes for all devices here
#include "xio_usb.h"
#include "xio_pgm.h"

/*
 * Combined initializations 
 */

void xio_init(void)
{	
	// USB port defaults are:	XIO_RDWR | XIO_ECHO | XIO_CRLF - open additionally:
	xio_usb_init(XIO_LINEMODE | XIO_SEMICOLONS | XIO_BAUD_115200);

	// PGM file defaults are:	XIO_RD | XIO_BLOCK
//	xio_pgm_init(XIO_ECHO | XIO_CRLF | XIO_LINEMODE | XIO_SEMICOLONS);
	xio_pgm_init(XIO_LINEMODE);

	stddev = &dev_usb;				// stddev is a convenience
	stdin = &dev_usb;				// define the console device
	stdout = &dev_usb;
	stderr = &dev_usb;

//	fprintf(stdout, "\r\n\r\n**** Xmega IO subsystem initialized ****\r\n");
	printf_P(PSTR("\n**** Xmega IO subsystem initialized ****\n"));
}

/*
 * Common stuff - used across multiple xio modules
 */

/* USART table lookups */
const uint8_t bsel[] PROGMEM = 				// baud rates. See xio.h
		{ 0, 207, 103, 51, 34, 33, 31, 27, 19, 1, 1 };

const uint8_t bscale[] PROGMEM =  			// more baud rate data
		{ 0, 0, 0, 0, 0, (-1<<4), (-2<<4), (-3<<4), (-4<<4), (1<<4), 1 };

/*
 * xio_null_signal() - signal handler with no effect
 */

int xio_null_signal(uint8_t sig)
{
	return (0);
}

/*
 * xio_null_line() - null line handler 
 *
 *	Install this as the line handler if you are going to toss the line
 *	Useful for keeping an input device open for receiving signals (but not lines)
 */

int xio_null_line(char * buf)
{
	return (0);
}

