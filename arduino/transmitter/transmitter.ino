#include <Arduino.h>
#include <cc1101.h> // https://github.com/veonik/arduino-cc1101
#include <ccpacket.h>

// Attach CC1101 pins to their corresponding SPI pins
// Uno pins:
// CSN (SS) => 10
// MOSI => 11
// MISO => 12
// SCK => 13
// GD0 => Not used in Transmitter

CC1101 radio;

byte syncWord[2] = {199, 10};

unsigned long lastSend = 0;
unsigned int sendDelay = 1000;

void setup() {
    Serial.begin(115200);
    radio.init();
    radio.setSyncWord(syncWord);
    radio.setCarrierFreq(CFREQ_433);
    radio.disableAddressCheck();
    radio.setChannel(0);
    radio.setTxPowerAmp(PA_LongDistance);

    Serial.print(F("CC1101_PARTNUM "));
    Serial.println(radio.readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
    Serial.print(F("CC1101_VERSION "));
    Serial.println(radio.readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
    Serial.print(F("CC1101_MARCSTATE "));
    Serial.println(radio.readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER) & 0x1f);

    Serial.println(F("CC1101 radio initialized."));
}

void loop() {
    unsigned long now = millis();
    if (now > lastSend + sendDelay) {

        lastSend = now;
        //const char *message = "hello world";
        char message[64];
        sprintf(message, "hello world %ld", now);
        CCPACKET packet;
        // We also need to include the 0 byte at the end of the string
        packet.length = strlen(message)  + 1;
        strncpy((char *) packet.data, message, packet.length);

        radio.sendData(packet);
        Serial.println(F("Sent packet..."));

    }
}
