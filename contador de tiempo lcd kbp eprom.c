#include <16f877.h>                                // Biblioteca de PIC16F977
#fuses XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP     // Configuración de fusibles
#use delay(clock=4000000) // FOSC = 4Mhz
#define use_portd_lcd true
#define use_portb_kbd true
#include <2404.c>                                  // Biblioteca de EEPROM
#include <kbd.c>                                   //Driver creado para manejo de teclado por puerto B
#include <lcd.c>                                   //Driver para el LCD por el puerto D
#include <stdlib.h>                                 
#define LED PORTC,0
#byte PORTC=7

// Variables globales
int minutos;
int n;            // Guarda el numero de digitos ingresados desde el teclado
int tiempo;       // Guarda el tiempo del temporizador
char k;           // Guarda el caracter que se ingresa el pulsar el teclado
char digit[3];    // Guarda los digitos ingresados desde el teclado 

void PedirTiempo()
{
   lcd_putc("\f INGRESE EL \n PRIMER DIGITO: ");
   while(n < 2)
   {
      k = kbd_getc();
      if(k =='*' || k == '#')
      {
         lcd_putc("\f Caracter invalido.");
         delay_ms(400);
         lcd_putc("\f INGRESE EL \n EL DIGITO: ");
         continue;
      }
      if( k != 0 )
      {
         printf(lcd_putc,"\f VALOR INGRESADO: \n %c  ", k );
         delay_ms(400);
         digit[n] = k;
               
         if(n < 1)
            lcd_putc("\f INTRODUZCA EL \n SEGUNDO DIGITO:");
                  
            n++;
      }
   }
   digit[2] = '\0';
   WRITE_EXT_EEPROM(0, digit);
   printf(lcd_putc,"\f VALOR INGRESADO: \n %d", atoi(digit));
}

void Contador()
{
   minutos = atoi(READ_EXT_EEPROM(0));
   //minutos = atoi(digit);
   tiempo = 0;
   printf(lcd_putc,"\f T: %d min \n ALARMA: %d min", tiempo, minutos);
   while(tiempo < minutos)
   {
      int i = 0;
      while(i < 60)
      {
         delay_ms(10);
         i++;
      }
      tiempo++;
      printf(lcd_putc,"\f T: %d min \n ALARMA: %d min", tiempo, minutos);
   }
   
   set_tris_C(0B11111110); // Configurar el puerto C
   bit_set(LED);
   lcd_putc("\f ALARMA \n FINALIZADO");
}

void main()
{
   // Inicializamos el LCD, teclado y memoria EEPROM
   lcd_init();
   kbd_init();
   INIT_EXT_EEPROM();
   
   port_b_pullups(true);
   bit_clear(LED);
   
   // Comenzamos el ciclo principal del programa
   lcd_putc("\f PULSA '*' \n PARA INICIAR.");
 
   while(true)
   {
      k = kbd_getc();   // Obtenemos el caracter
      // Si se pulsa * pedimos el tiempo
      if(k == '*')      
      {
          PedirTiempo();
          Contador();
      } 
   }
}

            
