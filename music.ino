
#include <avr/io.h>
#include <util/delay.h>
#include "music.h"

#define F_CPU 16000000UL
#define PRESCALER 256
int currentMusicState = 0;

int freqToTop(int freq)
{
  return (F_CPU / PRESCALER) / freq;
}

char lowerBits(int number)
{
  return number & 0b11111111;
}

char higherBits(int number)
{
  return number & 0b1111111100000000;
}

void play_tone(int freq, int duration)
{
  int TOP = freqToTop(freq);
  int half_freq_TOP = TOP;
  OCR1AH = higherBits(TOP);
  OCR1AL = lowerBits(TOP);
  OCR1BH = higherBits(TOP / 2);
  OCR1BL = lowerBits(TOP / 2);
  delay(duration);
}
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define REST 0

// change this to make the song slower or faster
int tempo = 250;
// int tempo = 190;

// change this to whichever pin you want to use
int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int rickRoll[] = {

    // Never Gonna Give You Up - Rick Astley
    // Score available at https://musescore.com/chlorondria_5/never-gonna-give-you-up_alto-sax
    // Arranged by Chlorondria

    NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4, // 1
    NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
    NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
    NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16,
    NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4, // repeat from 1
    NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
    NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
    NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16,
    REST, 4, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_D5, 8, NOTE_E5, 8, NOTE_CS5, -8,
    NOTE_B4, 16, NOTE_A4, 2, REST, 4,

    REST, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 4, NOTE_A4, 8, // 7
    NOTE_A5, 8, REST, 8, NOTE_A5, 8, NOTE_E5, -4, REST, 4,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 8, NOTE_D5, 8, NOTE_E5, 8, REST, 8,
    REST, 8, NOTE_CS5, 8, NOTE_B4, 8, NOTE_A4, -4, REST, 4,
    REST, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 8, NOTE_A4, 4,
    NOTE_E5, 8, NOTE_E5, 8, NOTE_E5, 8, NOTE_FS5, 8, NOTE_E5, 4, REST, 4,

    NOTE_D5, 2, NOTE_E5, 8, NOTE_FS5, 8, NOTE_D5, 8, // 13
    NOTE_E5, 8, NOTE_E5, 8, NOTE_E5, 8, NOTE_FS5, 8, NOTE_E5, 4, NOTE_A4, 4,
    REST, 2, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 8,
    REST, 8, NOTE_E5, 8, NOTE_FS5, 8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_FS5, -8, NOTE_FS5, -8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,

    NOTE_E5, -8, NOTE_E5, -8, NOTE_D5, -8, NOTE_CS5, 16, NOTE_B4, -8, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16, // 18
    NOTE_D5, 4, NOTE_E5, 8, NOTE_CS5, -8, NOTE_B4, 16, NOTE_A4, 8, NOTE_A4, 8, NOTE_A4, 8,
    NOTE_E5, 4, NOTE_D5, 2, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_FS5, -8, NOTE_FS5, -8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_A5, 4, NOTE_CS5, 8, NOTE_D5, -8, NOTE_CS5, 16, NOTE_B4, 8, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,

    NOTE_D5, 4, NOTE_E5, 8, NOTE_CS5, -8, NOTE_B4, 16, NOTE_A4, 4, NOTE_A4, 8, // 23
    NOTE_E5, 4, NOTE_D5, 2, REST, 4,
    REST, 8, NOTE_B4, 8, NOTE_D5, 8, NOTE_B4, 8, NOTE_D5, 8, NOTE_E5, 4, REST, 8,
    REST, 8, NOTE_CS5, 8, NOTE_B4, 8, NOTE_A4, -4, REST, 4,
    REST, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 8, NOTE_A4, 4,
    REST, 8, NOTE_A5, 8, NOTE_A5, 8, NOTE_E5, 8, NOTE_FS5, 8, NOTE_E5, 8, NOTE_D5, 8,

    REST, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 8, // 29
    REST, 8, NOTE_CS5, 8, NOTE_B4, 8, NOTE_A4, -4, REST, 4,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 8, NOTE_A4, 4, REST, 8,
    REST, 8, NOTE_E5, 8, NOTE_E5, 8, NOTE_FS5, 4, NOTE_E5, -4,
    NOTE_D5, 2, NOTE_D5, 8, NOTE_E5, 8, NOTE_FS5, 8, NOTE_E5, 4,
    NOTE_E5, 8, NOTE_E5, 8, NOTE_FS5, 8, NOTE_E5, 8, NOTE_A4, 8, NOTE_A4, 4,

    REST, -4, NOTE_A4, 8, NOTE_B4, 8, NOTE_CS5, 8, NOTE_D5, 8, NOTE_B4, 8, // 35
    REST, 8, NOTE_E5, 8, NOTE_FS5, 8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_FS5, -8, NOTE_FS5, -8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_E5, -8, NOTE_E5, -8, NOTE_D5, -8, NOTE_CS5, 16, NOTE_B4, 8, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_D5, 4, NOTE_E5, 8, NOTE_CS5, -8, NOTE_B4, 16, NOTE_A4, 4, NOTE_A4, 8,

    NOTE_E5, 4, NOTE_D5, 2, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16, // 40
    NOTE_FS5, -8, NOTE_FS5, -8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_A5, 4, NOTE_CS5, 8, NOTE_D5, -8, NOTE_CS5, 16, NOTE_B4, 8, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_D5, 4, NOTE_E5, 8, NOTE_CS5, -8, NOTE_B4, 16, NOTE_A4, 4, NOTE_A4, 8,
    NOTE_E5, 4, NOTE_D5, 2, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,

    NOTE_FS5, -8, NOTE_FS5, -8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16, // 45
    NOTE_A5, 4, NOTE_CS5, 8, NOTE_D5, -8, NOTE_CS5, 16, NOTE_B4, 8, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_D5, 4, NOTE_E5, 8, NOTE_CS5, -8, NOTE_B4, 16, NOTE_A4, 4, NOTE_A4, 8,
    NOTE_E5, 4, NOTE_D5, 2, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_FS5, -8, NOTE_FS5, -8, NOTE_E5, -4, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16, // 45

    NOTE_A5, 4, NOTE_CS5, 8, NOTE_D5, -8, NOTE_CS5, 16, NOTE_B4, 8, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 16, NOTE_B4, 16,
    NOTE_D5, 4, NOTE_E5, 8, NOTE_CS5, -8, NOTE_B4, 16, NOTE_A4, 4, NOTE_A4, 8,

    NOTE_E5, 4, NOTE_D5, 2, REST, 4};

