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

/*
 *  FILE: vfs_syscall.c
 *  AUTH: mcc | jal
 *  DESC:
 *  DATE: Wed Apr  8 02:46:19 1998
 *  $Id: vfs_syscall.c,v 1.2 2018/05/27 03:57:26 cvsps Exp $
 */

#include "kernel.h"
#include "errno.h"
#include "globals.h"
#include "fs/vfs.h"
#include "fs/file.h"
#include "fs/vnode.h"
#include "fs/vfs_syscall.h"
#include "fs/open.h"
#include "fs/fcntl.h"
#include "fs/lseek.h"
#include "mm/kmalloc.h"
#include "util/string.h"
#include "util/printf.h"
#include "fs/stat.h"
#include "util/debug.h"

/*
 * Syscalls for vfs. Refer to comments or man pages for implementation.
 * Do note that you don't need to set errno, you should just return the
 * negative error code.
 */

/* To read a file:
 *      o fget(fd)
 *      o call its virtual read vn_op
 *      o update f_pos
 *      o fput() it
 *      o return the number of bytes read, or an error
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd is not a valid file descriptor or is not open for reading.
 *      o EISDIR
 *        fd refers to a directory.
 *
 * In all cases, be sure you do not leak file refcounts by returning before
 * you fput() a file that you fget()'ed.
 */

// extern int getfilename(struct vnode *dir, const char *path);

