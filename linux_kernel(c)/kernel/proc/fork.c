/******************************************************************************/
/* Important Spring 2022 CSCI 402 usage information:                          */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5fd1e93dbf35cbffa3aef28f8c01d8cf2ffc51ef62b26a       */
/*         f9bda5a68e5ed8c972b17bab0f42e24b19daa7bd408305b1f7bd6c7208c1       */
/*         0e36230e913039b3046dd5fd0ba706a624d33dbaa4d6aab02c82fe09f561       */
/*         01b0fd977b0051f0b0ce0c69f7db857b1b5e007be2db6d42894bf93de848       */
/*         806d9152bd5715e9                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#include "types.h"
#include "globals.h"
#include "errno.h"

#include "util/debug.h"
#include "util/string.h"

#include "proc/proc.h"
#include "proc/kthread.h"

#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/page.h"
#include "mm/pframe.h"
#include "mm/mmobj.h"
#include "mm/pagetable.h"
#include "mm/tlb.h"

#include "fs/file.h"
#include "fs/vnode.h"

#include "vm/shadow.h"
#include "vm/vmmap.h"

#include "api/exec.h"

#include "main/interrupt.h"

/* Pushes the appropriate things onto the kernel stack of a newly forked thread
 * so that it can begin execution in userland_entry.
 * regs: registers the new thread should have on execution
 * kstack: location of the new thread's kernel stack
 * Returns the new stack pointer on success. */
static uint32_t
fork_setup_stack(const regs_t *regs, void *kstack)
{
        /* Pointer argument and dummy return address, and userland dummy return
         * address */
        uint32_t esp = ((uint32_t)kstack) + DEFAULT_STACK_SIZE - (sizeof(regs_t) + 12);
        *(void **)(esp + 4) = (void *)(esp + 8);         /* Set the argument to point to location of struct on stack */
        memcpy((void *)(esp + 8), regs, sizeof(regs_t)); /* Copy over struct */
        return esp;
}

/*
 * The implementation of fork(2). Once this works,
 * you're practically home free. This is what the
 * entirety of Weenix has been leading up to.
 * Go forth and conquer.
 */
int do_fork(struct regs *regs)
{
        // vmarea_t *vma, *clone_vma;
        // pframe_t *pf;
        // mmobj_t *to_delete, *new_shadowed;

        proc_t *newproc = proc_create("forkedchildproc");
        vmmap_t *clonedmap = vmmap_clone(curproc->p_vmmap);
        // vmmap_t *newmap = newproc->p_vmmap;
        vmarea_t *vmarea = NULL;
        vmarea_t *oldvmarea = NULL;
        mmobj_t *shadowmmobj1 = NULL;
        mmobj_t *shadowmmobj2 = NULL;
        mmobj_t *bottomobj = NULL;
        kthread_t *kthr = NULL;
        uintptr_t ctxstack = 0;
        int vfn = 0;
        vmmap_destroy(newproc->p_vmmap);
        newproc->p_vmmap = clonedmap;
        clonedmap->vmm_proc = newproc;
        newproc->p_brk = curproc->p_brk;
        newproc->p_start_brk = curproc->p_start_brk;
        for (int i = 0; i < NFILES; i++)
        {
                if (curproc->p_files[i])
                {
                        fref(curproc->p_files[i]);
                }
                newproc->p_files[i] = curproc->p_files[i];
        }
        list_iterate_begin(&(clonedmap->vmm_list), vmarea, vmarea_t, vma_plink)
        {
                // newvmarea = vmmap_create();
                KASSERT(vmarea);
                vfn = vmarea->vma_start;
                oldvmarea = vmmap_lookup(curproc->p_vmmap, vfn);
                KASSERT(oldvmarea);
                KASSERT(oldvmarea->vma_obj);
                oldvmarea->vma_obj->mmo_ops->ref(oldvmarea->vma_obj);
                if ((vmarea->vma_flags & MAP_TYPE) == MAP_PRIVATE)
                {
                        shadowmmobj1 = shadow_create();
                        shadowmmobj2 = shadow_create();
                        shadowmmobj1->mmo_shadowed = oldvmarea->vma_obj;
                        shadowmmobj2->mmo_shadowed = oldvmarea->vma_obj;
                        shadowmmobj1->mmo_un.mmo_bottom_obj = oldvmarea->vma_obj->mmo_un.mmo_bottom_obj;
                        shadowmmobj2->mmo_un.mmo_bottom_obj = oldvmarea->vma_obj->mmo_un.mmo_bottom_obj;
                        oldvmarea->vma_obj = shadowmmobj1;
                        vmarea->vma_obj = shadowmmobj2;
                }
                else if ((vmarea->vma_flags & MAP_SHARED) == MAP_SHARED)
                {
                        vmarea->vma_obj = oldvmarea->vma_obj;
                }
                list_insert_tail(mmobj_bottom_vmas(oldvmarea->vma_obj), &vmarea->vma_olink);
        }
        list_iterate_end();
        kthr = kthread_clone(curthr);
        kthr->kt_proc = newproc;

        list_insert_tail(&(newproc->p_threads), &(kthr->kt_plink));

        regs->r_eax = 0;
        kthr->kt_ctx.c_pdptr = newproc->p_pagedir;
        kthr->kt_ctx.c_eip = (uint32_t)userland_entry;

        ctxstack = fork_setup_stack(regs, kthr->kt_kstack);
        kthr->kt_ctx.c_esp = ctxstack;
        // newproc->p_pagedir = curproc->p_pagedir;

        sched_make_runnable(kthr);

        pt_unmap_range(curproc->p_pagedir, USER_MEM_LOW, USER_MEM_HIGH);
        tlb_flush_all();
        regs->r_eax = newproc->p_pid;
        return newproc->p_pid;
        // NOT_YET_IMPLEMENTED("VM: do_fork");
        // return 0;
}
