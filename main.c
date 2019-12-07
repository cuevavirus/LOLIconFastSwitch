/*
LOLIcon Fast Switch - main.c
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

#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>
#include <taihen.h>

extern SceUID _vshKernelSearchModuleByName(const char *name, SceUInt64 *unk);
extern int LOLIconFastSwitchStopUnloadModule(SceUID modid);

int main() {
	SceUInt64 unk;

	SceUID kmod_id = _vshKernelSearchModuleByName("LOLIconFastSwitchKernel", &unk);
	sceClibPrintf("LOLIconFastSwitchKernel search %08X\n", kmod_id);
	if (kmod_id < 0) {
		kmod_id = taiLoadStartKernelModule("ux0:app/AKRK00001/LOLIconFastSwitch_kernel.skprx", 0, NULL, 0);
		sceClibPrintf("LOLIconFastSwitchKernel LoadStart %08X\n", kmod_id);
	}

	SceUID umod_id = -1;
	if (kmod_id >= 0) {
		umod_id = sceKernelLoadStartModule("ux0:app/AKRK00001/LOLIconFastSwitch_user.suprx", 0, NULL, 0, NULL, NULL);
		sceClibPrintf("LOLIconFastSwitchUser LoadStart %08X\n", umod_id);
	}

	if (umod_id >= 0) {
		SceUID loli_id = _vshKernelSearchModuleByName("LOLIcon", &unk);
		if (loli_id < 0) {
			loli_id = taiLoadStartKernelModule("ur0:tai/LOLIcon.skprx", 0, NULL, 0);
			sceClibPrintf("LOLIcon LoadStart %08X\n", loli_id);
		} else {
			int ret = LOLIconFastSwitchStopUnloadModule(loli_id);
			sceClibPrintf("LOLIcon StopUnload %08X\n", ret);
		}

		int ret = sceKernelStopUnloadModule(umod_id, 0, NULL, 0, NULL, NULL);
		sceClibPrintf("LOLIconFastSwitchUser StopUnload %08X\n", ret);
	}

	return sceKernelExitProcess(0);
}
