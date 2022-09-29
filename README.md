# Spectrum-analyzer-2.4-GHz
![Arduino Pro Mini](Scanner4.jpg)

### Wi-Fi radio frequency bands
![Wi-Fi radio frequency bands](ch1.jpg)


### 1.44" 128 x 128 TFT Color Display Module for arduino pro mini SPI ILI9163
* VCC  -->  3V        
* GND  -->  GND       
* GND  -->  GND
* NC   -->  ---
* NC   -->  ---
* LED  -->  3V        
* CLK  -->  TFT_SCLK   3 
* SDI  -->  TFT_MOSI   4  
* RS   -->  TFT_DC     5 
* RST  -->  TFT_RST    6      
* CS   -->  TFT_CS     7

### Adapter nRF24L01+ arduino pro mini
* CE           9 (changes in the sketch) Mode selection: receiver / transmitter
* CSN (CS/SS) 10 (changes in the sketch) SPI bus - device selection
* SCK         13 (SCK) SPI bus - clock line
* MO          11 (MOSI) SPI bus - data line (master to slave)
* MI          12 (MISO) SPI bus - data line (slave to master)
* IRQ         Not used Interrupt

# Device diagram
![Arduino Pro Mini](SH.jpg)
