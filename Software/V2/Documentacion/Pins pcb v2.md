# Documentacion firmware V2
## Pines ATmega PCB V2

Pin TQFP | Puerto | Pin Arduino | Funciones internas | PCB            | Estados
---------|--------|-------------|--------------------|----------------|--------
1        | PD3    | 3           | PCINT19/OC2B/INT1  | EXP            | X
2        | PD4    | 4           | PCINT20/XCK/T0     | EXP            | X
9        | PD5    | 5           | PCINT21/OC0B/T1    | EXP            | X
10       | PD6    | 6           | PCINT22/OC0A/AIN0  | ESP_GPIO13_3V3 | 0 1 Z
11       | PD7    | 7           | PCINT23/AIN1       | RFID_OC_NRST   | 0 Z
12       | PB0    | 8           | PCINT0/CLKO/ICP1   | RFID_SS_3V3    | 0 1
13       | PB1    | 9           | PCINT1/OC1A        | LED            | 0 1
14       | PB2    | 10          | PCINT2/SS/OC1B     | EXP            | X
15       | PB3    | 11          | PCINT3/OC2A/MOSI   | RFID_MOSI_3V3  | 0 1
16       | PB4    | 12          | PCINT4/MISO        | RFID_MISO_3V3  | Z
17       | PB5    | 13          | SCK/PCINT5         | RFID_SCK_3V3   | 0 1
19       | ADC6   | -           |                    | EXP            |
22       | ADC7   | -           |                    | BOTON          | Z
23       | PC0    | A0          | ADC0/PCINT8        | BUZZER         | 0 1
24       | PC1    | A1          | ADC1/PCINT9        | RELE           | 0 1
25       | PC2    | A2          | ADC2/PCINT10       | HDMI_A_5V      | Z
26       | PC3    | A3          | ADC3/PCINT11       | HDMI_B_5V      | Z
27       | PC4    | A4          | ADC4/SDA/PCINT12   | SDA            | 0 1 Z
28       | PC5    | A5          | ADC5/SCL/PCINT13   | SCL            | 0 1 Z
29       | PC6    | RESET       | RESET/PCINT14      | RST_ATM_3V3    | Z
30       | PD0    | 0           | RXD/PCINT16        | ESP_TX_3V3     | Z
31       | PD1    | 1           | TXD/PCINT17        | ESP_RX_3V3     | 0 1
32       | PD2    | 2           | INT0/PCINT18       | EXP            | X

## Pines ESP PCB V2

Pin fisico | Puerto | Pin Arduino | Funciones internas | PCB            | Estados
-----------|--------|-------------|--------------------|----------------|--------
1          | RST    |             | RESET              | ESP_OC_NRST    |
2          | ADC    | A0          | TOUT               |                |
4          | GPIO16 | 16          | WAKE               |                | X
5          | GPIO14 | 14          | HSPICLK            | BOTON          | Z
6          | GPIO12 | 12          | HSPIQ              |                | X
7          | GPIO13 | 13          | HSPID/CTS0/RXD2    | ESP_GPIO13_3V3 | 0 1 Z
10         | GPIO15 | 15          | HSPICS/RTS0/TXD2   |                | Z
11         | GPIO2  | 2           | TXD1               |                | Z
12         | GPIO0  | 0           | SPICS2             | ESP_OC_NPROG   | Z
13         | GPIO4  | 4           |                    | RST_ATM_3V3    | 0 1
14         | GPIO5  | 5           |                    |                | X
15         | GPIO3  | 3           | RXD0               | ESP_RX_3V3     | Z
16         | GPIO1  | 1           | SPICS1/TXD0        | ESP_TX_3V3     | 0 1
17         | CS0    |             | SPI_INT/SD_D1      |                |
18         | MISO   |             | SPI_MISO/SD_D0     |                |
19         | GPIO9  |             | SD_D2              |                |
20         | GPIO10 |             | SPI_CS/SD_D3       |                |
21         | MOSI   |             | SPI_MOSI/SD_CMD    |                |
22         | SCLK   |             | SPI_CLK/SD_CLK     |                |

###Notas
Los estados permitidos de los pines se especifican para los pines configurables, ademas:
- Salida:
    - 0 = 0V
    - 1 = 3.3V 5V
- Entrada = Z (Alta impedancia)
- X = 0 1 Z (No importa pero deberian quedar en Z con pullup interno si no se usan)

El nodo ESP_GPIO13_3V3 conecta PD6 del ATmega con GPIO13 en el ESP, es importante que no queden ambos configurados como salida al mismo tiempo, siempre tiene que haber uno en Z.
