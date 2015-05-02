/*
  This file is part of LilyPond, the GNU music typesetter.

  Copyright (C) 2004--2015 Han-Wen Nienhuys <hanwen@xs4all.nl>

  LilyPond is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LilyPond is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "context.hh"

#include "context-def.hh"
#include "dispatcher.hh"
#include "global-context.hh"
#include "international.hh"
#include "main.hh"
#include "output-def.hh"
#include "profile.hh"
#include "program-option.hh"
#include "scm-hash.hh"
#include "translator-group.hh"
#include "warn.hh"

bool
Context::is_removable () const
{
  return scm_is_null (context_list_) && ! client_count_
         && !dynamic_cast<Global_context const *> (daddy_context_);
}

void
Context::check_removal ()
{
  for (SCM p = context_list_; scm_is_pair (p); p = scm_cdr (p))
    {
      Context *ctx = unsmob<Context> (scm_car (p));

      ctx->check_removal ();
      if (ctx->is_removable ())
        {
          recurse_over_translators (ctx, &Translator::finalize,
                                    &Translator_group::finalize,
                                    UP);
          send_stream_event (ctx, "RemoveContext", 0, 0);
        }
    }
}

Scheme_hash_table *
Context::properties_dict () const
{
  return unsmob<Scheme_hash_table> (properties_scm_);
}

void
Context::add_context (Context *child)
{
  context_list_ = ly_append2 (context_list_,
                              scm_cons (child->self_scm (), SCM_EOL));

  child->daddy_context_ = this;
  this->events_below_->register_as_listener (child->events_below_);
}

Context::Context ()
{
  daddy_context_ = 0;
  aliases_ = SCM_EOL;
  client_count_ = 0;
  implementation_ = 0;
  properties_scm_ = SCM_EOL;
  accepts_list_ = SCM_EOL;
  default_child_ = SCM_EOL;
  context_list_ = SCM_EOL;
  definition_ = SCM_EOL;
  definition_mods_ = SCM_EOL;
  event_source_ = 0;
  events_below_ = 0;

  smobify_self ();

  Scheme_hash_table *tab = new Scheme_hash_table;
  properties_scm_ = tab->unprotect ();
  event_source_ = new Dispatcher ();
  event_source_->unprotect ();
  events_below_ = new Dispatcher ();
  events_below_->unprotect ();
}

/* TODO:  this shares code with find_create_context ().  */
Context *
Context::create_unique_context (SCM name, const string &id, SCM operations)
{
  /*
    Don't create multiple score contexts.
  */
  Global_context *gthis = dynamic_cast<Global_context *> (this);
  if (gthis && gthis->get_score_context ())
    return gthis->get_score_context ()->create_unique_context (name, id, operations);

  vector<Context_def *> path = path_to_acceptable_context (name);
  if (path.size ())
    {
      Context *current = this;

      // Iterate through the path and create all of the implicit contexts.
      for (vsize i = 0; i < path.size (); i++)
        {
          SCM ops = SCM_EOL;
          string id_str = "\\new";
          if (i == path.size () - 1)
            {
              ops = operations;
              id_str = id;
            }
          current = current->create_context (path[i],
                                             id_str,
                                             ops);
        }

      return current;
    }

  /*
    Don't go up to Global_context, because global goes down to the
    Score context
  */
  Context *ret = 0;
  if (daddy_context_ && !dynamic_cast<Global_context *> (daddy_context_))
    ret = daddy_context_->create_unique_context (name, id, operations);
  else
    {
      warning (_f ("cannot find or create new `%s'",
                   ly_symbol2string (name).c_str ()));
      ret = 0;
    }
  return ret;
}

