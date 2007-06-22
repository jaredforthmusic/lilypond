/*
  spacing-spanner.cc -- implement Spacing_spanner

  source file of the GNU LilyPond music typesetter

  (c) 1999--2007 Han-Wen Nienhuys <hanwen@xs4all.nl>
*/

#include "spacing-spanner.hh"

#include <math.h>
#include <cstdio>

#include "spacing-options.hh"
#include "international.hh"
#include "main.hh"
#include "moment.hh"
#include "note-spacing.hh"
#include "output-def.hh"
#include "paper-column.hh"
#include "paper-score.hh"
#include "pointer-group-interface.hh"
#include "spaceable-grob.hh"
#include "spacing-interface.hh"
#include "staff-spacing.hh"
#include "system.hh"
#include "warn.hh"

vector<Grob*>
Spacing_spanner::get_columns (Grob *me_grob)
{
  Spanner *me = dynamic_cast<Spanner*> (me_grob);
  vector<Grob*> all (get_root_system (me)->used_columns ());
  vsize start = binary_search (all, (Grob*)me->get_bound (LEFT),
			       &Paper_column::less_than);
  vsize end = binary_search (all, (Grob*) me->get_bound (RIGHT),
			     &Paper_column::less_than);  
  
  all = vector<Grob*>::vector<Grob*> (all.begin () + start,
				      all.begin () + end + 1);
  return all;
}

MAKE_SCHEME_CALLBACK (Spacing_spanner, set_springs, 1);
SCM
Spacing_spanner::set_springs (SCM smob)
{
  Spanner *me = unsmob_spanner (smob);

  /*
    can't use get_system () ? --hwn.
  */
  Spacing_options options;
  options.init_from_grob (me);
  vector<Grob*> cols = Spacing_spanner::get_columns (me);
  set_explicit_neighbor_columns (cols);

  prune_loose_columns (me, &cols, &options);
  set_implicit_neighbor_columns (cols);
  generate_springs (me, cols, &options);

  return SCM_UNSPECIFIED;
}

/*
  We want the shortest note that is also "common" in the piece, so we
  find the shortest in each measure, and take the most frequently
  found duration.

  This probably gives weird effects with modern music, where every
  note has a different duration, but hey, don't write that kind of
  stuff, then.
*/

MAKE_SCHEME_CALLBACK (Spacing_spanner, calc_common_shortest_duration, 1);
SCM 
Spacing_spanner::calc_common_shortest_duration (SCM grob)
{
  Spanner *me = unsmob_spanner (grob);

  vector<Grob*> cols (get_columns (me));
  
  /*
    ascending in duration
  */
  vector<Rational> durations;
  vector<int> counts;

  Rational shortest_in_measure;
  shortest_in_measure.set_infinite (1);

  for (vsize i = 0; i < cols.size (); i++)
    {
      if (Paper_column::is_musical (cols[i]))
	{
	  Moment *when = unsmob_moment (cols[i]->get_property ("when"));

	  /*
	    ignore grace notes for shortest notes.
	  */
	  if (when && when->grace_part_)
	    continue;

	  SCM st = cols[i]->get_property ("shortest-starter-duration");
	  Moment this_shortest = *unsmob_moment (st);
	  assert (this_shortest.to_bool ());
	  shortest_in_measure = min (shortest_in_measure, this_shortest.main_part_);
	}
      else if (!shortest_in_measure.is_infinity ()
	       && Paper_column::is_breakable (cols[i]))
	{
	  vsize j = 0;
	  for (; j < durations.size (); j++)
	    {
	      if (durations[j] > shortest_in_measure)
		{
		  counts.insert (counts.begin () + j, 1);
		  durations.insert (durations.begin () + j, shortest_in_measure);
		  break;
		}
	      else if (durations[j] == shortest_in_measure)
		{
		  counts[j]++;
		  break;
		}
	    }

	  if (durations.size () == j)
	    {
	      durations.push_back (shortest_in_measure);
	      counts.push_back (1);
	    }

	  shortest_in_measure.set_infinite (1);
	}
    }

  int max_idx = -1;
  int max_count = 0;
  for (vsize i = durations.size (); i--;)
    {
      if (counts[i] >= max_count)
	{
	  max_idx = i;
	  max_count = counts[i];
	}
    }

  SCM bsd = me->get_property ("base-shortest-duration");
  Rational d = Rational (1, 8);
  if (Moment *m = unsmob_moment (bsd))
    d = m->main_part_;

  if (max_idx >= 0)
    d = min (d, durations[max_idx]);

  return Moment (d).smobbed_copy ();
}

