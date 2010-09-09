/*
	tg_protocol.c
	TinyG Protocol Handler
	Written by: Alden Hart
	Revision: 03/21/10

	References: "Writing Efficient State Machines in C" 
				 http://johnsantic.com/comp/state.html
*/

#include <stdio.h>					// language libs
#include <string.h>

#include <avr\io.h>					// AVR libs
#include <avr\pgmspace.h>

#include <tg_protocol.h>			// application


void initProtocol()
{
//  rPacket.inBuffer[0] = 0;		// initialize input string buffer
}

/************************************************************************************

	lookup tables (must precede the functions that use them)

***********************************************************************************/

/* ASCII Character Mapper
   maps ASCII character to prEvent emnumeration (gotXXXX)
   runs from program memory (requires pgm_read_byte macro to read) 
 */

//const uint8_t prCharArray[] = {			// version that works from data RAM 
											// must also change accesses to array
const uint8_t prCharArray[] PROGMEM = {

					// dec  hex symbol
		gotEOS, 	//	0	00	NUL	(Null char)
		gotSOH, 	//	1	01	SOH	(Start of Header)
		gotJunk, 	//	2	02	STX	(Start of Text)
		gotJunk, 	//	3	03	ETX	(End of Text)
		gotJunk, 	//	4	04	EOT	(End of Transmission)
		gotJunk, 	//	5	05	ENQ	(Enquiry)
		gotJunk, 	//	6	06	ACK	(Acknowledgment)
		gotJunk, 	//	7	07	BEL	(Bell)
		gotJunk, 	//	8	08	BS	(Backspace)
		gotWS, 		//	9	09	HT	(Horizontal Tab)
		gotEOS, 	//	10	0A	LF	(Line Feed)
		gotJunk, 	//	11	0B	VT	(Vertical Tab)
		gotJunk, 	//	12	0C	FF	(Form Feed)
		gotEOS, 	//	13	0D	CR	(Carriage Return)
		gotJunk, 	//	14	0E	SO	(Shift Out)
		gotJunk, 	//	15	0F	SI	(Shift In)
		gotJunk, 	//	16	10	DLE	(Data Link Escape)
		gotJunk, 	//	17	11	DC1 (XON) (Device Control 1)	
		gotJunk, 	//	18	12	DC2	(Device Control 2)
		gotJunk, 	//	19	13	DC3 (XOFF)(Device Control 3)	
		gotJunk, 	//	20	14	DC4	(Device Control 4)
		gotJunk, 	//	21	15	NAK (Negativ Acknowledgemnt)	
		gotJunk, 	//	22	16	SYN	(Synchronous Idle)
		gotJunk, 	//	23	17	ETB	(End of Trans. Block)
		gotJunk, 	//	24	18	CAN	(Cancel)
		gotJunk, 	//	25	19	EM	(End of Medium)
		gotJunk, 	//	26	1A	SUB	(Substitute)
		gotJunk, 	//	27	1B	ESC	(Escape)
		gotJunk, 	//	28	1C	FS	(File Separator)
		gotJunk, 	//	29	1D	GS	(Group Separator)
		gotJunk, 	//	30	1E	RS  (Reqst to Send)(Record Sep.)	
		gotJunk, 	//	31	1F	US	(Unit Separator)
		gotWS, 		//	32	20	SP	(Space)
		gotSOH, 	//	33	21	!	(exclamation mark)
		gotJunk, 	//	34	22	,	(double quote)	
		gotJunk, 	//	35	23	#	(number sign)
		gotJunk, 	//	36	24	$	(dollar sign)
		gotJunk, 	//	37	25	%	(percent)
		gotJunk, 	//	38	26	&	(ampersand)
		gotJunk, 	//	39	27	'	(single quote)
		gotJunk, 	//	40	28	(	(left/open parenthesis)
		gotJunk, 	//	41	29	)	(right/closing parenth.)
		gotJunk, 	//	42	2A	*	(asterisk)
		gotJunk, 	//	43	2B	+	(plus)
		gotJunk, 	//	44	2C		(comma)
		gotJunk, 	//	45	2D	-	(minus or dash)
		gotJunk, 	//	46	2E	.	(dot)
		gotJunk, 	//	47	2F	/	(forward slash)
		gotDigit, 	//	48	30	0	
		gotDigit, 	//	49	31	1	
		gotDigit, 	//	50	32	2	
		gotDigit, 	//	51	33	3	
		gotDigit, 	//	52	34	4	
		gotDigit, 	//	53	35	5	
		gotDigit, 	//	54	36	6	
		gotDigit, 	//	55	37	7	
		gotDigit, 	//	56	38	8	
		gotDigit, 	//	57	39	9	
		gotJunk, 	//	58	3A	:	(colon)
		gotEOS, 	//	59	3B	;	(semi-colon)
		gotJunk, 	//	60	3C	<	(less than)
		gotEqual, 	//	61	3D	=	(equal sign)
		gotJunk, 	//	62	3E	>	(greater than)
		gotJunk, 	//	63	3F	?	(question mark)
		gotJunk, 	//	64	40	@	(AT symbol)
		gotAlpha,	//	65	41	A	
		gotAlpha,	//	66	42	B	
		gotAlpha,	//	67	43	C	
		gotAlpha,	//	68	44	D	
		gotAlpha,	//	69	45	E	
		gotAlpha,	//	70	46	F	
		gotAlpha,	//	71	47	G	
		gotAlpha,	//	72	48	H	
		gotAlpha,	//	73	49	I	
		gotAlpha,	//	74	4A	J	
		gotAlpha,	//	75	4B	K	
		gotAlpha,	//	76	4C	L	
		gotAlpha,	//	77	4D	M	
		gotAlpha,	//	78	4E	N	
		gotAlpha,	//	79	4F	O	
		gotAlpha,	//	80	50	P	
		gotAlpha,	//	81	51	Q	
		gotAlpha,	//	82	52	R	
		gotAlpha,	//	83	53	S	
		gotAlpha,	//	84	54	T	
		gotAlpha,	//	85	55	U	
		gotAlpha,	//	86	56	V	
		gotAlpha,	//	87	57	W	
		gotAlpha,	//	88	58	X	
		gotAlpha,	//	89	59	Y	
		gotAlpha,	//	90	5A	Z	
		gotJunk,	//	91	5B	[	(left/opening bracket)
		gotJunk,	//	92	5C	\	(back slash)
		gotJunk,	//	93	5D	]	(right/closing bracket)
		gotJunk,	//	94	5E	^	(caret/circumflex)
		gotJunk,	//	95	5F	_	(underscore)
		gotJunk,	//	96	60	`	
		gotAlpha,	//	97	61	a	
		gotAlpha,	//	98	62	b	
		gotAlpha,	//	99	63	c	
		gotAlpha,	//	100	64	d	
		gotAlpha,	//	101	65	e	
		gotAlpha,	//	102	66	f	
		gotAlpha,	//	103	67	g	
		gotAlpha,	//	104	68	h	
		gotAlpha,	//	105	69	i	
		gotAlpha,	//	106	6A	j	
		gotAlpha,	//	107	6B	k	
		gotAlpha,	//	108	6C	l	
		gotAlpha,	//	109	6D	m	
		gotAlpha,	//	110	6E	n	
		gotAlpha,	//	111	6F	o	
		gotAlpha,	//	112	70	p	
		gotAlpha,	//	113	71	q	
		gotAlpha,	//	114	72	r	
		gotAlpha,	//	115	73	s	
		gotAlpha,	//	116	74	t	
		gotAlpha,	//	117	75	u	
		gotAlpha,	//	118	76	v	
		gotAlpha,	//	119	77	w	
		gotAlpha,	//	120	78	x	
		gotAlpha,	//	121	79	y	
		gotAlpha,	//	122	7A	z	
		gotJunk,	//	123	7B	{	(left/opening brace)
		gotJunk,	//	124	7C	|	(vertical bar)
		gotJunk,	//	125	7D	}	(right/closing brace)
		gotJunk,	//	126	7E	~	(tilde)
		gotJunk		//	127	7F	DEL	(delete)
};