int mii[] = {

    // Mii Channel theme
    // Score available at https://musescore.com/user/16403456/scores/4984153
    // Uploaded by Catalina Andrade

    NOTE_FS4, 8, REST, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_FS4, 8, // 1
    NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, REST, 8, REST, 4, REST, 8, NOTE_CS4, 8,
    NOTE_D4, 8, NOTE_FS4, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_F4, 8,
    NOTE_E5, -4, NOTE_DS5, 8, NOTE_D5, 8, REST, 8, REST, 4,

    NOTE_GS4, 8, REST, 8, NOTE_CS5, 8, NOTE_FS4, 8, REST, 8, NOTE_CS5, 8, REST, 8, NOTE_GS4, 8, // 5
    REST, 8, NOTE_CS5, 8, NOTE_G4, 8, NOTE_FS4, 8, REST, 8, NOTE_E4, 8, REST, 8,
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, REST, 8, REST, 4, NOTE_E4, 8, NOTE_E4, 8,
    NOTE_E4, 8, REST, 8, REST, 4, NOTE_DS4, 8, NOTE_D4, 8,

    NOTE_CS4, 8, REST, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_FS4, 8, // 9
    NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, REST, 8, NOTE_E5, 8, NOTE_E5, 8, NOTE_E5, 8, REST, 8,
    REST, 8, NOTE_FS4, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_F4, 8,
    NOTE_E5, 2, NOTE_D5, 8, REST, 8, REST, 4,

    NOTE_B4, 8, NOTE_G4, 8, NOTE_D4, 8, NOTE_CS4, 4, NOTE_B4, 8, NOTE_G4, 8, NOTE_CS4, 8, // 13
    NOTE_A4, 8, NOTE_FS4, 8, NOTE_C4, 8, NOTE_B3, 4, NOTE_F4, 8, NOTE_D4, 8, NOTE_B3, 8,
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, REST, 4, REST, 4, NOTE_AS4, 4,
    NOTE_CS5, 8, NOTE_D5, 8, NOTE_FS5, 8, NOTE_A5, 8, REST, 8, REST, 4,

    REST, 2, NOTE_A3, 4, NOTE_AS3, 4, // 17
    NOTE_A3, -4, NOTE_A3, 8, NOTE_A3, 2,
    REST, 4, NOTE_A3, 8, NOTE_AS3, 8, NOTE_A3, 8, NOTE_F4, 4, NOTE_C4, 8,
    NOTE_A3, -4, NOTE_A3, 8, NOTE_A3, 2,

    REST, 2, NOTE_B3, 4, NOTE_C4, 4, // 21
    NOTE_CS4, -4, NOTE_C4, 8, NOTE_CS4, 2,
    REST, 4, NOTE_CS4, 8, NOTE_C4, 8, NOTE_CS4, 8, NOTE_GS4, 4, NOTE_DS4, 8,
    NOTE_CS4, -4, NOTE_DS4, 8, NOTE_B3, 1,

    NOTE_E4, 4, NOTE_E4, 4, NOTE_E4, 4, REST, 8, // 25

    // repeats 1-25

    NOTE_FS4, 8, REST, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_FS4, 8, // 1
    NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, REST, 8, REST, 4, REST, 8, NOTE_CS4, 8,
    NOTE_D4, 8, NOTE_FS4, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_F4, 8,
    NOTE_E5, -4, NOTE_DS5, 8, NOTE_D5, 8, REST, 8, REST, 4,

    NOTE_GS4, 8, REST, 8, NOTE_CS5, 8, NOTE_FS4, 8, REST, 8, NOTE_CS5, 8, REST, 8, NOTE_GS4, 8, // 5
    REST, 8, NOTE_CS5, 8, NOTE_G4, 8, NOTE_FS4, 8, REST, 8, NOTE_E4, 8, REST, 8,
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, REST, 8, REST, 4, NOTE_E4, 8, NOTE_E4, 8,
    NOTE_E4, 8, REST, 8, REST, 4, NOTE_DS4, 8, NOTE_D4, 8,

    NOTE_CS4, 8, REST, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_FS4, 8, // 9
    NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, REST, 8, NOTE_E5, 8, NOTE_E5, 8, NOTE_E5, 8, REST, 8,
    REST, 8, NOTE_FS4, 8, NOTE_A4, 8, NOTE_CS5, 8, REST, 8, NOTE_A4, 8, REST, 8, NOTE_F4, 8,
    NOTE_E5, 2, NOTE_D5, 8, REST, 8, REST, 4,

    NOTE_B4, 8, NOTE_G4, 8, NOTE_D4, 8, NOTE_CS4, 4, NOTE_B4, 8, NOTE_G4, 8, NOTE_CS4, 8, // 13
    NOTE_A4, 8, NOTE_FS4, 8, NOTE_C4, 8, NOTE_B3, 4, NOTE_F4, 8, NOTE_D4, 8, NOTE_B3, 8,
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, REST, 4, REST, 4, NOTE_AS4, 4,
    NOTE_CS5, 8, NOTE_D5, 8, NOTE_FS5, 8, NOTE_A5, 8, REST, 8, REST, 4,

    REST, 2, NOTE_A3, 4, NOTE_AS3, 4, // 17
    NOTE_A3, -4, NOTE_A3, 8, NOTE_A3, 2,
    REST, 4, NOTE_A3, 8, NOTE_AS3, 8, NOTE_A3, 8, NOTE_F4, 4, NOTE_C4, 8,
    NOTE_A3, -4, NOTE_A3, 8, NOTE_A3, 2,

    REST, 2, NOTE_B3, 4, NOTE_C4, 4, // 21
    NOTE_CS4, -4, NOTE_C4, 8, NOTE_CS4, 2,
    REST, 4, NOTE_CS4, 8, NOTE_C4, 8, NOTE_CS4, 8, NOTE_GS4, 4, NOTE_DS4, 8,
    NOTE_CS4, -4, NOTE_DS4, 8, NOTE_B3, 1,

    NOTE_E4, 4, NOTE_E4, 4, NOTE_E4, 4, REST, 8, // 25

    // finishes with 26
    // NOTE_FS4,8, REST,8, NOTE_A4,8, NOTE_CS5,8, REST,8, NOTE_A4,8, REST,8, NOTE_FS4,8

};

