/******************************************************************************/
/* Important Spring 2017 CSCI 402 usage information:                          */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#include "kernel.h"
#include "errno.h"
#include "globals.h"

#include "vm/vmmap.h"
#include "vm/shadow.h"
#include "vm/anon.h"

#include "proc/proc.h"

#include "util/debug.h"
#include "util/list.h"
#include "util/string.h"
#include "util/printf.h"

#include "fs/vnode.h"
#include "fs/file.h"
#include "fs/fcntl.h"
#include "fs/vfs_syscall.h"

#include "mm/slab.h"
#include "mm/page.h"
#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/mmobj.h"
#include "mm/tlb.h"

static slab_allocator_t *vmmap_allocator;
static slab_allocator_t *vmarea_allocator;

void vmmap_init(void)
{
        vmmap_allocator = slab_allocator_create("vmmap", sizeof(vmmap_t));
        KASSERT(NULL != vmmap_allocator && "failed to create vmmap allocator!");
        vmarea_allocator = slab_allocator_create("vmarea", sizeof(vmarea_t));
        KASSERT(NULL != vmarea_allocator && "failed to create vmarea allocator!");
}

vmarea_t *
vmarea_alloc(void)
{
        vmarea_t *newvma = (vmarea_t *)slab_obj_alloc(vmarea_allocator);
        if (newvma)
        {
                newvma->vma_vmmap = NULL;
        }
        return newvma;
}

void vmarea_free(vmarea_t *vma)
{
        KASSERT(NULL != vma);
        slab_obj_free(vmarea_allocator, vma);
}

/* a debugging routine: dumps the mappings of the given address space. */
size_t
vmmap_mapping_info(const void *vmmap, char *buf, size_t osize)
{
        KASSERT(0 < osize);
        KASSERT(NULL != buf);
        KASSERT(NULL != vmmap);

        vmmap_t *map = (vmmap_t *)vmmap;
        vmarea_t *vma;
        ssize_t size = (ssize_t)osize;

        int len = snprintf(buf, size, "%21s %5s %7s %8s %10s %12s\n",
                           "VADDR RANGE", "PROT", "FLAGS", "MMOBJ", "OFFSET",
                           "VFN RANGE");

        list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink)
        {
                size -= len;
                buf += len;
                if (0 >= size)
                {
                        goto end;
                }

                len = snprintf(buf, size,
                               "%#.8x-%#.8x  %c%c%c  %7s 0x%p %#.5x %#.5x-%#.5x\n",
                               vma->vma_start << PAGE_SHIFT,
                               vma->vma_end << PAGE_SHIFT,
                               (vma->vma_prot & PROT_READ ? 'r' : '-'),
                               (vma->vma_prot & PROT_WRITE ? 'w' : '-'),
                               (vma->vma_prot & PROT_EXEC ? 'x' : '-'),
                               (vma->vma_flags & MAP_SHARED ? " SHARED" : "PRIVATE"),
                               vma->vma_obj, vma->vma_off, vma->vma_start, vma->vma_end);
        }
        list_iterate_end();

end:
        if (size <= 0)
        {
                size = osize;
                buf[osize - 1] = '\0';
        }
        /*
        KASSERT(0 <= size);
        if (0 == size) {
                size++;
                buf--;
                buf[0] = '\0';
        }
        */
        return osize - size;
}

/* Create a new vmmap, which has no vmareas and does
 * not refer to a process. */
vmmap_t *
vmmap_create(void)
{
        vmmap_t *vmmap = slab_obj_alloc(vmmap_allocator);
        // KASSERT(vmmap != NULL);
        if (vmmap)
        {
                list_init(&(vmmap->vmm_list));
                vmmap->vmm_proc = NULL;
        }
        return vmmap;
        // NOT_YET_IMPLEMENTED("VM: vmmap_create");
        // return NULL;
}

/* Removes all vmareas from the address space and frees the
 * vmmap struct. */
void vmmap_destroy(vmmap_t *map)
{
        KASSERT(map);
        vmarea_t *vmarea = NULL;
        list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
        {
                list_remove(&(vmarea->vma_plink));
                list_remove(&(vmarea->vma_olink));
                vmarea->vma_obj->mmo_ops->put(vmarea->vma_obj);
                slab_obj_free(vmarea_allocator, vmarea);
        }
        list_iterate_end();
        slab_obj_free(vmmap_allocator, map);
        // NOT_YET_IMPLEMENTED("VM: vmmap_destroy");
}

