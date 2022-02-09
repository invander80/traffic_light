#include <Streaming.h>
#include "RS_Schaltung.h"

RS_Schaltung RS;

void setup() {
  Serial.begin(9600);
  RS.schaltung_setup();
}

void loop() {
  RS.schaltung();
}
