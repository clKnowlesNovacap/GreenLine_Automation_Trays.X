/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F47Q10
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <xc.h>

#define LEFT 0
#define RIGHT 1

#define TRUE 1
#define FALSE 0

// Declarations
void TX_Buffer1(uint8_t*, uint8_t);
void TX_Buffer2(uint8_t*, uint8_t);
unsigned int distance(float);
char LaySheetsOnTrays(uint16_t, int);
extern int INT0_Read_Count(void); 
extern void INT0_clearRotaryCount(void);   
extern void INT0_Set_ScaledValuePast(void);
extern int INT0_GetDirection(void);
extern uint8_t EUSART1_Read(void);

// LCD Proto
typedef struct {
    int sheets; 
    int sheetSize;
} lcd_struct;

lcd_struct input;

int lcd_page_home(void);
char lcd_GUI(void);
void lcd_show_values(int,int);
int lcd_default_setup(void);
void lcd_page_1(void);
void lcd_page_2(void);
int lcd_page_3(void);
void lcd_print(char *str);
void lcd_clear(void);
void lcd_turn_on(void);
void lcd_set_brightness(uint8_t);
void lcd_set_cursor(uint8_t);
void home_motors(void);

// Motor Proto
void rmove(uint8_t, uint16_t, uint16_t);

char quickTime=FALSE, start=FALSE, status=0;
/*
                         Main application
 */
void main(void){
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    

//    uint8_t size; 
//    uint8_t a, setup=1;
//    lcd_struct values;
//    char lcd_string[16];
    
    
    lcd_clear();
    __delay_ms(500);
    EXT_INT0_InterruptEnable();
//    DATAEE_WriteByte(0, 250);
//    DATAEE_WriteByte(2, 6);
    input.sheets = 205;
    input.sheetSize = 6;
    while(1){
        
        if(lcd_GUI() == LEFT) {
            lcd_clear();
            __delay_ms(100);
            status = LaySheetsOnTrays(input.sheets,input.sheetSize);
        }
        else {
            lcd_clear();
            __delay_ms(500);
            lcd_set_cursor(64);
            lcd_page_1();
            lcd_page_2();
        }  
      
    }
} // End of main


