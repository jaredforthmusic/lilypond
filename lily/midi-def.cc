/*
  midi-def.cc -- implement Midi_def

  source file of the GNU LilyPond music typesetter

  (c) 1997--2004 Jan Nieuwenhuizen <janneke@gnu.org>

*/
#include <math.h>
#include "misc.hh"
#include "midi-def.hh"
#include "performance.hh"
#include "warn.hh"
#include "scm-hash.hh"

Midi_def::Midi_def ()
{
  // ugh
  set_tempo (Moment (Rational (1, 4)), 60);
}

int
Midi_def::get_tempo (Moment one_beat_mom)
{
  SCM wis  = ly_symbol2scm ("whole-in-seconds");
  Moment *w = unsmob_moment (lookup_variable (wis));

  Moment wholes_per_min = Moment (60);
  if (!w)
    {
      programming_error  ("wholes-in-seconds not set.");
      wholes_per_min /= 4;
    }
  else
    {
      wholes_per_min /= *w; 
    }
  
  int beats_per_min =  int ((wholes_per_min / one_beat_mom).main_part_);
  return int (beats_per_min);
}

void
Midi_def::set_tempo (Moment one_beat_mom, int beats_per_minute_i)
{
  Moment beats_per_second = Moment (beats_per_minute_i) / Moment (60);

  Moment m = Moment (1)/Moment (beats_per_second * one_beat_mom);
  set_variable (ly_symbol2scm ("whole-in-seconds"), m.smobbed_copy ());
}