Context *
Context::find_create_context (SCM n, const string &id, SCM operations)
{
  /*
    Don't create multiple score contexts.
  */
  Global_context *gthis = dynamic_cast<Global_context *> (this);
  if (gthis)
    {
      if (gthis->get_score_context ())
        return gthis->get_score_context ()->find_create_context (n, id, operations);

      // Special case: If we use \set Timing.xxx = whatever before
      // Score is established, the alias of Score to Timing will not
      // be taken into account.  We check for this particular case
      // here.  Aliases apart from Score-level ones don't warrant
      // context creation as they could create unwanted contexts, like
      // RhythmicVoice instead of Voice.  Creating a Score context,
      // however, can't really do anything wrong.

      SCM score_name = default_child_context_name ();
      SCM score_def = find_context_def (get_output_def (), score_name);

      if (Context_def *cd = unsmob<Context_def> (score_def))
        {
          if (cd->is_alias (n))
            return create_context (cd, id, operations);
        }
    }


  if (Context *existing = find_context_below (this, n, id))
    return existing;

  if (scm_is_eq (n, ly_symbol2scm ("Bottom")))
    {
      Context *tg = get_default_interpreter (id);
      return tg;
    }

  vector<Context_def *> path = path_to_acceptable_context (n);

  if (path.size ())
    {
      Context *current = this;

      // start at 1.  The first one (index 0) will be us.
      for (vsize i = 0; i < path.size (); i++)
        {
          SCM ops = (i == path.size () - 1) ? operations : SCM_EOL;

          string this_id = "";
          if (i == path.size () - 1)
            this_id = id;

          current = current->create_context (path[i],
                                             this_id,
                                             ops);
        }

      return current;
    }

  /*
    Don't go up to Global_context, because global goes down to the
    Score context
  */
  Context *ret = 0;
  if (daddy_context_ && !dynamic_cast<Global_context *> (daddy_context_))
    ret = daddy_context_->find_create_context (n, id, operations);
  else
    {
      warning (_f ("cannot find or create `%s' called `%s'",
                   ly_symbol2string (n).c_str (), id));
      ret = 0;
    }
  return ret;
}

void
Context::acknowledge_infant (SCM sev)
{
  infant_event_ = unsmob<Stream_event> (sev);
}

void
Context::set_property_from_event (SCM sev)
{
  Stream_event *ev = unsmob<Stream_event> (sev);

  SCM sym = ev->get_property ("symbol");
  if (scm_is_symbol (sym))
    {
      SCM val = ev->get_property ("value");

      if (SCM_UNBNDP (val)) {
        unset_property (sym);
        return;
      }

      bool ok = true;
      ok = type_check_assignment (sym, val, ly_symbol2scm ("translation-type?"));

      if (ok)
        set_property (sym, val);
    }
}

void
Context::unset_property_from_event (SCM sev)
{
  Stream_event *ev = unsmob<Stream_event> (sev);

  SCM sym = ev->get_property ("symbol");
  type_check_assignment (sym, SCM_EOL, ly_symbol2scm ("translation-type?"));
  unset_property (sym);
}

/*
  Creates a new context from a CreateContext event, and sends an
  AnnounceNewContext event to this context.
*/
void
Context::create_context_from_event (SCM sev)
{
  Stream_event *ev = unsmob<Stream_event> (sev);

  string id = ly_scm2string (ev->get_property ("id"));
  SCM ops = ev->get_property ("ops");
  SCM type_scm = ev->get_property ("type");
  string type = ly_symbol2string (type_scm);

  vector<Context_def *> path = path_to_acceptable_context (type_scm);

  if (path.size () != 1)
    {
      programming_error (to_string ("Invalid CreateContext event: Cannot create %s context", type.c_str ()));
      return;
    }
  Context_def *cdef = path[0];

  Context *new_context = cdef->instantiate (ops);

  new_context->id_string_ = id;

  /* Register various listeners:
      - Make the new context hear events that universally affect contexts
      - connect events_below etc. properly */
  /* We want to be the first ones to hear our own events. Therefore, wait
     before registering events_below_ */
  new_context->event_source ()->
  add_listener (new_context->GET_LISTENER (Context, create_context_from_event),
                ly_symbol2scm ("CreateContext"));
  new_context->event_source ()->
  add_listener (new_context->GET_LISTENER (Context, remove_context),
                ly_symbol2scm ("RemoveContext"));
  new_context->event_source ()->
  add_listener (new_context->GET_LISTENER (Context, change_parent),
                ly_symbol2scm ("ChangeParent"));
  new_context->event_source ()->
  add_listener (new_context->GET_LISTENER (Context, set_property_from_event),
                ly_symbol2scm ("SetProperty"));
  new_context->event_source ()->
  add_listener (new_context->GET_LISTENER (Context, unset_property_from_event),
                ly_symbol2scm ("UnsetProperty"));

  new_context->events_below_->register_as_listener (new_context->event_source_);
  this->add_context (new_context);

  new_context->unprotect ();

  Context_def *td = unsmob<Context_def> (new_context->definition_);

  /* This cannot move before add_context (), because \override
     operations require that we are in the hierarchy.  */
  td->apply_default_property_operations (new_context);
  apply_property_operations (new_context, ops);

  send_stream_event (this, "AnnounceNewContext", 0,
                     ly_symbol2scm ("context"), new_context->self_scm (),
                     ly_symbol2scm ("creator"), sev);
}

