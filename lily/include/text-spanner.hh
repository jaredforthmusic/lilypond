/*
  text-spanner.hh -- declare Text_spanner

  source file of the GNU LilyPond music typesetter

  (c) 2000--2004 Jan Nieuwenhuizen <janneke@gnu.org>
*/


#ifndef TEXT_SPANNER_HH
#define TEXT_SPANNER_HH

#include "lily-guile.hh"

class Text_spanner
{
public:
  DECLARE_SCHEME_CALLBACK (brew_molecule, (SCM));
  static bool has_interface (Grob*);
private:
  static void setup_pedal_bracket(Spanner *s);
};

#endif /* TEXT_SPANNER_HH */