const PROGMEM int castleVania[] = {

    // Bloody Tears, from Castlevania II
    // Arranged by Bobby Lee. THe flute part was used
    // https://musescore.com/user/263171/scores/883296

    // B-flat major Bb Eb
    REST, 4, NOTE_G5, 4,
    NOTE_A5, 4, NOTE_AS5, 4,
    NOTE_A5, 4, NOTE_F5, 4,
    NOTE_A5, 4, NOTE_G5, 4,
    REST, 4, NOTE_G5, 4,
    NOTE_A5, 4, NOTE_AS5, 4,
    NOTE_C6, 4, NOTE_AS5, 4,

    NOTE_A5, 4, NOTE_G5, 4, // 8
    REST, 4, NOTE_G5, 4,
    NOTE_A5, 4, NOTE_AS5, 4,
    NOTE_A5, 4, NOTE_F5, 4,
    NOTE_A5, 4, NOTE_G5, 4,
    NOTE_D6, 4, REST, 8, NOTE_C6, 8,
    REST, 4, NOTE_AS5, 4,

    NOTE_A5, 4, NOTE_AS5, 8, NOTE_C6, 8, // 15
    NOTE_F6, 8, REST, 8, REST, 4,
    NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,

    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16, // 20
    NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,

    NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, // 25
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_AS5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,

    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_C6, 16, NOTE_C6, 16, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8,
    REST, 4, NOTE_C6, 16, NOTE_AS5, 16,

    NOTE_C6, -8, NOTE_F6, -8, NOTE_D6, -4, // 35
    NOTE_C6, 8, NOTE_AS5, 8,
    NOTE_C6, 8, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8,
    REST, 4, NOTE_C6, 8, NOTE_D6, 8,
    NOTE_DS6, -8, NOTE_F6, -8,

    NOTE_D6, -8, REST, 16, NOTE_DS6, 8, REST, 8, // 40
    NOTE_C6, 8, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8,
    REST, 4, NOTE_C6, 8, NOTE_AS5, 8,
    NOTE_C6, -8, NOTE_F6, -8, NOTE_D6, -4,
    NOTE_C6, 8, NOTE_AS5, 8,

    NOTE_C6, 8, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8, // 45
    REST, 4, NOTE_C6, 8, NOTE_D6, 8,
    NOTE_DS6, -8, NOTE_F6, -8,
    NOTE_D5, 8, NOTE_FS5, 8, NOTE_F5, 8, NOTE_A5, 8,
    NOTE_A5, -8, NOTE_G5, -4,

    NOTE_A5, -8, NOTE_G5, -4, // 50
    NOTE_A5, -8, NOTE_G5, -4,
    NOTE_AS5, 8, NOTE_A5, 8, NOTE_G5, 8, NOTE_F5, 8,
    NOTE_A5, -8, NOTE_G5, -8, NOTE_D5, 8,
    NOTE_A5, -8, NOTE_G5, -8, NOTE_D5, 8,
    NOTE_A5, -8, NOTE_G5, -8, NOTE_D5, 8,

    NOTE_AS5, 4, NOTE_C6, 4, NOTE_A5, 4, NOTE_AS5, 4,
    NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, // 56 //r
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,

    NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, // 61
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,

    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16, // 66
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
    NOTE_AS5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,

    NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, // 71 //
    NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 8, NOTE_G5, 32, NOTE_A5, 32, NOTE_AS5, 32, NOTE_C6, 32,
    NOTE_D6, 16, NOTE_G5, 16, NOTE_AS5, 16, NOTE_G5, 16, NOTE_C6, 16, NOTE_G5, 16, NOTE_D6, 16, NOTE_G5, 16,
    NOTE_C6, 16, NOTE_G5, 16, NOTE_A5, 16, NOTE_G5, 16, NOTE_F6, 16, NOTE_G5, 16, NOTE_D6, 16, NOTE_DS5, 16,
    NOTE_D6, 4, REST, 4,

    NOTE_C5, 8, REST, 8, NOTE_A4, -16, NOTE_AS4, -16, NOTE_C5, 16, // 76
    NOTE_D6, 16, NOTE_G4, 16, NOTE_AS4, 16, NOTE_G4, 16, NOTE_C5, 16, NOTE_G4, 16, NOTE_D6, 16, NOTE_G4, 16,
    NOTE_C6, 16, NOTE_F4, 16, NOTE_A4, 16, NOTE_F4, 16, NOTE_F5, 16, NOTE_F4, 16, NOTE_D6, 16, NOTE_DS4, 16,
    NOTE_D6, 16, REST, 8, NOTE_E4, 16, NOTE_F4, 16,

    // change of key B Major A# C# D# F# G#
    NOTE_GS4, 8, REST, 8, NOTE_AS4, 8, REST, 8,

    NOTE_DS5, 16, NOTE_GS4, 16, NOTE_B4, 16, NOTE_GS4, 16, NOTE_CS5, 16, NOTE_GS4, 16, NOTE_DS5, 16, NOTE_GS4, 16, // 81
    NOTE_CS5, 16, NOTE_FS4, 16, NOTE_AS4, 16, NOTE_FS4, 16, NOTE_FS5, 16, NOTE_FS4, 16, NOTE_DS5, 16, NOTE_E5, 16,
    NOTE_D5, 4, REST, 4,
    NOTE_CS5, 8, REST, 8, NOTE_AS4, -16, NOTE_B4, -16, NOTE_CS5, 16,
    NOTE_DS5, 16, NOTE_GS4, 16, NOTE_B4, 16, NOTE_GS4, 16, NOTE_CS5, 16, NOTE_GS4, 16, NOTE_DS5, 16, NOTE_GS4, 16,

    NOTE_CS5, 16, NOTE_FS4, 16, NOTE_AS4, 16, NOTE_FS4, 16, NOTE_FS5, 16, NOTE_FS4, 16, NOTE_DS5, 16, NOTE_E5, 16,
    NOTE_DS5, 4, REST, 8, NOTE_DS5, 16, NOTE_E5, 16,
    NOTE_FS5, 16, NOTE_CS5, 16, NOTE_E5, 16, NOTE_CS4, 16, NOTE_DS5, 16, NOTE_E5, 16, NOTE_G5, 16, NOTE_AS5, 16,
    NOTE_GS5, 16, NOTE_DS5, 16, NOTE_DS6, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,

    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16, // 90
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_FS5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16,
    NOTE_GS5, 16, NOTE_DS5, 16, NOTE_DS6, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,

    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16, // 94
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_FS5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16,
    NOTE_GS5, 16, NOTE_DS5, 16, NOTE_DS6, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,

    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16, // 98
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_FS5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16,
    NOTE_GS5, 16, NOTE_DS5, 16, NOTE_DS6, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,

    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16, // 102
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16, NOTE_CS6, 16, NOTE_DS5, 16, NOTE_B5, 16, NOTE_DS5, 16,
    NOTE_AS5, 16, NOTE_DS5, 16, NOTE_FS5, 16, NOTE_DS5, 16, NOTE_AS5, 16, NOTE_DS5, 16, NOTE_GS5, 16, NOTE_DS5, 16,

    NOTE_CS6, 8, NOTE_FS6, 16, NOTE_DS6, 8, REST, 16, REST, 8, // 107
    REST, 4, NOTE_CS6, 8, NOTE_B5, 8,
    NOTE_CS6, -8, NOTE_FS6, -8, NOTE_DS6, -4,
    NOTE_CS6, 8, NOTE_B5, 8,
    NOTE_CS6, 8, NOTE_FS6, 16, NOTE_DS6, 8, REST, 16, REST, 8,
    REST, 4, NOTE_CS6, 8, NOTE_B5, 8,
    NOTE_E6, -8, NOTE_F6, -8,

    NOTE_DS6, -8, REST, 16, NOTE_E6, 8, REST, 16, REST, 16, // 112
    NOTE_CS6, 8, NOTE_FS6, 16, NOTE_DS6, 8, REST, 16, REST, 8,
    REST, 4, NOTE_CS6, 8, NOTE_B5, 8,
    NOTE_CS6, -8, NOTE_FS6, -8, NOTE_DS6, -4,
    NOTE_CS6, 8, NOTE_B5, 8,

    NOTE_CS6, 8, NOTE_FS6, 16, NOTE_DS6, 8, REST, 16, REST, 8, // 117
    REST, 4, NOTE_CS5, 8, NOTE_DS5, 8,
    NOTE_E5, -8, NOTE_F5, -8,
    NOTE_DS5, 8, NOTE_G5, 8, NOTE_GS5, 8, NOTE_AS5, 8,
    NOTE_AS5, -8, NOTE_GS5, -8,

    NOTE_AS5, -8, NOTE_GS5, -8, // 122
    NOTE_AS5, -8, NOTE_GS5, -8,
    NOTE_B6, 8, NOTE_AS5, 8, NOTE_GS5, 8, NOTE_FS5, 8,
    NOTE_AS5, -8, NOTE_GS6, -8, NOTE_DS5, 8,
    NOTE_AS5, -8, NOTE_GS6, -8, NOTE_DS5, 8,
    NOTE_AS5, -8, NOTE_GS6, -8, NOTE_DS5, 8,

    NOTE_B5, 8, NOTE_CS6, 8, NOTE_AS5, 8, NOTE_B5, 8, // 128
    NOTE_GS5, 8, REST, 8, REST, 16

};

