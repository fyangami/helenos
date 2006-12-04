/*
 * Copyright (C) 2005 Jakub Jermar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @addtogroup sparc64mm	
 * @{
 */
/** @file
 */

#ifndef KERN_sparc64_AS_H_
#define KERN_sparc64_AS_H_

#ifdef CONFIG_TSB
#include <arch/mm/tsb.h>
#endif

#define KERNEL_ADDRESS_SPACE_SHADOWED_ARCH	1

#define KERNEL_ADDRESS_SPACE_START_ARCH		(unsigned long) 0x0000000000000000
#define KERNEL_ADDRESS_SPACE_END_ARCH		(unsigned long) 0xffffffffffffffff
#define USER_ADDRESS_SPACE_START_ARCH		(unsigned long) 0x0000000000000000
#define USER_ADDRESS_SPACE_END_ARCH		(unsigned long) 0xffffffffffffffff

#define USTACK_ADDRESS_ARCH	(0xffffffffffffffffULL-(PAGE_SIZE-1))

typedef struct {
#ifdef CONFIG_TSB
	tsb_entry_t *itsb;
	tsb_entry_t *dtsb;
#endif /* CONFIG_TSB */
} as_arch_t;

#ifdef CONFIG_TSB
#	define as_invalidate_translation_cache(as, page, cnt)	tsb_invalidate(as, page, cnt)
#else
#	define as_invalidate_translation_cache(as, page, cnt)
#endif

extern void as_arch_init(void);

#endif

/** @}
 */
