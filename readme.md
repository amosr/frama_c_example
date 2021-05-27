# Verifying stuff with Frama-C

This is a very small example of using Frama-C to verify some list code.
This repo is supplementary material for a talk I did at the Functional Programming user group of Sydney (FP-Syd).

## Talk abstract

Pure functional programming is great for writing programs that are easy to reason about.
Unfortunately, it's not always great at running those programs in restricted environments.
What do we do when we need to reason about programs, but can't afford the runtime overhead of a garbage collector or memory allocator, and we don't want to alienate the systems engineers?
One option is to write a pure (but not necessarily functional) specification and prove that the C implementation refines it.
This is the premise of Frama-C.

In this talk I'll show off some of the fun I've been having verifying safety-critical C code with Frama-C, and why I think it's more practical than other automated theorem provers. 

## Installing dependencies
First, make sure you have [opam installed](https://opam.ocaml.org/doc/Install.html) and up-to-date.
```
sh <(curl -sL https://raw.githubusercontent.com/ocaml/opam/master/shell/install.sh)
opam update
```

You also need some system prerequisites:
```
brew install adwaita-icon-theme libgnomecanvas graphviz
```
and version 1.6 of cvc4 (try `brew install cvc4@1.6`?)

Then, the `install-deps` target will install Frama-C in a local opam sandbox:
```
make install-deps
```

This also creates a local Why3 configuration, so it's worth doing even if you already have Frama-C installed. If you want to use your existing configuration, then `export WHY3CONFIG=~/.why3.conf` or copy it into this directory.

## Verifying
After installing the dependencies, the `check` target will verify the program:
```
make check
```

You can start the Frama-C GUI by running `make gui`.
The GUI is useful for exploring the verification conditions.
You can apply ad-hoc tactics to the verification conditions to try to prove them, but I find this more useful for understanding why they cannot be automatically proven, rather than as an actual proof tool.
Often, once you prove it in the GUI, it will become obvious what intermediate assertions or lemmas or extra preconditions you need.


## What is proven

Finding an element in a global list, and pushing an element onto a global list.

## Summary: judgment on Frama-C

What is good about Frama-C:
* requires less formal methods expertise - code and invariants may be able to be modified by systems engineers
* has a Coq escape hatch for proving the hard stuff
* decent GUI for exploring the proof state
* it's LGPL
* we can write plugins for missing features

What is bad about Frama-C:
* it doesn't implement all of the C specification language (ACSL). The documentation can be confusing because it describes things that aren't yet implemented.
* unclear if it is expressive enough to specify _everything_ we need; Coq/VST definitely is.
* it has a larger trusted computing base (TCB) than Coq/VST: does its C semantics align with gcc or CompCert?; can we trust CVC4/Z3? (yes, we do already); etc.
* it's not clear what the [concurrency support](https://frama-c.com/fc-plugins/conc2seq.html) is like. That said, Coq/VST's concurrency support is known to be inadequate for proving racy programs at least.

What about Coq/VST?
* If we come up against the limits of Frama-C, then Coq/VST may still be useful.

Summary:
my experience with Frama-C has been good.
I was able to learn it and be productive in very little time.
It took about a week to (partially) specify and implement a much-simplified version of the event subsystem.
The GUI is good for exploring the proof state, so you usually have good visibility into why things cannot be proved.
Even if it were necessary to switch to Coq/VST later, the time spent figuring out the data structure and loop invariants for Frama-C should carry over. It is not wasted effort.

## References and resources

The [Frama-C WP plugin official website](https://frama-c.com/fc-plugins/wp.html) provides an overview and some further reading.
WP is the weakest precondition plugin which allows us to reason about function contracts, loop invariants and so on.

The [Frama-C WP tutorial by Allan Blanchard](https://allan-blanchard.fr/publis/frama-c-wp-tutorial-en.pdf) has some useful examples.

The [ACSL implementation reference](https://frama-c.com/download/frama-c-acsl-implementation.pdf) describes the "Ansi C specification language", which is the language used for writing contracts and assertions.
This reference is useful but it mentions many features that are not yet implemented in Frama-C-WP.
The unimplemented parts are highlighted in red or have red footnotes.

