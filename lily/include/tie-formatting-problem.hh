/*
  tie-formatting-problem.hh -- declare

  source file of the GNU LilyPond music typesetter

  (c) 2005 Han-Wen Nienhuys <hanwen@xs4all.nl>

*/

#ifndef TIE_FORMATTING_PROBLEM_HH
#define TIE_FORMATTING_PROBLEM_HH

#include "drul-array.hh"
#include "parray.hh"
#include "skyline.hh"
#include "lily-proto.hh"
#include "tie-configuration.hh"

#include <map>
#include <set>

struct Tie_details
{
  Real height_limit_;
  Real ratio_;
  Real staff_space_;
  Real x_gap_;
  Real between_length_limit_;
  Grob *staff_symbol_referencer_;
  
  Tie_details ();
  void from_grob (Grob *);
};


typedef map< pair<int, int>, Tie_configuration *> Tie_configuration_map;

struct Tie_specification
{
  int position_;

  bool has_manual_position_;
  bool has_manual_dir_;
  
  Real manual_position_;
  Direction manual_dir_;
  
  Tie_specification ();
};

struct Tie_configuration_variation
{
  int index_;
  Tie_configuration *suggestion_;
  Tie_configuration_variation ();
};

class Tie_formatting_problem
{
  Drul_array< Array<Skyline_entry> > chord_outlines_;
  set<int> dot_positions_;
  Interval dot_x_;
  Array<Tie_specification> specifications_;
  
  Tie_configuration_map possibilities_;

  Grob *x_refpoint_;

  
  Tie_configuration *get_configuration (int position, Direction dir);
  Tie_configuration *generate_configuration (int position, Direction dir) const;
  Array<Tie_configuration_variation> get_variations (Ties_configuration const &ties);

  Real score_configuration (Tie_configuration const&) const;
  Real score_aptitude (Tie_configuration const&, int) const;
  Real score_ties_aptitude (Ties_configuration const &ties) const;
  Real score_ties_configuration (Ties_configuration const &ties) const;
  void set_ties_config_standard_directions (Ties_configuration *tie_configs_ptr);
  Real score_ties (Ties_configuration const&) const;
  Ties_configuration generate_base_chord_configuration ();
  
public:
  Tie_details details_;

public:
  Tie_formatting_problem ();
  ~Tie_formatting_problem ();


  Ties_configuration generate_optimal_chord_configuration ();
  Ties_configuration generate_ties_configuration (Ties_configuration const &);
  Tie_configuration find_optimal_tie_configuration (int p, Direction d) const;
  void from_ties (Link_array<Grob> const &ties);
  void from_tie (Grob *tie);
  void from_lv_ties (Link_array<Grob> const &);
  void set_chord_outline (Link_array<Item>, Direction);
  void set_manual_tie_configuration (SCM);
  Interval get_attachment (Real) const;
  Grob *common_x_refpoint () const;
};

#endif /* TIE_FORMATTING_PROBLEM_HH */