/* Add a vmarea to an address space. Assumes (i.e. asserts to some extent)
 * the vmarea is valid.  This involves finding where to put it in the list
 * of VM areas, and adding it. Don't forget to set the vma_vmmap for the
 * area. */
void vmmap_insert(vmmap_t *map, vmarea_t *newvma)
{
        KASSERT(map);
        KASSERT(newvma);
        vmarea_t *vmarea = NULL;
        list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
        {
                if (newvma->vma_start <= vmarea->vma_start)
                {
                        list_insert_before(&(vmarea->vma_plink), &(newvma->vma_plink));
                        newvma->vma_vmmap = map;
                        return;
                }
        }
        list_iterate_end();
        list_insert_tail(&(map->vmm_list), &(newvma->vma_plink));
        newvma->vma_vmmap = map;
        // NOT_YET_IMPLEMENTED("VM: vmmap_insert");
}

/* Find a contiguous range of free virtual pages of length npages in
 * the given address space. Returns starting vfn for the range,
 * without altering the map. Returns -1 if no such range exists.
 *
 * Your algorithm should be first fit. If dir is VMMAP_DIR_HILO, you
 * should find a gap as high in the address space as possible; if dir
 * is VMMAP_DIR_LOHI, the gap should be as low as possible. */
int vmmap_find_range(vmmap_t *map, uint32_t npages, int dir)
{
        KASSERT(map);
        vmarea_t *vmarea = NULL;
        int isfirst = 1;

        if (dir == VMMAP_DIR_LOHI)
        {
                list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
                {
                        if (isfirst)
                        {
                                if (vmarea->vma_start - ADDR_TO_PN(USER_MEM_LOW) >= npages)
                                {
                                        return ADDR_TO_PN(USER_MEM_LOW);
                                }
                                isfirst = 0;
                        }
                        if (vmarea->vma_end - vmarea->vma_start >= npages)
                        {
                                return vmarea->vma_start;
                        }
                }
                list_iterate_end();
                if (ADDR_TO_PN(USER_MEM_HIGH) - vmarea->vma_end >= npages)
                {
                        return vmarea->vma_end;
                }
        }
        else if (dir == VMMAP_DIR_HILO)
        {
                list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
                {
                        if (isfirst)
                        {
                                if (ADDR_TO_PN(USER_MEM_HIGH) - vmarea->vma_start >= npages)
                                {
                                        return ADDR_TO_PN(USER_MEM_HIGH) - npages;
                                }
                                isfirst = 0;
                        }
                        if (vmarea->vma_start - vmarea->vma_end >= npages)
                        {
                                return vmarea->vma_start - npages;
                        }
                }
                list_iterate_end();
                if (vmarea->vma_end - ADDR_TO_PN(USER_MEM_LOW) >= npages)
                {
                        return vmarea->vma_end - npages;
                }
        }
        return -1;
        // NOT_YET_IMPLEMENTED("VM: vmmap_find_range");
        // return -1;
}
/* Find the vm_area that vfn lies in. Simply scan the address space
 * looking for a vma whose range covers vfn. If the page is unmapped,
 * return NULL. */
vmarea_t *
vmmap_lookup(vmmap_t *map, uint32_t vfn)
{
        KASSERT(map);
        vmarea_t *vmarea = NULL;
        list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
        {
                if (vmarea->vma_start <= vfn && vmarea->vma_end > vfn)
                {
                        return vmarea;
                }
        }
        list_iterate_end();
        return NULL;
        // NOT_YET_IMPLEMENTED("VM: vmmap_lookup");
        // return NULL;
}

/* Allocates a new vmmap containing a new vmarea for each area in the
 * given map. The areas should have no mmobjs set yet. Returns pointer
 * to the new vmmap on success, NULL on failure. This function is
 * called when implementing fork(2). */
