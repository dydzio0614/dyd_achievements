//JASS - Jedi Academy Server Security
//Copyright (C) 2010 Zlyden'
//http://jass.ucoz.net/
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//Created by:
//       Zlyden' <zlyden007@mail.ru>

/* v 1.0.2 29.08.2010 15:55:36 Zlyden' */

#ifndef __JASSAPI_H__
#define __JASSAPI_H__

//Плагины
#ifdef WIN32
 #define DLLEXPORT __declspec(dllexport)
#else
 #define DLLEXPORT
#endif
#ifdef __cplusplus
 #define C_DLLEXPORT extern "C" DLLEXPORT
#else
 #define C_DLLEXPORT DLLEXPORT
#endif

typedef int (*eng_syscall_t)(int, ...);
typedef int (*mod_vmMain_t)(int, int, int, int, int, int, int, int, int, int, int, int, int);
typedef void (*mod_dllEntry_t)(eng_syscall_t);

//Major plugin interface version
#define JASS_PIFV_MAJOR	1
//Minor plugin interface version
#define JASS_PIFV_MINOR	2

//Plugin info for JASS
typedef struct plugininfo_s {
	char* name;			//Plugin name, not yours
	char* version;
	char* desc;
	char* author;		//Now your name
	char* url;
	int canpause;		//Can plugin be paused via "jass pause"
	int loadcmd;		//Can plugin be loaded via "jass load"
	int unloadcmd;		//Can plugin be unloaded via "jass unload"

	int pifv_major;		//Major plugin interface version
	int pifv_minor;		//Minor plugin interface version
} plugininfo_t;


//Прототип для интерфейсных функций джаза
typedef struct pluginfuncs_s {
	int (*pfnWriteGameLog)(const char*);
	int (*pfnWriteJASSLog)(const char*);
	char* (*pfnVarArgs)(char*, ...);
	const char* (*pfnEngMsgName)(int);
	const char* (*pfnModMsgName)(int);
	int (*pfnGetIntCvar)(const char*);
	const char* (*pfnGetStrCvar)(const char*);
	int (*pfnGetGetLogHandle)();
	const char* (*pfnHomePath)();
	int (*pfnGetPluginID)(char*);
	const char* (*pfnGetPluginInfo)(int);
	int (*pfnPluginCall)(int, int, ...);//up to 13 args totally

} pluginfuncs_t;
//Macro for JASS functions
#define JASS_WRITEGAMELOG	(g_pluginfuncs->pfnWriteGameLog)
#define JASS_WRITEJASSLOG	(g_pluginfuncs->pfnWriteJASSLog)
#define JASS_VARARGS		(g_pluginfuncs->pfnVarArgs)
#define JASS_ENGMSGNAME		(g_pluginfuncs->pfnEngMsgName)
#define JASS_MODMSGNAME		(g_pluginfuncs->pfnModMsgName)
#define JASS_GETINTCVAR		(g_pluginfuncs->pfnGetIntCvar)
#define JASS_GETSTRCVAR		(g_pluginfuncs->pfnGetStrCvar)
#define JASS_GETLOGHANDLE	(g_pluginfuncs->pfnGetGetLogHandle)
#define JASS_GETHOMEPATH	(g_pluginfuncs->pfnHomePath)
#define JASS_GETPLUGINID	(g_pluginfuncs->pfnGetPluginID)
#define JASS_GETPLUGININFO	(g_pluginfuncs->pfnGetPluginInfo)
#define JASS_PLUGINCALL		(g_pluginfuncs->pfnPluginCall)

//Use IGNORED, OVERRIDE, and SUPERCEDE only
//UNUSED and ERROR is created for debug
typedef enum pluginres_e {
	JASS_UNUSED = -2,
	JASS_ERROR = -1,
	JASS_IGNORED = 0,
	JASS_OVERRIDE,
	JASS_SUPERCEDE
} pluginres_t;

//JASS_Query
typedef void (*plugin_query)(plugininfo_t**);
//JASS_Attach
typedef int (*plugin_attach)(eng_syscall_t, mod_vmMain_t, pluginres_t*, pluginfuncs_t*, int);
//JASS_Detach
typedef void (*plugin_detach)(int);
//JASS_syscall
typedef int (*plugin_syscall)(int, int, int, int, int, int, int, int, int, int, int, int, int, int);
//JASS_vmMain
typedef int (*plugin_vmmain)(int, int, int, int, int, int, int, int, int, int, int, int, int);
//JASS_pluginCall
typedef int (*plugin_interface)(int, int, int, int, int, int, int, int, int, int, int, int, int, int);

extern pluginres_t* g_result;			//'result' в JASS_Attach
extern plugininfo_t g_plugininfo;		//'*pinfo' к &g_plugininfo в JASS_Query
extern eng_syscall_t g_syscall;			//'engfunc' в JASS_Attach
extern mod_vmMain_t g_vmMain;			//'modfunc' в JASS_Attach
extern pluginfuncs_t* g_pluginfuncs;	//'pluginfuncs' в JASS_Attach

#define JASS_GIVE_PINFO() *pinfo = &g_plugininfo
#define JASS_SAVE_VARS() do { \
				g_syscall = engfunc; \
				g_vmMain = modfunc; \
				g_result = presult; \
				g_pluginfuncs = pluginfuncs; \
			} while(0)

//Prototypes for entry points
C_DLLEXPORT void JASS_Query(plugininfo_t** pinfo);
C_DLLEXPORT int JASS_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, int iscmd);
C_DLLEXPORT void JASS_Detach(int iscmd);
C_DLLEXPORT int JASS_vmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);
C_DLLEXPORT int JASS_syscall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12);
C_DLLEXPORT int JASS_vmMain_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);
C_DLLEXPORT int JASS_syscall_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12);
C_DLLEXPORT int JASS_pluginCall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12);

//Macro for plugin result
#define JASS_RETURN(x, y)	return (*g_result = (pluginres_t)(x), (y))
#define JASS_SET_RESULT(x)	*g_result = (pluginres_t)(x)
//JASS shows error message with name of cmd
#define JASS_RET_ERROR(x)	JASS_RETURN(JASS_ERROR, (x))
//plugin does not chenges the result
#define JASS_RET_IGNORED(x)	JASS_RETURN(JASS_IGNORED, (x))
//original func will be called, but it's result will be overriden
#define JASS_RET_OVERRIDE(x)	JASS_RETURN(JASS_OVERRIDE, (x))
//original func will NOT be called and this result will be returned
#define JASS_RET_SUPERCEDE(x)	JASS_RETURN(JASS_SUPERCEDE, (x))

#define ENT_FROM_NUM(index)		((gentity_t*)((byte*)g_gents + g_gentsize * (index)))
#define NUM_FROM_ENT(ent)		((int)((byte*)(ent) - (byte*)g_gents) / g_gentsize)
#define CLIENT_FROM_NUM(index)		((gclient_t*)((byte*)g_clients + g_clientsize * (index)))
#define NUM_FROM_CLIENT(ent)		((int)((byte*)(ent) - (byte*)g_clients) / g_clientsize)

#endif //__JASSAPI_H__
