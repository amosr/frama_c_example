#include <stdint.h>

typedef uint8_t payload;

typedef struct list_s {
	payload				 data;
	struct list_s *next;
} list_t;

/*@
	logic \list<list_t*> logic_list(list_t *l) =
		(l == \null)? \Nil : \Cons(l, logic_list(l->next));

	predicate all_ok(list_t *l) =
		(l == \null) || (\valid(l) && all_ok(l->next));

	inductive reachable(list_t *needle, list_t *haystack) {
    case Reachable0: \forall list_t *l;
			\valid(l) || l == \null ==> reachable(l, l);
    case ReachableS: \forall list_t *l1, *l2;
      \valid(l2) && reachable(l1, l2->next) ==> reachable(l1, l2);
	}
*/


/*@ ghost
/@
	requires haystack;
	ensures logic_list(haystack) == \Cons(haystack, logic_list(haystack->next));
	assigns \nothing;
@/
void lemma_unfold_abstract_ptr(list_t *haystack) {
}
*/



/*@ axiomatic reachability_preserved_across_heaps {
	axiom
		not_reachable__no_change{A, B}:
			\forall list_t *mod, *root;
			\at(!reachable(mod, root), A) ==>
			(\forall list_t *l; l != mod ==> \at(l, A) == \at(l, B)) ==>
			\at(logic_list(root), A) == \at(logic_list(root), B);
	}
*/

list_t *the_list;

/*@
	requires \valid(l);
	requires all_ok(the_list);
	requires !reachable(l, the_list);
	ensures logic_list(the_list) == \Cons(l, \old(logic_list(the_list)));
	assigns the_list, l->next;
*/
void list_push(list_t *l) {
	l->next = the_list;
	//@ghost lemma_unfold_abstract_ptr(l);
	the_list = l;
}


/*@ ghost
/@
	requires reachable(needle, haystack);
	requires 0 < \length(logic_list(needle));
	requires all_ok(haystack);
	// terminates reachable(\null, haystack);
	ensures reachable(needle->next, haystack);
	assigns \nothing;
@/
void lemma_nonempty_reachable_next(list_t *needle, list_t *haystack) {
	if (needle == haystack) {
	} else if (haystack) {
		lemma_nonempty_reachable_next(needle, haystack->next);
	} else {
	}
}
*/



/*@
	requires all_ok(the_list);
	requires reachable(\null, the_list);
	ensures reachable(\result, the_list);
	ensures \result == \null || \result->data == p;
	assigns \nothing;
*/
list_t *list_find(payload p) {
	list_t *l = the_list;
	/*@
		loop invariant reachable(l, the_list);
		loop invariant reachable(\null, l);
		loop invariant all_ok(l);
		loop assigns l;
		loop variant \length(logic_list(l));
	*/
	while (l) {
		if (l->data == p) {
			return l;
		}
		//@ghost lemma_unfold_abstract_ptr(l);
		//@ghost lemma_nonempty_reachable_next(l, the_list);
		l = l->next;
	}
	return l;
}



