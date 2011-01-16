typedef enum {
    HTML, JS, CSS, OTHER
} FileType;

FileType GetFileType(const char* path)
{
    if (path)
    {
        if (strstr(path, ".htm") || !strcmp(path, "/"))
        {
            return HTML;
        }
        if (strstr(path, ".js")) 
        {
            return JS;
        }
        if (strstr(path, ".css"))
        {
            return CSS;
        }
    }
    return OTHER;
}

DWORD GetFileCurrentPointer(HANDLE hFile)
{
    return SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
}
void WriteLog(const char* info_format, ...)
{
    SYSTEMTIME timeNow;
    GetLocalTime(&timeNow);
	va_list arg_ptr;
	va_start(arg_ptr, info_format);
    FILE *pfLog;
    //time_t t;
    //time(&t);
    char time_str[64] = {0};
    sprintf(time_str, "%4d/%2d/%2d %2d:%2d:%2d %4dms ... ", timeNow.wYear, timeNow.wMonth, timeNow.wDay,
        timeNow.wHour, timeNow.wMinute, timeNow.wSecond, timeNow.wMilliseconds);
    //strftime(time_str, sizeof(time_str), "%Y/%m/%d %H:%M:%S %z...", localtime(&t));
	pfLog = fopen("soap.log", "a+");
	fprintf(pfLog, "%s", time_str);
	vfprintf(pfLog, info_format, arg_ptr);
	fprintf(pfLog, "\n");
    fclose(pfLog);
	va_end(arg_ptr);
}
FILE* OpenFileReadCRT(LPCSTR lpFilename)
{
    if (strlen(lpFilename) > 0)
    {
        return fopen(lpFilename, "rt"); // read text file.
    }
    return NULL;
}
DWORD ReadFileBytesCRT(FILE* pf, void* pDstBuf, DWORD lengthBuf)
{
    return fread(pDstBuf, sizeof(char), lengthBuf, pf);
}
DWORD WriteFileBytesCRT(FILE *pf, const void* pSrcBuf, DWORD lengthBuf)
{
    return fwrite(pSrcBuf, sizeof(char), lengthBuf, pf);
}
int CloseFile(FILE* pf)
{
    if (pf == NULL)
    {
        return 0;
    }
    return fclose(pf);
}
HANDLE MyOpenFile(
                LPCTSTR lpszFilename,
                DWORD dwDesiredAccess, 
                DWORD dwShareMode)
{
    return CreateFile(
                    lpszFilename, 
                    dwDesiredAccess,
                    dwShareMode,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
}

HANDLE OpenWebFile(LPCTSTR lpszFilename)
{
    return MyOpenFile(lpszFilename, 
                    GENERIC_READ,
                    FILE_SHARE_READ);
}

HRESULT ReadFileToBuffer(HANDLE hFile, LPVOID read_buf, DWORD buf_len, LPDWORD lpdwBytesRead)
{
    HRESULT hr = S_OK;
    ZeroMemory(read_buf, buf_len);
    *lpdwBytesRead = 0;
    if (FALSE == ReadFile(hFile, read_buf, buf_len, lpdwBytesRead, NULL))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    
    return hr;
}
int MbToWc(LPCSTR lpMultiByteStr,
           int    cbMultiByte,
           LPWSTR lpWideCharStr,
           int    cchWideChar)
{
    return MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

HANDLE WINAPI MyThread(
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPDWORD lpThreadId,
    LPVOID lpParameter,
    DWORD dwCreationFlags)
{
    return CreateThread(NULL, 0, 
        lpStartAddress, 
        lpParameter, 
        dwCreationFlags, 
        lpThreadId);
}

int ShowInfo(LPCSTR lpInfo, int lenInfo = -1)
{
    wchar_t *p = NULL;
    int lenWideChar = 0;
    if (lenInfo > 0)
    {
        lenWideChar = lenInfo;
    }
    else
    {
        lenWideChar = strlen(lpInfo);
    }
    p = new wchar_t[lenWideChar + 1];
    MbToWc(lpInfo, (lenInfo > 0 ? lenInfo : (-1)), p, lenWideChar + 1);
    int reVal = ::MessageBox(0, p, _T("info"), MB_OK);
    delete[] p;
    return reVal;
}
