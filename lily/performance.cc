/*
  performance.cc -- implement Performance

  source file of the GNU LilyPond music typesetter

  (c)  1997--1999 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include <time.h>
#include "debug.hh"
#include "string.hh"
#include "string-convert.hh"
#include "main.hh"
#include "midi-def.hh"
#include "midi-item.hh"
#include "midi-stream.hh"
#include "audio-column.hh"
#include "audio-item.hh"
#include "audio-staff.hh"
#include "performance.hh"
#include "score.hh"
#include "file-results.hh"
#include "lily-version.hh"

#include "killing-cons.tcc"

Performance::Performance ()
{
  midi_l_ =0;
  audio_elem_p_list_ = 0;
}


Performance::~Performance()
{
  delete audio_elem_p_list_;
}

void
Performance::output (Midi_stream& midi_stream)
{
  int tracks_i = audio_staff_l_arr_.size() + 1;

  // ugh
  int clocks_per_4_i = 384;

  midi_stream << Midi_header (1, tracks_i, clocks_per_4_i);
  output_header_track (midi_stream);
  *mlog << "\n";
  *mlog << _ ("Track ... ");
  int channel = 1;
  for (int i =0; i < audio_staff_l_arr_.size (); i++)
    {
      *mlog << '[' << flush;
      Audio_staff *s = audio_staff_l_arr_[i];

      *mlog << i << flush;

      /*
	Aargh, let's hear it for the MIDI standard.
	MIDI players tend to ignore instrument settings on
	channel 10, the percussion channel by default.
       */
      if (channel == 9)
	channel++;
      s->output (midi_stream, channel++);
      *mlog << ']' << flush;
    }
}

void
Performance::output_header_track (Midi_stream& midi_stream)
{
  Midi_track midi_track;

  // perhaps multiple text events?
  String str = String (_("Creator: "));
  if (no_timestamps_global_b)
    str += gnu_lilypond_str ();
  else
    str += gnu_lilypond_version_str();
  str += "\n";

  /*
    This seems silly, but in fact the audio elements should
    be generated elsewhere: not midi-specific.
   */
  Audio_text creator_a (Audio_text::TEXT, str);
  Midi_text creator (&creator_a);
  midi_track.add (Moment (0), &creator);

  str = _("Automatically generated");
  if (no_timestamps_global_b)
    str += ".\n";
  else
    {
      str += _(", at ");
      time_t t (time (0));
      str += ctime (&t);
      str = str.left_str (str.length_i() - 1);
    }
  Audio_text generate_a (Audio_text::TEXT, str);
  Midi_text generate (&generate_a);
  midi_track.add (Moment (0), &generate);

  str = _f ("from musical definition: %s", origin_str_);

  Audio_text from_a (Audio_text::TEXT, str);
  Midi_text from (&from_a);
  midi_track.add (Moment (0), &from);

  Audio_text track_name_a (Audio_text::TRACK_NAME, "Track "
			   + String_convert::i2dec_str (0, 0, '0'));
  Midi_text track_name (&track_name_a);
			
  midi_track.add (Moment (0), &track_name);

  Audio_tempo tempo_a (midi_l_->get_tempo_i (Moment (1, 4)));
  Midi_tempo tempo (&tempo_a);
  midi_track.add (Moment (0), &tempo);

  midi_stream << midi_track;
}

void
Performance::add_element (Audio_element *p)
{
  if (Audio_staff*s=dynamic_cast<Audio_staff *> (p)) 
    {
      audio_staff_l_arr_.push (s);
    }
  else if (Audio_column *c = dynamic_cast<Audio_column*>(p))
    {
      c->performance_l_ = this;
    }
  audio_elem_p_list_ = new Killing_cons<Audio_element> (p, audio_elem_p_list_);
}

void
Performance::print() const
{
#ifndef NPRINT
  DOUT << "Performance { ";
  DOUT << "Items: ";
  for (Cons<Audio_element>* i =audio_elem_p_list_; i; i = i->next_)
    i->car_->print ();
  DOUT << "}";
#endif
}

void
Performance::process()
{
  print ();

  String out = midi_l_->get_default_output ();
  if (out.empty_b ())
    {
      
      out = default_outname_base_global;
      if (out == "-")
        out = "lelie";
      int def = midi_l_->get_next_default_count ();
      if (def)
	{
	  out += "-" + to_str (def);
	}

      out += ".midi";
    }
  
  Midi_stream midi_stream (out);
  *mlog << _f ("MIDI output to %s...", out) << endl;
  target_str_global_array.push (out);

  output (midi_stream);
  *mlog << endl;
}
