/*
  chord-name-engraver.cc -- implement Chord_name_engraver

  source file of the GNU LilyPond music typesetter

  (c) 1998--2000 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include "chord-name-engraver.hh"
#include "chord-name.hh"
#include "musical-request.hh"
#include "paper-def.hh"
#include "lookup.hh"
#include "paper-def.hh"
#include "main.hh"
#include "dimensions.hh"
#include "text-item.hh"


ADD_THIS_TRANSLATOR (Chord_name_engraver);

Chord_name_engraver::Chord_name_engraver ()
{
  chord_name_p_ = 0;
  tonic_req_ = 0;
  inversion_req_ = 0;
  bass_req_ = 0;
}

void
Chord_name_engraver::acknowledge_element (Score_element_info i)
{
  if (Note_req* n = dynamic_cast<Note_req*> (i.req_l_))
    pitch_arr_.push (n->pitch_);
}

bool
Chord_name_engraver::do_try_music (Music* m)
{
  if (Note_req* n = dynamic_cast<Note_req*> (m))
    {
      pitch_arr_.push (n->pitch_);
      return true;
    }
  if (Tonic_req* t = dynamic_cast<Tonic_req*> (m))
    {
      tonic_req_ = t;
      return true;
    }
  if (Inversion_req* i = dynamic_cast<Inversion_req*> (m))
    {
      inversion_req_ = i;
      return true;
    }
  if (Bass_req* b = dynamic_cast<Bass_req*> (m))
    {
      bass_req_ = b;
      return true;
    }
  return false;
}

void
Chord_name_engraver::do_process_music ()
{
  if (chord_name_p_)
    return;
  if (!pitch_arr_.size ())
    return;

  bool find_inversion_b = false;
  SCM chord_inversion = get_property ("chordInversion");
  if (gh_boolean_p (chord_inversion))
    find_inversion_b = gh_scm2bool (chord_inversion);

  chord_name_p_ = new Chord_name (SCM_EOL);
  Chord chord = to_chord (pitch_arr_, tonic_req_, inversion_req_, bass_req_,
			  find_inversion_b);

  /*
    Hmm, why not represent complete chord as list?
    ((tonic third fifth) (inversion bass))
  */
  SCM plist = SCM_EOL;
  for (int i= chord.pitch_arr_.size (); i--; )
    plist = gh_cons (chord.pitch_arr_[i].to_scm (), plist);
  
  chord_name_p_->set_elt_property ("pitches", plist);
  if (chord.inversion_b_)
    chord_name_p_->set_elt_property ("inversion",
				     chord.inversion_pitch_.to_scm ());
  if (chord.bass_b_)
    chord_name_p_->set_elt_property ("bass", chord.bass_pitch_.to_scm ());

  announce_element (Score_element_info (chord_name_p_, 0));
}

void
Chord_name_engraver::do_pre_move_processing ()
{
  if (chord_name_p_)
    {
      typeset_element (chord_name_p_);
    }
  pitch_arr_.clear ();
  chord_name_p_ = 0;
  tonic_req_ = 0;
  inversion_req_ = 0;
  bass_req_ = 0;
}

