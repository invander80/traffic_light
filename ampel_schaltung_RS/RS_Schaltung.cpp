#include "RS_Schaltung.h"
#include <Streaming.h>

// ### PinModes ################################################################
void RS_Schaltung::schaltung_setup()
{
  pinMode(ampelRot, OUTPUT);
  pinMode(ampelGelb, OUTPUT);
  pinMode(ampelGruen, OUTPUT);
  pinMode(rot, OUTPUT);
  pinMode(gelb, OUTPUT);
  pinMode(ampelTaster, INPUT);
  pinMode(r, INPUT_PULLUP);
  pinMode(s, INPUT_PULLUP);
}

// ### R-Taster ################################################################
void RS_Schaltung::reset_taster()
{
  if (Serial.available()) {
    serielle_ausgabe();
    }
  if (digitalRead(r) == HIGH && intervall(250) == true) {
    ampelAus = true;
    ampelState = false;
    Serial << "\nR-Taster wurde betätigt. (Arduino)" << endl;
  }
}

// ### S-Taster ################################################################
void RS_Schaltung::start_taster()
{
  if (Serial.available()) {
    serielle_ausgabe();
    }
  if (digitalRead(s) == HIGH && intervall(300) == true) {
    ampelState = true;
    ampelAus = false;
    Serial << "\nS-Taster wurde betätigt. (Arduino)" << endl;
  }
}

void RS_Schaltung::ampel_taster()
{
  if (Serial.available()) {
    if (counter == 1) {
      serielle_ausgabe();
    }
  }
}

// ### Analoges Schreiben (Funktion für Python) ################################
void RS_Schaltung::analoger_taster_s(char c, int parser)
{
  if (c == 's') {
    ampelState = true;
    ampelAus = false;
    analogWrite(s, parser);
    Serial << "\nS-Taster wurde betätigt. (Py-Controller)" << endl;
    return;
  }
  if (c == 'r') {
    ampelAus = true;
    ampelState = false;
    Serial << "\nR-Taster wurde betätigt. (Py-Controller)" << endl;
    analogWrite(r, parser);
    return;
  }
  if (c == 'a') {
    if (counter == 1) {
      if (intervall(2500) == true) {
        phase_an({ampelGelb}, 2);
        phase_aus({ampelRot, ampelGruen, gelb});
        Serial << "\nAmpel-Taster wurde betätigt. (Py-Controller)" << endl;
        analogWrite(ampelTaster, parser);
        return;
      }
    }
  }
}


void RS_Schaltung::serielle_ausgabe()
{
  char taster = Serial.read();
  int parser = Serial.parseInt();
  analoger_taster_s(taster, parser);
}

// ### LED blinken lassen ######################################################
void RS_Schaltung::blinken(int led, int an, int aus)
{
  digitalWrite(led, (millis()%(an+aus)<an));
}

// ### Intervall / delay #=> millis() ##########################################
bool RS_Schaltung::intervall(int zeit)
{
  if (millis() - _start >= zeit) {
    _start = millis();
    return true;
  }
  else return false;
}

// ### Akutelle LED anzeigen für die Serielle Ausgabe ##########################
void RS_Schaltung::zeige_leds(const NumCheck& led, size_t i, String zusatz)
{
  if (led[i] == 13) {
    Serial << "Ampel-Rot " << zusatz << endl;
  }
  if (led[i] == 12) {
    Serial << "Ampel-Gelb " << zusatz << endl;
  }
  if (led[i] == 11) {
    Serial << "Ampel-Grün " << zusatz << endl;
  }
  if (led[i] == 10) {
    Serial << "Fußgänger-Rot " << zusatz <<endl;
  }
  if (led[i] == 9) {
    Serial << "Fußgänger-Gelb " << zusatz << endl;
  }
}

// ### LED Array (LED's anschalten) ############################################
void RS_Schaltung::phase_an(const LedArray& led , int num)
{
  Serial << "\n####################" << " Phase: " << counter+1
          << " ####################" << endl;
  for (int i = 0; i < 3; i++) {
    digitalWrite(led[i], HIGH);
    zeige_leds(led, i, "an");
  }
  // #=> Counter wird für die nächste Ampelphase erhöht
  counter = num;
}

// ### LED Array (LED's ausschalten)  ##########################################
void RS_Schaltung::phase_aus(const LedArray& led)
{
  for (int i = 0; i < 3; i++) {
    digitalWrite(led[i], LOW);
    zeige_leds(led, i, "aus");
  }
}


// ### Gesamte Schaltung  ######################################################
void RS_Schaltung::schaltung()
{
  start_taster();
  reset_taster();
  ampel_taster();

  if (ampelState == true) {
    if (counter == 0) {
      phase_an({ampelGruen, rot}, 1);
      phase_aus({ampelRot, ampelGelb, gelb});
    }
  }

  if (digitalRead(ampelTaster) == HIGH) {
    if (counter == 1) {
      _start = millis();
      counter = 2;
    }
  }

  for (size_t i = 2; i <=7; i++) {
    if (counter == i) {
      if (intervall(zeit[i-2]) == true) {
        phase_an({an[i-2]}, i+1);
        phase_aus({aus[i-2]});
        if (i == 7) {
          counter = 0;
        }
      }
    }
  }

  if (ampelAus == true){
    phase_an({ampelRot, rot}, 0);
    phase_aus({ampelGruen, ampelGelb, gelb});
  }
}
