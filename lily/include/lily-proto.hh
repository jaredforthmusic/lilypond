/*
  lily-proto.hh -- declare class names.

  source file of the GNU LilyPond music typesetter

  (c)  1997--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#ifndef LILY_PROTO_HH
#define LILY_PROTO_HH
#include "proto.hh"



struct Absolute_dynamic_req;
struct Adobe_font_char_metric;
struct Adobe_font_metric;
struct Align_element;
struct All_font_metrics;
struct Articulation_req;
struct Audio_column;
struct Audio_dynamic;
struct Audio_element;
struct Audio_element_info;
struct Audio_instrument;
struct Audio_item;
struct Audio_key;
struct Audio_note;
struct Audio_note_off;
struct Audio_staff;
struct Audio_tempo;
struct Audio_text;
struct Audio_tie;
struct Audio_time_signature;
struct Auto_beam_engraver;
struct Axis_align_item;
struct Bar;
struct Bar_column;
struct Bar_column_engraver;
struct Bar_engraver;
struct Bar_req;
struct Barcheck_req;
struct Bass_req;
struct Beam;
struct Beam_engraver;
struct Beaming_info;
struct Beaming_info_list;
struct Bezier;
struct Bezier_bow;
struct Blank_req;
struct Bow;
struct Box;
struct Bracket_req;
struct Break_align_item;
struct Break_req;
struct CHyphen_req;
struct Cadenza_req;
struct Change_iterator;
struct Change_translator;
struct Chord;
struct Chord_name;
struct Chord_name_engraver;
struct Chord_tremolo_engraver;
struct Chord_tremolo_req;
struct Clef_change_req;
struct Clef_engraver;
struct Clef_item;
struct Clef_performer;
struct Collision;
struct Collision_engraver;
struct Column_info;
struct Column_spring;
struct Column_x_positions;
struct Command_req;
struct Command_script_req;
struct Command_tie_engraver;
struct Crescendo ;
struct Dimension_cache;
struct Dynamic_performer;
struct Spanner;
struct Dot_column;
struct Dots;
struct Durational_req;
struct Dynamic;
struct Dynamic_req;
struct Element_group;
struct Element_group_item;
struct Engraver;
struct Engraver_group_engraver;
struct Extender;
struct Extender_req;
struct Folded_repeat_iterator;
struct Font_metric;
struct General_script_def;
struct Global_translator;
struct Grace_align_item;
struct Hara_kiri_group_spanner;
struct Hara_kiri_line_group_engraver;
struct Head_column;
struct Idealspacing;
struct Identifier;
struct Input_file;
struct Inversion_req;
struct Item;
struct Key;
struct Key_change_req;
struct Key_engraver;
struct Key_item;
struct Key_performer;
struct Keyword;
struct Keyword_table;
struct Left_edge_item;
struct Lily_stream;
struct Line_group_engraver;
struct Line_of_score;
struct Line_of_staff;
struct Linestaff;
struct Local_key;
struct Local_key_engraver;
struct Local_key_item;
struct Lookup;
struct Lyric_item;
struct Lyric_req;
struct Mark_req;
struct Measure_grouping_req;
struct Melodic_req;
struct Midi_def;
struct Midi_duration;
struct Midi_dynamic;
struct Midi_header;
struct Midi_item;
struct Midi_key;
struct Midi_note;
struct Midi_note_event;
struct Midi_note_off;
struct Midi_output;
struct Midi_score;
struct Midi_stream;
struct Midi_tempo;
struct Midi_time_signature;
struct Midi_track;
struct Midi_walker;
struct Mixed_qp;
struct Molecule;
struct Multi_measure_rest;
struct Multi_measure_rest_engraver;
struct Multi_measure_rest_req;
struct Music;
struct Music_iterator;
struct Music_list;
struct Music_list_iterator;
struct Music_output;
struct Music_output_def;
struct Music_sequence;
struct Music_wrapper;
struct Music_wrapper_iterator;
struct Musical_pitch;
struct Musical_req;
struct Musical_script_req;
struct Musical_span_req;
struct My_lily_lexer;
struct My_lily_parser;
struct Note_column;
struct Note_column_engraver;
struct Note_head;
struct Note_head_engraver;
struct Note_performer;
struct Note_req;
struct Notename_table;
struct Offset;
struct Paper_column;
struct Polynomial;
struct Paper_def;
struct Paper_outputter;
struct Paper_score;
struct Paper_stream;
struct Partial_measure_req;
struct Performance;
struct Performer;
struct Performer;
struct Performer_group_performer;
struct Piano_brace;
struct Property_iterator;
struct Rational;
struct Re_rhythmed_music;
struct Relative_octave_music;
struct Repeat_engraver;
struct Repeated_music;
struct Repeated_music;
struct Repeated_music_iterator;
struct Request;
struct Request_chord;
struct Request_column;
struct Rest;
struct Rest_collision;
struct Rest_collision_engraver;
struct Rest_req;
struct Rhythmic_grouping;
struct Rhythmic_grouping_req;
struct Rhythmic_head;
struct Rhythmic_req;
struct Scope;
struct Score;
struct Score_column;
struct Score_element;
struct Score_element_info;
struct Score_performer;
struct Script;
struct Script_req;
struct Separating_group_spanner;
struct Sequential_music;
struct Simple_music;
struct Simple_spacer;
struct Simultaneous_music;
struct Single_malt_grouping_item;
struct Skip_req;
struct Slur;
struct Slur_bezier_bow;
struct Slur_engraver;
struct Spacing_spanner;
struct Span_bar;
struct Span_req;
struct Span_score_bar;
struct Spanner;
struct Spring;
struct Spring_spacer;
struct Staff_bracket;
struct Staff_performer;
struct Staff_side;
struct Staff_side_item;
struct Staff_symbol;
struct Stem;
struct Stem_beam_engraver;
struct Stem_req;
struct Stem_tremolo;
struct String;
struct Super_element;
struct Symtable;
struct Symtables;
struct Tempo_req;
struct Tex_font_char_metric;
struct Tex_font_metric;
struct Tex_font_metric_reader;
struct Text_def;
struct Text_engraver;
struct Text_gob;
struct Text_item ;
struct Text_item;
struct Text_req;
struct Text_spanner;
struct Tie;
struct Tie_engraver;
struct Tie_req;
struct Time_description;
struct Time_scaled_music;
struct Time_scaled_music_iterator;
struct Time_signature;
struct Time_signature_change_req;
struct Time_signature_engraver;
struct Time_signature_performer;
struct Timing_engraver;
struct Timing_req;
struct Timing_translator;
struct Tonic_req;
struct Translation_property;
struct Translator;
struct Translator_group;
struct Tremolo_req;
struct Tuplet_spanner;
struct Unfolded_repeat_iterator;
struct Vertical_brace;
struct Volta_spanner;
struct Moment;


#endif // LILY_PROTO_HH
