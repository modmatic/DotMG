/**
 * @file DotMGAudio.cpp
 * \brief
 * The DotMGAudio class for speaker and sound control.
 */

#include "DotMG.h"
#include "DotMGAudio.h"

bool DotMGAudio::audio_enabled = false;

void DotMGAudio::on()
{
  audio_enabled = true;
  DAC->CTRLA.bit.ENABLE = 1;
  while (DAC->STATUS.bit.SYNCBUSY);
}

void DotMGAudio::off()
{
  audio_enabled = false;
  DAC->CTRLA.bit.ENABLE = 0;
  while (DAC->STATUS.bit.SYNCBUSY);
}

void DotMGAudio::toggle()
{
  if (audio_enabled)
    off();
  else
    on();
}

void DotMGAudio::saveOnOff()
{
  EEPROM.update(EEPROM_AUDIO_ON_OFF, audio_enabled);
}

void DotMGAudio::begin()
{
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF))
    on();
  else
    off();
}

bool DotMGAudio::enabled()
{
  return audio_enabled;
}
