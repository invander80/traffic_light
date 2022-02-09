#ifndef RS_SCHALTUNG_H
#define RS_SCHALTUNG_H
#include <Arduino.h>
//#include <Vector.h>

#define LEN(x) (sizeof(x)/sizeof(*(x)))

class RS_Schaltung
{
public:
  // ###### VARS/VALS ##########################################################
  // ### AmpelPins
  const int ampelRot = 13;
  const int ampelGelb = 12;
  const int ampelGruen = 11;

  // ### LED Pins (Fußgänger)
  const int rot = 10;
  const int gelb = 9;

  // ### Taster Pins
  const int s = 8;
  const int r = 7;
  const int ampelTaster = 6;

  unsigned long _start;

  // ### diverse Vars
  bool ampelState;
  bool ampelAus;
  size_t counter = 0;
  size_t doppelKlickCount = 0;

  int an[6] = {ampelGelb, ampelRot, gelb, rot, ampelGelb, ampelGruen};
  int aus[6] = {ampelGruen, ampelGelb, rot, gelb, ampelRot, ampelGelb};
  int zeit[6] = {5000, 2000, 2000, 5000, 2000, 2000};

  // ###### FUNKTIONEN #########################################################
  // ### PinModes
  void schaltung_setup();

  // ### Ampelstatus auf false setzen
  void reset_taster();

  // ### Ampelstatus auf true setzen
  void start_taster();

  void ampel_taster();

  void analoger_taster_s(char, int);


  void serielle_ausgabe();

  // ###
  void schaltung();

  // ### LED Blinken (LED, an , aus)
  void blinken(int, int, int);

  // ### Intervall mit millis() / int = intervall
  bool intervall(int);

  typedef int NumCheck[3];
  void zeige_leds(const NumCheck& led, size_t i, String zusatz);
  // ### An und Aus Phasen. Array; Größe 3

  typedef int LedArray[3];
  void phase_an(const LedArray& led, int num);
  void phase_aus(const LedArray& led);
};

#endif
