#include <Windows.h>  
#include <iostream> 
#include <math.h>  
#include "HackProcess.h" 
using namespace std;  
CHackProcess fProcess;  

// Credits to NubTIK For creating this 
struct MyPlayer_t  
{ 
	DWORD CLocalPlayer; 
	int Team; 
	int Health; 
	float Position[3];  
	void ReadInformation() 
	{
		// Reading CLocalPlayer Pointer to our "CLocalPlayer" DWORD. 
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + 0x00780CD0), &CLocalPlayer, sizeof(DWORD), 0);
		// Reading out our Team to our "Team" Varible. 
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(CLocalPlayer + 0x98), &Team, sizeof(int), 0);
		// Reading out our Health to our "Health" Varible.     
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(CLocalPlayer + 0x90), &Health, sizeof(int), 0); 
		// Reading out our Position to our "Position" Varible. 
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(CLocalPlayer + 0x22C), &Position, sizeof(float[3]), 0); 
	}
}MyPlayer;   

struct PlayerList_t 
{
	DWORD CBaseEntity; 
	int Team; 
	int Health; 
	float Position[3]; 
	float AimbotAngle[3]; 
	char Name[39]; 
	DWORD CRadarBaseOffset; 

	void ReadInformation(int Player) 
	{
		// Reading CBaseEntity Pointer to our "CBaseEntity" DWORD + Current Player in the loop. 0x10 is the CBaseEntity List Size 
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + 0x00751FFC + (Player * 0x10)), &CBaseEntity, sizeof(DWORD), 0);
		// Reading out our Team to our "Team" Varible. 
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(CBaseEntity + 0x98), &Team, sizeof(int), 0);
		// Reading out our Health to our "Health" Varible.     
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(CBaseEntity + 0x90), &Health, sizeof(int), 0); 
		// Reading out our Position to our "Position" Varible. 
		ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(CBaseEntity + 0x22C), &Position, sizeof(float[3]), 0); 
		
		// Here we're using the RadarBasestruct (one of the results) To find the players names.. and store it in a char array of 39 Characters.
		ReadProcessMemory(fProcess.__HandleProcess , (PBYTE*)(fProcess.__dwordClient + 0x00791240) ,&CRadarBaseOffset, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess , (LPCVOID)(CRadarBaseOffset -0x140 + (0x140*Player) + 0x38), &Name, 39, 0);
 	}
}PlayerList[32];  

void CalcAngle( float *src, float *dst, float *angles )
{
	double delta[3] = { (src[0]-dst[0]), (src[1]-dst[1]), (src[2]-dst[2]) };
	double hyp = sqrt(delta[0]*delta[0] + delta[1]*delta[1]);
	angles[0] = (float) (asinf(delta[2]/hyp) * 57.295779513082f);
	angles[1] = (float) (atanf(delta[1]/delta[0]) * 57.295779513082f);
	angles[2] = 0.0f;
	if(delta[0] >= 0.0) { angles[1] += 180.0f; }
}
int main() 
{   
	fProcess.RunProcess(); // Waiting for CSS......
	cout << "CSS Found! Running hack...." << endl;  
	while (true) 
	{
		Sleep (1);  
		for (int i = 0;i < 32;i++) 
		{
			PlayerList[i].ReadInformation(i);       
			MyPlayer.ReadInformation(); 
			//Skip if enemy health is under 2 
			if (PlayerList[i].Health < 2) continue; 
			// Skip if they're my teammates. 
			if (PlayerList[i].Team == MyPlayer.Team) continue;  
			//IMPORTANT 
			//Now we need to read out our own name so we can compare our name to the enemy name and then skip. 
			// So we dont include our own position in the aimbot because if we do we will mess up  
			DWORD LocalName; // Can be found in engine.dll 
			ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + 0x0040FAEC), &LocalName, 4, 0); 
			char MyName[39]; 
			ReadProcessMemory (fProcess.__HandleProcess, (PBYTE*)(LocalName), &MyName, 39, 0);   
			// Skip if they are you.. 
			if (strcmp(PlayerList[i].Name, MyName) == 0) continue;   
			CalcAngle (MyPlayer.Position, PlayerList[i].Position, PlayerList[i].AimbotAngle);  
			if (GetAsyncKeyState(0x01)) 
				WriteProcessMemory (fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + 0x43140C), &PlayerList[i].AimbotAngle, 12, 0); 
		} 
	}
	getchar(); 
	return 0; 
}