void
Spacing_spanner::generate_pair_spacing (Grob *me,
					Paper_column *left_col, Paper_column *right_col,
					Paper_column *after_right_col,
					Spacing_options const *options)
{
  if (Paper_column::is_musical (left_col))
    {
      if (!Paper_column::is_musical (right_col)
	  && options->float_nonmusical_columns_
	  && after_right_col
	  && Paper_column::is_musical (after_right_col))
	{
	  /*
	    TODO: should generate rods to prevent collisions.
	  */
	  musical_column_spacing (me, left_col, after_right_col, options);
	  right_col->set_object ("between-cols", scm_cons (left_col->self_scm (),
							   after_right_col->self_scm ()));
	}
      else
	musical_column_spacing (me, left_col, right_col, options);

      if (Item *rb = right_col->find_prebroken_piece (LEFT))
	musical_column_spacing (me, left_col, rb, options);
    }
  else
    {
      /*
	The case that the right part is broken as well is rather
	rare, but it is possible, eg. with a single empty measure,
	or if one staff finishes a tad earlier than the rest.
      */
      Item *lb = left_col->find_prebroken_piece (RIGHT);
      Item *rb = right_col->find_prebroken_piece (LEFT);

      if (left_col && right_col)
	breakable_column_spacing (me, left_col, right_col, options);

      if (lb && right_col)
	breakable_column_spacing (me, lb, right_col, options);

      if (left_col && rb)
	breakable_column_spacing (me, left_col, rb, options);

      if (lb && rb)
	breakable_column_spacing (me, lb, rb, options);
    }
}

void
Spacing_spanner::generate_springs (Grob *me,
				   vector<Grob*> const &cols,
				   Spacing_options const *options)
{
  Paper_column *prev = 0;
  for (vsize i = 0; i < cols.size (); i++)
    {
      Paper_column *col = dynamic_cast<Paper_column *> (cols[i]);
      Paper_column *next = (i + 1 < cols.size ()) ? dynamic_cast<Paper_column *> (cols[i+1]) : 0;
      
      if (i > 0)
	generate_pair_spacing (me, prev, col, next, options);

      prev = col;
    }
}

/*
  Generate the space between two musical columns LEFT_COL and RIGHT_COL.
*/
void
Spacing_spanner::musical_column_spacing (Grob *me,
					 Item *left_col,
					 Item *right_col,
					 Spacing_options const *options)
{
  Real base_note_space = note_spacing (me, left_col, right_col, options);
  Spring spring;

  if (options->stretch_uniformly_)
    spring = Spring (base_note_space, 0.0);
  else
    {
      vector<Spring> springs;
      extract_grob_set (left_col, "right-neighbors", neighbors);

      for (vsize i = 0; i < neighbors.size (); i++)
	{
	  Grob *wish = neighbors[i];

	  Item *wish_rcol = Spacing_interface::right_column (wish);
	  if (Spacing_interface::left_column (wish) != left_col
	      || (wish_rcol != right_col && wish_rcol != right_col->original ()))
	    continue;

	  /*
	    This is probably a waste of time in the case of polyphonic
	    music.  */
	  if (Note_spacing::has_interface (wish))
	    springs.push_back (Note_spacing::get_spacing (wish, right_col, base_note_space, options->increment_));
	}

      if (springs.empty ())
	{

	  if (!Paper_column::is_musical (right_col))
	    {
	      /*
		There used to be code that examined left_col->extent
		(X_AXIS), but this is resulted in unexpected wide
		spacing, because the width of s^"text" output is also
		taken into account here.
	       */
	      spring = Spring (max (base_note_space, options->increment_),
			       options->increment_);
	    }
	  else
	    {
	      /*
		Fixed should be 0.0. If there are no spacing wishes, we're
		likely dealing with polyphonic spacing of hemiolas.
	    
		We used to have min_distance_ = options->increment_

		but this can lead to numeric instability problems when we
		do
	    
		inverse_strength = (distance_ - min_distance_)
      
	      */
	      spring = Spring (base_note_space, 0.0);
	    }
	}
      else
	spring = merge_springs (springs);
    }

  if (Paper_column::when_mom (right_col).grace_part_
      && !Paper_column::when_mom (left_col).grace_part_)
    {
      /*
	Ugh. 0.8 is arbitrary.
      */
      spring *= 0.8;
    }

  /*
    TODO: make sure that the space doesn't exceed the right margin.
  */
  if (options->packed_)
    {
      /*
	In packed mode, pack notes as tight as possible.  This makes
	sense mostly in combination with raggedright mode: the notes
	are then printed at minimum distance.  This is mostly useful
	for ancient notation, but may also be useful for some flavours
	of contemporary music.  If not in raggedright mode, lily will
	pack as much bars of music as possible into a line, but the
	line will then be stretched to fill the whole linewidth.
      */
      spring.set_distance (spring.min_distance ());
      spring.set_inverse_stretch_strength (1.0);
    }

  Spaceable_grob::add_spring (left_col, right_col, spring);
}

