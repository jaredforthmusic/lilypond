/*
  axis-group.cc -- implement Graphical_axis_group

  source file of the GNU LilyPond music typesetter

  (c)  1997--1998 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/
#include "interval.hh"
#include "graphical-axis-group.hh"
#include "axis-group-element.hh"
#include "graphical-element.hh"
#include "debug.hh"

/** don't copy anything: an element can only be in one
  Graphical_axis_group at one time.  */
Graphical_axis_group::Graphical_axis_group (Graphical_axis_group const&s)
{
  axes_[0] = s.axes_[0];
  axes_[1] = s.axes_[1];
}

bool 
Graphical_axis_group::contains_b (Graphical_element const *e) const
{
  return elem_l_arr_.find_l (e);
}

Interval
Graphical_axis_group::extent (Axis axis) const
{
  Interval r;
  for (int i=0; i < elem_l_arr_.size(); i++) 
    r.unite (elem_l_arr_[i]->extent (axis));
  return r;
}

void
Graphical_axis_group::add_element (Graphical_element*e)
{
  for (int i = 0; i < 2; i++)
    {
      Axis a = axes_[i];
      Dimension_cache * &d = e->dim_cache_[a].parent_l_;
      assert (!d || d == &dim_cache_[a]);
      d = &dim_cache_[a];
      d->dependencies_l_arr_.push (&dim_cache_[a]);
    }

  elem_l_arr_.push (e);
}



void
Graphical_axis_group::remove_element (Graphical_element*e)
{
  assert (contains_b (e));
  elem_l_arr_.unordered_substitute (e,0);
  
  for (int i=0; i<  2; i++)
    {
      Axis a=axes_[i];
      Dimension_cache * d = &e->dim_cache_[a];
      d->parent_l_ = 0;
      d->dependencies_l_arr_.unordered_substitute (&dim_cache_[a], 0);
    }
}

void
Graphical_axis_group::remove_all ()
{
  for (int i=0; i < elem_l_arr_.size(); i++) 
    {
      Graphical_element*e=elem_l_arr_[i];
      for (int i=0; i<  2; i++)
	{
	  Axis a=axes_[i];
	  Dimension_cache * d = &e->dim_cache_[a];
	  d->parent_l_ = 0;
	  d->dependencies_l_arr_.clear ();
	}
      
    }
  elem_l_arr_.clear ();
}


void    
Graphical_axis_group::do_print() const
{
#ifndef NPRINT
  for (int i=0; i < elem_l_arr_.size(); i++) 
    DOUT << classname(elem_l_arr_[i]) << " ";
#endif
}

Graphical_axis_group::Graphical_axis_group (Axis a1, Axis a2)
{
  axes_[0] = a1;
  axes_[1] = a2;
}