vector<Context_def *>
Context::path_to_acceptable_context (SCM name) const
{
  // The 'accepts elements in definition_mods_ is a list of ('accepts string),
  // but the Context_def expects to see elements of the form ('accepts symbol).
  SCM accepts = SCM_EOL;
  for (SCM s = definition_mods_; scm_is_pair (s); s = scm_cdr (s))
    if (scm_is_eq (scm_caar (s), ly_symbol2scm ("accepts")))
      {
        SCM elt = scm_list_2 (scm_caar (s), scm_string_to_symbol (scm_cadar (s)));
        accepts = scm_cons (elt, accepts);
      }

  return unsmob<Context_def> (definition_)->path_to_acceptable_context (name,
         get_output_def (),
         scm_reverse_x (accepts, SCM_EOL));

}

Context *
Context::create_context (Context_def *cdef,
                         const string &id,
                         SCM ops)
{
  infant_event_ = 0;
  /* TODO: This is fairly misplaced. We can fix this when we have taken out all
     iterator specific stuff from the Context class */
  event_source_->
  add_listener (GET_LISTENER (Context, acknowledge_infant),
                ly_symbol2scm ("AnnounceNewContext"));
  /* The CreateContext creates a new context, and sends an announcement of the
     new context through another event. That event will be stored in
     infant_event_ to create a return value. */
  send_stream_event (this, "CreateContext", 0,
                     ly_symbol2scm ("ops"), ops,
                     ly_symbol2scm ("type"), cdef->get_context_name (),
                     ly_symbol2scm ("id"), ly_string2scm (id));
  event_source_->
  remove_listener (GET_LISTENER (Context, acknowledge_infant),
                   ly_symbol2scm ("AnnounceNewContext"));

  assert (infant_event_);
  SCM infant_scm = infant_event_->get_property ("context");
  Context *infant = unsmob<Context> (infant_scm);

  if (!infant || infant->get_parent_context () != this)
    {
      programming_error ("create_context: can't locate newly created context");
      return 0;
    }

  return infant;
}

/*
  Default child context as a SCM string, or something else if there is
  none.
*/
SCM
Context::default_child_context_name () const
{
  return default_child_;
}

bool
Context::is_bottom_context () const
{
  return !scm_is_symbol (default_child_context_name ());
}

Context *
Context::get_default_interpreter (const string &context_id)
{
  if (!is_bottom_context ())
    {
      SCM nm = default_child_context_name ();
      SCM st = find_context_def (get_output_def (), nm);

      string name = ly_symbol2string (nm);
      Context_def *t = unsmob<Context_def> (st);
      if (!t)
        {
          warning (_f ("cannot find or create: `%s'", name.c_str ()));
          t = unsmob<Context_def> (this->definition_);
        }
      if (scm_is_symbol (t->get_default_child (SCM_EOL)))
        {
          Context *tg = create_context (t, "\\new", SCM_EOL);
          return tg->get_default_interpreter (context_id);
        }
      return create_context (t, context_id, SCM_EOL);
    }
  else if (!context_id.empty () && context_id != id_string ())
    {
      if (daddy_context_ && !dynamic_cast<Global_context *> (daddy_context_))
        return daddy_context_->get_default_interpreter (context_id);
      warning (_f ("cannot find or create new Bottom = \"%s\"",
                   context_id.c_str ()));
    }
  return this;
}

/*
  PROPERTIES
*/
Context *
Context::where_defined (SCM sym, SCM *value) const
{
#ifdef DEBUG
  if (profile_property_accesses)
    note_property_access (&context_property_lookup_table, sym);
#endif

  if (properties_dict ()->try_retrieve (sym, value))
    return (Context *)this;

  return (daddy_context_) ? daddy_context_->where_defined (sym, value) : 0;
}

/* Quick variant of where_defined.  Checks only the context itself. */

bool
Context::here_defined (SCM sym, SCM *value) const
{
#ifdef DEBUG
  if (profile_property_accesses)
    note_property_access (&context_property_lookup_table, sym);
#endif

  return properties_dict ()->try_retrieve (sym, value);
}

