#pragma once

#include "stdafx.h"

#include <string>

#pragma pack(1)
typedef struct _KLine {
	DWORD time_low;
	DWORD time_high;
	char pad0[2];

	float open;
	float high;
	float low;
	float close;

	char pad1[4];
	float vol;
	float amount;
	char pad2[8];
}KLine;
#pragma pack()

#define MAX_STOCK_NUMBER 7000
typedef struct _stock {
	char code[24];
}stock;

typedef struct _CDBAccessBuf {
	KLine *KBuffer;
	DWORD a1;
	DWORD a2;
	DWORD a3;
	DWORD Count;
	DWORD Length;
	DWORD reserve[0x500];
} CDBAccessBuf;

typedef struct __kmam_STG_INDEX {
	DWORD pad[0x30];
}kmam_STG_INDEX;

typedef struct __kama_SECURITY_IDENTIFY {
	DWORD pad[0x30];
}kama_SECURITY_IDENTIFY;

typedef struct _CDBAccess {
	int type;
	int alen; //0x2E
	DWORD *Count;
	DWORD *reserve[0x500];
} CDBAccess;

#define KPERIOD_NONE 0x00
#define KPERIOD_TICK 0x04
#define KPERIOD_MIN1 0x05
#define KPERIOD_MIN5 0x06
#define KPERIOD_DAY  0x0C
#define KPERIOD_ALL  0xFF

#define KLINE_SIZE   sizeof(KLine)

typedef class std::basic_string<wchar_t, struct std::char_traits<wchar_t>, class std::allocator<wchar_t>> String;
typedef bool(__cdecl* CDBAccess_DBInitial)(wchar_t *pDbPath); //14
typedef void(__cdecl* CDBAccess_DBCleanUp)(void);//13

typedef CDBAccess *(__cdecl* CDBAccess_GetDBInstance4)(LPWSTR marketname, int type, int idx2, int idx3, CDBAccessBuf *buf); //22
typedef CDBAccess *(__cdecl* CDBAccess_GetDBInstance3)(LPWSTR marketname, int type, int idx2, CDBAccessBuf *buf);  //23

typedef bool(__thiscall* CDBAccess_CheckOpen)(CDBAccess *dbAccess, CDBAccessBuf *buf);

typedef bool(__thiscall* CDBAccess_ReadDataByStkCode)(CDBAccess *dbAccess, const char *code, DWORD *a3, DWORD *a4, DWORD *a5, DWORD *a6); //38
typedef bool(__thiscall* CDBAccess_ReadDataByTime)(CDBAccess *dbAccess, const char *code, __int64 &a3, __int64 &a4, unsigned int &a5, __int64 &a6, unsigned short &a7, CDBAccessBuf *buf); //39
typedef bool(__thiscall* CDBAccess_ReadDataByPosTime)(CDBAccess *dbAccess, const char *code, int a3, int a4, int a5, int a6, int a7, int a8);

//protected: bool __thiscall CDBAccess::ReadFirstLastNItem(             char const *,     int,     int,       unsigned int &,   __int64 &,   unsigned short &,   class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > *,__int64)
typedef bool(__thiscall* CDBAccess_ReadFirstLastNItem)(CDBAccess *dbAccess, const char *code, int idx, int count, unsigned int &a5, __int64 &a6, unsigned short &a7, CDBAccessBuf *buf, __int64 a9); //42
typedef int(__thiscall* CDBAccess_WriteData)(CDBAccess *dbAccess, const char *code, int, int, int, int, int, int); // idb
typedef void(__thiscall* CDBAccess_DelData)(CDBAccess *dbAccess, const char *code); // idb
typedef bool(__thiscall* CDBAccess_DelAllStk)(CDBAccess *dbAccess);
typedef bool(__thiscall* CDBAccess_ReadFirstDataItem)(CDBAccess *dbAccess, const char *code, DWORD *a3);
typedef bool(__thiscall* CDBAccess_ReadLastDataItem)(CDBAccess *dbAccess, const char *code, DWORD *a3);
typedef int(__thiscall* CDBAccess_ReadDataCount)(CDBAccess *dbAccess, const char *code); // idb
typedef void(__thiscall* CDBAccess_ReadVerAndSeq)(CDBAccess *dbAccess, const char *code, unsigned int *, __int64 *); // idb
typedef void(__thiscall* CDBAccess_SetVerAndSeq)(CDBAccess *dbAccess, const char *code, unsigned int, __int64); // idb
typedef int(__thiscall* CDBAccess_GetStgIndexCount)(CDBAccess *dbAccess); // idb
typedef int(__thiscall* CDBAccess_GetStgDbVersion)(CDBAccess *dbAccess); // idb
typedef bool(__thiscall* CDBAccess_SetFileVer)(CDBAccess *dbAccess, unsigned int a2);
typedef void(__thiscall* CDBAccess_ReadVerAndSeqByIndex)(CDBAccess *dbAccess, int, unsigned int *, __int64 *); // idb
typedef bool(__thiscall* CDBAccess_ReadLastNItem)(CDBAccess *dbAccess, const char *code, int count, unsigned int *a0, __int64 *a1, unsigned short *a2, DWORD *a3, __int64 y); //44
typedef bool(__thiscall* CDBAccess_ReadNextNItem)(CDBAccess *dbAccess, const char *code, int a3, int a4, int a5, int a6, int a7, char a8, int a9);
typedef int(__thiscall* CDBAccess_GetStkList)(CDBAccess *dbAccess, stock *pstockList, int); // idb
typedef int(__thiscall* CDBAccess_ConfigSetting)(CDBAccess *dbAccess); // idb