/* State/Event table
   prNop indicates an unused event: should never be called from that state 
 */

void (*const prStateTable[MAX_STATES][MAX_EVENTS])(void) = {
//void (*const prStateTable[MAX_STATES][MAX_EVENTS])(void) PROGMEM = {
//(PGM_VOID_P) (prStateTable[MAX_STATES][MAX_EVENTS])(void) __ATTR_CONST__ PROGMEM = {

	// INSERT ACTION ROUTINES FOR STATE/EVENT IN THIS TABLE
    
	// listen state
	{	
	 	prAnyEventStart,			// gotSOH
		prAnyLoopDiscard,			// gotDigit
	 	prAnyLoopDiscard,			// gotAlpha
	 	prAnyLoopDiscard,			// gotEqual
	 	prAnyLoopDiscard,			// gotEOS
	 	prAnyLoopDiscard,			// gotJunk
	 	prAnyLoopDiscard,			// gotWS
	 	prNop						// gotTO	- There is no timeout in listen mode
	},
	
	// startPacket
	{	
	 	prAnyEventStart,			// gotSOH
		prStartPacketDigitExit,		// gotDigit	- exit to rxFromNode state
	 	prStartPacketAlphaExit,		// gotAlpha - exit to exPacketType 
	 	prThrowMalformedStartPacket,// gotEqual	- kick back to listen
	 	prThrowMalformedStartPacket,// gotEOS
	 	prThrowMalformedStartPacket,// gotJunk
	 	prAnyLoopDiscard,			// gotWS	- discard the whitespace
	 	prThrowTimeoutStartPacket	// gotTO
	},

	// rxFromNode
	{	
	 	prAnyEventStart,			// gotSOH
		prAnyLoopSave,				// gotDigit	- save the digit 
	 	prThrowMalformedFromNode,	// gotAlpha	
	 	prThrowMalformedFromNode,	// gotEqual
	 	prThrowMalformedFromNode,	// gotEOS
	 	prThrowMalformedFromNode,	// gotJunk
	 	prRxFromNodeSpaceExit,		// gotWS	- exit to endFromNode
	 	prThrowTimeoutFromNode		// gotTO
	},

	// endFromNode
	{	
	 	prAnyEventStart,			// gotSOH
		prEndFromNodeDigitExit,		// gotDigit	- exit to rxToNode 
	 	prThrowMalformedToNode,		// gotAlpha	
	 	prThrowMalformedToNode,		// gotEqual
	 	prThrowMalformedToNode,		// gotEOS
	 	prThrowMalformedToNode,		// gotJunk
	 	prAnyLoopDiscard,			// gotWS
	 	prThrowTimeoutToNode		// gotTO
	},

	// rxToNode
	{	
	 	prAnyEventStart,			// gotSOH
		prAnyLoopSave,				// gotDigit	- save the digit 
	 	prThrowMalformedToNode,		// gotAlpha	
	 	prThrowMalformedToNode,		// gotEqual
	 	prThrowMalformedToNode,		// gotEOS
	 	prThrowMalformedToNode,		// gotJunk
	 	prRxToNodeSpaceExit,		// gotWS	- exit to endToNode
	 	prThrowTimeoutToNode		// gotTO
	},

	// endToNode
	{	
	 	prAnyEventStart,			// gotSOH
		prEndToNodeDigitExit,		// gotDigit	- exit to rxNextTalker 
	 	prEndToNodeAlphaExit,		// gotAlpha	- exit to rxPacketType
	 	prThrowMalformedNextTalker,	// gotEqual
	 	prThrowMalformedNextTalker,	// gotEOS
	 	prThrowMalformedNextTalker,	// gotJunk
	 	prAnyLoopDiscard,			// gotWS
	 	prThrowTimeoutToNode			// gotTO
	},

	// rxNextTalker
	{	
	 	prAnyEventStart,			// gotSOH
		prAnyLoopSave,				// gotDigit	- save the digit 
	 	prThrowMalformedNextTalker,	// gotAlpha	
	 	prThrowMalformedNextTalker,	// gotEqual
	 	prThrowMalformedNextTalker,	// gotEOS
	 	prThrowMalformedNextTalker,	// gotJunk
	 	prRxNextTalkerSpaceExit,	// gotWS	- exit to endNextTalker
	 	prThrowTimeoutNextTalker	// gotTO
	},

	// endNextTalker
	{	
	 	prAnyEventStart,			// gotSOH
		prThrowMalformedPacketType,	// gotDigit
	 	prEndNextTalkerAlphaExit,	// gotAlpha - exit to rxPacketType
	 	prThrowMalformedPacketType,	// gotEqual
	 	prThrowMalformedPacketType,	// gotEOS
	 	prThrowMalformedPacketType,	// gotJunk
	 	prAnyLoopDiscard,			// gotWS
	 	prThrowTimeoutPacketType	// gotTO
	},

	// rxPacketType
	{	
	 	prAnyEventStart,			// gotSOH
		prThrowMalformedPacketType,	// gotDigit 
	 	prAnyLoopSave,				// gotAlpha	
	 	prRxPacketTypeEqualExit,	// gotEqual
	 	prThrowMalformedPacketType,	// gotEOS
	 	prThrowMalformedPacketType,	// gotJunk
	 	prRxPacketTypeSpaceExit,	// gotWS
	 	prThrowTimeoutPacketType	// gotTO
	},

	// rxTypeValue
	{	
	 	prAnyEventStart,			// gotSOH
		prAnyLoopSave,				// gotDigit 
	 	prAnyLoopSave,				// gotAlpha	
	 	prThrowMalformedTypeValue,	// gotEqual
	 	prThrowMalformedTypeValue,	// gotEOS
	 	prThrowMalformedTypeValue,	// gotJunk
	 	prRxTypeValueSpaceExit,		// gotWS
	 	prThrowTimeoutTypeValue		// gotTO
	},

	// rxPayload
	{	
	 	prAnyLoopSave,				// gotSOH
		prAnyLoopSave,				// gotDigit 
	 	prAnyLoopSave,				// gotAlpha	
	 	prAnyLoopSave,				// gotEqual
	 	prRxPayloadEndExit,			// gotEOS
	 	prAnyLoopSave,				// gotJunk
	 	prAnyLoopSave,				// gotWS
	 	prThrowTimeoutPayload		// gotTO
	}
};


