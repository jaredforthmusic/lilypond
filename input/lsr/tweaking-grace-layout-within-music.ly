%% Do not edit this file; it is auto-generated from input/new
%% This file is in the public domain.
%% Note: this file works from version 2.12.0
\version "2.13.1"

\header {
  texidoces = "

La disposición de las expresiones de adorno se puede cambiar a lo
largo de toda la música usando las funciones
@code{add-grace-property} y @code{remove-grace-property}.  El
ejemplo siguiente borra la definición de la dirección de la plica
para esta nota de adorno, de manera que las plicas no siemmpre
apuntan hacia arriba, y cambia la forma predeterminada de las
cabezas a aspas.

"

  doctitlees = "Trucar la disposición de las notas de adorno dentro de la música"

  lsrtags = "rhythms"
  texidoc = "
The layout of grace expressions can be changed throughout the
music using the functions @code{add-grace-property} and
@code{remove-grace-property}.  The following example undefines
the @code{Stem} direction for this grace, so that stems do not
always point up, and changes the default note heads to crosses.
"
  doctitle = "Tweaking grace layout within music"
} % begin verbatim


\relative c'' {
  \new Staff {
    #(remove-grace-property 'Voice 'Stem 'direction)
    #(add-grace-property 'Voice 'NoteHead 'style 'cross)
    \new Voice {
       \acciaccatura { f16 } g4
       \grace { d16[ e] } f4
       \appoggiatura { f,32[ g a] } e2
    }
  }
}
