/*                   Library Interface for EasyLase USB
 *                     (C) 2005,2006 Mueller Elektronik
 *                            All rights reserved.
 *
 */


#ifndef _EASYLASE
#define _EASYLASE

/**  -- Introduction --
 * Only 2 functions have to be called and affect all connected devices. These are:
 *      EasyLaseGetCardNum
 *        Must be called befor using any other function.  Can also be called to
 *        try to reopen the connection when an error occurred.
 *  and EasyLaseClose
 *       Closes all devices. Should be called at the end of the application
 *       program.
 *  All other functions are optional and can be used independently from each
 *  other by the application.
 *
 * -- Lasershow output --
 * EasyLase outputs the data frame based. This means, always a complete show frame
 * will be transmitted via USB. When the frame is received by the device,
 * EasyLase will display it. A frame will always be displayed completely without
 * any interruptions. A running frame will not be interrupted by a new incoming frame.
 * The system is double buffered. A new frame can be received via USB, while another
 * frame is being output to the D/A-converters.
 * Important:
 * Frames need time to be output. The time is the result of number of points
 * multiplicated with the output time for each point (points per second).
 * Example:
 * At an output speed of 10000 pps and 1000 points, the frame needs 1000 / 10000 = 0.1
 * seconds or 100 milliseconds for display.
 * When more frames are transmitted via USB as can be displayed in the same time,
 * they will be discarded. This costs transmission bandwith and can lower the
 * communication speed. To avoid such data jam, a handshake can be implemented.
 * Using the function EasyLaseGetStatus, the application can check if the device is
 * ready to receive a new frame. This handshake should be used, but doesn't have to
 * be used. The application can send frames using EasyLaseWriteFrame every time,
 * without checking the status.
 * Frames will be repeated when no new frame is available. To avoid repetition,
 * use the EasyLaseWriteFrameNR function. Whenever a new frame is ready for
 * display, it will be output next.
 * To stop output the Function EasyLaseStop is implemented.
 */

/** Data Format Frame
 * A laser frame for output is an array of the following 8 byte structure:
 */
/*
 typedef struct {
 unsigned short x;  // 2 Bytes  Value 0 - 4095  X-Coordinate
 unsigned short y;  // 2 Bytes  Value 0 - 4095  Y-coordinate
 unsigned char  r;  // 1 Byte   Value 0 - 255   Red
 unsigned char  g;  // 1 Byte   Value 0 - 255   Green
 unsigned char  b;  // 1 Byte   Value 0 - 255   Blue
 unsigned char  i;  // 1 Byte   Value 0 - 255   Intensity
 } __attribute__((packed)) EasyLaseData, *EasyLasePoint;
 */
/** Data Format DMX
 * There are 2 basic functions available for reading or writing DMX-data.
 * All 512 channels can be accessed or only a selected number of channels.
 * The functions for accessing a limited number of channels are newly
 * implemented. The 2 other functions for accessing all 512 channels will
 * be available for compatibility. For lowering the data traffic, it is
 * recommended to use as few channels as possible. For both functions,
 * data will be accessed via a data array, where the lowest address
 * represents the lowest DMX-channel.
 * EasyLase USB generally reads all DMX-channels available on the DMX-line.
 * The application must decide what channels have to be transferred via USB.
 * For DMX-out, EasyLase USB outputs all channels beginning from channel 0
 * up to the highest channel written via USB. When the highest channel used
 * is significantly lower than 512, the repetition rate of the generated
 * DMX-signal can be increased, giving a higher refresh rate for DMX-devices.
 */

/** Data Format TTL
 * TTL-values are for output only. 16 Bits can be transferred via USB, 
 * but EasyLase will support the lower 8 bits only.
 */

/** Functions
 * Important: 	All functions, except EasyLaseGetCardNum and EasyLaseClose
 * refer to a selected device, which is addressed by the card number.
 * While the number of all cards can be 0 (no card) to 16, the selection
 * is 0 (first card) to 15. So when using 1 card, the number of cards is
 * 1, but the cardNumber is 0.
 */