/************************************************************************************

	protocol main functions

************************************************************************************/

/****** prRunProtocol(inChar)	main entry point for protocol
		Normally this would be a nested set of routines, but this is an inner loop
		So it's a set of nested table lookups implemented in 1 line of code.

		The broken-out lookup code looks like this (which works if inChar < 128):

		  prEvent = prCharArray[inChar];				// get next character event
		  prStateTable [prState][prEvent]();			// call the action procedure

		A range checked version is:

		  if (((prEvent >= 0) && (prEvent < MAX_EVENTS)) && 
		  	  ((prState >= 0) && (prState < MAX_STATES))) 
		  {
			prStateTable [prState][prEvent] ();			// call the action procedure
		  } else {
			// invalid state or event
		  };
*/

void prRunProtocol(uint8_t inChar)
{
  rx.inChar = inChar & 0x7F;						// mask out MSB (just in case)

  prEvent = pgm_read_byte(&(prCharArray[inChar]));	// get next character event
  prStateTable[prState][prEvent]();					// call the action procedure
//  prStateTable[prState][pgm_read_byte(&(prCharArray[inChar]))](); // call the action procedure


// Failed attempt to also try to put the state table in program memory
//  prEvent = prCharArray[inChar];				// get next character event
//  ((prStateTable[prState][prEvent]()));		// call action procedure
};