typedef int(__thiscall* CDBAccess_Create)(CDBAccess *dbAccess, LPWSTR fdbfile, int, char); // idb
typedef int(__thiscall* CDBAccess_CreateStgDbFile)(CDBAccess *dbAccess, int); // idb
typedef void(__stdcall* sub_1000D140)(int a1);
typedef int(__thiscall* CDBAccess_OnReadStgDbHeader)(CDBAccess *dbAccess, const char *code, int a3, int a4);
typedef int(__thiscall* CDBAccess_OnReadStgIndexPage)(CDBAccess *dbAccess, const char *code, rsize_t MaxCount, unsigned int); // idb
typedef int(__thiscall* CDBAccess_OnReadRecyclePagesPage)(CDBAccess *dbAccess, const char *code, rsize_t MaxCount, unsigned int); // idb
typedef int(__thiscall* CDBAccess_ReCreateStgDbFile)(CDBAccess *dbAccess, int); // idb
typedef int(__thiscall* CDBAccess_BuildStgDbHeader)(CDBAccess *dbAccess, int); // idb
typedef void(__thiscall* CDBAccess_CloseStgDbFile)(CDBAccess *dbAccess); // idb
typedef int(__thiscall* CDBAccess_ReadStgDbHeader)(CDBAccess *dbAccess, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, int a4);
typedef int(__thiscall* CDBAccess_ReadOnePage)(CDBAccess *dbAccess, unsigned int, char *lpBuffer, int); // idb
typedef void(__thiscall* CDBAccess_RecycleOnePage)(CDBAccess *dbAccess, unsigned int); // idb
typedef int(__thiscall* CDBAccess_WriteBackStgDbHeader)(CDBAccess *dbAccess); // idb
typedef int(__thiscall* CDBAccess_WriteDataInPage)(CDBAccess *dbAccess, unsigned int, unsigned __int16, LPCVOID lpBuffer, int); // idb
typedef int(__thiscall* CDBAccess_WriteBackStgIndex)(CDBAccess *dbAccess, LPCVOID lpBuffer, int); // idb
typedef DWORD(__thiscall* CDBAccess_WriteBackStgIndexPageHeader)(CDBAccess *dbAccess, unsigned int a2);
typedef int(__thiscall* CDBAccess_WriteBackAllRecyclePagesPage)(CDBAccess *dbAccess); // idb
typedef int(__thiscall* CDBAccess_WriteOnePage)(CDBAccess *dbAccess, unsigned int, const char *); // idb
typedef int(__thiscall* CDBAccess_AllocatePage)(CDBAccess *dbAccess, unsigned int *); // idb
typedef bool(__thiscall* CDBAccess_FindStgIndex)(CDBAccess *dbAccess, struct __kmam_STG_INDEX *, const struct __kama_SECURITY_IDENTIFY *); // idb
typedef bool(__thiscall* CDBAccess_EraseStgIndex)(CDBAccess *dbAccess, struct __kmam_STG_INDEX *, const struct __kama_SECURITY_IDENTIFY *, int); // idb
typedef void(__thiscall* CDBAccess_Flush)(CDBAccess *dbAccess); // idb
typedef void(__thiscall* CDBAccess_FlushFile)(CDBAccess *dbAccess); // idb
typedef void(__thiscall* CDBAccess_NeedFlushWhenClose)(CDBAccess *dbAccess); // idb
typedef int(__thiscall* CDBAccess_ScanStgDbFile)(CDBAccess *dbAccess, int); // idb
typedef bool(__thiscall* CDBAccess_ScanFile)(CDBAccess *dbAccess);
typedef int(__thiscall* CDBAccess_CompareStgRecord)(CDBAccess *dbAccess, const struct __kama_TIME_SERIES_DATA *, const struct __kama_TIME_SERIES_DATA *);
typedef int(__thiscall* CDBAccess_DataItemsCount)(CDBAccess *dbAccess); //15