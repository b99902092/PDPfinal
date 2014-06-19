//Don't forget to link libws2_32.a to your program as well for socket in windows visual studio 2012
//only for 1366 * 768 now and use bluestacks simulator
#include <winsock.h>
#include <stdio.h>

int map[5][6][2]=
{
	35, 350, //(0, 0)
	100, 350, //(0, 1)
	165, 350, //(0, 2)
	230, 350, //(0, 3)
	295, 350, //(0, 4)
	360, 350, //(0, 5)
	35, 415, //(1, 0)
	100, 415, //(1, 1)
	165, 415, //(1, 2)
	230, 415, //(1, 3)
	295, 415, //(1, 4)
	360, 415, //(1, 5)
	35, 480, //(2, 0)
	100, 480, //(2, 1)
	165, 480, //(2, 2)
	230, 480, //(2, 3)
	295, 480, //(2, 4)
	360, 480, //(2, 5)
	35, 545, //(3, 0)
	100, 545, //(3, 1)
	165, 545, //(3, 2)
	230, 545, //(3, 3)
	295, 545, //(3, 4)
	360, 545, //(3, 5)
	35, 610, //(4, 0)
	100, 610, //(4, 1)
	165, 610, //(4, 2)
	230, 610, //(4, 3)
	295, 610, //(4, 4)
	360, 610, //(4, 5)
};

SOCKET s; //Socket handle
HWND hWindow;

// calculate scale factor
const double XSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
const double YSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);

bool in_range(int num, int middle)
{
	int range = 35;
	if( middle - range <= num && num <= middle + range)
		return true;
	else
		return false;
}

int color(int r, int g, int b)
{
	//printf("(%3d,%3d,%3d)->", r, g, b);
	if((in_range(r, 190) && in_range(g, 39) && in_range(b, 9)) || (in_range(r, 230) && in_range(g, 70) && in_range(b, 50) && printf("~")))
		return 1;
	else if((in_range(r, 70) && in_range(g, 190) && in_range(b, 250)) || (in_range(r, 115) && in_range(g, 200) && in_range(b, 250) && printf("~")))
		return 2;
	else if((in_range(r, 40) && in_range(g, 160) && in_range(b, 30)) || (in_range(r, 70) && in_range(g, 230) && in_range(b, 60) && printf("~")))
		return 3;
	else if((in_range(r, 239) && in_range(g, 188) && in_range(b, 0)) || (in_range(r, 250) && in_range(g, 250) && in_range(b, 35) && printf("~")))
		return 4;
	else if((in_range(r, 170) && in_range(g, 40) && in_range(b, 200)) || (in_range(r, 210) && in_range(g, 50) && in_range(b, 220) && printf("~")))
		return 5;
	else if((in_range(r, 200) && in_range(g, 100) && in_range(b, 160)) || (in_range(r, 230) && in_range(g, 160) && in_range(b, 180) && printf("~")))
		return 6;
	else
	{
		printf(" (%3d,%3d,%3d) ", r, g, b);
		return 6;
	}
}

void showBoard(char* str)
{
	int len = 0;                                  
	HDC _hdcwindow = GetDC(hWindow);
	HDC _hdc = GetDC(NULL);
	if(_hdc)
	{
		RECT rect;
		GetWindowRect(hWindow, &rect);
		for(int i=0;i<5;i++)
		{
			for(int j=0;j<6;j++)
			{
				int s_r=0, s_g=0, s_b=0;

				COLORREF _color = GetPixel(_hdc, rect.left + map[i][j][0] -10, rect.top + map[i][j][1]-10);
				s_r += GetRValue(_color);
				s_g += GetGValue(_color);
				s_b += GetBValue(_color);
				_color = GetPixel(_hdc, rect.left + map[i][j][0] +10, rect.top + map[i][j][1]+10);
				s_r += GetRValue(_color);
				s_g += GetGValue(_color);
				s_b += GetBValue(_color);
				_color = GetPixel(_hdc, rect.left + map[i][j][0] -10, rect.top + map[i][j][1]+10);
				s_r += GetRValue(_color);
				s_g += GetGValue(_color);
				s_b += GetBValue(_color);
				_color = GetPixel(_hdc, rect.left + map[i][j][0] +10, rect.top + map[i][j][1]-10);
				s_r += GetRValue(_color);
				s_g += GetGValue(_color);
				s_b += GetBValue(_color);

				s_r /= 4;
				s_g /= 4;
				s_b /= 4;

				//printf("(%3d,%3d,%3d) ", s_r, s_g, s_b);
				int tmp=color(s_r, s_g, s_b);
				switch(tmp)
				{
					case 1:
						printf("f");
						break;
					case 2:
						printf("b");
						break;
					case 3:
						printf("w");
						break;
					case 4:
						printf("l");
						break;
					case 5:
						printf("d");
						break;
					case 6:
						printf("h");
						break;
					default:
						printf("?");
						break;
				}
				str[len] = tmp + '0';
				len++;
				//printf("%d", color(s_r, s_g, s_b));
			}
			str[len] = '\n';
			len++;
			printf("\n");
		}
	}
}

