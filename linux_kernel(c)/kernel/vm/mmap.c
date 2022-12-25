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

#include "globals.h"
#include "errno.h"
#include "types.h"

#include "mm/mm.h"
#include "mm/tlb.h"
#include "mm/mman.h"
#include "mm/page.h"

#include "proc/proc.h"

#include "util/string.h"
#include "util/debug.h"

#include "fs/vnode.h"
#include "fs/vfs.h"
#include "fs/file.h"

#include "vm/vmmap.h"
#include "vm/mmap.h"

/*
 * This function implements the mmap(2) syscall, but only
 * supports the MAP_SHARED, MAP_PRIVATE, MAP_FIXED, and
 * MAP_ANON flags.
 *
 * Add a mapping to the current process's address space.
 * You need to do some error checking; see the ERRORS section
 * of the manpage for the problems you should anticipate.
 * After error checking most of the work of this function is
 * done by vmmap_map(), but remember to clear the TLB.
 */
int do_mmap(void *addr, size_t len, int prot, int flags,
            int fd, off_t off, void **ret)
{
        file_t *f = fget(fd);
        int lopage = ADDR_TO_PN(addr);
        int endpage = ADDR_TO_PN((char *)addr + len);
        int npages = endpage - lopage;
        int dir = VMMAP_DIR_LOHI;
        int retval = 0;
        vmarea_t *vmarea = NULL;

        if (!((flags & MAP_PRIVATE) == MAP_PRIVATE) || ((flags & MAP_SHARED) == MAP_SHARED) || ((flags & MAP_FIXED) == MAP_FIXED) || ((flags & MAP_ANON) == MAP_ANON))
        {
                return -1;
        }
        if (!((flags & MAP_FIXED) == MAP_FIXED))
        {
                lopage = 0;
        }
        else if ((flags & MAP_FIXED) == MAP_FIXED)
        {
                if (((unsigned int)addr < USER_MEM_LOW) || ((unsigned int)addr > USER_MEM_HIGH))
                {
                        return -1;
                }
        }
        if (!f)
        {
                return -1;
        }
        if (!(f->f_vnode))
        {
                fput(f);
                return -1;
        }
        if ((retval = vmmap_map(curproc->p_vmmap, f->f_vnode, lopage, npages, prot, flags, off, dir, &vmarea)) != 0)
        {
                fput(f);
                return -1;
        }
        // tlb_flush_range(addr, len);
        tlb_flush_all();
        *ret = PN_TO_ADDR(vmarea->vma_start);
        fput(f);
        return 0;
        // NOT_YET_IMPLEMENTED("VM: do_mmap");
        // return -1;
}

/*
 * This function implements the munmap(2) syscall.
 *
 * As with do_mmap() it should perform the required error checking,
 * before calling upon vmmap_remove() to do most of the work.
 * Remember to clear the TLB.
 */
int do_munmap(void *addr, size_t len)
{
        int lopage = ADDR_TO_PN(addr);
        int endpage = ADDR_TO_PN((char *)addr + len);
        int npages = endpage - lopage;
        int ret = 0;

        if ((unsigned int)addr < USER_MEM_LOW || (unsigned int)addr > USER_MEM_HIGH)
        {
                return -1;
        }
        // if (len < 0)
        // {
        //         return -1;
        // }
        if ((unsigned int)((char *)addr + len) > USER_MEM_HIGH)
        {
                return -1;
        }
        if ((ret = vmmap_remove(curproc->p_vmmap, lopage, npages)) != 0)
        {
                return -1;
        }
        tlb_flush_all();
        // NOT_YET_IMPLEMENTED("VM: do_munmap");
        // return -1;
}