vmmap_t *
vmmap_clone(vmmap_t *map)
{
        KASSERT(map);
        vmmap_t *newmap = slab_obj_alloc(vmmap_allocator);
        vmarea_t *vmarea = NULL;
        vmarea_t *newvmarea = NULL;

        if (newmap)
        {
                list_init(&(newmap->vmm_list));
                list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
                {
                        newvmarea = slab_obj_alloc(vmarea_allocator);
                        newvmarea->vma_start = vmarea->vma_start;
                        newvmarea->vma_end = vmarea->vma_end;
                        newvmarea->vma_off = vmarea->vma_off;
                        newvmarea->vma_prot = vmarea->vma_prot;
                        newvmarea->vma_flags = vmarea->vma_flags;
                        newvmarea->vma_vmmap = newmap;
                        newvmarea->vma_obj = NULL;

                        list_insert_tail(&(newmap->vmm_list), &(newvmarea->vma_plink));
                }
                list_iterate_end();
        }
        return newmap;
        // NOT_YET_IMPLEMENTED("VM: vmmap_clone");
        // return NULL;
}
/* Insert a mapping into the map starting at lopage for npages pages.
 * If lopage is zero, we will find a range of virtual addresses in the
 * process that is big enough, by using vmmap_find_range with the same
 * dir argument.  If lopage is non-zero and the specified region
 * contains another mapping that mapping should be unmapped.
 *
 * If file is NULL an anon mmobj will be used to create a mapping
 * of 0's.  If file is non-null that vnode's file will be mapped in
 * for the given range.  Use the vnode's mmap operation to get the
 * mmobj for the file; do not assume it is file->vn_obj. Make sure all
 * of the area's fields except for vma_obj have been set before
 * calling mmap.
 *
 * If MAP_PRIVATE is specified set up a shadow object for the mmobj.
 *
 * All of the input to this function should be valid (KASSERT!).
 * See mmap(2) for for description of legal input.
 * Note that off should be page aligned.
 *
 * Be very careful about the order operations are performed in here. Some
 * operation are impossible to undo and should be saved until there
 * is no chance of failure.
 *
 * If 'new' is non-NULL a pointer to the new vmarea_t should be stored in it.
 */
int vmmap_map(vmmap_t *map, vnode_t *file, uint32_t lopage, uint32_t npages,
              int prot, int flags, off_t off, int dir, vmarea_t **new)
{
        KASSERT(map);
        KASSERT(((flags & MAP_PRIVATE) == MAP_PRIVATE) || ((flags & MAP_SHARED) == MAP_SHARED));
        KASSERT((dir == VMMAP_DIR_HILO) || (dir == VMMAP_DIR_LOHI));
        int ret = 0;
        vmarea_t *vmarea = NULL;
        mmobj_t *mmobj = NULL;
        mmobj_t *shadowmmobj = NULL;

        if (lopage == 0)
        {
                if ((ret = vmmap_find_range(map, npages, dir)) == -1)
                {
                        return ret;
                }
                lopage = ret;
        }
        else
        {
                if (!vmmap_is_range_empty(map, lopage, npages))
                {
                        if ((ret = vmmap_remove(map, lopage, npages)) != 0)
                        {
                                return ret;
                        }
                }
        }

        vmarea = vmarea_alloc();
        vmarea->vma_start = lopage;
        vmarea->vma_end = lopage + npages;
        vmarea->vma_off = ADDR_TO_PN(off);
        vmarea->vma_flags = flags;
        vmarea->vma_prot = prot;

        if (file)
        {
                if ((ret = file->vn_ops->mmap(file, vmarea, &mmobj)) != 0)
                {
                        return ret;
                }
        }
        else
        {
                mmobj = anon_create();
        }

        if ((flags & MAP_PRIVATE) == MAP_PRIVATE)
        {
                shadowmmobj = shadow_create();
                shadowmmobj->mmo_shadowed = mmobj;
                shadowmmobj->mmo_un.mmo_bottom_obj = mmobj;
                vmarea->vma_obj = shadowmmobj;
        }
        else
        {
                vmarea->vma_obj = mmobj;
        }
        list_insert_tail(&(mmobj->mmo_un.mmo_vmas), &(vmarea->vma_olink));
        vmmap_insert(map, vmarea);
        if (new)
        {
                *new = vmarea;
        }
        return 0;
        // NOT_YET_IMPLEMENTED("VM: vmmap_map");
        // return -1;
}

/*
 * We have no guarantee that the region of the address space being
 * unmapped will play nicely with our list of vmareas.
 *
 * You must iterate over each vmarea that is partially or wholly covered
 * by the address range [addr ... addr+len). The vm-area will fall into one
 * of four cases, as illustrated below:
 *
 * key:
 *          [             ]   Existing VM Area
 *        *******             Region to be unmapped
 *
 * Case 1:  [   ******    ]
 * The region to be unmapped lies completely inside the vmarea. We need to
 * split the old vmarea into two vmareas. be sure to increment the
 * reference count to the file associated with the vmarea.
 *
 * Case 2:  [      *******]**
 * The region overlaps the end of the vmarea. Just shorten the length of
 * the mapping.
 *
 * Case 3: *[*****        ]
 * The region overlaps the beginning of the vmarea. Move the beginning of
 * the mapping (remember to update vma_off), and shorten its length.
 *
 * Case 4: *[*************]**
 * The region completely contains the vmarea. Remove the vmarea from the
 * list.
 */
