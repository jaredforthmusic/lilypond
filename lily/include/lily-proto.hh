/*
  lily-proto.hh -- declare class names.

  source file of the GNU LilyPond music typesetter

  (c) 1997 Han-Wen Nienhuys <hanwen@stack.nl>
*/

#ifndef LILY_PROTO_HH
#define LILY_PROTO_HH
#include "proto.hh"

struct My_lily_lexer;
struct Absolute_dynamic_req;
struct Translator;
struct Atom;
struct Bar;
struct Bar_engraver;
struct Bar_req;
struct Barcheck_req;
struct Beam;
struct Beam_req;
struct Blank_req;
struct Box;
struct Break_align_item;
struct Bracket_req;
struct Cadenza_req;
struct Change_iterator;
struct Change_reg;
struct Chord;
struct Clef_change_req;
struct Clef_item;
struct Clef_engraver;
struct Col_hpositions;
struct Chord;
struct Colinfo;
struct Collision;
struct Collision_engraver;
struct Command_req;
struct Command_script_req;
struct Complex_music;
struct Cresc_req;
struct Crescendo ;
struct Decresc_req;
struct Directional_spanner;
struct Durational_req;
struct Dynamic;
struct Dynamic_req;
struct Element_group;
struct Element_group_item;
struct Feature;
struct General_script_def;
struct Global_translator;
struct Group_change_req;
struct Group_feature_req;
struct Horizontal_align_item;
struct Horizontal_group;
struct Horizontal_group_item;
struct Horizontal_vertical_group;
struct Idealspacing;
struct Identifier;
struct Interpreter;
struct Input_file;
struct Input_engraver;
struct Input_score;
struct Item;
struct Key;
struct Key_change_req;
struct Key_item;
struct Key_engraver;
struct Keyword;
struct Keyword_table;
struct Lily_stream;
struct Line_of_score;
struct Line_of_staff;
struct Linestaff;
struct Local_key;
struct Local_key_item;
struct Local_key_engraver;
struct Lookup;
struct Lyric_item;
struct Lyric_req;
struct Measure_grouping_req;
struct Melodic_req;
struct Meter;
struct Meter_change_req;
struct Meter_engraver;
struct Midi_def;
struct Midi_duration;
struct Midi_header;
struct Midi_item;
struct Midi_output;
struct Midi_pitch;
struct Midi_score;
struct Midi_stream;
struct Midi_track;
struct Mixed_qp;
struct Molecule;
struct Music_general_chord;
struct Musical_script_req;
struct Music_voice;
struct Musical_req;
struct Music;
struct Music_list;
struct My_lily_parser;
struct Note_column;
struct Note_column_engraver;
struct Note_req;
struct Note_head;
struct Note_head_engraver;
struct Notename_table;
struct Offset;
struct Output;
struct PCol;
struct PScore;
struct Paper_def;
struct Partial_measure_req;
struct Plet;
struct Pulk_voice;
struct Pulk_voices;
struct Rational;
struct Engraver_group_engraver;
struct Request;
struct Request_column;
struct Request_engraver;
struct Rest;
struct Rest_collision;
struct Rest_collision_engraver;
struct Rest_column;
struct Rest_req;
struct Rhythmic_grouping;
struct Rhythmic_grouping_req;
struct Rhythmic_req;
struct Score;
struct Score_column;
struct Score_elem;
struct Score_elem_info;
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
struct Spacing_req ;
struct Span_dynamic_req;
struct Span_req;
struct Spanner;
struct Staff_side;
struct Staff_symbol;
struct Stem;
struct Stem_beam_engraver;
struct Stem_req;
struct String;
struct Subtle_req;
struct Symbol;
struct Symtable;
struct Symtables;
struct Super_elem;
struct Terminate_voice_req;
struct Tex_stream;
struct Text_def;
struct Text_gob;
struct Text_item ;
struct Text_engraver;
struct Text_req;
struct Tie;
struct Tie_engraver;
struct Tie_req;
struct Time_description;
struct Timing_req;
struct Vertical_brace;
struct Vertical_spanner;
struct Vertical_group;

struct Voice;
struct Voice_element;
struct Voice_group_engravers;
struct Voice_list;
struct Voice_engravers;
struct Voicegroup;
struct Walker_engravers;
typedef Rational Moment;
#endif // LILY_PROTO_HH
