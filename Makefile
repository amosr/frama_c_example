# Use the local why3 configuration unless overridden
WHY3CONFIG        ?= .why3.conf
FRAMAC_PROVERS    ?= cvc4
FRAMAC_TOP        ?= full.c

FRAMAC_TIMEOUT    ?= 10


OPTS=-wp-prover $(FRAMAC_PROVERS) \
     -wp-rte -wp-no-warn-memory-model \
     -wp-session session \
     $(FRAMAC_TOP) \

# Replay existing proof scripts
REPLAY=-wp-prover script
# Use tip prover to save the proof scripts.
# Also enable the wp_ignore for the GUI, as otherwise the GUI complains if you look at the ignored functions
GUI=-wp-prover tip

.PHONY: check
# Check all of the proof obligations
check:
	WHY3CONFIG=${WHY3CONFIG} frama-c -wp $(REPLAY) $(OPTS) -wp-timeout $(FRAMAC_TIMEOUT) $(EXTRA)

.PHONY: gui
# Open the GUI
gui:
	WHY3CONFIG=${WHY3CONFIG} frama-c-gui $(GUI) $(OPTS) -wp-timeout 1 $(EXTRA) 2>/dev/null &

.PHONY: install-deps
# Install Frama-C and stuff. See readme.md.
install-deps:
	@ [ ! -e _opam ] || (echo 'Switch already exists; deleting' && opam switch remove .)
	opam switch create . ocaml-base-compiler.4.11.2
	opam switch import opam.switch.4.11.2-coq
	WHY3CONFIG=${WHY3CONFIG} why3 config --detect

