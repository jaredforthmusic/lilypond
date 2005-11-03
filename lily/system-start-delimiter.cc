/*
  system-start-delimiter.cc -- implement System_start_delimiter

  source file of the GNU LilyPond music typesetter

  (c) 2000--2005 Han-Wen Nienhuys <hanwen@xs4all.nl>
*/

#include "system-start-delimiter.hh"


#include "spanner.hh"
#include "axis-group-interface.hh"
#include "output-def.hh"
#include "font-interface.hh"
#include "all-font-metrics.hh"
#include "staff-symbol-referencer.hh"
#include "lookup.hh"
#include "item.hh"

#include "pointer-group-interface.hh"

Stencil
System_start_delimiter::staff_bracket (Grob *me, Real height)
{
  Font_metric *fm = Font_interface::get_default_font (me);
  Drul_array<Stencil> tips (fm->find_by_name ("brackettips.down"),
			    fm->find_by_name ("brackettips.up"));

  Real thickness = robust_scm2double (me->get_property ("thickness"), 0.25);

  Real overlap = 0.1 * thickness;

  Stencil bracket = Lookup::filled_box (Box (Interval (0, thickness),
					     Interval (-1, 1)
					     * (height / 2 + overlap)));

  Direction d = DOWN;
  do
    bracket.add_at_edge (Y_AXIS, d, tips[d], -overlap, 0.0);
  while (flip (&d) != DOWN)
    ;

  return bracket;
}

Stencil
System_start_delimiter::simple_bar (Grob *me, Real h)
{
  Real lt = me->layout ()->get_dimension (ly_symbol2scm ("linethickness"));
  Real w = lt * robust_scm2double (me->get_property ("thickness"), 1);
  return Lookup::round_filled_box (Box (Interval (0, w), Interval (-h / 2, h / 2)),
				   lt);
}

MAKE_SCHEME_CALLBACK (System_start_delimiter, print, 1);
SCM
System_start_delimiter::print (SCM smob)
{
  Spanner *me = unsmob_spanner (smob);
  if (!me)
    return SCM_EOL;

  SCM s = me->get_property ("glyph");
  if (!scm_is_string (s))
    return SCM_EOL;
  SCM gsym = scm_string_to_symbol (s);

  Real staff_space = Staff_symbol_referencer::staff_space (me);

  extract_grob_set (me, "elements", elts);
  Grob *common = common_refpoint_of_array (elts, me, Y_AXIS);

  Interval ext;

  int non_empty_count = 0;
  for (int i = elts.size (); i--;)
    {
      Spanner *sp = dynamic_cast<Spanner *> (elts[i]);

      if (sp
	  && sp->get_bound (LEFT) == me->get_bound (LEFT))
	{
	  Interval dims = sp->extent (common, Y_AXIS);
	  if (!dims.is_empty ())
	    {
	      non_empty_count ++;
	      ext.unite (dims);
	    }
	}
    }

  if (gsym == ly_symbol2scm ("bar-line")
      && non_empty_count <= 1)
    {
      me->suicide ();
      return SCM_EOL;
    }
  
  ext -= me->relative_coordinate (common, Y_AXIS);

  Real len = ext.length () / staff_space;

  if (ext.is_empty ()
      || (robust_scm2double (me->get_property ("collapse-height"), 0.0) >= len))
    {
      me->suicide ();
      return SCM_EOL;
    }

  Stencil m;

  if (gsym == ly_symbol2scm ("bracket"))
    m = staff_bracket (me, len);
  else if (gsym == ly_symbol2scm ("brace"))
    m = staff_brace (me, len);
  else if (gsym == ly_symbol2scm ("bar-line"))
    m = simple_bar (me, len);

  m.translate_axis (ext.center (), Y_AXIS);
  return m.smobbed_copy ();
}

Stencil
System_start_delimiter::staff_brace (Grob *me, Real y)
{
  Font_metric *fm = 0;
  /* We go through the style sheet to lookup the font file
     name.  This is better than using find_font directly,
     esp. because that triggers mktextfm for non-existent
     fonts. */
  SCM fam = scm_cons (ly_symbol2scm ("font-encoding"),
		      ly_symbol2scm ("fetaBraces"));

  SCM alist = scm_list_n (fam, SCM_UNDEFINED);
  fm = select_font (me->layout (), scm_list_n (alist, SCM_UNDEFINED));

  int lo = 0;
  int hi = max (fm->count () - 1, 2);

  /* do a binary search for each Y, not very efficient, but passable?  */
  Box b;
  do
    {
      int cmp = (lo + hi) / 2;
      b = fm->get_indexed_char (cmp);
      if (b[Y_AXIS].is_empty () || b[Y_AXIS].length () > y)
	hi = cmp;
      else
	lo = cmp;
    }
  while (hi - lo > 1);

  Stencil stil (fm->find_by_name ("brace" + to_string (lo)));
  stil.translate_axis (-b[X_AXIS].length()/2, X_AXIS);

  return stil;
}

ADD_INTERFACE (System_start_delimiter, "system-start-delimiter-interface",
	       "The brace, bracket or bar in front of the system. "
	       "It is implemented as a spanner.",

	       /* properties */
	       "collapse-height "
	       "glyph "
	       "thickness "
	       );
