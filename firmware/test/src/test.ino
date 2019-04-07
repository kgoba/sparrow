/*
  (c) Dragino Project, https://github.com/dragino/Lora
  (c) 2017 Karlis Goba
*/

#include <avr/wdt.h>
#include <RH_RF95.h>

#define LED_PIN        13

#define LORA_CS_PIN     8
#define LORA_RST_PIN    4
#define LORA_IRQ_PIN    7

#define FREQUENCY_MHZ 434.25
#define TX_POWER_DBM    5

#define MODEM_MODE RH_RF95::Bw125Cr45Sf128

RH_RF95     lora(LORA_CS_PIN, LORA_IRQ_PIN);

void setup() {
		Serial.begin(9600);

		// mega32u4 only
  	while (!Serial);

    Serial.println("RESET");
	  
		//wdt_enable(WDTO_2S);

    lora_setup();

		pinMode(LED_PIN, OUTPUT);
}

void lora_setup()
{
    for (int n_try = 0; n_try < 5; n_try++) {
        // reset LoRa module to make sure it will works properly
        pinMode(LORA_RST_PIN, OUTPUT);
        digitalWrite(LORA_RST_PIN, LOW);   
        delay(1000);
        digitalWrite(LORA_RST_PIN, HIGH);

        //wdt_reset();

        if (lora.init()) break;
        Serial.println("LoRa init failed, retrying...");
    }

    // Defaults after init are 434.0MHz, 13dBm
    // Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    lora.setModemConfig(MODEM_MODE);
    lora.setSpreadingFactor(11);
    lora.setFrequency(FREQUENCY_MHZ);
    lora.setTxPower(TX_POWER_DBM);	
}


void loop() {
  	digitalWrite(LED_PIN, HIGH);
    lora_transmit();
		//lora.waitPacketSent();
		delay(100);
		digitalWrite(LED_PIN, LOW);
		delay(3000);
}

/// Constructs payload message and transmits it via radio
void lora_transmit() {
    char tx_buf[80];    // Temporary buffer for LoRa message

    strcpy(tx_buf, "Hello");
		
    // Log the message on serial
    Serial.print(">>> "); Serial.println(tx_buf);
    // Send the data to server
    lora.send((const uint8_t *)tx_buf, 64 /*strlen(tx_buf)*/);
}
