/*
  dynamic-reg.cc -- implement Dynamic_engraver

  source file of the GNU LilyPond music typesetter

  (c)  1997--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/
#include "debug.hh"
#include "crescendo.hh"
#include "musical-request.hh"
#include "lookup.hh"
#include "paper-def.hh"
#include "paper-column.hh"
#include "staff-symbol.hh"
#include "note-column.hh"
#include "text-item.hh"
#include "side-position-interface.hh"
#include "engraver.hh"
#include "stem.hh"
#include "note-head.hh"
#include "group-interface.hh"

/*
  TODO:
    multiple type of span dynamic:
    * hairpin
    * text:
      - `cresc. --  --  --'
      - `cresc. poco a poco -- -- --'
 */

/**
   print text & hairpin dynamics.
 */
class Dynamic_engraver : public Engraver
{
  Text_item * text_p_;
  Crescendo * to_end_cresc_p_;
  Crescendo * cresc_p_;

  Span_req * cresc_req_l_;
  Array<Request*> dynamic_req_l_arr_;
  void  typeset_all ();
public:
  VIRTUAL_COPY_CONS(Translator);
  Dynamic_engraver();
  
protected:

  void announce_element (Score_element_info);
  
  virtual void do_removal_processing ();
  virtual void acknowledge_element (Score_element_info);
  virtual bool do_try_music (Music *req_l);
  virtual void do_process_requests();
  virtual void do_pre_move_processing();
  virtual void do_post_move_processing();
  virtual void typeset_element (Score_element*);
};

void
Dynamic_engraver::announce_element (Score_element_info i)
{
  group (i.elem_l_, "interfaces").add_thing (ly_symbol2scm ("dynamic"));
  
  Engraver::announce_element (i);
}


Dynamic_engraver::Dynamic_engraver()
{
  do_post_move_processing();
  text_p_ =0;

  to_end_cresc_p_ = cresc_p_ = 0;

  cresc_req_l_ = 0;
}

void
Dynamic_engraver::do_post_move_processing()
{
  dynamic_req_l_arr_.clear();
}

bool
Dynamic_engraver::do_try_music (Music * m)
{
  Request * r = dynamic_cast<Request*> (m);

  if(Text_script_req * d = dynamic_cast <Text_script_req *> (r))
    {
      if (d->style_str_ != "dynamic")
	return false;
    }
  else if (Span_req * s =  dynamic_cast <Span_req*> (r))
    {
      if (s-> span_type_str_ != "crescendo"
	  && s->span_type_str_ != "decrescendo")
	return false;
    }
  else
    return false;
  
  for (int i=0; i < dynamic_req_l_arr_.size (); i++)
    if (r->equal_b (dynamic_req_l_arr_[i]))
      return true;
  
  dynamic_req_l_arr_.push (r);
  return true;
}


void
Dynamic_engraver::do_process_requests()
{
  Crescendo*  new_cresc_p=0;

  for (int i=0; i < dynamic_req_l_arr_.size(); i++)
    {
      if (Text_script_req *absd =
	  dynamic_cast<Text_script_req *> ( dynamic_req_l_arr_[i]))
	{
	  if (text_p_)
	    {
	      dynamic_req_l_arr_[i]->warning (_("Got a dynamic already.  Continuing dazed and confused."));
	      continue;
	    }
	  
	  String loud = absd->text_str_;

	  text_p_ = new Text_item;
	  text_p_->set_elt_property ("text",
				     ly_str02scm (loud.ch_C()));
	  text_p_->set_elt_property ("style", gh_str02scm ("dynamic"));
	  text_p_->set_elt_property ("script-priority",
				     gh_int2scm (100));
	  
	  Side_position_interface (text_p_).set_axis (Y_AXIS);

	  
	  if (absd->get_direction ())
	    {
	      text_p_->set_elt_property ("direction", gh_int2scm (absd->get_direction ()));
	    }


	  /*
	    UGH UGH 
	   */
	  SCM prop = get_property ("dynamicDirection");
	  if (!isdir_b (prop))
	    {
	      prop = get_property ("verticalDirection");
	    }

	  if (isdir_b (prop) && to_dir (prop))
	    text_p_->set_elt_property ("direction", prop);

	  prop = get_property ("dynamicPadding");
	  if (gh_number_p(prop))
	    {
	      text_p_->set_elt_property ("padding", prop);
	    }
	  announce_element (Score_element_info (text_p_, absd));
	}
      else if (Span_req *span_l
	       = dynamic_cast <Span_req *> (dynamic_req_l_arr_[i]))
	{
	  if (span_l->span_dir_ == STOP)
	    {
	      if (!cresc_p_)
		{
		  span_l->warning (_ ("Can't find (de)crescendo to end"));
		}
	      else
		{
		  assert (!to_end_cresc_p_);
		  to_end_cresc_p_ =cresc_p_;
		  
		  cresc_p_ = 0;
		}
	    }
	  else if (span_l->span_dir_ == START)
	    {
	      cresc_req_l_ = span_l;
	      assert (!new_cresc_p);
	      new_cresc_p  = new Crescendo;
	      new_cresc_p
		->set_elt_property ("grow-direction",
				    gh_int2scm ((span_l->span_type_str_ == "crescendo") ? BIGGER : SMALLER));
	      
	      side_position (new_cresc_p).set_axis (Y_AXIS);
	      announce_element (Score_element_info (new_cresc_p, span_l));
	    }
	}
    }

  if (new_cresc_p)
    {
      if (cresc_p_)
	{
	  ::warning (_ ("Too many crescendi here"));

	  typeset_element (cresc_p_);

	  cresc_p_ = 0;
	}
      
      cresc_p_ = new_cresc_p;
      cresc_p_->set_bounds(LEFT,get_staff_info().musical_pcol_l ());

      if (text_p_)
	{
	  index_set_cell (cresc_p_->get_elt_property ("dynamic-drul"),
			  LEFT, SCM_BOOL_T);
	  if (to_end_cresc_p_)
	    index_set_cell (to_end_cresc_p_->get_elt_property ("dynamic-drul"),
			    RIGHT, SCM_BOOL_T);
	}
    }
}

void
Dynamic_engraver::do_pre_move_processing()
{
  typeset_all ();
}



ADD_THIS_TRANSLATOR(Dynamic_engraver);

void
Dynamic_engraver::do_removal_processing ()
{
  if (cresc_p_)
    {
      typeset_element (cresc_p_ );
      cresc_req_l_->warning (_ ("unended crescendo"));
      cresc_p_ =0;
    }
  typeset_all ();
}


void
Dynamic_engraver::typeset_all ()
{  
  if (to_end_cresc_p_)
    {
      to_end_cresc_p_->set_bounds(RIGHT,get_staff_info().musical_pcol_l ());
      typeset_element (to_end_cresc_p_);

      to_end_cresc_p_ =0;

    }
  
  if (text_p_)
    {
      typeset_element (text_p_);
      text_p_ =0;
    }
}

void
Dynamic_engraver::typeset_element (Score_element * e)
{
  side_position(e).add_staff_support ();
  Engraver::typeset_element (e);
}

void
Dynamic_engraver::acknowledge_element (Score_element_info i)
{
  if (dynamic_cast<Stem *> (i.elem_l_)
      || dynamic_cast<Note_head *> (i.elem_l_)
      )
    {
      if (text_p_)
	Side_position_interface (text_p_).add_support (i.elem_l_);

      if (to_end_cresc_p_)
	Side_position_interface (to_end_cresc_p_).add_support (i.elem_l_);

      if (cresc_p_)
	Side_position_interface(cresc_p_).add_support (i.elem_l_);
    }
}

