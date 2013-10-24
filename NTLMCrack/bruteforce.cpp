#include "stdafx.h"
#include "bruteforce.h"
#include "des.h"

DWORD WINAPI BruteForcePwd2(
  LPVOID lpParameter
  ) {
  	static const char LM_MAGIC[] = "KGS!@#$%";
	PCB *pcb = (PCB*)lpParameter;
	int i,j;
	unsigned char alphabet[256],current[7+1],x2;
	unsigned char * currenttxt = (unsigned char *)&(pcb->CurrentPwd); 
	gl_des_ctx des_ctx;
	unsigned char hash[9],buf[8];
	memset(alphabet,0,sizeof(alphabet));
	memset(current,0,sizeof(current));
	pcb->progress = 0;

	///////////Alphabet///////////////////////////
	int alphabetlen = strlen(pcb->Alphabet);
	int maxlen = pcb->MaxLen;
	memcpy(alphabet, pcb->Alphabet,alphabetlen);
	
	////////////Initialize current[]///////////////
	int curlen = strlen(pcb->CurrentPwd);
	for(i=0;i<curlen;i++) 
		for(j=0;j<alphabetlen;j++) 
			if (alphabet[j]==pcb->CurrentPwd[i])
					current[i] = j;

	///////////Clear unused currenttxt/////////////
	for(i=curlen;i<8;i++) *(currenttxt+i)=0;
	while (1) {
		pcb->progress++;
		////////////Map current to actual pwd2////////////
		for(i=0;i<curlen;i++) currenttxt[i] = alphabet[current[i]];

		////////////Calculate LM Hash of pwd2 ////////////
		gl_des_setkey7(&des_ctx,(unsigned char*)currenttxt);
		gl_des_ecb_encrypt(&des_ctx,LM_MAGIC,(char *)hash+1);

		////////////Compare it with YY2///////////////////
		if((hash[7]==pcb->YY2[0])&&(hash[8]==pcb->YY2[1])) {
			////////////////Try 256 possibilities of X2///////////
			x2 = 0;
			for(j=0;j<=0xff;j++) {
				///////////////Calculate Response/////////////////////
				hash[0] = x2;

				gl_des_setkey7(&des_ctx,(unsigned char*)hash);
				gl_des_ecb_encrypt(&des_ctx,pcb->challenge,(char *)buf);
				if (memcmp(buf,pcb->response2,8)==0) 
				{
					if (AfxMessageBox((LPCTSTR)currenttxt, MB_OKCANCEL, 0) == IDCANCEL)
						goto finish;
				}
				x2++;
			}
		}
		//////////////Move current forward by 1////////////
		j = 1; i=0;
		while ((j!=0)&&(i<curlen)) {
			current[i]++;

			if (current[i]==alphabetlen) 
				current[i] = 0;
			else j = 0;
			i++;
		}
		
		if ((i==curlen)&&(j==1)) {curlen++; current[curlen] = 0; }
		////////////if curlen>maxLen then break///////////////////////////////////////
		if (curlen>maxlen) break;
	}
finish:
	pcb->X2 = x2;
MessageBox(0,"BruteForcePwd2 finishes",0,0);
return 0;
}



