#include <out.h>
#include <em.h>
#include "back.h"

common(n)
arith n;
{
	extern int Label, label_waiting;
	register struct outname *nm = &symbol_table[Label];

	if (label_waiting && (nm->on_type & S_EXT)) {
		symbol_table[Label].on_type |= S_COM | (S_MIN+SEGBSS);
		if (n > symbol_table[Label].on_valu) {
			symbol_table[Label].on_valu = n;
		}
		label_waiting = 0;
		return;
	}
	switchseg(SEGBSS);
	dump_label();
	bss(n);
}