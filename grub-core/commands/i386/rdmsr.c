/* rdmsr.c - Read CPU model-specific registers */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2019  Free Software Foundation, Inc.
 *  Based on gcc/gcc/config/i386/driver-i386.c
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/dl.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/env.h>
#include <grub/command.h>
#include <grub/extcmd.h>
#include <grub/i386/rdmsr.h>
#include <grub/i18n.h>

GRUB_MOD_LICENSE("GPLv3+");

static grub_extcmd_t cmd_read;

static const struct grub_arg_option options[] =
{
    {0, 'v', 0, N_("Save read value into variable VARNAME."),
    N_("VARNAME"), ARG_TYPE_STRING},
    {0, 0, 0, 0, 0, 0}
};

static grub_err_t
grub_cmd_msr_read (grub_extcmd_context_t ctxt, int argc, char **argv)
{
    grub_uint64_t addr, value;
    char buf[sizeof("XXXXXXXX")];

    if (argc != 1)
        return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("one argument expected"));

    addr = grub_strtoul (argv[0], 0, 0);

    value = grub_msr_read (addr);

    if (ctxt->state[0].set)
    {
        grub_snprintf (buf, sizeof(buf), "%llx", value);
        grub_env_set (ctxt->state[0].arg, buf);
    }
    else
        grub_printf ("0x%llx\n", value);

    return GRUB_ERR_NONE;
}

GRUB_MOD_INIT(rdmsr)
{
    cmd_read = grub_register_extcmd ("rdmsr", grub_cmd_msr_read, 0,
                                    N_("ADDR"),
                                    N_("Read a CPU model specific register."),
                                    options);
}

GRUB_MOD_FINI(rdmsr)
{
    grub_unregister_extcmd (cmd_read);
}