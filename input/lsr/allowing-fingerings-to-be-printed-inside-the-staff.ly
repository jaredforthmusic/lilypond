%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "editorial-annotations, fretted-strings, spacing"

%% Translation of GIT committish: 55ea64f469d9c1703222654c9e5bc8490b04a67a
  texidoces = "
Las cifras de digitación se imprimen de forma predeterminada fuera
del pentagrama.  Sin embargo, este comportamiento se puede
cancelar.

"
  doctitlees = "Permitir que las digitaciones se impriman dentro del pentagrama"

%% Translation of GIT committish: 3f880f886831b8c72c9e944b3872458c30c6c839
  texidocfr = "
L'empilement des indications de doigté se fait par défaut à l'extérieur de la portée.  Néanmoins, il est possible d'annuler ce comportement.

"
  doctitlefr = "Impression des doigtés à l'intérieur de la portée"

%% Translation of GIT committish: 0364058d18eb91836302a567c18289209d6e9706
  texidocde = "
Normalerweise werden vertikal orientierte Fingersatzzahlen außerhalb des Systems
gesetzt.  Das kann aber verändert werden.

"
  doctitlede = "Fingersatz auch innerhalb des Systems setzen"

  texidoc = "
By default, vertically oriented fingerings are positioned outside the
staff.  However, this behavior can be canceled.

"
  doctitle = "Allowing fingerings to be printed inside the staff"
} % begin verbatim

\relative c' {
  <c-1 e-2 g-3 b-5>2
  \once \override Fingering #'staff-padding = #'()
  <c-1 e-2 g-3 b-5>2
}
