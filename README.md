# platformio
source ~/.platformio/penv/bin/activate
pio project init --ide eclipse
# weather2



# esp8266 reference
http://www.kloppenborg.net/blog/microcontrollers/2016/08/02/getting-started-with-the-esp8266

##wemos
Pin | Function    | ESP-8266 Pin | used
TX  | TXD         | TXD          |
RX  | RXD         | RXD          |
A0  | ADC, 3.3V   | A0           | photoresistor
D0  | IO          | GPIO16       |
D1  | IO, SCL     | GPIO5        | 
D2  | IO, SDA     | GPIO4        |
D3  | IO, 10k P-up| GPIO0        | 
D4  | IO, 10k P-up,LED|   GPIO2  | 
D5  | IO, SCK     | GPIO14       | 
D6  | IO, MISO    | GPIO12       | DHT.data
D7  | IO, MOSI    | GPIO13       | 
D8  | IO, 10k P-down, SS|  GPIO15|
G   | Ground      | GND          |
5V  | 5V          | -            |
3V3 | 3.3V        | 3.3V         |
RST | Reset       | RST          |


#ADC wemos
 -----A0
  |
 220K
  |--- ADC
 100K
  |
 GND
 
#photoresistor
(+5) -termistor- (A0) -resistor 10k- (GND)

#DHT
1 vcc (3.3V)
2 data (D6)
3 nc
4 GND (G)


