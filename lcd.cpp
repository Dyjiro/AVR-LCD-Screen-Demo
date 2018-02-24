// Date:           28 January 2018
//----------------------------------------------------------------------//

#include "lcd.h"
#include "timer.h"

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins()
{
    //Pins connected to Enable and RS
    DDRB |= (1 << DDB4 | 1 << DDB6);

    //Pins connected to DB[7:4]
    DDRA |= (
      1 << DDA0 |
      1 << DDA1 |
      1 << DDA2 |
      1 << DDA3);
}

/*
 * Sends an 4 bit command to the LCD control
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay)
{
  PORTA = ((data & 0x0F));         //Set the lower 4 bits in PORTA

  PORTB &= ~(1 << PORTB6);         //Sets RS low
  PORTB |= (1 << PORTB4);          //Sets E high to enable

  delayUs(1);                      //Delay for the enable bit to switch

  PORTB &= ~(1 << PORTB4);         //Sets E low to enable

  if(delay > 0)
  {
    delayUs(delay);                //Delay the amount to send a command + a buffer
  }
}

/*
 * Sends an 8 bit command to the LCD control
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay)
{
  //Sends the higher 4 bits of character
  PORTA = ((command & 0xF0) >> 4); //Set the higher 4 bits in PORTA

  PORTB &= ~(1 << PORTB6);         //Sets RS high
  PORTB |= (1 << PORTB4);          //Sets E high to enable

  delayUs(1);                      //Delay for the enable bit to switch

  PORTB &= ~(1 << PORTB4);         //Sets E low to enable

  //Sends the lower 4 bits of character
  PORTA = ((command & 0x0F));      //Set the lower 4 bits in PORTA

  PORTB &= ~(1 << PORTB6);         //Sets RS low
  PORTB |= (1 << PORTB4);          //Sets E high to enable

  delayUs(1);                      //Delay for the enable bit to switch

  PORTB &= ~(1 << PORTB4);         //Sets E low to enable

  if(delay > 0)
  {
    delayUs(delay);                //Delay the amount to send a command + a buffer
  }
}

/*
 * Writes a provided character to the LCD screen
 */
void writeCharacter(unsigned char character)
{

  //Sends the higher 4 bits of character
  PORTA = ((character & 0xF0) >> 4);   //Set the higher 4 bits in PORTA

  PORTB |= (1 << PORTB6);         //Sets RS high
  PORTB |= (1 << PORTB4);         //Sets E high to enable

  delayUs(1);                     //Delay for the enable bit to switch

  PORTB &= ~(1 << PORTB4);        //Sets E low to enable


  //Sends the lower 4 bits of character
  PORTA = ((character & 0x0F));   //Set the lower 4 bits in PORTA

  PORTB |= (1 << PORTB6);         //Sets RS high
  PORTB |= (1 << PORTB4);         //Sets E high to enable

  delayUs(1);                     //Delay for the enable bit to switch

  PORTB &= ~(1 << PORTB4);        //Sets E low to enable


  delayUs(50);                    //Delay the amount to write a character + a buffer
}

/*
 * Writes a provided string to the LCD screen.
 */
void writeString(const char *string)  // Easter Egg
{
  //Goes through each character in string to write to LCD
  for(unsigned int i = 0; i < 100; i++)
  {
    if(string[i] == '\0')       //end of string, end it
    {
      break;
    }
    writeCharacter(string[i]);  //write character to the LCD
  }
}

/*
 * This moves the LCD cursor to a specific place on the screen.
 * Using the eightBitCommandWithDelay
 */
void moveCursor(unsigned char x, unsigned char y)
{
  unsigned int position = ((y * 16) + x);         //Uses y as which row to use, and x as pos in row in Hex
  eightBitCommandWithDelay(0x80 | position, 50);  //Moves cursor to assigned position given 0x80 as initialization
}


/*
 *Initializes the LCD and turns it on using data sheet procedure
 */
void initLCDProcedure(){
  // Delay 15 milliseconds to wait till VDD reaches 4.5V
  delayUs(15000);

  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(0b00000011, 4100);

  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  fourBitCommandWithDelay(0b00000011, 100);

  // write 0b0011 to DB[7:4] and no delay
  // write 0b0010 to DB[7:4] and delay.
  eightBitCommandWithDelay(0b00110010, 45);

  // Function set in the command table
  eightBitCommandWithDelay(0b00101000, 45);

  // Display off in the command table
  eightBitCommandWithDelay(0b00001000, 45);

  // Clear display in the command table.
  eightBitCommandWithDelay(0b00000001, 1700);

  // Entry Mode Set in the command table.
  eightBitCommandWithDelay(0b00000110, 45);

  // Display ON/OFF Control in the command table.
  eightBitCommandWithDelay(0b00001111, 45);

}

/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}
