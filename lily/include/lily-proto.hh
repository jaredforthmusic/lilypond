/*
  lily-proto.hh -- declare class names.

  source file of the GNU LilyPond music typesetter

  (c)  1997--1998 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#ifndef LILY_PROTO_HH
#define LILY_PROTO_HH
#include "proto.hh"


struct Absolute_dynamic_req;
struct Abbreviation;
struct Abbreviation_req;
struct Abbreviation_beam_req;
struct Abbreviation_beam_engraver;
struct Axis_group_element;
struct Axis_group;
struct Translator_group;
struct Translator;
struct Abbreviation_beam; 
struct Audio_element;
struct Audio_column;
struct Audio_item;
struct Audio_key;
struct Audio_time_signature;
struct Audio_note;
struct Audio_note_off;
struct Performance;
struct Audio_staff;
struct Audio_tempo;
struct Atom;
struct Bar;
struct Bar_column_engraver;
struct Bar_column;
struct Bar_engraver;
struct Bar_req;
struct Barcheck_req;
struct Beam;
struct Beam_engraver;
struct Beam_req;
struct Blank_req;
struct Bow;
struct Box;
struct Bracket_req;
struct Break_align_item;
struct Break_req;
struct Cadenza_req;
struct Change_iterator;
struct Change_translator;
struct Simultaneous_music;
struct Clef_change_req;
struct Clef_item;
struct Clef_engraver;
struct Clef_performer;
struct Column_x_positions;
struct Column_info;
struct Collision;
struct Collision_engraver;
struct Command_req;
struct Command_script_req;
struct Compressed_music;
struct Compressed_music_iterator;
struct Cresc_req;
struct Crescendo ;
struct Decresc_req;
struct Dots;
struct Dot_column;
struct Directional_spanner;
struct Durational_req;
struct Dynamic;
struct Dynamic_req;
struct Element_group;
struct Element_group_item;
struct Engraver_group_engraver;
struct General_script_def;
struct Graphical_element;

struct Graphical_axis_group;
struct Mark_req;
struct Music_output;
struct Musical_pitch;
struct Music_output_def;
struct Global_translator;
struct Hara_kiri_line_group_engraver;
struct Hara_kiri_vertical_group_spanner;
struct Head_column;

struct Horizontal_align_item;
struct Horizontal_group_element;
struct Horizontal_group_item;
struct Horizontal_vertical_group;
struct Idealspacing;
struct Identifier;
struct Input_file;
struct Item;
struct Key;
struct Key_change_req;
struct Key_item;
struct Key_engraver;
struct Key_performer;
struct Keyword;
struct Keyword_table;
struct Lily_stream;
struct Line_group_engraver;
struct Line_of_score;
struct Line_of_staff;
struct Line_spacer;
struct Linestaff;
struct Local_key;
struct Local_key_item;
struct Local_key_engraver;
struct Lookup;
struct Lyric_item;
struct Lyric_req;
struct My_lily_lexer;
struct Measure_grouping_req;
struct Melodic_req;
struct Time_signature;
struct Time_signature_change_req;
struct Time_signature_engraver;
struct Time_signature_performer;
struct Midi_def;
struct Midi_duration;
struct Midi_header;
struct Midi_item;
struct Midi_key;
struct Midi_time_signature;
struct Midi_note;
struct Midi_note_event;
struct Midi_note_off;
struct Midi_output;
struct Midi_score;
struct Midi_stream;
struct Midi_tempo;
struct Midi_track;
struct Midi_walker;
struct Mixed_qp;
struct Molecule;
struct Multi_measure_rest;
struct Multi_measure_rest_req;
struct Multi_measure_rest_engraver;
struct Musical_script_req;
struct Music_list;
struct Musical_req;
struct Musical_span_req;
struct Music;
struct Music_sequence;
struct Music_wrapper_iterator;
struct Music_wrapper;
struct My_lily_parser;
struct Note_column;
struct Note_column_engraver;
struct Note_performer;
struct Note_req;
struct Note_head;
struct Note_head_engraver;
struct Notename_table;
struct Offset;
struct Paper_column;
struct Paper_def;
struct Paper_outputter;
struct Paper_score;
struct Paper_stream;
struct Partial_measure_req;
struct Performer;
struct Plet;
struct Plet_engraver;
struct Plet_req;
struct Plet_spanner;
struct Rational;
struct Piano_brace;
struct Performer;
struct Performer_group_performer;
struct Property_iterator;
struct Request;
struct Request_column;
struct Engraver;
struct Relative_octave_music;
struct Rest;
struct Rest_collision;
struct Rest_collision_engraver;
struct Rest_req;
struct Rhythmic_grouping;
struct Rhythmic_head;
struct Rhythmic_grouping_req;
struct Rhythmic_req;
struct Single_malt_grouping_item;
struct Scope;
struct Separating_group_spanner;
struct Score;
struct Score_column;
struct Score_element;
struct Score_element_info;
struct Score_performer;
struct Script;
struct Script_column;
struct Script_def;
struct Script_engraver;
struct Script_req;
struct Simple_music;
struct Skip_req;
struct Slur;
struct Slur_engraver;
struct Slur_req;
struct Spacing_req;
struct Span_bar;
struct Span_score_bar;
struct Span_dynamic_req;
struct Span_req;
struct Spanner;
struct Spring_spacer;
struct Staff_bracket;
struct Staff_performer;
struct Staff_side;
struct Staff_symbol;
struct Stem;
struct Stem_beam_engraver;
struct Stem_req;
struct String;
struct Atom;
struct Symtable;
struct Symtables;
struct Super_element;
struct Translation_property;
struct Tempo_req;
struct Text_def;
struct Text_gob;
struct Text_item ;
struct Text_engraver;
struct Text_req;
struct Text_spanner;
struct Tie;
struct Tie_engraver;
struct Tie_req;
struct Time_description;
struct Timing_req;
struct Vertical_brace;
struct Vertical_spanner;
struct Vertical_group_element;
struct Vertical_group_spanner;
struct Vertical_align_spanner;
struct Vertical_align_engraver;
struct Align_element;
struct Sequential_music;
struct Request_chord;

typedef Rational Moment;
typedef Scope Header;

#endif // LILY_PROTO_HH