/************************************************************************************

	generic action routines - used by many states
	naming convention is: State - Event - Action or next state

***********************************************************************************/

void prNop(void) {}				// no-op filler routine

void prAnyEventListen(void)		// exit to listen from any event (with no error)
{
  prState = listen;				// no inits are necessary - occur at packet start
}

void prAnyEventStart(void)		// start a new packet from any event
{
  rx.bufferPtr = 0;				// initialize buffer
  prState = startPacket;		// return to start packet
}

void prAnyLoopDiscard(void)		// loop in current state, do not save character
{
  // no operation
}

void prAnyLoopSave(void)		// loop in current state, save rx char to rxBuffer
{
  // append character to rxBuffer
}


/************************************************************************************

	state-specific action routines
	naming convention is: State - Event - Action or next state

***********************************************************************************/

void prStartPacketDigitExit(void)	// exit startPacket to rxFromNode
{
//  strcat(rx.buffer, &rx.inChar);  	// append digit to receive buffer
  prState = rxFromNode;				// next state
}

void prStartPacketAlphaExit(void)	// exit startPacket to rxPacketType
{
  prState = rxPacketType;
}

void prRxFromNodeSpaceExit(void)	// exit rxFromNode to endFromNode
{
  // atoi fromNode
  // validate fromNode
  //	throw invalidFromNode
  // otherwise 
  // rPacket.fromNode =  
  prState = endFromNode;
}

