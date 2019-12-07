/*
LOLIcon Fast Switch - main_user.c
Copyright (C) 2019 浅倉麗子

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <psp2/kernel/modulemgr.h>

extern int LOLIconFastSwitchStopUnloadModuleForUser(SceUID modid);

int LOLIconFastSwitchStopUnloadModule(SceUID modid) {
	return LOLIconFastSwitchStopUnloadModuleForUser(modid);
}

int _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize argc, const void *argv) { (void)argc; (void)argv;
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *argv) { (void)argc; (void)argv;
	return SCE_KERNEL_STOP_SUCCESS;
}