extern "C" {
	int easyLaseGetCardNum();
	
	int easyLaseWriteFrameUncompressed(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed);
	int easyLaseWriteFrame(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed);
	int easyLaseWriteFrameNR(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed, unsigned int repeat);
	int easyLaseStop(int cardNumber);
	
	int easyLaseWriteDMX(int cardNumber, unsigned char * dmxBuffer);
	int easyLaseDMXOut(int cardNumber, unsigned char * dmxBuffer, unsigned int baseAddress, unsigned int channelCount);
	int easyLaseGetDMX(int cardNumber, unsigned char * dmxBuffer);
	int easyLaseDMXIn(int cardNumber, unsigned char * dmxBuffer, unsigned int baseAddress, unsigned int channelCount);
	
	int easyLaseWriteTTL(int cardNumber, unsigned int ttl);
	
	int easyLaseGetLastError(int cardNumber);
	int easyLaseGetStatus(int cardNumber);
	
	int easyLaseClose();
	
	typedef struct {
		unsigned short x;  // 2 Bytes  Value 0 - 4095  X-Coordinate
		unsigned short y;  // 2 Bytes  Value 0 - 4095  Y-coordinate
		unsigned char  r;  // 1 Byte   Value 0 - 255   Red
		unsigned char  g;  // 1 Byte   Value 0 - 255   Green
		unsigned char  b;  // 1 Byte   Value 0 - 255   Blue
		unsigned char  i;  // 1 Byte   Value 0 - 255   Intensity
	} __attribute__((packed)) EasyLaseData, *EasyLasePoint;
	
	
}


//int easyLaseGetCardNum();
/** int easyLaseGetCardNum(); //must be used before all other functions
 * Will return the number of connected cards and opens the devices.
 * Return value: 0 = no cards, 1-20 number of cards connected.
 */


#define EASYLASE_MIN_SPEED 		    500
#define EASYLASE_MAX_SPEED 		    65000
#define EASYLASE_SPEED_STEP		    500

// int easyLaseWriteFrameUncompressed(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed);


// int easyLaseWriteFrame(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed);
/** int easyLaseWriteFrame(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed);
 * Send a frame from dataBuffer to card number cardNumber. Maxmimum framesize
 * is 16.000 points or 128 Kbytes. 
 * cardNumber: selected device, 0 .. number of cards - 1
 * dataBuffer: pointer to a frame, i.e. an array of EasyLasePoint s 
 * dataCounter: number of bytes in dataBuffer = number of points in frame
 *              multiplied by 8
 * speed: output speed in points per second, 500 .. 65000
 * Return value: 0 = success, < 0 = error.
 */

// int easyLaseWriteFrameNR(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed, unsigned int repeat);
/** int easyLaseWriteFrameNR(int cardNumber, unsigned char *dataBuffer, unsigned int dataCounter, unsigned int speed, unsigned int repeat);
 * Send a frame like EasyLaseWriteFrame, but without automatic frame repeat, if
 * there is no new frame following the actual one. Output stops at the last
 * point until new frame comes in.
 * cardNumber: selected device, 0 .. number of cards - 1
 * dataBuffer: pointer to a frame, i.e. an array of EasyLasePoint s 
 * dataCounter: number of bytes in dataBuffer = number of points in frame * 8
 * speed: output speed in points per second, 500 .. 65000
 * repeat: number of repetitions
 * Return value: 0 = success,  <0 = error.
 */


//int easyLaseStop(int cardNumber);
/** int easyLaseStop(int cardNumber);
 * Output is stopped. USB connection will be left open.
 * Return value: 0 = success,  <0 = error.
 */