void prEndFromNodeDigitExit(void)	// exit from endFromNode to rxToNode
{
  prState = rxToNode;
}

void prRxToNodeSpaceExit(void)		// exit rxFromNode to endFromNode
{
  prState = endToNode;
  // atoi from node in 
}

void prEndToNodeDigitExit(void)		// exit from endFromNode to rxToNode
{
  prState = rxNextTalker;
}

void prEndToNodeAlphaExit(void)		// exit from endFromNode to rxPacketType
{
  prState = rxPacketType;
}

void prRxNextTalkerSpaceExit(void)	// exit rxNextTalker to endNextTalker
{
  prState = endNextTalker;
  // atoi from node in 
}

void prEndNextTalkerAlphaExit(void)	// exit endNextTalker to rxPacketType
{
  prState = rxPayload;
}

void prRxPacketTypeEqualExit(void)	// exit rxPacketType to rxTypeValue
{
  prState = rxPacketType;
}

void prRxPacketTypeSpaceExit(void)	// exit rxPacketType to endPacketType
{
  prState = rxTypeValue;
}

void prRxTypeValueSpaceExit(void)	// exit rxTypeValue to endPacketType
{
  prState = rxPayload;
}

void prRxPayloadEndExit(void)		// exit rxPayload to endPacket
{
  // replace EOS with NULL in string
  // queue ACK
  // save ackMode for app level lack
  // exec packet (pass to app layer)
  prState = listen;
} 

/************************************************************************************

	exception handler action routines
	naming convention is: "Throw" - ErrorType - State

***********************************************************************************/

void prThrowTimeoutStartPacket(void) // throw timeout startPacket NAK 
{
  prState = listen;
}

void prThrowMalformedStartPacket(void) // throw malformed startPacket NAK
{
  prState = listen;
}

void prThrowTimeoutFromNode(void)	// throw timeout fromNode NAK 
{
  prState = listen;
}

void prThrowMalformedFromNode(void)	// throw malformed fromNode NAK
{
  prState = listen;
}

void prThrowInvalidFromNode(void)	// throw invalid fromNode NAK
{
  prState = listen;
}

void prThrowTimeoutToNode(void)		// throw timeout toNode NAK 
{
  prState = listen;
}

void prThrowMalformedToNode(void)	// throw malformed toNode NAK
{
  prState = listen;
}

void prThrowInvalidToNode(void)		// throw invalid toNode NAK
{
  prState = listen;
}

void prThrowTimeoutNextTalker(void)	// throw timeout nextTalker NAK 
{
  prState = listen;
}

void prThrowMalformedNextTalker(void) // throw malformed nextTalker NAK
{
  prState = listen;
}

void prThrowInvalidNextTalker(void)	// throw invalid nextTalker NAK
{
  prState = listen;
}

void prThrowTimeoutPacketType(void)	// throw timeout PacketType NAK 
{
  prState = listen;
}

void prThrowMalformedPacketType(void) // throw malformed PacketType NAK
{
  prState = listen;
}

void prThrowInvalidPacketType(void) // throw invalid PacketType NAK
{
  prState = listen;
}

void prThrowOverrunPacketType(void) // throw overrun PacketType NAK
{
  prState = listen;
}

void prThrowTimeoutTypeValue(void)	// throw timeout TypeValue NAK 
{
  prState = listen;
}

void prThrowMalformedTypeValue(void) // throw malformed TypeValue NAK
{
  prState = listen;
}

void prThrowInvalidTypeValue(void)	// throw invalid TypeValue NAK
{
  prState = listen;
}

void prThrowOverrunTypeValue(void)	// throw overrun TypeValue NAK
{
  prState = listen;
}

void prThrowTimeoutPayload(void)	// throw timeout Payload NAK 
{
  prState = listen;
}

void prThrowOverrunPayload(void)	// throw overrun Payload NAK
{
  prState = listen;
}

