/*
  tie.cc -- implement Tie

  source file of the GNU LilyPond music typesetter

  (c)  1997--2003 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/
#include <math.h>


#include "spanner.hh"
#include "lookup.hh"
#include "paper-def.hh"
#include "tie.hh"
#include "rhythmic-head.hh"
#include "bezier.hh"
#include "paper-column.hh"
#include "warn.hh"
#include "staff-symbol-referencer.hh"
#include "directional-element-interface.hh"
#include "molecule.hh"
#include "bezier-bow.hh"
#include "stem.hh"
#include "note-head.hh"

/*
  tie: Connect two noteheads.

  What if we have

  c4 ~ \clef bass ; c4 or

  c4 \staffchange c4

  do we have non-horizontal ties then?
  */


void
Tie::set_head (Grob*me,Direction d, Grob * h)
{
  assert (!head (me,d));
  index_set_cell (me->get_grob_property ("head-pair"), d, h->self_scm ());
  
  dynamic_cast<Spanner*> (me)->set_bound (d, h);
  me->add_dependency (h);
}

void
Tie::set_interface (Grob*me)
{
  me->set_grob_property ("head-pair", gh_cons (SCM_EOL, SCM_EOL));
}


Grob*
Tie::head (Grob*me, Direction d) 
{
  SCM c = me->get_grob_property ("head-pair");
  c = index_get_cell (c, d);

  return unsmob_grob (c);
}

Real
Tie::get_position (Grob*me) 
{
  Direction d = head (me,LEFT) ? LEFT:RIGHT;
  return Staff_symbol_referencer::get_position (head (me,d));
}


/*
  Default:  Put the tie oppositie of the stem [Wanske p231]

  In case of chords: Tie_column takes over
  
  The direction of the Tie is more complicated (See [Ross] p136 and
  further).

  (what about linebreaks? )  
  
*/
Direction
Tie::get_default_dir (Grob*me) 
{
  Item * sl =  head (me,LEFT) ? Rhythmic_head::get_stem (head (me,LEFT)) :0;
  Item * sr =  head (me,RIGHT) ? Rhythmic_head::get_stem (head (me,RIGHT)) :0;  
  if (sl && sr)
    {
      if (get_grob_direction (sl) == UP
	  && get_grob_direction (sr) == UP)
	return DOWN;
    }
  else if (sl || sr)
    {
      Item *s = sl ? sl : sr;
      return - get_grob_direction (s);
    }

  
  return UP;
}

/*
  TODO: we should also use thickness for computing the clearance
  between head and tie. Very thick ties will now touch the note head.
  
  */
