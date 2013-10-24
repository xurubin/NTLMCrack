
struct PCB {
	unsigned char YY2[2],X2;
	char CurrentPwd[9];
	char Alphabet[255];
	int MinLen,MaxLen;
	char Pwd1[7],Pwd2[7];
	char challenge[8],response1[8],response2[8];
	int progress;
};


DWORD WINAPI BruteForcePwd2(
  LPVOID lpParameter
);

DWORD WINAPI BruteForcePwd1(
  LPVOID lpParameter
);