/*
  return SCM_EOL when not found.
*/
SCM
Context::internal_get_property (SCM sym) const
{
#ifdef DEBUG
  if (profile_property_accesses)
    note_property_access (&context_property_lookup_table, sym);
#endif

  SCM val = SCM_EOL;
  if (properties_dict ()->try_retrieve (sym, &val))
    return val;

  if (daddy_context_)
    return daddy_context_->internal_get_property (sym);

  return val;
}

/*
Called by the send_stream_event macro. props is a 0-terminated array of
properties and corresponding values, interleaved. This method should not
be called from any other place than the send_stream_event macro.
*/
void
Context::internal_send_stream_event (SCM type, Input *origin, SCM props[])
{
  Stream_event *e = new Stream_event
    (scm_call_1 (ly_lily_module_constant ("ly:make-event-class"), type),
     origin);
  for (int i = 0; props[i]; i += 2)
    {
      e->set_property (props[i], props[i + 1]);
    }
  event_source_->broadcast (e);
  e->unprotect ();
}

bool
Context::is_alias (SCM sym) const
{
  if (scm_is_eq (sym, ly_symbol2scm ("Bottom")))
    return is_bottom_context ();
  if (scm_is_eq (sym, context_name_symbol ()))
    return true;

  return scm_is_true (scm_c_memq (sym, aliases_));
}

void
Context::add_alias (SCM sym)
{
  aliases_ = scm_cons (sym, aliases_);
}

/* we don't (yet) instrument context properties */
void
Context::instrumented_set_property (SCM sym, SCM val, const char *, int, const char *)
{
  internal_set_property (sym, val);
}

void
Context::internal_set_property (SCM sym, SCM val)
{
  bool type_check_ok = type_check_assignment (sym, val, ly_symbol2scm ("translation-type?"));

  if (do_internal_type_checking_global)
    assert (type_check_ok);

  if (type_check_ok)
    properties_dict ()->set (sym, val);
}

/*
  TODO: look up to check whether we have inherited var?
*/
void
Context::unset_property (SCM sym)
{
  properties_dict ()->remove (sym);
}

void
Context::change_parent (SCM sev)
{
  Stream_event *ev = unsmob<Stream_event> (sev);
  Context *to = unsmob<Context> (ev->get_property ("context"));

  disconnect_from_parent ();
  to->add_context (this);
}

/*
  Die. The next GC sweep should take care of the actual death.
 */
void
Context::remove_context (SCM)
{
  /* ugh, the translator group should listen to RemoveContext events by itself */
  Translator_group *impl = implementation ();
  if (impl)
    impl->disconnect_from_context ();
  disconnect_from_parent ();
}

void
Context::disconnect_from_parent ()
{
  daddy_context_->events_below_->unregister_as_listener (this->events_below_);
  daddy_context_->context_list_ = scm_delq_x (this->self_scm (), daddy_context_->context_list_);
  daddy_context_ = 0;
}

/*
  ID == "" means accept any ID.
*/
Context *
find_context_below (Context *where,
                    SCM type, const string &id)
{
  if (where->is_alias (type))
    {
      if (id == "" || where->id_string () == id)
        return where;
    }

  Context *found = 0;
  for (SCM s = where->children_contexts ();
       !found && scm_is_pair (s); s = scm_cdr (s))
    {
      Context *tr = unsmob<Context> (scm_car (s));

      found = find_context_below (tr, type, id);
    }

  return found;
}

SCM
Context::properties_as_alist () const
{
  return properties_dict ()->to_alist ();
}

SCM
Context::context_name_symbol () const
{
  Context_def *td = unsmob<Context_def> (definition_);
  return td->get_context_name ();
}

string
Context::context_name () const
{
  return ly_symbol2string (context_name_symbol ());
}

Context *
Context::get_score_context () const
{
  if (daddy_context_)
    return daddy_context_->get_score_context ();
  else
    return 0;
}

Output_def *
Context::get_output_def () const
{
  return daddy_context_ ? daddy_context_->get_output_def () : 0;
}

Context::~Context ()
{
}

Moment
Context::now_mom () const
{
  Context const *p = this;
  while (p->daddy_context_)
    p = p->daddy_context_;

  return p->now_mom ();
}

