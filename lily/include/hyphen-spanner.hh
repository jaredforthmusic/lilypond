/*
  hyphen-spanner.hh -- part of GNU LilyPond

  (c)  1999--2003 Glen Prideaux <glenprideaux@iname.com>
*/

#ifndef HYPHEN_SPANNER_HH
#define HYPHEN_SPANNER_HH

#include "spanner.hh"

struct Hyphen_spanner 
{
public:
  Spanner* elt_;

  DECLARE_SCHEME_CALLBACK(set_spacing_rods, (SCM));
  Hyphen_spanner (Spanner*);
  void set_textitem (Direction, Grob*);
  bool has_interface (Grob*);
  DECLARE_SCHEME_CALLBACK (brew_molecule, (SCM ));
};

#endif // HYPHEN_SPANNER_HH

