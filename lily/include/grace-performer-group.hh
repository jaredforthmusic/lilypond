/*   
  grace-performer-group.hh -- declare 
  
  source file of the GNU LilyPond music typesetter
  
   (c) 1999 Jan Nieuwenhuizen <janneke@gnu.org>
  
 */

#ifndef GRACE_PERFORMER_GROUP_HH
#define GRACE_PERFORMER_GROUP_HH

#include "performer-group-performer.hh"
#include "global-translator.hh"

class Grace_performer_group : public Performer_group_performer, public Global_translator
{
  Link_array<Audio_element> play_us_;
  Array<Audio_element_info> announce_to_top_;
  bool calling_self_b_;
public:
  VIRTUAL_COPY_CONS(Translator);
  Grace_performer_group ();
protected:
  virtual void announce_element (Audio_element_info);
  virtual void start ();
  virtual void finish ();
  virtual void process ();
  virtual void each (Method_pointer);
  virtual void each (Const_method_pointer) const;
  virtual void do_removal_processing () ;
  virtual void play_element (Audio_element*);
  virtual bool do_try_music (Music *m);
};


#endif /* GRACE_PERFORMER_GROUP_HH */
