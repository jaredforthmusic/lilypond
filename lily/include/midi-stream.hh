/*
  midi-stream.hh -- declare Midi_stream

  (c) 1997--2007 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#ifndef MIDI_STREAM_HH
#define MIDI_STREAM_HH

#include <cstdio>
using namespace std;

#include "std-string.hh"

class Midi_item;

/// Midi outputfile
struct Midi_stream
{
  Midi_stream (string file_name_string);
  ~Midi_stream ();

  void write (string);
  void write (Midi_item const &);
  void write (int);

  void open ();

  FILE *out_file_;
  string file_name_string_;
};

#endif // MIDI_STREAM_HH