SCM
Tie::get_control_points (SCM smob)
{  
  Spanner*me = unsmob_spanner (smob);
  Direction headdir = CENTER; 
  if (head (me,LEFT))
    headdir = LEFT;
  else if (head (me,RIGHT))
    headdir = RIGHT;
  else
    {
      programming_error ("Tie without heads.");
      me->suicide ();
      return SCM_UNSPECIFIED;
    }

  
  if (!get_grob_direction (me))
    set_grob_direction (me, Tie::get_default_dir (me));
  Direction dir = get_grob_direction (me);
  
  Real staff_space = Staff_symbol_referencer::staff_space (me);

  Real x_gap_f = robust_scm2double (me->get_grob_property ("x-gap"), 0);

  Grob* l = me->get_bound (LEFT);
  Grob* r = me->get_bound (RIGHT);  

  Grob* commonx = me->common_refpoint (l, X_AXIS);
  commonx = me->common_refpoint (r, X_AXIS);
  
  Real left_x;

  /*
     the tie has to be long enough to be visible, but should not go
    through key sigs. In the 1.5 series the pref.matter - note
    distance is fixed , so this won't be a problem anymore.
   */
  Real lambda = 0.9;		
  
  if (Note_head::has_interface (l))
    {
      Real where = RIGHT;

      /*
	This correction is due te the shape of the black note head.
       */
      if (Rhythmic_head::duration_log (l) == 2)
	where += dir* 0.2;
      left_x = l->extent (l, X_AXIS).linear_combination (where)
	+ x_gap_f;
    }
  else
    left_x = l->extent (l, X_AXIS).linear_combination (lambda);
  

  Real width;
  if (Note_head::has_interface (l) && Note_head::has_interface (r))
    {
      width = 
	+ r->extent (commonx,X_AXIS)[LEFT]
	- l->extent (commonx, X_AXIS)[RIGHT]
	-2 * x_gap_f;
    }
  else
    {
      if (Note_head::has_interface (l))
	width = r->relative_coordinate (commonx, X_AXIS)
	  - l->extent (commonx, X_AXIS)[RIGHT]
	  - 2 * x_gap_f;
      else
	width =
	  - l->extent (commonx, X_AXIS).linear_combination (lambda)  
	  + r->extent (commonx, X_AXIS)[LEFT]
	  - 2 * x_gap_f;
    }
  


  SCM details = me->get_grob_property ("details");

  SCM lim // groetjes aan de chirurgendochter.
    = scm_assq (ly_symbol2scm ("height-limit"),details);
  
  Real h_inf = gh_scm2double (ly_cdr (lim)) *  staff_space;
  Real r_0 = gh_scm2double (ly_cdr (scm_assq (ly_symbol2scm ("ratio"),details)));

  Bezier b  = slur_shape (width, h_inf, r_0);
  
  /*
    I think this better, particularly for small ties. It always allows the user to move ties if
    they seem in the wrong place

    TODO: what if 2 heads have different size.

  */

  Real ypos = Tie::get_position (me) * staff_space/2
    + dir * gh_scm2double (me->get_grob_property ("y-offset"));;

  /*
    Make sure we don't start on a dots
   */
  if (Note_head::has_interface (l) && Rhythmic_head::get_dots (l))
    {
      Grob* dots = Rhythmic_head::get_dots (l);
      if(fabs (staff_space * Staff_symbol_referencer::get_position (dots) /2
	       - ypos) < 0.5)
	{
	  ypos += 0.5 * dir ;
	}
    }

  
  /*
    todo: prevent ending / staffline collision.

    todo: tie / stem collision
   */

  b = slur_shape (width,h_inf, r_0);
  b.scale (1, dir);
  b.translate (Offset (left_x, ypos));
  

  /*
    Avoid colliding of the horizontal part with stafflines.

    
    TODO: redo this, heuristic is half-baken, and ties often look ugly
    as a result.

    TODO: doesn't work when on staff with even number of lines.
   */
  Array<Real> horizontal (b.solve_derivative (Offset (1,0)));
  if (horizontal.size ())
    {
      /*
	ugh. Doesnt work for non-horizontal curves.
       */
      Real y = b.curve_point (horizontal[0])[Y_AXIS];

      Real ry = rint (y/staff_space) * staff_space;
      Real diff = ry - y;
      Real newy = y;

      Real clear = staff_space * gh_scm2double (me->get_grob_property ("staffline-clearance"));

      if (fabs (y) <=
	  Staff_symbol_referencer::staff_radius (me) * staff_space + clear
	  && fabs (diff) < clear)
	{
	  Real y1 = ry + clear;
	  Real y2 = ry - clear;

	  /*
	    ugh, we shove the 0.5 out of our sleeves.

	    Any way. This test is to make sure that staffline
	    collision avoidance does not result in completely flat
	    ties.
	   */
	  if (fabs (y1 - ypos) < 0.5)
	    y1 = y2;
	  else if (fabs (y2 - ypos) < 0.5)
	    y2 = y1;
	  
	  newy = (fabs (y1 - y) < fabs (y2 - y)) ? y1 : y2;
	  
	  // newy = ry - 0.5 * staff_space * sign (diff) ;

	  /*
	    we don't want horizontal ties
	   */
	  if (fabs (newy - b.control_[0][Y_AXIS]) < 1e-2)
	    {
	      newy = newy + dir * staff_space; 
	    }
	}

      Real y0 = b.control_ [0][Y_AXIS];
      b.control_[2][Y_AXIS] = 
      b.control_[1][Y_AXIS] =
 (b.control_[1][Y_AXIS] - y0)  * ((newy - y0) / (y - y0)) + y0; 
    }
  else
    programming_error ("Tie is nowhere horizontal");



  SCM controls = SCM_EOL;
  for (int i= 4; i--;)
    controls = gh_cons (ly_offset2scm (b.control_[i]), controls);
  return controls;
}


MAKE_SCHEME_CALLBACK (Tie,brew_molecule,1);
SCM
Tie::brew_molecule (SCM smob) 
{
  Grob*me = unsmob_grob (smob);

  SCM cp = me->get_grob_property ("control-points");
  if (cp == SCM_EOL)
    {
      cp = get_control_points (smob);
      me->set_grob_property ("control-points", cp);
    }
  
  Real thick =
    gh_scm2double (me->get_grob_property ("thickness"))
    * me->get_paper ()->get_realvar (ly_symbol2scm ("linethickness"));

  Bezier b;
  int i = 0;
  for (SCM s= cp; s != SCM_EOL; s = ly_cdr (s))
    {
      b.control_[i] = ly_scm2offset (ly_car (s));
      i++;
    }
  
   Molecule a = Lookup::slur (b, get_grob_direction (me) * thick, thick);
   
   return a.smobbed_copy ();
}



ADD_INTERFACE (Tie,"tie-interface",
  "A tie connecting two noteheads.\n"
"direction = Forced direction for all ties",
  "y-offset staffline-clearance control-points head-pair details thickness x-gap direction minimum-length");
