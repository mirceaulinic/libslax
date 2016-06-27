/*
 * Copyright (c) 2016, Juniper Networks, Inc.
 * All rights reserved.
 * This SOFTWARE is licensed under the LICENSE provided in the
 * ../Copyright file. By downloading, installing, copying, or otherwise
 * using the SOFTWARE, you agree to be bound by the terms of that
 * LICENSE.
 *
 * Phil Shafer <phil@>, April 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>

#include "slaxconfig.h"
#include <libslax/slax.h>
#include <libslax/pa_common.h>
#include <libslax/pa_config.h>
#include <libslax/pa_mmap.h>
#include <libslax/pa_fixed.h>

#define TEST_PRINT_DULL
#include "pa_main.h"

pa_mmap_t *pmp;
pa_fixed_t *pfp;

void
test_init (void)
{
    return;
}

void
test_open (void)
{
    pmp = pa_mmap_open(opt_filename, 0, 0644);
    assert(pmp != NULL);

    pfp = pa_fixed_open(pmp, "pa_01", opt_shift, opt_size, opt_max_atoms);
    assert(pfp != NULL);
}

void
test_alloc (unsigned slot, unsigned size UNUSED)
{
    pa_atom_t atom = pa_fixed_alloc_atom(pfp);
    test_t *tp = pa_fixed_atom_addr(pfp, atom);
    trec[slot] = tp;
    if (tp) {
	tp->t_magic = opt_magic;
	tp->t_id = atom;
	tp->t_slot = slot;
	memset(tp->t_val, opt_value, opt_size - sizeof(*tp));
    }

    if (!opt_quiet)
	printf("in %u : %u -> %p (%u)\n", slot, atom, tp, pfp->pf_free);
}

void
test_free (unsigned slot)
{
    pa_atom_t atom = trec[slot]->t_id;
    printf("free %u : %u -> %p (%u)\n", slot, atom, trec[slot], pfp->pf_free);

    pa_fixed_free_atom(pfp, atom);

    trec[slot] = NULL;
}

void
test_close (void)
{
    pa_fixed_close(pfp);
}
