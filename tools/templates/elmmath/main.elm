module Main exposing (..)

import Html exposing (..)
import Markdown exposing (..)

equation : Html msg
equation =
  text "When $a \\ne 0$, there are two solutions to $ax^2 + bx + c = 0$ and
  they are $$x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}.$$"

eqnmkdown : Html msg
eqnmkdown =
  Markdown.toHtml [] markdown

markdown = """

# This is Markdown!!

[Markdown](http://daringfireball.net/projects/markdown/) lets you
write content in a really natural way.

* You can have lists, like this one
* Make things **bold** or *italic*
* Embed snippets of `code`
* Create [links](/)
* ...

The [elm-markdown][] package parses all this content, allowing you
to easily generate blocks of `Element` or `Html`.

[elm-markdown]: http://package.elm-lang.org/packages/evancz/elm-markdown/latest

## Math in Elm! ##

When $a \\ne 0$, there are two solutions to $ax^2 + bx + c = 0$ and
  they are $$x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}.$$

"""

main =
  eqnmkdown
