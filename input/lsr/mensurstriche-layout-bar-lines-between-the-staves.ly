%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "staff-notation, ancient-notation, contexts-and-engravers, tweaks-and-overrides"

%% Translation of GIT committish: aea975539ec44fd0f1a8fd25930b88b5ab64b53a
  texidoces = "
La disposición «mensurstriche» en que las líneas divisorias no
están dibujadas sobre los pentagramas, sino entre ellos, se puede
conseguir con un @code{StaffGroup} en vez de un @code{ChoirStaff}.
La línea divisoria sobre los pentagramas se borra estableciendo la
propiedad @code{transparent}.

"

  doctitlees = "Disposición Mensurstriche (líneas divisorias entre pentagramas)"

%% Translation of GIT committish: 0364058d18eb91836302a567c18289209d6e9706
  texidocde = "
Das Mensurstiche-Layout, in welchem die Taktlinien nicht auf den Systemen,
sondern zwischen den Systemen gesetzt werden, kann mit einer @code{StaffGroup}
anstelle von @code{ChoirStaff} erreicht werden.  Die Taktlinien auf den
Systemen werden mit der the @code{transparent}-Eigenschaft ausgelöscht.

"
  doctitlede = "Mensurstriche-Layout (Taktstriche zwischen den Systemen"

  texidoc = "
The mensurstriche-layout where the bar lines do not show on the staves
but between staves can be achieved with a @code{StaffGroup} instead of
a @code{ChoirStaff}.  The bar line on staves is blanked out by setting
the @code{transparent} property.

"
  doctitle = "Mensurstriche layout (bar lines between the staves)"
} % begin verbatim

global = {
  \override Staff.BarLine #'transparent = ##t
  s1 s
  % the final bar line is not interrupted
  \revert Staff.BarLine #'transparent
  \bar "|."
}
\new StaffGroup \relative c'' {
  <<
    \new Staff { << \global { c1 c } >> }
    \new Staff { << \global { c c } >> }
  >>
}