int vmmap_remove(vmmap_t *map, uint32_t lopage, uint32_t npages)
{
        KASSERT(map);
        vmarea_t *newvmarea = NULL;
        vmarea_t *vmarea = NULL;
        mmobj_t *shadowmmobj1 = NULL;
        mmobj_t *shadowmmobj2 = NULL;
        mmobj_t *mmobj = NULL;

        list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
        {
                if (vmarea->vma_start < lopage && vmarea->vma_end <= lopage + npages)
                {
                        vmarea->vma_end = lopage;
                }
                else if (vmarea->vma_start < lopage && vmarea->vma_end > lopage + npages)
                {
                        newvmarea->vma_start = vmarea->vma_start;
                        newvmarea->vma_end = lopage;
                        newvmarea->vma_off = vmarea->vma_off;
                        newvmarea->vma_prot = vmarea->vma_prot;
                        newvmarea->vma_flags = vmarea->vma_flags;
                        newvmarea->vma_vmmap = map;
                        mmobj = vmarea->vma_obj;

                        shadowmmobj1 = shadow_create();
                        shadowmmobj2 = shadow_create();

                        newvmarea->vma_obj = shadowmmobj1;
                        vmarea->vma_obj = shadowmmobj2;

                        shadowmmobj1->mmo_un.mmo_bottom_obj = mmobj->mmo_un.mmo_bottom_obj;
                        shadowmmobj2->mmo_un.mmo_bottom_obj = mmobj->mmo_un.mmo_bottom_obj;

                        shadowmmobj1->mmo_shadowed = mmobj;
                        shadowmmobj2->mmo_shadowed = mmobj;

                        mmobj->mmo_ops->ref(mmobj);

                        list_insert_tail(&(mmobj->mmo_un.mmo_vmas), &(newvmarea->vma_olink));
                        list_insert_before(&(vmarea->vma_plink), &(newvmarea->vma_plink));

                        vmarea->vma_off = lopage + npages - vmarea->vma_start + vmarea->vma_off;
                }
                else if (vmarea->vma_start >= lopage && vmarea->vma_end <= lopage + npages)
                {
                        mmobj = vmarea->vma_obj;
                        mmobj->mmo_ops->put(mmobj);
                        list_remove(&vmarea->vma_plink);
                        list_remove(&(vmarea->vma_olink));
                        vmarea_free(vmarea);
                }
                else if (vmarea->vma_start >= lopage && vmarea->vma_end > lopage + npages)
                {
                        vmarea->vma_start = lopage;
                }
        }
        list_iterate_end();
        return 0;
        // NOT_YET_IMPLEMENTED("VM: vmmap_remove");
        // return -1;
}

/*
 * Returns 1 if the given address space has no mappings for the
 * given range, 0 otherwise.
 */
int vmmap_is_range_empty(vmmap_t *map, uint32_t startvfn, uint32_t npages)
{
        KASSERT(map);
        vmarea_t *vmarea = NULL;
        list_iterate_begin(&(map->vmm_list), vmarea, vmarea_t, vma_plink)
        {
                if ((vmarea->vma_start <= startvfn && vmarea->vma_end > startvfn) && (vmarea->vma_start <= startvfn + npages && vmarea->vma_end > startvfn + npages))
                {
                        return 0;
                }
        }
        list_iterate_end();
        return 1;
        // NOT_YET_IMPLEMENTED("VM: vmmap_is_range_empty");
        // return 0;
}

/* Read into 'buf' from the virtual address space of 'map' starting at
 * 'vaddr' for size 'count'. To do so, you will want to find the vmareas
 * to read from, then find the pframes within those vmareas corresponding
 * to the virtual addresses you want to read, and then read from the
 * physical memory that pframe points to. You should not check permissions
 * of the areas. Assume (KASSERT) that all the areas you are accessing exist.
 * Returns 0 on success, -errno on error.
 */