void moveStone(int x, int y, char* path)
{ 
    // get the window position
    RECT rect;
    GetWindowRect(hWindow, &rect);

    // calculate target position relative to application
    double nx = (map[x][y][0] + rect.left) * XSCALEFACTOR;
    double ny = (map[x][y][1] + rect.top) * YSCALEFACTOR;

	printf("%d %d %d %d %ld %ld\n",x, y, map[x][y][0], map[x][y][1], rect.left, rect.top);
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dx = (LONG)nx;
    Input.mi.dy = (LONG)ny;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
    SendInput(1,&Input,sizeof(INPUT));

	for(int i = 0; i < strlen(path); i++)
	{
		Sleep(150);
		int num = path[i] - '0';
		if(num == 2)
			ny += 65 * YSCALEFACTOR;
		else if(num == 8)
			ny -= 65 * YSCALEFACTOR;
		else if(num == 4)
			nx -= 65 * XSCALEFACTOR;
		else if(num == 6)
			nx += 65 * XSCALEFACTOR;

		Input.mi.dx = (LONG)nx;
		Input.mi.dy = (LONG)ny;
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		SendInput(1,&Input,sizeof(INPUT));
	}

	Sleep(100);
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1,&Input,sizeof(INPUT));
}

//CONNECTTOHOST ¡V Connects to a remote host
bool ConnectToHost(int PortNo, char* IPAddress)
{
    //Start up Winsock¡K
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error)
        return false;

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return false;
    }

    //Fill out the information needed to initialize a socket¡K
    SOCKADDR_IN target; //Socket address information

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (PortNo); //Port to connect on
    target.sin_addr.s_addr = inet_addr (IPAddress); //Target IP

    s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (s == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }  

    //Try connecting...

    if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
        return false; //Couldn't connect
    }
    else
        return true; //Success
}

//CLOSECONNECTION ¡V shuts down the socket and closes any connection on it
void CloseConnection()
{
    //Close the socket if it exists
    if (s)
        closesocket(s);

    WSACleanup(); //Clean up Winsock
}

int main(int argc, char* argv)
{
		/*while(1){
		HDC _hdc = GetDC(NULL);
		POINT _cursor;
		GetCursorPos(&_cursor);
		COLORREF _color = GetPixel(_hdc, _cursor.x, _cursor.y);
		int _red = GetRValue(_color);
		int _green = GetGValue(_color);
		int _blue = GetBValue(_color);
		printf("(%ld,%ld):[%3d,%3d,%3d]\n", _cursor.x, _cursor.y, _red, _green, _blue);
		system("pause");
		}*/
	if(!ConnectToHost(7788, "10.5.0.150"))
	{
		printf("connect failed.\n");
		system("pause");
		return 0;
	}
	else
	{
		hWindow = FindWindow(NULL, "BlueStacks App Player");
		if (hWindow == NULL) {
    		printf("Couldn't find application.\n");
		}else{

			if (!SetForegroundWindow(hWindow)) 
			{
    			printf("Couldn't set application to foreground.\n");
				system("pause");
				return 0;
			}
		}
		system("pause");
		while(1)
		{
			char str[100]={};
			showBoard(str);
			printf("========\n");
			printf("%s",str);
			printf("========\n");
			send(s, str, strlen(str)+1, 0);
			//system("pause");

			char buffer[80];
			memset(buffer, 0, sizeof(buffer));
			recv (s, buffer, sizeof(buffer)-1, 0); 
			printf("%s\n",buffer);

			int x ,y;
			x = buffer[0] - '0';
			y = buffer[2] - '0';
			printf("%d %d path:%s\n", x, y, &buffer[4]);
			SetForegroundWindow(hWindow);
			moveStone(x, y, &buffer[4]);
			system("pause");
		}
		CloseConnection();
	}
	return 0;
}