DWORD WINAPI BruteForcePwd1(
  LPVOID lpParameter
  ) {
  	static const char LM_MAGIC[] = "KGS!@#$%";
	PCB *pcb = (PCB*)lpParameter;
	int i,j;
	unsigned char alphabet[128],next[128],firstchar;
	unsigned char * currenttxt = (unsigned char *)&(pcb->CurrentPwd); 
	gl_des_ctx des_ctx;
	unsigned char hash[8],buf[8];
	memset(alphabet,0,sizeof(alphabet));
	memset(next,0,sizeof(next));
	pcb->progress = 0;

	///////////Alphabet///////////////////////////
	int alphabetlen = strlen(pcb->Alphabet);
	int maxlen = pcb->MaxLen;
	memcpy(alphabet, pcb->Alphabet,alphabetlen);
	firstchar = alphabet[0];

	/////////Initialize Next[]/////////////////////
	for (i=0;i<alphabetlen-1;i++)
	{
		next[alphabet[i]] = alphabet[i+1];
	}

	////////////Initialize curlen[]///////////////
	int curlen = strlen((const char*)currenttxt);

	///////////Clear unused currenttxt/////////////
	for(i=curlen;i<8;i++) *(currenttxt+i)=0;

	while (1) {
		pcb->progress++;

		////////////Map current to actual pwd1////////////

		////////////Calculate LM Hash of pwd1////////////
		gl_des_setkey7(&des_ctx,(unsigned char*)currenttxt);
		gl_des_ecb_encrypt(&des_ctx,LM_MAGIC,(char *)hash);

		////////////Compare it with X2///////////////////
		if(hash[7]==pcb->X2) {
			///////////////Calculate Response/////////////////////
			gl_des_setkey7(&des_ctx,(unsigned char*)hash);
			gl_des_ecb_encrypt(&des_ctx,pcb->challenge,(char *)buf);
			if (memcmp(buf,pcb->response1,8)==0) goto finish;
		}

		//////////////Move current forward by 1////////////
		j = 1; i=0;
		while ((j!=0)&&(i<curlen)) {
			currenttxt[i] = next[currenttxt[i]];

			if (currenttxt[i]==0) 
				currenttxt[i] = firstchar;
			else j = 0;
			i++;
		}
		
		if ((i==curlen)&&(j==1)) {curlen++; currenttxt[curlen-1] = firstchar; }
		////////////if curlen>maxLen then break///////////////////////////////////////
		if (curlen>maxlen) break;
	}
finish:
MessageBox(0,"BruteForcePwd1 finishes",0,0);
return 0;
}

DWORD WINAPI BruteForcePwd1_OLD(
  LPVOID lpParameter
  ) {
  	static const char LM_MAGIC[] = "KGS!@#$%";
	PCB *pcb = (PCB*)lpParameter;
	int i,j;
	unsigned char alphabet[256],current[7+1];
	unsigned char * currenttxt = (unsigned char *)&(pcb->CurrentPwd); 
	gl_des_ctx des_ctx;
	unsigned char hash[8],buf[8];
	memset(alphabet,0,sizeof(alphabet));
	memset(current,0,sizeof(current));
	pcb->progress = 0;

	///////////Alphabet///////////////////////////
	int alphabetlen = strlen(pcb->Alphabet);
	int maxlen = pcb->MaxLen;
	memcpy(alphabet, pcb->Alphabet,alphabetlen);
	
	////////////Initialize current[]///////////////
	int curlen = strlen(pcb->CurrentPwd);
	for(i=0;i<curlen;i++) 
		for(j=0;j<alphabetlen;j++) 
			if (alphabet[j]==pcb->CurrentPwd[i])
					current[i] = j;

	///////////Clear unused currenttxt/////////////
	for(i=curlen;i<8;i++) *(currenttxt+i)=0;
	while (1) {
		pcb->progress++;

		////////////Map current to actual pwd1////////////
		for(i=0;i<curlen;i++) currenttxt[i] = alphabet[current[i]];

		////////////Calculate LM Hash of pwd1////////////
		gl_des_setkey7(&des_ctx,(unsigned char*)currenttxt);
		gl_des_ecb_encrypt(&des_ctx,LM_MAGIC,(char *)hash);

		////////////Compare it with X2///////////////////
		if(hash[7]==pcb->X2) {
			///////////////Calculate Response/////////////////////
			gl_des_setkey7(&des_ctx,(unsigned char*)hash);
			gl_des_ecb_encrypt(&des_ctx,pcb->challenge,(char *)buf);
			if (memcmp(buf,pcb->response1,8)==0) goto finish;
		}

		//////////////Move current forward by 1////////////
		j = 1; i=0;
		while ((j!=0)&&(i<curlen)) {
			current[i]++;

			if (current[i]==alphabetlen) 
				current[i] = 0;
			else j = 0;
			i++;
		}
		
		if ((i==curlen)&&(j==1)) {curlen++; current[curlen] = 0; }
		////////////if curlen>maxLen then break///////////////////////////////////////
		if (curlen>maxlen) break;
		
	}
finish:
MessageBox(0,"BruteForcePwd1 finishes",0,0);
return 0;
}