/*
  Check if COL fills the whole measure.
 */
bool
Spacing_spanner::fills_measure (Grob *me, Item *left, Item *col)
{
  System *sys = get_root_system (me);
  Item *next = sys->column (col->get_column ()->get_rank () + 1);
  if (!next)
    return false;

  if (Paper_column::is_musical (next)
      || Paper_column::is_musical (left)
      || !Paper_column::is_musical (col)
      || !Paper_column::is_used (next))
    return false;
  
  Moment dt =
    Paper_column::when_mom (next) - Paper_column::when_mom (col);
  
  Moment *len = unsmob_moment (left->get_property ("measure-length"));
  if (!len)
    return false;
  
  /*
    Don't check for exact measure length, since ending measures are
    often shortened due to pickups.
   */
  if (dt.main_part_ > len->main_part_ / Rational (2)
      && (next->is_broken ()
	  || next->break_status_dir ()))
    return true;

  return false;
}

/*
  Read hints from L and generate springs.
*/
void
Spacing_spanner::breakable_column_spacing (Grob *me, Item *l, Item *r,
					   Spacing_options const *options)
{
  vector<Spring> springs;
  Spring spring;

  Moment dt = Paper_column::when_mom (r) - Paper_column::when_mom (l);

  if (dt == Moment (0, 0))
    {
      extract_grob_set (l, "spacing-wishes", wishes);

      for (vsize i = 0; i < wishes.size (); i++)
	{
	  Item *spacing_grob = dynamic_cast<Item *> (wishes[i]);

	  if (!spacing_grob || !Staff_spacing::has_interface (spacing_grob))
	    continue;

	  /*
	    column for the left one settings should be ok due automatic
	    pointer munging.
	  */
	  assert (spacing_grob->get_column () == l);

	  springs.push_back (Staff_spacing::get_spacing (spacing_grob, r));
	}
    }

  if (springs.empty ())
    spring = standard_breakable_column_spacing (me, l, r, options);
  else
    spring = merge_springs (springs);

  if (Paper_column::when_mom (r).grace_part_)
    {
      /*
	Correct for grace notes.
	
	Ugh. The 0.8 is arbitrary.
      */
      spring *= 0.8;
    }

  if (Paper_column::is_musical (r)
      && l->break_status_dir () == CENTER
      && fills_measure (me, l, r))
    {
      spring.set_distance (spring.distance () + 1.0);
      spring.set_default_strength ();
    }
  
  if (options->stretch_uniformly_ && l->break_status_dir () != RIGHT)
    {
      spring.set_min_distance (0.0);
      spring.set_default_strength ();
    }

  Spaceable_grob::add_spring (l, r, spring);
}

ADD_INTERFACE (Spacing_spanner,
	       "The space taken by a note is dependent on its duration. Doubling a\n"
	       "duration adds spacing-increment to the space. The most common shortest\n"
	       "note gets @code{shortest-duration-space}. Notes that are even shorter are\n"
	       "spaced proportonial to their duration.\n"
	       "\n"
	       "Typically, the increment is the width of a black note head.  In a\n"
	       "piece with lots of 8th notes, and some 16th notes, the eighth note\n"
	       "gets 2 note heads width (i.e. the space following a note is 1 note\n"
	       "head width) A 16th note is followed by 0.5 note head width. The\n"
	       "quarter note is followed by  3 NHW, the half by 4 NHW, etc.\n",

	       
	       "average-spacing-wishes "
	       "base-shortest-duration "
	       "common-shortest-duration "
	       "packed-spacing "
	       "shortest-duration-space "
	       "spacing-increment "
	       "strict-grace-spacing "
	       "strict-note-spacing "
	       "uniform-stretching "
	       
	       );

