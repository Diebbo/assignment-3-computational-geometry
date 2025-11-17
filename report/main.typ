#import "@preview/arkheion:0.1.0": arkheion, arkheion-appendices

#show: arkheion.with(
  title: "Computational Geometry (2025) - Convex Hull",
  authors: (
    (name: "Diego Barbieri", email: "au802245@uni.au.dk", affiliation: "Aarhus University"),
    (name: "Gioele Scandaletti", email: "au803277@uni.au.dk", affiliation: "Aarhus University"),
    (name: "Samuele Esposito", email: "au803278@uni.au.dk", affiliation: "Aarhus University"),
  ),
  // Insert your abstract after the colon, wrapped in brackets.
  // Example: `abstract: [This is my abstract...]`
  abstract: [
  Convex Hull algorithms implementations and analysis.
  ],
  // keywords: ("Cryptology", "Information Theory", "Entropy"),
)
#set cite(style: "chicago-author-date")
#show link: underline

#line(length: 100%)

#show figure: set block(breakable: true, width: 100%)

#outline()

= Introduction

#include "chapters/introduction.typ"

= Convex Hulls

#include "chapters/graham-description.typ"
#include "chapters/graham-complexity.typ"
#include "chapters/graham-bench.typ"
#include "chapters/quickhull.typ"
#include "chapters/mbc.typ"

#include "chapters/conclusion.typ"

// Configure appendix

#set heading(numbering: "A.", supplement: [Appendix])
#show heading: it => stack(
  spacing: 1.2em,
  if it.numbering != none {
    text(size: 1.2em, weight: "semibold")[Appendix #counter(heading).display()]
  },
  text(size: 1.5em, weight: "bold", it.body),
  [],
)
#counter(heading).update(0)
#pagebreak()

#include "appendix/graham_impl.typ"
#include "appendix/graham_deque.typ"
