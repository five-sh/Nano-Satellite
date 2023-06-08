#include "send.h"

const int ss = 5;
const int rst = 14;
const int dio0 = 2;

void setup() {
        init_LoRa_sender(ss, rst, dio0);
}

void loop() {
        String s = "Message";
        send_message(s);
}