// int easyLaseWriteDMX(int cardNumber, unsigned char * dmxBuffer);
/** int easyLaseWriteDMX(int cardNumber, unsigned char * dmxBuffer);
 * (old) Send of 512 bytes DMX-data from dmxBuffer to card number cardNumber.
 * The byte array dmxBuffer contains exactly  512 bytes.
 * It is recommended not to use this function, when significantly
 * less than 512 channels are used!
 * Return value: 0 = success,  <0 = error.
 */

// int easyLaseDMXOut(int cardNumber, unsigned char * dmxBuffer, unsigned int baseAddress, unsigned int channelCount);
/** int easyLaseDMXOut(int cardNumber, unsigned char * dmxBuffer, unsigned int baseAddress, unsigned int channelCount);
 * (new) Output of n-DMX-databytes (ChannelCount) starting at dmxBuffer
 * from card number cardNumber. The card outputs these DMX-channels starting
 * from base address baseAddress.
 * The length of the byte array dmxBuffer is the number of channels channelCount.
 * Example: Writing 4 channels from baseaddress 16, the array contains 4 bytes
 * (not 20).
 * cardNumber: selected device, 0 .. number of cards - 1
 * baseAddress: start address, value 0 ..– 511 (note that Baseaddress starts from 0)
 * channelCount: number of channels to transfer, value 1 .. 512
 * Return value: 0 = success,  <0 = error or invalid parameters.
 * A value of baseAddress > 511 or channelCount = 0 or the sum of baseAddress and
 * channelCount >512 will return -1.
 * It is recommended to use this function, when significantly less than 512
 * DMX-channels are used.
 */

// int easyLaseGetDMX(int cardNumber, unsigned char * dmxBuffer);
/** (old) Read 512 bytes DMX data from card number cardNumber to buffer dmxBuffer.
 * The dataarray dmxBuffer exactly contains 512 bytes.
 * Return value: 0 = success,  <0 = error.
 * It is recommended not to use this function, when significantly less than 512
 * channels are used!
 */


// int easyLaseDMXIn(int cardNumber, unsigned char * dmxBuffer, unsigned int baseAddress, unsigned int channelCount);
/** (new)  Reading of n-DMX-databytes channelCount starting at dmxBuffer from card
 * number cardNumber. The card sends these DMX-channels via USB starting from base
 * address baseAddress.
 * The length of the byte array @dmxbuffer is the number of channels channelCount.
 * Example: Writing 4 channels from baseaddress 16, the array contains 4 bytes
 * (not 20).
 * cardNumber: selected device, 0 .. number of cards - 1
 * baseAddress: start address, value 0 ..– 511 (note that Baseaddress starts from 0)
 * channelCount: number of channels to transfer, value 1 .. 512
 * Return value: 0 = success,  <0 = error or invalid parameters.
 * A value of baseAddress > 511 or channelCount = 0 or the sum of baseAddress and
 * channelCount >512 will return -1.
 * It is recommended to use this function, when significantly less than 512
 * DMX-channels are used.
 */

// int easyLaseWriteTTL(int cardNumber, unsigned int ttl);
/** int easyLaseWriteTTL(int cardNumber, unsigned int ttl);
 * Send 16 Bit TTL-value ttl to card number cardNumber.
 */

// int easyLaseGetLastError(int cardNumber);
/** int easyLaseGetLastError(int cardNumber);
 * returns the last status of the USB driver (0= okay, !=0 = error).
 */

#define EASYLASE_GET_STATUS_READY       0x01
#define EASYLASE_GET_STATUS_BUSY        0x02

// int easyLaseGetStatus(int cardNumber);
/** int easyLaseGetStatus(int cardNumber);
 * Returns the status of the on board framebuffers.
 * Return Value: 0 = USB error
 *               1 = Ready (a new frame can be received)
 *               2 = Busy (all buffers full, incoming frames will be discarded)
 */


// int easyLaseClose();
/** int easyLaseClose();  //Call this when closing the application!
 * Close all USB connections to EasyLase cards.
 * Return value: 0 = success,  <0 = error.
 */

#endif