void sus()
{
  // Serttings 2.5 playback and 66% of note
  // https://sshdl-7.extramaster.net/tools/midiToArduino/processFile/

  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(228.5712);
  play_tone(466, 37.714248);
  delay(57.1428);
  play_tone(587, 37.714248);
  delay(57.1428);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(571.428);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  delay(342.8568);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  delay(228.5712);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(228.5712);
  play_tone(466, 37.714248);
  delay(57.1428);
  play_tone(587, 37.714248);
  delay(57.1428);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(571.428);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  delay(342.8568);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  delay(228.5712);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(228.5712);
  play_tone(466, 37.714248);
  delay(57.1428);
  play_tone(587, 37.714248);
  delay(57.1428);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(571.428);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  delay(342.8568);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  delay(228.5712);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(228.5712);
  play_tone(466, 37.714248);
  delay(57.1428);
  play_tone(587, 37.714248);
  delay(57.1428);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(571.428);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  delay(342.8568);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  delay(7542.8496);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(228.5712);
  play_tone(466, 37.714248);
  delay(57.1428);
  play_tone(587, 37.714248);
  delay(57.1428);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(571.428);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  delay(342.8568);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  delay(228.5712);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(228.5712);
  play_tone(466, 37.714248);
  delay(57.1428);
  play_tone(587, 37.714248);
  delay(57.1428);
  play_tone(523, 75.428496);
  delay(114.2856);
  delay(571.428);
  play_tone(523, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  play_tone(698, 75.428496);
  delay(114.2856);
  play_tone(622, 75.428496);
  delay(114.2856);
  play_tone(739, 75.428496);
  delay(114.2856);
  delay(342.8568);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
  play_tone(739, 50.285664);
  delay(76.1904);
  play_tone(698, 50.285664);
  delay(76.1904);
  play_tone(622, 50.285664);
  delay(76.1904);
}

#define MELODY rickRoll

void music_play()
{
  // Doesnt work with sizeof
  //  int *melody = rickRoll;

  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(MELODY) / sizeof(MELODY[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
  {

    // calculates the duration of each note
    divider = MELODY[thisNote + 1];
    if (divider > 0)
    {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    }
    else if (divider < 0)
    {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    play_tone(MELODY[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
  }
}