int do_read(int fd, void *buf, size_t nbytes)
{
        int ret = 0;
        int nbytesread = 0;
        // if (fd == -1)
        // {
        //         dbg(DBG_PRINT, "(GRADING2B)\n");
        //         return -EBADF;
        // }
        file_t *f = fget(fd);
        if (!f)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (S_ISDIR(f->f_vnode->vn_mode))
        {
                fput(f);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EISDIR;
        }
        if ((f->f_mode & FMODE_READ) != FMODE_READ)
        {
                fput(f);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if ((nbytesread = f->f_vnode->vn_ops->read(f->f_vnode, f->f_pos, buf, nbytes)) < 0)
        {
                fput(f);
                return nbytesread;
        }
        // f->f_pos += bytesread;
        if ((ret = do_lseek(fd, nbytesread, SEEK_CUR)) < 0)
        {
                fput(f);
                return ret;
        }
        fput(f);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        return nbytesread;
        // NOT_YET_IMPLEMENTED("VFS: do_read");
}

/* Very similar to do_read.  Check f_mode to be sure the file is writable.  If
 * f_mode & FMODE_APPEND, do_lseek() to the end of the file, call the write
 * vn_op, and fput the file.  As always, be mindful of refcount leaks.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd is not a valid file descriptor or is not open for writing.
 */
int do_write(int fd, const void *buf, size_t nbytes)
{
        // dbg(DBG_VFS, "fd=%d, buf=%s\n", fd, buf);
        int byteswritten = 0;
        if (fd == -1)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        struct file *f = fget(fd);
        if (f == NULL)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if ((f->f_mode & FMODE_WRITE) != FMODE_WRITE)
        {
                fput(f);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (f->f_mode & FMODE_APPEND)
        {
                do_lseek(fd, 0, SEEK_END);
                byteswritten = f->f_vnode->vn_ops->write(f->f_vnode, f->f_pos, buf, nbytes);
                f->f_pos += byteswritten;
                dbg(DBG_PRINT, "(GRADING2B)\n");
        }
        else
        {
                byteswritten = f->f_vnode->vn_ops->write(f->f_vnode, f->f_pos, buf, nbytes);
                f->f_pos += byteswritten;
                dbg(DBG_PRINT, "(GRADING2B)\n");
        }
        KASSERT((S_ISCHR(f->f_vnode->vn_mode)) || (S_ISBLK(f->f_vnode->vn_mode)) || (S_ISREG(f->f_vnode->vn_mode) && f->f_pos <= f->f_vnode->vn_len));
        dbg(DBG_PRINT, "(GRADING2A 3.a)\n");
        fput(f);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_write");
        return byteswritten;
}

/*
 * Zero curproc->p_files[fd], and fput() the file. Return 0 on success
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd isn't a valid open file descriptor.
 */
int do_close(int fd)
{
        if (fd < 0 || fd > NFILES)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (curproc->p_files[fd] != NULL)
        {
                fput(curproc->p_files[fd]);
                curproc->p_files[fd] = NULL;
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return 0;
        }
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_close");
        return -EBADF;
}

/* To dup a file:
 *      o fget(fd) to up fd's refcount
 *      o get_empty_fd()
 *      o point the new fd to the same file_t* as the given fd
 *      o return the new file descriptor
 *
 * Don't fput() the fd unless something goes wrong.  Since we are creating
 * another reference to the file_t*, we want to up the refcount.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd isn't an open file descriptor.
 *      o EMFILE
 *        The process already has the maximum number of file descriptors open
 *        and tried to open a new one.
 */
int do_dup(int fd)
{
        if (fd == -1)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        file_t *f = fget(fd);
        if (f == NULL)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        int newfd = get_empty_fd(curproc);
        if (newfd == -EMFILE)
        {
                fput(f);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EMFILE;
        }
        curproc->p_files[newfd] = f;
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_dup");
        return newfd;
}

/* Same as do_dup, but insted of using get_empty_fd() to get the new fd,
 * they give it to us in 'nfd'.  If nfd is in use (and not the same as ofd)
 * do_close() it first.  Then return the new file descriptor.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        ofd isn't an open file descriptor, or nfd is out of the allowed
 *        range for file descriptors.
 */
int do_dup2(int ofd, int nfd)
{
        if (ofd == -1 || nfd == -1)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        file_t *f = NULL;
        if (nfd < 0 || nfd > NFILES)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (ofd < 0 || ofd > NFILES)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (curproc->p_files[nfd] != NULL)
        {
                if (ofd != nfd)
                {
                        do_close(nfd);
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                }
                else
                {
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        return ofd;
                }
        }
        f = fget(ofd);
        if (f == NULL)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        curproc->p_files[nfd] = f;
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_dup2");
        return nfd;
}

/*
 * This routine creates a special file of the type specified by 'mode' at
 * the location specified by 'path'. 'mode' should be one of S_IFCHR or
 * S_IFBLK (you might note that mknod(2) normally allows one to create
 * regular files as well-- for simplicity this is not the case in Weenix).
 * 'devid', as you might expect, is the device identifier of the device
 * that the new special file should represent.
 *
 * You might use a combination of dir_namev, lookup, and the fs-specific
 * mknod (that is, the containing directory's 'mknod' vnode operation).
 * Return the result of the fs-specific mknod, or an error.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EINVAL
 *        mode requested creation of something other than a device special
 *        file.
 *      o EEXIST
 *        path already exists.
 *      o ENOENT
 *        A directory component in path does not exist.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int do_mknod(const char *path, int mode, unsigned devid)
{
        //         if (strlen(path) > NAME_LEN)
        //         {
        //                 dbg(DBG_PRINT, "(GRADING2B)\n");
        //                 return -ENAMETOOLONG;
        //         }
        if (!S_ISCHR(mode) && !S_ISBLK(mode))
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EINVAL;
        }
        const char *name = NULL;
        struct vnode *dir_vnode = NULL;
        size_t namelen = 0;
        int ret = 0;
        struct vnode *resvnode = NULL;

        ret = dir_namev(path, &namelen, &name, NULL, &dir_vnode);
        if (ret != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if (namelen == 0)
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EEXIST;
        }
        ret = lookup(dir_vnode, name, namelen, &resvnode);
        if (ret == 0)
        {
                vput(dir_vnode);
                vput(resvnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EEXIST;
        }
        if (dir_vnode->vn_ops->mknod == NULL)
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -ENOTDIR;
        }
        KASSERT(NULL != dir_vnode->vn_ops->mknod);
        dbg(DBG_PRINT, "(GRADING2A 3.b)\n");
        ret = dir_vnode->vn_ops->mknod(dir_vnode, name, namelen, mode, devid);
        vput(dir_vnode);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_mknod");
        return ret;
}

/* Use dir_namev() to find the vnode of the dir we want to make the new
 * directory in.  Then use lookup() to make sure it doesn't already exist.
 * Finally call the dir's mkdir vn_ops. Return what it returns.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EEXIST
 *        path already exists.
 *      o ENOENT
 *        A directory component in path does not exist.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int do_mkdir(const char *path)
{
        // if (strlen(path) >= NAME_LEN)
        // {
        //         dbg(DBG_PRINT, "(GRADING2B)\n");
        //         return -ENAMETOOLONG;
        // }
        int ret;
        const char *name = NULL;
        size_t namelen = 0;
        struct vnode *namevnode = NULL;
        struct vnode *dir_vnode = NULL;
        if ((ret = dir_namev(path, &namelen, &name, NULL, &dir_vnode)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if (namelen == 0)
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EEXIST;
        }
        if ((ret = lookup(dir_vnode, name, namelen, &namevnode)) == 0)
        {
                vput(dir_vnode);
                vput(namevnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EEXIST;
        }

        if (dir_vnode->vn_ops->mkdir == NULL)
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -ENOTDIR;
        }
        KASSERT(NULL != dir_vnode->vn_ops->mkdir);
        dbg(DBG_PRINT, "(GRADING2A 3.c)\n");
        ret = dir_vnode->vn_ops->mkdir(dir_vnode, name, namelen);
        vput(dir_vnode);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_mkdir");
        return ret;
}

/* Use dir_namev() to find the vnode of the directory containing the dir to be
 * removed. Then call the containing dir's rmdir v_op.  The rmdir v_op will
 * return an error if the dir to be removed does not exist or is not empty, so
 * you don't need to worry about that here. Return the value of the v_op,
 * or an error.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EINVAL
 *        path has "." as its final component.
 *      o ENOTEMPTY
 *        path has ".." as its final component.
 *      o ENOENT
 *        A directory component in path does not exist.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int do_rmdir(const char *path)
{
        struct vnode *dir_vnode = NULL;
        const char *name = NULL;
        size_t namelen = 0;
        int ret = 0;

        // if (strlen(path) > NAME_LEN)
        // {
        //         dbg(DBG_PRINT, "(GRADING2B)\n");
        //         return -ENAMETOOLONG;
        // }
        if ((ret = dir_namev(path, &namelen, &name, NULL, &dir_vnode)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if (!strcmp(name, "."))
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EINVAL;
        }
        if (!strcmp(name, ".."))
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -ENOTEMPTY;
        }
        if (dir_vnode->vn_ops->rmdir == NULL)
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -ENOTDIR;
        }
        KASSERT(NULL != dir_vnode->vn_ops->rmdir);
        dbg(DBG_PRINT, "(GRADING2A 3.d)\n");
        dbg(DBG_PRINT, "(GRADING2B)\n");
        ret = dir_vnode->vn_ops->rmdir(dir_vnode, name, namelen);
        vput(dir_vnode);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_rmdir");
        return ret;
}

/*
 * Similar to do_rmdir, but for files.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EPERM
 *        path refers to a directory.
 *      o ENOENT
 *        Any component in path does not exist, including the element at the
 *        very end.
 *      o ENOTDIR
 *        A component used as a directory in path is not, in fact, a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 */
int do_unlink(const char *path)
{
        struct vnode *dir_vnode = NULL;
        int ret = 0;
        const char *name = NULL;
        size_t namelen = 0;
        struct vnode *namevnode = NULL;

        if (strlen(path) > NAME_LEN)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ENAMETOOLONG;
        }
        if ((ret = dir_namev(path, &namelen, &name, NULL, &dir_vnode)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if (namelen == 0)
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EPERM;
        }
        if ((ret = lookup(dir_vnode, name, namelen, &namevnode)) != 0)
        {
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if (S_ISDIR(namevnode->vn_mode))
        {
                vput(dir_vnode);
                vput(namevnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EPERM;
        }
        vput(namevnode);
        KASSERT(NULL != dir_vnode->vn_ops->unlink);
        dbg(DBG_PRINT, "(GRADING2A 3.e)\n");
        dbg(DBG_PRINT, "(GRADING2B)\n");
        ret = dir_vnode->vn_ops->unlink(dir_vnode, name, namelen);
        vput(dir_vnode);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_unlink");
        return ret;
}

/* To link:
 *      o open_namev(from)
 *      o dir_namev(to)
 *      o call the destination dir's (to) link vn_ops.
 *      o return the result of link, or an error
 *
 * Remember to vput the vnodes returned from open_namev and dir_namev.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EEXIST
 *        to already exists.
 *      o ENOENT
 *        A directory component in from or to does not exist.
 *      o ENOTDIR
 *        A component used as a directory in from or to is not, in fact, a
 *        directory.
 *      o ENAMETOOLONG
 *        A component of from or to was too long.
 *      o EPERM
 *        from is a directory.
 */
int do_link(const char *from, const char *to)
{
        struct vnode *fromvnode = NULL;
        size_t namelen = 0;
        const char *name = NULL;
        struct vnode *dir_vnode = NULL;
        struct vnode *tovnode = NULL;
        int ret = 0;

        // if (strlen(from) > NAME_LEN || strlen(to) > NAME_LEN)
        // {
        //         dbg(DBG_PRINT, "(GRADING2B)\n");
        //         return -ENAMETOOLONG;
        // }
        if ((ret = open_namev(from, O_CREAT, &fromvnode, NULL)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if (S_ISDIR(fromvnode->vn_mode))
        {
                vput(fromvnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EPERM;
        }
        if ((ret = dir_namev(to, &namelen, &name, NULL, &dir_vnode)) != 0)
        {
                vput(fromvnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if ((ret = lookup(dir_vnode, name, namelen, &tovnode)) == 0)
        {
                vput(dir_vnode);
                vput(tovnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EEXIST;
        }
        if (dir_vnode->vn_ops->link == NULL)
        {
                vput(fromvnode);
                vput(dir_vnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -ENOTDIR;
        }
        ret = dir_vnode->vn_ops->link(fromvnode, dir_vnode, name, namelen);
        vput(fromvnode);
        vput(dir_vnode);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_link");
        return ret;
}

/*      o link newname to oldname
 *      o unlink oldname
 *      o return the value of unlink, or an error
 *
 * Note that this does not provide the same behavior as the
 * Linux system call (if unlink fails then two links to the
 * file could exist).
 */
int do_rename(const char *oldname, const char *newname)
{
        int ret = 0;
        if ((ret = do_link(oldname, newname)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if ((ret = do_unlink(oldname)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_rename");
        return 0;
}

/* Make the named directory the current process's cwd (current working
 * directory).  Don't forget to down the refcount to the old cwd (vput()) and
 * up the refcount to the new cwd (open_namev() or vget()). Return 0 on
 * success.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o ENOENT
 *        path does not exist.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 *      o ENOTDIR
 *        A component of path is not a directory.
 */
int do_chdir(const char *path)
{
        struct vnode *resvnode;
        int ret = 0;

        // if (strlen(path) > NAME_LEN)
        // {
        //         dbg(DBG_PRINT, "(GRADING2B)\n");
        //         return -ENAMETOOLONG;
        // }
        if ((ret = open_namev(path, 0, &resvnode, NULL)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        if (!S_ISDIR(resvnode->vn_mode))
        {
                vput(resvnode);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -ENOTDIR;
        }
        vput(curproc->p_cwd);
        curproc->p_cwd = resvnode;
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_chdir");
        return 0;
}

/* Call the readdir vn_op on the given fd, filling in the given dirent_t*.
 * If the readdir vn_op is successful, it will return a positive value which
 * is the number of bytes copied to the dirent_t.  You need to increment the
 * file_t's f_pos by this amount.  As always, be aware of refcounts, check
 * the return value of the fget and the virtual function, and be sure the
 * virtual function exists (is not null) before calling it.
 *
 * Return either 0 or sizeof(dirent_t), or -errno.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        Invalid file descriptor fd.
 *      o ENOTDIR
 *        File descriptor does not refer to a directory.
 */
int do_getdent(int fd, struct dirent *dirp)
{
        if (fd == -1)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        file_t *f = fget(fd);
        if (f == NULL)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (f->f_vnode->vn_ops->readdir == NULL)
        {
                fput(f);
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -ENOTDIR;
        }
        int bytesread = f->f_vnode->vn_ops->readdir(f->f_vnode, f->f_pos, dirp);
        fput(f);
        f->f_pos += bytesread;
        if (bytesread == 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return 0;
        }
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_getdent");
        return sizeof(dirent_t);
}

/*
 * Modify f_pos according to offset and whence.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o EBADF
 *        fd is not an open file descriptor.
 *      o EINVAL
 *        whence is not one of SEEK_SET, SEEK_CUR, SEEK_END; or the resulting
 *        file offset would be negative.
 */
int do_lseek(int fd, int offset, int whence)
{
        int ret = 0;
        if (fd == -1)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (whence != SEEK_CUR && whence != SEEK_END && whence != SEEK_SET)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EINVAL;
        }
        struct file *f = fget(fd);
        if (f == NULL)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EBADF;
        }
        if (whence == SEEK_CUR)
        {
                if (f->f_pos + offset < 0)
                {
                        fput(f);
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        return -EINVAL;
                }
                f->f_pos = f->f_pos + offset;
        }
        else if (whence == SEEK_END)
        {
                if (f->f_vnode->vn_len + offset < 0)
                {
                        fput(f);
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        return -EINVAL;
                }
                f->f_pos = f->f_vnode->vn_len + offset;
        }
        else if (whence == SEEK_SET)
        {
                if (offset < 0)
                {
                        fput(f);
                        dbg(DBG_PRINT, "(GRADING2B)\n");
                        return -EINVAL;
                }
                f->f_pos = offset;
        }
        ret = f->f_pos;
        fput(f);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        return ret;
        // NOT_YET_IMPLEMENTED("VFS: do_lseek");
}

/*
 * Find the vnode associated with the path, and call the stat() vnode operation.
 *
 * Error cases you must handle for this function at the VFS level:
 *      o ENOENT
 *        A component of path does not exist.
 *      o ENOTDIR
 *        A component of the path prefix of path is not a directory.
 *      o ENAMETOOLONG
 *        A component of path was too long.
 *      o EINVAL
 *        path is an empty string.
 */
int do_stat(const char *path, struct stat *buf)
{
        struct vnode *dir_vnode = NULL;
        int ret = 0;

        // if (strlen(path) > NAME_LEN)
        // {
        //         dbg(DBG_PRINT, "(GRADING2B)\n");
        //         return -ENAMETOOLONG;
        // }
        if (strlen(path) == 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return -EINVAL;
        }
        if ((ret = open_namev(path, 0, &dir_vnode, NULL)) != 0)
        {
                dbg(DBG_PRINT, "(GRADING2B)\n");
                return ret;
        }
        // if (dir_vnode->vn_ops->stat == NULL)
        // {
        //         vput(dir_vnode);
        //         return -ENOTDIR;
        // }
        ret = dir_vnode->vn_ops->stat(dir_vnode, buf);
        dbg(DBG_PRINT, "(GRADING2A 3.f)\n");
        dbg(DBG_PRINT, "(GRADING2B)\n");
        vput(dir_vnode);
        dbg(DBG_PRINT, "(GRADING2B)\n");
        // NOT_YET_IMPLEMENTED("VFS: do_stat");
        return ret;
}

#ifdef __MOUNTING__
/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutely sure your Weenix is perfect.
 *
 * This is the syscall entry point into vfs for mounting. You will need to
 * create the fs_t struct and populate its fs_dev and fs_type fields before
 * calling vfs's mountfunc(). mountfunc() will use the fields you populated
 * in order to determine which underlying filesystem's mount function should
 * be run, then it will finish setting up the fs_t struct. At this point you
 * have a fully functioning file system, however it is not mounted on the
 * virtual file system, you will need to call vfs_mount to do this.
 *
 * There are lots of things which can go wrong here. Make sure you have good
 * error handling. Remember the fs_dev and fs_type buffers have limited size
 * so you should not write arbitrary length strings to them.
 */
int do_mount(const char *source, const char *target, const char *type)
{
        NOT_YET_IMPLEMENTED("MOUNTING: do_mount");
        return -EINVAL;
}

/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutley sure your Weenix is perfect.
 *
 * This function delegates all of the real work to vfs_umount. You should not worry
 * about freeing the fs_t struct here, that is done in vfs_umount. All this function
 * does is figure out which file system to pass to vfs_umount and do good error
 * checking.
 */
int do_umount(const char *target)
{
        NOT_YET_IMPLEMENTED("MOUNTING: do_umount");
        return -EINVAL;
}
#endif
