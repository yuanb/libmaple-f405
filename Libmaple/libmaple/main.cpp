// Sample main.cpp file. Demonstrates use of the ethernet, SDCARD peripherals.
// ALso scans I2C bus.

#include "wirish.h"
#include "HardWire.h"
#include "Wire.h"
#include "HardwareSPI.h"
#include <EtherCard/EtherCard.h>
#include <L3G4200D/L3G4200D.h>
#include <mapleSDfat/SdFat.h>
#include <mapleSDfat/SdFatUtil.h>
#include "HardwareSerial.h"

#define PWM_PIN  2

/**********************************************
 * Dedicated Netduino Plus 2 Peripheral I/O
 *********************************************/

#define NETDUINO_PLUS2_ETHERNET_SPI_INTERFACE 1  // STM32 SPI 1
#define NETDUINO_PLUS2_AVAILABLE_SPI_INTERFACE 2 // STM32 SPI 2
#define NETDUINO_PLUS2_SDCARD_SPI_INTERFACE 3    // STM32 SPI 3

// NOTE:  Hardware SPI is not yet ported to
// the STM32F405.
//
//HardWire HardWire(1,I2C_BUS_RESET);
//#define Wire HardWire

/******************************************************
 * Ethernet Global Variables
 *****************************************************/

HardwareSPI spi_eth(NETDUINO_PLUS2_ETHERNET_SPI_INTERFACE);
static byte mac_address[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[700];

/******************************************************
 * SDCard Global Variables
 *****************************************************/

HardwareSPI spi_sdcard(NETDUINO_PLUS2_SDCARD_SPI_INTERFACE);

Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

/******************************************************
 * Test I2C device (L3G4200D Gyro) Global Variables
 *****************************************************/

L3G4200D gyro;

/******************************************************
 * Test functions
 *****************************************************/

void testDNS();
void scanI2CBus();
void etherCardTest();
void sdCardTest();
void printIP( uint8_t *buf );
void gyroTest();

void setup() {
    /* Set up the LED to blink  */
    pinMode(BOARD_LED_PIN, OUTPUT);
    SerialUSB.begin();

    /* Turn on PWM on pin PWM_PIN */
    //pinMode(PWM_PIN, PWM);
    //pwmWrite(PWM_PIN, 0x8000);

    pinMode(28,OUTPUT);
    digitalWrite(28,HIGH);

    /* Send a message out USART2  */
    Serial2.begin(9600);
    Serial2.println("Hello world!");

    /* Send a message out the usb virtual serial port  */
    SerialUSB.println("Hello!");

    Wire.begin();
	gyro.initialize();
    //spi_eth.begin();
    spi_sdcard.begin(SPI_10_5MHZ,MSBFIRST,SPI_MODE_0);

    ENC28J60::setSPI(&spi_eth);

    //sdCardTest();

    //etherCardTest();

    Serial2.begin(9600); // 4800 baud is default for XBEE

	interrupts(); // Enable interrupts, required for HardWare.  Is this required?
}

void loop() {

    toggleLED();

    SerialUSB.println("Hello!");

    scanI2CBus();
    //testDNS();
    //gyroTest();

    delay(1500);
    Serial2.write("+++"); // enter AT command mode
    delay(1500);
    Serial2.write("AT");
    delay(10);
    bool got_data = false;
    while ( Serial2.available() )
    {
    	SerialUSB.print(Serial2.read());
    	got_data = true;
    }
    if ( got_data )
    {
    	SerialUSB.println();
    }

    delay(2000);	// wait a while before starting next scan
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
    init();
}

int main(void) {
    setup();

    while (true) {
        loop();
    }

    return 0;
}

// Output a ip address from buffer
void printIP( uint8_t *buf ) {
  for( int i = 0; i < 4; i++ ) {
    SerialUSB.print( buf[i], DEC );
    if( i<3 )
      SerialUSB.print( "." );
  }
}

/******************************************************
 * Basic test of the Netduino Plus 2's onboard
 * SD Card peripheral.  This particular test code
 * verifies basic read/write operation on a FAT32
 * volume.
 ******************************************************/

void sdCardTest() {

    if ( !card.init(&spi_sdcard) ) {
    	SerialUSB.println("Error initializing SD Card.");
    	return;
    }
    else {
    	SerialUSB.println("Successfully initialized SD Card.");
    }

    if ( !volume.init(&card) ) {
    	SerialUSB.println("Error initializing SD Card Volume.");
    	return;
    }
    else {
    	SerialUSB.println("Successfully initialized SD Card Volume.");
    }

    SerialUSB.print("File System Type:  ");
    uint8 fat_type = volume.fatType();
    switch( fat_type ) {
    case FAT_FILE_TYPE_CLOSED:
    	SerialUSB.println("CLOSED");
    	break;
    case FAT_FILE_TYPE_NORMAL:
    	SerialUSB.println("NORMAL");
    	break;
    case FAT_FILE_TYPE_ROOT16:
    	SerialUSB.println("FAT16");
    	break;
    case FAT_FILE_TYPE_ROOT32:
    	SerialUSB.println("FAT32");
    	break;
    default:
    	SerialUSB.println("UKNOWN!");
    	break;
    }

    // Open the root directory
    if ( !root.openRoot(&volume) ) {
    	SerialUSB.println("Error opening SD Card Volume Root.");
    	return;
    }
    else {
    	SerialUSB.println("Successfully opened SD Card Volume Root.");
    }

    // create a new file
    char name[] = "PRINT00.TXT";
    for (uint8_t i = 0; i < 100; i++) {
      name[5] = i/10 + '0';
      name[6] = i%10 + '0';
      // only create new file for write
      if (file.open(&root, name, O_CREAT | O_EXCL | O_WRITE)) break;
    }
    if (!file.isOpen()) {
    	SerialUSB.println("Unable to open file PRINTXX.TXT");
    	return;
    }

    // clear print error
    file.writeError = 0;

    // print 100 timestamped lines to file
    for (uint8_t i = 0; i < 100; i++) {
      file.print("line ");
      file.print(i, DEC);
      file.print(" millis = ");
      file.println(millis());
    }
    // force write of all data to the SD card
    if (file.writeError || !file.sync()) {
    	SerialUSB.println("Error printing to file or syncing the file.");
    }

    file.close();
}

/******************************************
 * Basic test of the Netduino Plus 2 onboard
 * ENC28J60 ethernet card, and also the
 * EtherCard library DHCP support.
 ******************************************/

void etherCardTest() {

	/*    if ( ether.doBIST() == 0 ) {
    	SerialUSB.println("ethernet nic built-in self test failed.");
    }
    else {
    	SerialUSB.println("ethernet nic built-in self test passed.");
    }*/

    if ( ether.begin(sizeof(Ethernet::buffer),mac_address) ) {
    	SerialUSB.println("Successfully initialized ENC28J60");
    }
    else {
    	SerialUSB.println("Error initializing ENC28J60");
    }

    SerialUSB.println("Requesting IP Address");
    // Get IP Address details
    if( ether.dhcpSetup() > 0 ) {
      // Display the results:
    	SerialUSB.print( "My IP: " );
    	printIP( ether.myip );
    	SerialUSB.println();

    	SerialUSB.print( "Netmask: " );
    	printIP( ether.mymask );
    	SerialUSB.println();

    	SerialUSB.print( "DNS IP: " );
    	printIP( ether.dnsip );
    	SerialUSB.println();

    	SerialUSB.print( "GW IP: " );
    	printIP( ether.gwip );
    	SerialUSB.println();
    } else {
      SerialUSB.println("Failed to get IP address");
    }
}

/***********************************************
 * Verify the libmaple Wire class can correctly
 * perform basic communication and also handle
 * a negative acknolwledgement by scanning the
 * I2C bus.
 ***********************************************/

void scanI2CBus() {

	byte error, address;
	int nDevices;

	SerialUSB.println("Scanning...");

	nDevices = 0;
	for(address = 1; address < 127; address++ )
	{
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		// This return value will be 2 (ENACKADDR)
		// if the i2c device doesn't acknowledge
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0)
		{
			SerialUSB.print("I2C device found at address 0x");
			if (address<16)
				SerialUSB.print("0");
			SerialUSB.print(address,HEX);
			SerialUSB.println("  !");

			nDevices++;
		}
		else if (error==4)
		{
			SerialUSB.print("Unknown error at address 0x");
			if (address<16)
				SerialUSB.print("0");
			SerialUSB.println(address,HEX);
		}
	}
	if (nDevices == 0)
		SerialUSB.println("No I2C devices found\n");
	else
		SerialUSB.println("done\n");

}

const char url[] PROGMEM = "www.kauailabs.com";

/*********************************************
 * Verify TCP/IP transmission reception
 * by acquiring an IP address from a DNS
 * Server
 ********************************************/

void testDNS() {

	if ( ether.dnsLookup( url ,true) ) {
		SerialUSB.print( url );
    	SerialUSB.print( " IP: " );
    	printIP( ether.hisip );
    	SerialUSB.println();
    }
    else {
   	 SerialUSB.println("DNS Lookup Failure");
    }

}

/**********************************************
 * Verify the i2cdevlib is working via the
 * libmaple Wire class by testing communication
 * with a STM Gyro.  Note that the gyro is not
 * included on the Netduino Plus 2 board.
 **********************************************/

void gyroTest() {

    if ( gyro.testConnection() ) {
    	SerialUSB.println("Gyro connection test successful.");
    }
    else {
    	SerialUSB.println("Gyro connection test failed.");
    }
    uint8_t devID = gyro.getDeviceID();
    SerialUSB.print("Gyro Device ID:  ");
    SerialUSB.println(devID);

}
