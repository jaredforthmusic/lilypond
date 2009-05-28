%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "repeats"

%% Translation of GIT committish: 00e3a8264e138ad61b4f115a173190d5e9dc7533
  texidoces = "
El grabador @code{Volta_engraver} reside de forma predeterminada
dentro del contexto de @code{Score}, y los corchetes de la repetición
se imprimen así normalmente sólo encima del pentagrama superior.  Esto
se puede ajustar añadiendo el grabador @code{Volta_engraver} al
contexto de @code{Staff} en que deban aparecer los corchetes; véase
también el fragmento de código \"Volta multi staff\".

"
  doctitlees = "Añadir corchetes de primera y segunda vez a más pentagramas"

%% Translation of GIT committish: 0364058d18eb91836302a567c18289209d6e9706
  texidocde = "
Der @code{Volta_engraver} befindet sich im @code{Score}-Kontext und Klammern
werden deshalb nur auf dem obersten System dargestellt.  Das kann umgangen
werden, indem man den @code{Volta_engraver} zu dem @code{Staff}-Kontext
hinzufügt, in dem die Klammern zusätzlichen vorkommen sollen.  Siehe auch
das \"Volta multi staff\"-Schnipsel.

"
  doctitlede = "Volta-Klammern zu zusätzlichen Systemen hinzufügen"

  texidoc = "
The @code{Volta_engraver} by default resides in the @code{Score}
context, and brackets for the repeat are thus normally only printed
over the topmost staff. This can be adjusted by adding the
@code{Volta_engraver} to the @code{Staff} context where the brackets
should appear; see also the \"Volta multi staff\" snippet.

"
  doctitle = "Adding volta brackets to additional staves"
} % begin verbatim

<<
  \new Staff { \repeat volta 2 { c'1 } \alternative { c' } }
  \new Staff { \repeat volta 2 { c'1 } \alternative { c' } }
  \new Staff \with { \consists "Volta_engraver" } { c'2 g' e' a' }
  \new Staff { \repeat volta 2 { c'1 } \alternative { c' } }
>>