int
Context::print_smob (SCM port, scm_print_state *)
{
  scm_puts ("#<", port);
  scm_puts (class_name (), port);
  if (Context_def *d = unsmob<Context_def> (definition_))
    {
      scm_puts (" ", port);
      scm_display (d->get_context_name (), port);
    }

  if (!id_string_.empty ())
    {
      scm_puts ("=", port);
      scm_puts (id_string_.c_str (), port);
    }

  scm_puts (" ", port);

  scm_display (context_list_, port);
  scm_puts (" >", port);

  return 1;
}

SCM
Context::mark_smob ()
{
  scm_gc_mark (context_list_);
  scm_gc_mark (aliases_);
  scm_gc_mark (definition_);
  scm_gc_mark (definition_mods_);
  scm_gc_mark (properties_scm_);
  scm_gc_mark (accepts_list_);
  scm_gc_mark (default_child_);

  if (implementation_)
    scm_gc_mark (implementation_->self_scm ());

  if (event_source_)
    scm_gc_mark (event_source_->self_scm ());

  if (events_below_)
    scm_gc_mark (events_below_->self_scm ());

  return properties_scm_;
}

const char Context::type_p_name_[] = "ly:context?";

Global_context *
Context::get_global_context () const
{
  if (dynamic_cast<Global_context *> ((Context *) this))
    return dynamic_cast<Global_context *> ((Context *) this);

  if (daddy_context_)
    return daddy_context_->get_global_context ();

  programming_error ("no Global context");
  return 0;
}

Context *
Context::get_parent_context () const
{
  return daddy_context_;
}

/*
  Ugh. Where to put this?
*/
Rational
measure_length (Context const *context)
{
  SCM l = context->get_property ("measureLength");
  Rational length (1);
  if (unsmob<Moment> (l))
    length = unsmob<Moment> (l)->main_part_;
  return length;
}

Moment
measure_position (Context const *context)
{
  SCM sm = context->get_property ("measurePosition");

  Moment m = 0;
  if (unsmob<Moment> (sm))
    {
      m = *unsmob<Moment> (sm);

      if (m.main_part_ < Rational (0))
        {
          Rational length (measure_length (context));
          while (m.main_part_ < Rational (0))
            m.main_part_ += length;
        }
    }

  return m;
}

/* Finds the measure position after a note of length DUR that
   begins at the current measure position. */
Moment
measure_position (Context const *context, Duration const *dur)
{
  Moment pos = measure_position (context);
  Rational dur_length = dur ? dur->get_length () : Rational (0);

  Moment end_pos = pos.grace_part_ < Rational (0)
                   ? Moment (pos.main_part_, pos.grace_part_ + dur_length)
                   : Moment (pos.main_part_ + dur_length, 0);

  return end_pos;
}

int
measure_number (Context const *context)
{
  SCM barnum = context->get_property ("internalBarNumber");
  SCM smp = context->get_property ("measurePosition");

  int bn = robust_scm2int (barnum, 0);
  Moment mp = robust_scm2moment (smp, Moment (0));
  if (mp.main_part_ < Rational (0))
    bn--;

  return bn;
}

void
set_context_property_on_children (Context *trans, SCM sym, SCM val)
{
  trans->set_property (sym, ly_deep_copy (val));
  for (SCM p = trans->children_contexts (); scm_is_pair (p); p = scm_cdr (p))
    {
      Context *trg = unsmob<Context> (scm_car (p));
      set_context_property_on_children (trg, sym, ly_deep_copy (val));
    }
}

bool
melisma_busy (Context *tr)
{
  // When there are subcontexts, they are responsible for maintaining
  // melismata.
  SCM ch = tr->children_contexts ();
  if (scm_is_pair (ch))
    {
      // all contexts need to have a busy melisma for this to evaluate
      // to true.

      do {
        if (!melisma_busy (unsmob<Context> (scm_car (ch))))
          return false;
        ch = scm_cdr (ch);
      } while (scm_is_pair (ch));
      return true;
    }

  for (SCM melisma_properties = tr->get_property ("melismaBusyProperties");
       scm_is_pair (melisma_properties);
       melisma_properties = scm_cdr (melisma_properties))
    if (to_boolean (tr->get_property (scm_car (melisma_properties))))
      return true;

  return false;
}

bool
check_repeat_count_visibility (Context const *context, SCM count)
{
  SCM proc = context->get_property ("repeatCountVisibility");
  return (ly_is_procedure (proc)
          && to_boolean (scm_call_2 (proc,
                                     count,
                                     context->self_scm ())));
}
