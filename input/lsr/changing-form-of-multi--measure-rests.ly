%% Do not edit this file; it is auto-generated from input/new
%% This file is in the public domain.
%% Note: this file works from version 2.12.0
\version "2.13.1"
\header {
 doctitlees = "Cambiar la forma de los silencios multicompás"
 texidoces = "
Si hay diez compases de silencio o menos, se imprime en el pentagrama
una serie de silencios de breve y longa (conocidos en alemán como
\"Kirchenpausen\", «silencios eclesiásticos»); en caso contrario se
muestra una barra normal.  Este número predeterminado de diez se
puede cambiar sobreescribiendo la propiedad @code{expand-limit}:

"

texidocde = "
Wenn zehn oder weniger Pausentakte vorkommen, wird eine Reihe von Longa-
und Brevispausen (auch Kirchenpausen genannt) gesetzt, bei mehr Takten
wird eine Line mit der Taktanzahl ausgegeben.  Der vorgegebene Wert von
zehn kann geändert werden, indem man die @code{expand-limit}-Eigenschaft
setzt:
"
  doctitlede = "Die Erscheinung von Pausentakten ändern"


%% Translation of GIT committish :<e71f19ad847d3e94ac89750f34de8b6bb28611df>
texidocfr = "
Dans le cas où ce silence dure mois de dix mesures, LilyPond imprime sur
la portée des @qq{ silences d'église } -- @emph{Kirchenpause} en
allemand -- et qui sont une simple suite de rectangles.  La propriété
@code{expand-limit} permet d'obtenir un silence unique :

"
  doctitlefr = "Modifier l'apparence d'un silence multi-mesures"

  lsrtags = "rhythms,tweaks-and-overrides"
  texidoc = "
If there are ten or fewer measures of rests, a series of longa
and breve rests (called in German \"Kirchenpausen\" - church rests)
is printed within the staff; otherwise a simple line is shown.
This default number of ten may be changed by overriding the
@code{expand-limit} property:
"
  doctitle = "Changing form of multi-measure rests"
} % begin verbatim


\relative c'' {
  \compressFullBarRests
  R1*2 | R1*5 | R1*9
  \override MultiMeasureRest #'expand-limit = #3
  R1*2 | R1*5 | R1*9
}
