/*
 * $Id$
 *
 * Copyright (c) 2010, Juniper Networks, Inc.
 * All rights reserved.
 * See ../Copyright for the status of this software
 */

#include <sys/time.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* ----------------------------------------------------------------------
 * Functions that work on generic strings
 */

#ifndef HAVE_STREQ
/**
 * @brief
 * Given two strings, return true if they are the same.
 * 
 * Tests the first characters for equality before calling strcmp.
 *
 * @param[in] red, blue
 *     The strings to be compared
 *
 * @return
 *     Nonzero (true) if the strings are the same. 
 */
static inline int
streq (const char *red, const char *blue)
{
    return (red && blue && *red == *blue && strcmp(red + 1, blue + 1) == 0);
}
#endif /* HAVE_STREQ */

#ifndef HAVE_CONST_DROP

/*
 * See const_* method below
 */
typedef union {
    void        *cg_vp;
    const void  *cg_cvp;
} const_remove_glue_t;

/*
 * NOTE:
 *     This is EVIL.  The ONLY time you cast from a const is when calling some
 *     legacy function that does not require const:
 *          - you KNOW does not modify the data
 *          - you can't change
 *     That is why this is provided.  In that situation, the legacy API should
 *     have been written to accept const argument.  If you can change the 
 *     function you are calling to accept const, do THAT and DO NOT use this
 *     HACK.
 */
static inline void *
const_drop (const void *ptr)
{
    const_remove_glue_t cg;
    cg.cg_cvp = ptr;
    return cg.cg_vp;
}

#endif /* HAVE_CONST_DROP */

#ifndef ALLOCADUP
/**
 * ALLOCADUP(): think of is as strdup + alloca
 *
 * @to destination string
 * @from source string
 */
static inline char *
allocadupx (char *to, const char *from)
{
    if (to) strcpy(to, from);
    return to;
}
#define ALLOCADUP(s) allocadupx((char *) alloca(strlen(s) + 1), s)
#define ALLOCADUPX(s) \
    ((s) ? allocadupx((char *) alloca(strlen(s) + 1), s) : NULL)

#endif /* ALLOCADUP */

/*
 * A "safe" snprintf that never eats more than it can handle.
 */
#define SNPRINTF(_start, _end, _fmt...) \
    do { \
	(_start) += snprintf((_start), (_end) - (_start), _fmt); \
	if ((_start) > (_end)) \
	    (_start) = (_end); \
    } while (0)

/* ---------------------------------------------------------------------- */

/*
 * A type for holding micro seconds.  This is for comparisons, so the fact
 * that we leak seconds off the top is not a concern.
 */
typedef unsigned long time_usecs_t;

static inline time_usecs_t
timeval_to_usecs (struct timeval *tvp)
{
    return tvp->tv_sec * 1000000 + tvp->tv_usec;
}

/* ---------------------------------------------------------------------- */

#ifndef HAVE_STRLCPY

static inline size_t
strlcpy (char *dst, const char *src, size_t sz)
{
    size_t len = strlen(src);

    if (sz > len)
        sz = len;
    memmove(dst, src, sz);
    dst[sz] = '\0';

    return len;
}

#endif /* HAVE_STRLCPY */

/* ---------------------------------------------------------------------- */

/* Useful time-related constants */
#define NSEC_PER_SEC 1000000000
#define NSEC_PER_MSEC 1000000
#define NSEC_PER_USEC 1000
#define USEC_PER_SEC 1000000
#define USEC_PER_MSEC 1000
#define MSEC_PER_SEC 1000

/* ---------------------------------------------------------------------- */

#ifndef HAVE_STRERROR

static inline const char *
strerror (int num)
{
    return "unknown error";
}

#endif /* HAVE_STRERROR */