/************** Functions *****************/

    char lcd_GUI(void){
        /*
        This Function contains all the LCD functions
        that will be used to control and monitor
        the LCD screen and asks the user if the
        default values are okays
        */

        int sheetsEEprom;
        int LengthEEprom;
        char lcd_string[16];
        int select_bool = 0;
        int i = 0, pastCount, presentCount;
        char location = LEFT;
        char selectSwitch = FALSE;
        
        presentCount=pastCount=INT0_Read_Count();
        
        sheetsEEprom = DATAEE_ReadByte(0);      // Read sheets from EEPROM 
        sheetsEEprom = sheetsEEprom + (DATAEE_ReadByte(1) <<8);
        
        LengthEEprom = DATAEE_ReadByte(2);      // Read sheets from EEPROM 
        LengthEEprom = LengthEEprom + (DATAEE_ReadByte(3) <<8);
        if(input.sheets <= 9){
            sprintf(lcd_string,"%u    Sheets %u\"     ",input.sheets,input.sheetSize);
            }
        else if(input.sheets <= 99 && input.sheets > 9){
            sprintf(lcd_string,"%u   Sheets %u\"    ",input.sheets,input.sheetSize);
            }
        else{
            sprintf(lcd_string,"%u Sheets %u\"  ",input.sheets,input.sheetSize);
        }
        lcd_set_cursor(0);
        lcd_print(lcd_string);
        lcd_set_cursor(64);
        lcd_print("  Start  Change   ");

        while(1){ // If start was selected Show Current Values   
            
            
            if(location == LEFT)
            {    
                lcd_set_cursor(64);
                lcd_print("  Start  Change   ");  
                __delay_ms(300);
                lcd_set_cursor(64);
                lcd_print("         Change   "); 
                __delay_ms(300);
            }
            else
            {    
                lcd_set_cursor(64);
                lcd_print("  Start  Change   ");  
                __delay_ms(300);
                lcd_set_cursor(64);
                lcd_print("  Start           "); 
                __delay_ms(300);
            }
            
            presentCount=INT0_Read_Count();
            
            if(pastCount != presentCount)
            {
                if(pastCount>presentCount)
                    location = LEFT;
                else
                    location = RIGHT;
                
                pastCount = presentCount;
            }
            
            if(PORTCbits.RC2 == 0)
                selectSwitch=TRUE;
            else
                selectSwitch=FALSE;
                        
            
            if(selectSwitch==TRUE) 
            {
                if(location == LEFT ) {
                    __delay_ms(1);
                    return LEFT;
                    break;
                }
                else {
                    __delay_ms(1);
                    return RIGHT;
                    break;                    
                }
            }
        } // End of while(1)
    } // End LCD_GUI

    void lcd_page_1(void){
        //Setup
        int sheets = 0;
        INT0_clearRotaryCount();
        
        while(1){   
            sheets = INT0_Read_Count();   // Reads Encoder for sheets 
            char lcd_string[16];    
            if(sheets <= 9){
                sprintf(lcd_string,"# of Sheets:  %i", sheets); // Display sheets
            }
            else if(sheets <= 99 && sheets > 9){
                sprintf(lcd_string,"# of Sheets: %i", sheets); // Display sheets
            }
            else{
                sprintf(lcd_string,"# of Sheets:%i", sheets); // Display sheets
            }
            
            lcd_set_cursor(0);
            lcd_print(lcd_string);       
            lcd_set_cursor(64);
            lcd_print("        Max 999");

            if(PORTCbits.RC2==0) { // If switch is pressed goto screen 2
                __delay_ms(50);
                while(PORTCbits.RC2==0);  // Wait for switch to be un-pressed
                input.sheets = sheets;    // Store sheets in structure
                break;
            }
                
        }
    } // End Lcd Page 1
    

    void lcd_page_2(void){
        int i = 0, pastCount, presentCount;
        char location = LEFT;
        char selectSwitch = FALSE;
        lcd_set_cursor(0);
        lcd_print("   Tape Length  " );
        lcd_set_cursor(64);
        lcd_print("     6\"  12\"    "); 
        while(1){

            if(location == LEFT) {    
                lcd_set_cursor(64);
                lcd_print("     6\"  12\"    ");  
                __delay_ms(300);
                lcd_set_cursor(64);
                lcd_print("         12\"    "); 
                __delay_ms(300);
            }
            else {    
                lcd_set_cursor(64);
                lcd_print("     6\"  12\"    ");   
                __delay_ms(300);
                lcd_set_cursor(64);
                lcd_print("     6\"         "); 
                __delay_ms(300);
            }
            
            presentCount=INT0_Read_Count();
            
            if(pastCount != presentCount)
            {
                if(pastCount>presentCount)
                    location = LEFT;
                else
                    location = RIGHT;
                
                pastCount = presentCount;
            }
            
            if(PORTCbits.RC2 == 0)
                selectSwitch=TRUE;
            else
                selectSwitch=FALSE;
                        
            if(selectSwitch==TRUE) 
            {
                if(location == LEFT ) {
                    __delay_ms(1);
                    input.sheetSize = 6;
                    break;
                }
                else {
                    __delay_ms(1);
                    input.sheetSize = 12;
                    break;                    
                }
            }
        }
    }

    int lcd_page_3(void) {
       //EXT_INT0_InterruptDisable();
       int sheets = input.sheets;       // Load sheets and trays from Global structure
       int tape_length = input.sheetSize;
       char Byte_;
       char lcd_string[40];

       lcd_set_cursor(0);
       lcd_show_values(sheets, tape_length);   // Display sheets and tray value on LCD
       
       Byte_= (char)(sheets & 0xff);    //Save sheets EEPROM at address 0, 1
       DATAEE_WriteByte(0, Byte_);
       __delay_ms(11);
       Byte_= (char)(sheets>>8);
       DATAEE_WriteByte(1, Byte_);
       __delay_ms(11);
       Byte_= (char)(tape_length & 0xff);     //Save trays EEPROM at address 2, 3
       DATAEE_WriteByte(2, Byte_);
       __delay_ms(11);
       Byte_= (char)(tape_length>>8);
       DATAEE_WriteByte(3, Byte_);
       __delay_ms(11);
       return 1;
   }
   // LCD Functions

   void lcd_print(char *str){
       char Buffer[16];
       strcpy(Buffer,str);
       TX_Buffer2(Buffer, 15);
   }

   void lcd_clear(void){
       EUSART2_Write(0xfe);
       EUSART2_Write(0x51);
       __delay_ms(2);
   }
    void lcd_turn_on(void){
       EUSART2_Write(0xfe);
       EUSART2_Write(0x41);
       __delay_us(200);
   }
    
    void lcd_set_brightness(uint8_t level){
        EUSART2_Write(0xfe);
        EUSART2_Write(0x53);
        EUSART2_Write(level);
        __delay_us(200);
    }

    void lcd_set_cursor(uint8_t pos){
        EUSART2_Write(0xfe);
        EUSART2_Write(0x45);
        EUSART2_Write(pos);
        __delay_us(200);
    }

    // MOTOR FUNCTIONS
    void rmove(uint8_t motor, uint16_t speed, uint16_t pos){
        uint8_t speed_buf[40];
        uint8_t move_buf[40];
        sprintf(speed_buf,"@%i ACCF %i\n",motor, speed);
        TX_Buffer1(speed_buf, strlen(speed_buf));
        __delay_ms(1);
        sprintf(move_buf,"@%i RMOV %i\n",motor, pos);
        TX_Buffer1(move_buf, strlen(move_buf));
        __delay_ms(1);
    }
    
    void home_motors(void){
        // Sends the Motors Back
        uint8_t speed_buf[40];
        uint8_t move_buf[60];
        sprintf(speed_buf,"@1 ACCF 5000\n");
        TX_Buffer1(speed_buf, strlen(speed_buf));
        __delay_ms(1);
        sprintf(move_buf,"@1 RMOV -100000 -100000 -100000 -100000 -100000\n");
        TX_Buffer1(move_buf, strlen(move_buf));
        __delay_ms(1);
        rmove(6,3000,-1000000);
        // Unstucks the Motors from Limit Switches 
        for(int i = 0; i >= 50; i++){
                sprintf(move_buf,"@1 RMOV 1 1 1 1 1 1\n");
                TX_Buffer1(move_buf, strlen(move_buf));
                __delay_ms(1);
        }
    }

    char LaySheetsOnTrays(uint16_t sheetCount, int sheetSize){
        home_motors();
        uint16_t forwardCount, returnCount, lateralCount, totalSheetsPlaced=0;
        uint8_t motorNumber;
        char lcd_string[16];
        int trayLength = 26;                    //inches
        int sheetsPerTray = sheetCount/5;
        float trayStepSize = (trayLength - sheetSize)/(sheetsPerTray- 1);
        if(sheetsPerTray > 5){
            forwardCount = distance(sheetSize);       // Distance to lay down one sheet
            returnCount = -distance(sheetSize - trayStepSize);     // Distance to move back for next sheet
            lateralCount = distance(8.25);    // Distance to move to next tray 
        }
        else {
            forwardCount = distance(6);       // Distance to lay down one sheet
            returnCount = -distance(5.75);     // Distance to move back for next sheet
            lateralCount = distance(8.25);    // Distance to move to next tray
        }       
        lcd_set_cursor(0);
        sprintf(lcd_string,"Total Sheets %u ",sheetCount);
        lcd_print(lcd_string);
       
        motorNumber = 1;
       
        while(sheetCount) {
           while (PORTCbits.RC3==1);
        
           rmove(motorNumber, 5000,  -forwardCount);    // Move motor to lay down sheet
           while( EUSART1_Read != '!'); // Wait for motor to stop moving
           
           rmove(motorNumber, 5000,  returnCount);    // Return motor for next sheet
           while( EUSART1_Read != '!'); // Wait for motor to stop moving
           
           if(--sheetCount <= 0)
               break;
           else {
               ++totalSheetsPlaced;
               lcd_set_cursor(0);
               sprintf(lcd_string,"Total Sheets %u ",totalSheetsPlaced);
               lcd_print(lcd_string);       
           }   
           
           while (PORTCbits.RC3==0);    // Wait for sheet to pass through
       }     
   }
   void waitForSuccessSignal(void){
       // Put code here for checking if the motors are done
       // Read in From Ocean Controller for "!" after the 
       // Motors are done turning
   }

    void TX_Buffer1(uint8_t *buff, uint8_t size) {
        uint8_t i;
        
        for(i=0;  i<size; i++)
            EUSART1_Write(buff[i]);                
       EUSART1_Write(0x0A);
       EUSART1_Write(0x0D);
   } 
   void TX_Buffer2(uint8_t *buff, uint8_t size){ 
        uint8_t i;
    
        for(i=0;  i<size; i++)
            EUSART2_Write(buff[i]);
   } 
   
      unsigned int distance(float inch) {
       return (uint16_t)inch*4064;
   }

/*
 End of File
*/