int vmmap_read(vmmap_t *map, const void *vaddr, void *buf, size_t count)
{
        KASSERT(map);
        int vfn1 = ADDR_TO_PN(vaddr);
        int offset1 = PAGE_OFFSET(vaddr);
        int vfn2 = ADDR_TO_PN((char *)vaddr + count);
        int offset2 = PAGE_OFFSET((char *)vaddr + count);
        vmarea_t *vmarea = NULL;
        pframe_t *pframe = NULL;
        int ret = 0;
        int curvfn = vfn1;
        char *curpos = buf;
        int pagenum = 0;

        if (vfn1 > vfn2)
        {
                return -1;
        }
        while (curvfn <= vfn2)
        {
                if (!(vmarea = vmmap_lookup(map, curvfn)))
                {
                        return -1;
                }
                pagenum = curvfn - vmarea->vma_start + vmarea->vma_off;
                // if ((ret = pframe_get(vmarea->vma_obj, pagenum, &pframe)) != 0)
                // {
                //         return -1;
                // }
                if ((ret = pframe_lookup(vmarea->vma_obj, pagenum, 0, &pframe)) != 0)
                {
                        return -1;
                }
                if (curvfn == vfn1)
                {
                        if (vfn1 == vfn2)
                        {
                                memcpy(curpos, (char *)pframe->pf_addr + offset1, offset2 - offset1);
                        }
                        else
                        {
                                memcpy(curpos, (char *)pframe->pf_addr + offset1, PAGE_SIZE - offset1);
                                curpos += PAGE_SIZE - offset1;
                        }
                }
                else if (curvfn == vfn2)
                {
                        memcpy(curpos, (char *)pframe->pf_addr, offset2);
                }
                else
                {
                        memcpy(curpos, (char *)pframe->pf_addr, PAGE_SIZE);
                        curpos += PAGE_SIZE;
                }
                curvfn++;
        }
        return 0;
        // NOT_YET_IMPLEMENTED("VM: vmmap_read");
        // return 0;
}

/* Write from 'buf' into the virtual address space of 'map' starting at
 * 'vaddr' for size 'count'. To do this, you will need to find the correct
 * vmareas to write into, then find the correct pframes within those vmareas,
 * and finally write into the physical addresses that those pframes correspond
 * to. You should not check permissions of the areas you use. Assume (KASSERT)
 * that all the areas you are accessing exist. Remember to dirty pages!
 * Returns 0 on success, -errno on error.
 */
int vmmap_write(vmmap_t *map, void *vaddr, const void *buf, size_t count)
{
        KASSERT(map);
        int vfn1 = ADDR_TO_PN(vaddr);
        int offset1 = PAGE_OFFSET((char *)vaddr);
        int vfn2 = ADDR_TO_PN((char *)vaddr + count);
        int offset2 = PAGE_OFFSET((char *)vaddr + count);
        vmarea_t *vmarea = NULL;
        pframe_t *pframe = NULL;
        int ret = 0;
        int pagenum = 0;
        int curvfn = vfn1;
        const char *curpos = buf;

        if (vfn1 > vfn2)
        {
                return -1;
        }
        while (curvfn <= vfn2)
        {
                if (!(vmarea = vmmap_lookup(map, curvfn)))
                {
                        return -1;
                }
                pagenum = curvfn - vmarea->vma_start + vmarea->vma_off;
                // if ((ret = pframe_get(vmarea->vma_obj, pagenum, &pframe)) != 0)
                // {
                //         return -1;
                // }
                if ((ret = pframe_lookup(vmarea->vma_obj, pagenum, 0, &pframe)) != 0)
                {
                        return -1;
                }
                if (curvfn == vfn1)
                {
                        if (vfn1 == vfn2)
                        {
                                memcpy((char *)pframe->pf_addr + offset1, curpos, offset2 - offset1);
                        }
                        else
                        {
                                memcpy((char *)pframe->pf_addr + offset1, curpos, PAGE_SIZE - offset1);
                                curpos += PAGE_SIZE - offset1;
                        }
                }
                else if (curvfn == vfn2)
                {
                        memcpy((char *)pframe->pf_addr, curpos, offset2);
                }
                else
                {
                        memcpy((char *)pframe->pf_addr, curpos, PAGE_SIZE);
                        curpos += PAGE_SIZE;
                }
                curvfn++;
        }
        return 0;
        // NOT_YET_IMPLEMENTED("VM: vmmap_write");
        // return 0;
}
