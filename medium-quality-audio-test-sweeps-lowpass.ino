/*
  Demo of the audio sweep function.
  The user specifies the amplitude,
  start and end frequencies (which can sweep up or down)
  and the length of time of the sweep.

  Modified to eliminate the audio shield, and use MQS output.  Because of the
  use of fixed pins (10, 12), you probably cannot use the main SPI bus when you
  are using the MQS output.

  Pins:		Teensy 4.0/4.1

  MQSR:		Pin 10
  MQSL:		Pin 12.  */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthToneSweep      tonesweep; //xy=613,773
AudioFilterBiquad        biquad;        //xy=767,772
AudioMixer4              mixer2;         //xy=933,809
AudioMixer4              mixer1;         //xy=935,737
AudioOutputMQS           mqs1; //xy=1079,768
AudioConnection          patchCord1(tonesweep, biquad);
AudioConnection          patchCord2(biquad, 0, mixer1, 0);
AudioConnection          patchCord3(biquad, 0, mixer2, 0);
AudioConnection          patchCord4(mixer2, 0, mqs1, 1);
AudioConnection          patchCord5(mixer1, 0, mqs1, 0);
// GUItool: end automatically generated code

const float	t_ampx	= 0.8;
const int	t_lox	= 10;
const int	t_hix	= 20000;
const float	t_timex	= 10;		// Length of time for the sweep in seconds

// Do a sweep in both directions, enabling or disabling the left/right speakers
void do_sweep (int i)
{
  int do_left  = (i & 1) != 0;
  int do_right = (i & 2) != 0;
  float gain   = (do_left && do_right) ? 0.5f : 1.0f;

  Serial.printf ("Sweep up,   left = %c, right = %c\n",
		 (do_left)  ? 'Y' : 'N',
		 (do_right) ? 'Y' : 'N');

  mixer1.gain (0, do_left  ? gain : 0.0f);
  mixer2.gain (0, do_right ? gain : 0.0f);

  if (!tonesweep.play (t_ampx, t_lox, t_hix, t_timex)) {
    Serial.println ("ToneSweep - play failed");
    while (1)
      ;
  }

  // wait for the sweep to end
  while (tonesweep.isPlaying ())
    ;

  // and now reverse the sweep
  Serial.printf ("Sweep down, left = %c, right = %c\n",
		 (do_left)  ? 'Y' : 'N',
		 (do_right) ? 'Y' : 'N');

  if (!tonesweep.play (t_ampx, t_hix, t_lox, t_timex)) {
    Serial.println("ToneSweep - play failed");
    while (1)
      ;
  }

  // wait for the sweep to end
  while (tonesweep.isPlaying ())
    ;

  Serial.println ("Sweep done");
}

void setup(void)
{
  // Wait for at least 3 seconds for the USB serial connection
  Serial.begin (9600);
  while (!Serial && millis () < 3000)
    ;

  AudioMemory (8);
  Serial.println ("setup done");

  biquad.setLowpass(0,10000);
  biquad.setLowpass(1,10000);
  biquad.setLowpass(2,10000);
  biquad.setLowpass(3,10000);

  for (int i = 1; i <= 3; i++)
    do_sweep (i);

  Serial.println ("Done");
}

void loop (void)
{
}