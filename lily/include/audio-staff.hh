/*
  audio-staff.hh -- declare Audio_staff

  (c) 1996--2004 Jan Nieuwenhuizen <janneke@gnu.org>
 */

#ifndef AUDIO_STAFF_HH
#define AUDIO_STAFF_HH

#include "parray.hh"
#include "lily-proto.hh"
#include "audio-element.hh"

struct Audio_staff : public Audio_element {
  void add_audio_item (Audio_item*  l);
  void output (Midi_stream& midi_stream_r, int track_i);

  Link_array<Audio_item> audio_items_;
  int channel_;
};

#endif // AUDIO_STAFF_HH
