/*   
  auto-change-iterator.cc -- implement  Auto_change_iterator

  source file of the GNU LilyPond music typesetter
  
  (c) 1999--2004 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#include "music.hh"
#include "translator-group.hh"
#include "event.hh"
#include "music-wrapper-iterator.hh"
#include "direction.hh"

class Auto_change_iterator : public Music_wrapper_iterator
{
public:
  DECLARE_SCHEME_CALLBACK(constructor, ());
  
  Auto_change_iterator ();

protected:
  virtual void construct_children ();
  virtual void process (Moment);  
  Array<Pitch> pending_pitch (Moment)const;
private:
  SCM split_list_;
  Direction where_dir_;
  void change_to (Music_iterator* , SCM, String);
};



void
Auto_change_iterator::change_to (Music_iterator *it, SCM to_type_sym,
				 String to_id)
{
  Translator_group * current = it->get_outlet ();
  Translator_group * last = 0;

  /*
    Cut & Paste from Change_iterator (ugh).

    TODO: abstract this function 
   */
  
  /* find the type  of translator that we're changing.
     
     If \translator Staff = bass, then look for Staff = *
   */
  while (current && !current->is_alias (to_type_sym))
    {
      last = current;
      current = current->daddy_trans_;
    }

  if (current && current->id_string_ == to_id)
    {
      String msg;
      msg += _ ("Can't switch translators, I'm there already");
    }
  
  if (current) 
    if (last)
      {
	Translator_group * dest = 
	  it->get_outlet ()->find_create_translator (to_type_sym, to_id, SCM_EOL);
	current->remove_translator (last);
	dest->add_used_group_translator (last);
      }
    else
      {
	/*
	  We could change the current translator's id, but that would make 
	  errors hard to catch
	  
	*/
      }
  else
    ;

}

void
Auto_change_iterator::process (Moment m)
{
  Music_wrapper_iterator::process (m);

  
  Moment now = get_outlet ()->now_mom ();
  Moment *splitm = 0;
  
  for (; gh_pair_p (split_list_); split_list_ = gh_cdr (split_list_))
    {
      splitm = unsmob_moment (gh_caar (split_list_));
      if (*splitm > now)
	break ;

      SCM tag = gh_cdar (split_list_);
      Direction d = to_dir  (tag);
      
      if (d && d != where_dir_)
	{
	  where_dir_ = d;
	  String to_id = (d >= 0) ?  "up" : "down";
	  change_to (child_iter_,
		     ly_symbol2scm ("Staff"),
		     to_id);
	}
    }
}

Auto_change_iterator::Auto_change_iterator ()
{
  where_dir_ = CENTER;
  split_list_ = SCM_EOL;
}

void
Auto_change_iterator::construct_children ()
{
  Music_wrapper_iterator::construct_children ();
  split_list_ =  get_music ()->get_mus_property ("split-list");
}

IMPLEMENT_CTOR_CALLBACK (Auto_change_iterator);
