#ifndef SHIP_H
#define SHIP_H

const uint16_t shipWidth = 51;
const uint16_t shipHeight = 40;
const uint16_t shipData[] = {
  0x82D6, 0x82DD, 0x82D6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82D6, 0x82DD, 0x82D6,
  0x82DE, 0x82DF, 0x82DE, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82DE, 0x82DF, 0x82DE,
  0x82DF, 0x82DF, 0x82DF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82DF, 0x82DF, 0x82DF,
  0x82DF, 0x82DF, 0x82DF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3578, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82DF, 0x82DF, 0x82DF,
  0x82DF, 0x82DF, 0x82DF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3575, 0x357F, 0x3575, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82DF, 0x82DF, 0x82DF,
  0x82CF, 0x82DF, 0x82DF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5570, 0x457D, 0x357F, 0x457D, 0x5570, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82DF, 0x82DF, 0x83CF,
  0x63BF, 0x55AF, 0x63BF, 0x4790, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4577, 0x357F, 0x357F, 0x357F, 0x4577, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x69B0, 0x74CF, 0x67BF, 0x74CF,
  0x82DF, 0x55AF, 0x469F, 0x469E, 0x4699, 0x4693, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6769, 0x686B, 0x686B, 0x686B, 0x6769, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x68A3, 0x68B9, 0x68BE, 0x68BF, 0x66BF, 0x82DF,
  0x82DF, 0x82CF, 0x469F, 0x469F, 0x469F, 0x469F, 0x469C, 0x4696, 0x4691, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xEF51, 0xEF57, 0xEF57, 0xDF47, 0xDF47, 0xDF37, 0xDF31, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x68B1, 0x68B6, 0x68BC, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x82DF, 0x82DF,
  0x82DF, 0x82DF, 0x63BF, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469E, 0x4699, 0x4693, 0x00F0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFF70, 0xEF56, 0xEF57, 0xEF57, 0xDF47, 0xDF47, 0xDF37, 0xDF36, 0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F0, 0x68B3, 0x68B9, 0x68BE, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x74CF, 0x82DF, 0x82DF,
  0x82DF, 0x82DF, 0x82DF, 0x469A, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469D, 0x4697, 0x4691, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xEF53, 0xEF57, 0xEF57, 0xDF47, 0xDF47, 0xDF37, 0xDF37, 0xCF27, 0xCF23, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x69B1, 0x68B7, 0x68BD, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BA, 0x82DF, 0x82DF, 0x82DF,
  0x82DF, 0x82DF, 0x82DF, 0x4691, 0x469E, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469E, 0x469A, 0x4694, 0x05A0, 0x0000, 0x0000, 0x0000, 0xFF50, 0xEF57, 0xEF57, 0xDF57, 0xDF47, 0xDF47, 0xDF37, 0xDF37, 0xCF27, 0xCF27, 0xCF20, 0x0000, 0x0000, 0x0000, 0x05A0, 0x68B4, 0x68BA, 0x68BE, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BE, 0x68A1, 0x82DF, 0x82DF, 0x82DF,
  0x82DF, 0x82DF, 0x82DF, 0x0000, 0x4695, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469D, 0x4697, 0x4691, 0x0000, 0xEF55, 0xEF57, 0xEF57, 0xDF47, 0xDF47, 0xDF37, 0xDF37, 0xCF27, 0xCF27, 0xCF27, 0xCF25, 0x0000, 0x58A1, 0x68B7, 0x68BD, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68B5, 0x0000, 0x82DF, 0x82DF, 0x82DF,
  0x82DE, 0x82DF, 0x82DE, 0x0000, 0x0000, 0x469A, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469E, 0x8974, 0xEF57, 0xEF57, 0xEF57, 0xDF47, 0xDF47, 0xDF37, 0xDF37, 0xCF27, 0xCF27, 0xCF27, 0xCF27, 0x8A74, 0x68BE, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BA, 0x0000, 0x0000, 0x82DE, 0x82DF, 0x82DE,
  0x82D6, 0x82DD, 0x82D6, 0x0000, 0x0000, 0x4691, 0x469E, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0xAB68, 0xEF57, 0xEF57, 0xDF47, 0xDF47, 0xDF37, 0xDF37, 0xDF37, 0xCF27, 0xCF27, 0xCF27, 0xCF27, 0xAC58, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BE, 0x68A1, 0x0000, 0x0000, 0x82D6, 0x82DD, 0x82D6,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4695, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0xAC69, 0xEF57, 0xDF57, 0xDF47, 0xDF47, 0xDF37, 0xDF37, 0xCF27, 0xCF27, 0xCF27, 0xCF27, 0xCF27, 0xAC59, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x469A, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4691, 0x469E, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BE, 0x68A1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4695, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x469A, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4691, 0x469E, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BE, 0x68A1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4695, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x469A, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4691, 0x469E, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BE, 0x68A1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4695, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x469A, 0x469F, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x53AF, 0x82CF, 0x53AF, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4691, 0x469E, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68BE, 0x68A1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4696, 0x469F, 0x469F, 0x469F, 0x469F, 0x357F, 0x357F, 0x357F, 0x357F, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357F, 0x357F, 0x357F, 0x458F, 0x68BF, 0x68BF, 0x68BF, 0x68BF, 0x68B6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x469A, 0x469F, 0x469F, 0x469F, 0x357E, 0x357F, 0x357F, 0x357F, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357F, 0x357F, 0x357F, 0x468E, 0x68BF, 0x68BF, 0x68BF, 0x68BA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4691, 0x469E, 0x469F, 0x469F, 0x3589, 0x357F, 0x357F, 0x357F, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357F, 0x357F, 0x357F, 0x4687, 0x68BF, 0x68BF, 0x68BE, 0x68A1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4696, 0x469F, 0x469F, 0x4683, 0x357D, 0x357F, 0x357F, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357F, 0x357F, 0x357B, 0x58A3, 0x68BF, 0x68BF, 0x68B6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x469A, 0x469F, 0x4693, 0x3574, 0x357F, 0x357F, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357F, 0x357E, 0x3572, 0x58A3, 0x68BF, 0x68BA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4691, 0x469E, 0x4693, 0x0000, 0x357A, 0x357F, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357F, 0x3577, 0x0000, 0x58A3, 0x68BE, 0x68A1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4696, 0x4693, 0x0000, 0x3571, 0x357E, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357F, 0x357C, 0x5570, 0x0000, 0x58A3, 0x68B6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x37A0, 0x0000, 0x0000, 0x3576, 0x357F, 0x82DF, 0x82DF, 0x82DF, 0x357E, 0x3573, 0x0000, 0x0000, 0x68B0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0070, 0x357B, 0x82DF, 0x82DF, 0x82DF, 0x3579, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3572, 0x82DF, 0x82DF, 0x82DF, 0x3470, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82DF, 0x82DF, 0x82DF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82DE, 0x82DF, 0x82DE, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x82D6, 0x82DD, 0x82D6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
const Color *ship = (Color *)shipData;

#endif // SHIP_H
