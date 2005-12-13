/*
  lily-proto.hh -- declare class names.

  source file of the GNU LilyPond music typesetter

  (c) 1997--2005 Han-Wen Nienhuys <hanwen@xs4all.nl>
*/

#ifndef LILY_PROTO_HH
#define LILY_PROTO_HH
#include "flower-proto.hh"

class Adobe_font_metric;
class All_font_metrics;
class Audio_column;
class Audio_dynamic;
class Audio_element;
class Audio_instrument;
class Audio_item;
class Audio_key;
class Audio_note;
class Audio_piano_pedal;
class Audio_staff;
class Audio_tempo;
class Audio_text;
class Audio_tie;
class Audio_time_signature;
class Auto_change_iterator;
class Auto_change_music;
class Axis_group_engraver;
class Bar_engraver;
class Bar_req_collect_engraver;
class Beaming_info_list;
class Bezier;
class Bezier_bow;
class Book;
class Box;
class Break_algorithm;
class Change_iterator;
class Change_translator;
class Chord_tremolo_iterator;
class Cluster_engraver;
class Column_x_positions;
class Context;
class Context_def;
class Context_specced_music;
class Engraver;
class Engraver;
class Engraver_group;
class Event;
class Event_chord;
class Event_chord_iterator;
class Folded_repeat_iterator;
class Font_metric;
class Font_size_engraver;
class Global_context;
class Gourlay_breaking;
class Grace_fixup;
class Grace_iterator;
class Grace_music;
class Grob;
class Grob_array;
class Grob_info;
class Hara_kiri_engraver;
class Hara_kiri_line_group_engraver;
class Includable_lexer;
class Input;
class Input_file_results;
class Item;
class Key_performer;
class Keyword_ent;
class Keyword_table;
class Ligature_bracket_engraver;
class Ligature_engraver;
class Lily_lexer;
class Lilypond_context_key;
class Lilypond_grob_key;
class Line_group_engraver_group;
class Lookup;
class Lyric_combine_music;
class Lyric_combine_music_iterator;
class Lyric_engraver;
class Lyric_performer;
class Lyric_phrasing_engraver;
class Mensural_ligature_engraver;
class Midi_chunk;
class Midi_duration;
class Midi_dynamic;
class Midi_header;
class Midi_instrument;
class Midi_item;
class Midi_key;
class Midi_note;
class Midi_note_event;
class Midi_note_off;
class Midi_piano_pedal;
class Midi_stream;
class Midi_tempo;
class Midi_text;
class Midi_time_signature;
class Midi_track;
class Modified_font_metric;
class Moment;
class Music;
class Music_iterator;
class Music_list;
class Music_output;
class Music_sequence;
class Music_wrapper;
class Music_wrapper_iterator;
class Note_performer;
class Output_def;
class Object_key;
class Object_key_dumper;
class Object_key_undumper;
class Open_type_font;
class Output_property;
class Page;
class Pango_font;
class Paper_book;
class Paper_column;
class Paper_outputter;
class Paper_score;
class Paper_system;
class Performance;
class Performer;
class Performer_group;
class Piano_bar_engraver;
class Pitch;
class Pitch_squash_engraver;
class Property_iterator;
class Rational;
class Relative_octave_music;
class Repeated_music;
class Scheme_hash_table;
class Score;
class Score_context;
class Score_engraver;
class Score_performer;
class Sequential_music;
class Sequential_music_iterator;
class Simple_music_iterator;
class Simple_spacer;
class Simple_spacer_wrapper;
class Simultaneous_music;
class Simultaneous_music_iterator;
class Skyline_entry;
class Slur_configuration;
class Slur_score_state;
class Span_score_bar_engraver;
class Spanner;
class Staff_group_bar_engraver;
class Staff_performer;
class Stencil;
class Swallow_engraver;
class Swallow_performer;
class System;
class Tempo_performer;
class Tex_font_metric;
class Tie;
class Tie_details;
class Tie_configuration;
class Tie_formatting_problem;
class Tie_performer;
class Time_scaled_music;
class Time_scaled_music_iterator;
class Time_signature_performer;
class Timing_engraver;
class Timing_translator;
class Translation_property;
class Translator;
class Translator_change;
class Translator_group;
class Transposed_music;
class Type_swallow_translator;
class yyFlexLexer;

typedef void (*Engraver_void_function_engraver_grob_info) (Engraver *, Grob_info);
typedef void (*Translator_void_method_ptr) (Translator *);


#endif /* LILY_PROTO_HH */
