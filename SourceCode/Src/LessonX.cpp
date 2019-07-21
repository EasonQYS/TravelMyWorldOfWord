/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		Game1Run( float fDeltaTime );
void		Game2Run( float fDeltaTime );
void		Game3Run( float fDeltaTime );
void		Game4Run( float fDeltaTime );
void		Game5Run( float fDeltaTime );
void		GameEnd();

int g_iWordAlready=0;
int g_iWordRemain=0;
int g_iWordState[4096];
int g_iWordNum=1;
int g_iWordCnt=0;
int g_iCharCnt=0;
int g_iWordC;
int g_iCutChar=0;
int g_iCutRan[128];
int g_iGameWait;
int g_iWordListNum=1;
int g_iToLearn[128];

string g_sWordList[128];
string g_sCorE;

float g_fWaitTime=1;
float g_fvolum=1;
float g_fRandomSeed;

char g_sTouch[128];
char g_cFindchar=0;
char g_sWordE[128];
char g_cCutList[128];
char g_cTouchChar;
char g_sFWordList[128]="TOEFL570.txt";
char g_sFWordState[128]="wordstate570.txt";
char g_sFWordInfo[128]="wordinfo.txt";
//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。
//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态.
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float	fDeltaTime )
{
    g_fRandomSeed=fDeltaTime*2911;
	switch( g_iGameState )
	{
		// 初始化游戏，清空上一局相关数据
	case 0:
		{
		    dStopAllSound();
		    dPlaySound("beijing",1,g_fvolum-0.2);
			GameInit();
			g_iGameState	=	-1; // 初始化之后，将游戏状态设置为等待中
		}
		break;
		// 游戏进行中，处理各种游戏逻辑
    case 1:
        {
            Game1Run(fDeltaTime);
            break;
        }
    case 2:
        {
            Game2Run(fDeltaTime);
            break;
        }
    case 3:
        {
            Game3Run(fDeltaTime);
            break;
        }
    case 4:
        {
            Game4Run(fDeltaTime);
            break;
        }
    case 5:
        {
            Game5Run(fDeltaTime);
            break;
        }
	case 20:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始

	default:
		break;
	};
}

//==============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
    dShowCursor(1);
    dLoadMap("level.t2d");
    memset(g_iWordState,0,sizeof g_iWordState);

    ifstream fininfo(g_sFWordInfo);
    if(!fininfo)cout<<"error!";
    else
    {
        fininfo>>g_iWordNum;
        fininfo>>g_fvolum;
        fininfo>>g_iWordListNum;
        if(g_iWordListNum==1)
        {
            strcpy(g_sFWordList,"TOEFL570.txt");
            strcpy(g_sFWordState,"wordstate570.txt");
        }
        if(g_iWordListNum==2)
        {
            strcpy(g_sFWordList,"CET4.txt");
            strcpy(g_sFWordState,"wordstate3371.txt");
        }
        if(g_iWordListNum==3)
        {
            strcpy(g_sFWordList,"CET6.txt");
            strcpy(g_sFWordState,"wordstate2076.txt");
        }
    }
    fininfo.close();

    ifstream finstate(g_sFWordState);
    if(!finstate)cout<<"error!\n";
    else
    {
        string s;
        g_iWordAlready=0;
        getline(finstate,s);

        for(int i=1;;i++)
        {
            if(s[i]==0){g_iWordState[0]=i-1;break;}
            if(s[i]=='1'){g_iWordState[i]=1;g_iWordAlready++;}
            else g_iWordState[i]=0;
        }
        finstate.close();
        g_iWordRemain=g_iWordState[0]-g_iWordAlready;
    }

    dSetTextValue("already",g_iWordAlready);
    dSetTextValue("remain",g_iWordRemain);
}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{
}
void Game1Run( float fDeltaTime )
{
    dShowCursor(1);
    dSetTextString("wordec",&g_sWordList[g_iWordCnt][0]);
    if(g_iGameWait==1)
    {
        g_fWaitTime-=fDeltaTime;
        if(g_fWaitTime>0.2f)
        {
        }
        else
        {
             g_iCharCnt++;
             g_iGameWait=0;
             g_fWaitTime=1;
             g_sCorE="";
             dSetTextString("game1display","");
             if(g_iWordCnt==g_iWordNum)
            {
                dPlaySound("success.wav",0,g_fvolum);
                g_iGameState=2;
                dLoadMap("game2.t2d");
                g_iWordCnt=0;
                g_cFindchar=0;

                int i;
                for(i=0;;i++)
                {
                    if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                        g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                    else
                    {
                        g_sWordE[i]=0;
                        break;
                    }
                }
                g_iWordC=i;
                g_iCharCnt=0;
            }
            else
            {
                int i=0;
                for(i=0;;i++)
                {
                    if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                        g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                    else
                    {
                        g_sWordE[i]=0;
                        break;
                    }
                }
                g_iWordC=i;
                g_iCharCnt=0;
            }
        }
    }
    if(g_iGameWait==0&&g_cFindchar==g_sWordE[g_iCharCnt])
    {
        dPlaySound("music",0,g_fvolum);
        if(g_cFindchar=='a')
        {
            dPlaySound("A.wav",0,g_fvolum);
        }
        if(g_cFindchar=='b')
        {
            dPlaySound("B.wav",0,g_fvolum);
        }
        if(g_cFindchar=='c')
        {
            dPlaySound("C.wav",0,g_fvolum);
        }
        if(g_cFindchar=='d')
        {
            dPlaySound("D.wav",0,g_fvolum);
        }
        if(g_cFindchar=='e')
        {
            dPlaySound("E.wav",0,g_fvolum);
        }
        if(g_cFindchar=='f')
        {
            dPlaySound("F.wav",0,g_fvolum);
        }
        if(g_cFindchar=='h')
        {
            dPlaySound("H.wav",0,g_fvolum);
        }
        if(g_cFindchar=='i')
        {
            dPlaySound("I.wav",0,g_fvolum);
        }
        if(g_cFindchar=='g')
        {
            dPlaySound("G.wav",0,g_fvolum);
        }
        if(g_cFindchar=='k')
        {
            dPlaySound("K.wav",0,g_fvolum);
        }
        if(g_cFindchar=='l')
        {
            dPlaySound("L.wav",0,g_fvolum);
        }
        if(g_cFindchar=='m')
        {
            dPlaySound("M.wav",0,g_fvolum);
        }
        if(g_cFindchar=='n')
        {
            dPlaySound("N.wav",0,g_fvolum);
        }
        if(g_cFindchar=='o')
        {
            dPlaySound("O.wav",0,g_fvolum);
        }
        if(g_cFindchar=='p')
        {
            dPlaySound("P.wav",0,g_fvolum);
        }
        if(g_cFindchar=='q')
        {
            dPlaySound("Q.wav",0,g_fvolum);
        }
        if(g_cFindchar=='r')
        {
            dPlaySound("R.wav",0,g_fvolum);
        }
        if(g_cFindchar=='s')
        {
            dPlaySound("S.wav",0,g_fvolum);
        }
        if(g_cFindchar=='t')
        {
            dPlaySound("T.wav",0,g_fvolum);
        }
        if(g_cFindchar=='u')
        {
            dPlaySound("U.wav",0,g_fvolum);
        }
        if(g_cFindchar=='v')
        {
            dPlaySound("V.wav",0,g_fvolum);
        }
        if(g_cFindchar=='w')
        {
            dPlaySound("W.wav",0,g_fvolum);
        }
        if(g_cFindchar=='x')
        {
            dPlaySound("X.wav",0,g_fvolum);
        }
        if(g_cFindchar=='y')
        {
            dPlaySound("Y.wav",0,g_fvolum);
        }
        if(g_cFindchar=='z')
        {
            dPlaySound("Z.wav",0,g_fvolum);
        }

        if(g_sWordE[g_iCharCnt+1]==0)
        {
            g_sCorE=g_sCorE+g_cFindchar;
            g_iWordState[g_iToLearn[g_iWordCnt]]=1;
            cout<<g_iWordCnt<<' '<<g_iToLearn[g_iWordCnt]<<endl;
            g_iWordCnt++;
            g_iGameWait=1;

            ofstream foutstate(g_sFWordState);
            if(!foutstate)cout<<"error!\n";
            else
            {
                foutstate<<0;
                for(int i=1;i<=g_iWordState[0];i++)
                    foutstate<<g_iWordState[i];
            }
            foutstate.close();
        }
        else
        {
            g_sCorE=g_sCorE+g_cFindchar;
            g_cFindchar=0;
            g_iCharCnt++;
        }
        dSetTextString("game1display",&g_sCorE[0]);
    }
}

void Game2Run( float fDeltaTime )
{
    dShowCursor(0);
    dSetTextString("game2chinese",&g_sWordList[g_iWordCnt][0]+g_iWordC);
    if(g_iGameWait==1)
    {
        g_fWaitTime-=fDeltaTime;
        if(g_fWaitTime>0.2f)
        {
            dSetTextChar("game2chinese",' ');
            g_iWordC=0;
        }
        else
        {
            g_iCharCnt++;
            g_iGameWait=0;
            g_fWaitTime=1;
            g_sCorE="";
            dSetTextString("game2english",&g_sCorE[0]);
            if(g_iWordCnt==g_iWordNum)
            {
                dPlaySound("success.wav",0,g_fvolum);
                dSetSpriteVisible("passgame2",1);
                g_sWordList[0][0]=0;
                g_sWordE[0]=0;
                dSetTextString("game2english",&g_sCorE[0]);
                dSetTextChar("game2chinese",' ');
            }
            else
            {
                int i;
                for(i=0;;i++)
                {
                    if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                        g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                    else
                    {
                        g_sWordE[i]=0;
                        break;
                    }
                }
                g_iWordC=i;
                g_iCharCnt=0;
            }
        }
    }
    if(g_iGameWait==0&&g_cFindchar==g_sWordE[g_iCharCnt])
    {
        dPlaySound("luo.wav",0,g_fvolum);
        if(g_sWordE[g_iCharCnt+1]==0)
        {
            g_iWordCnt++;
            g_iGameWait=1;
            g_sCorE=g_sCorE+g_cFindchar;
            dSetTextString("game2english",&g_sCorE[0]);
            dSetTextString("game2chinese","");
        }
        else
        {
            g_sCorE=g_sCorE+g_cFindchar;
            g_iCharCnt++;
        }
        dSetTextString("game2english",&g_sCorE[0]);
    }
}

void Game3Run( float fDeltaTime )
{
    dShowCursor(0);
    if(g_iGameWait==1)
    {
        dSetTextChar("game2chinese",' ');
        g_fWaitTime-=fDeltaTime;
        if(g_fWaitTime>0.2f)
        {
            g_iWordC=0;
        }
        else
        {
            g_iGameWait=0;
            g_fWaitTime=1;
            g_sCorE="";

            if(g_iWordCnt==g_iWordNum)
            {
                dPlaySound("success.wav",0,g_fvolum);
                dLoadMap("game4.t2d");
                dShowCursor(1);
                g_iGameState=4;
                g_iGameWait=0;

                g_iWordCnt=0;
                g_iCharCnt=0;
                int i;
                for(i=0;;i++)
                {
                    if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                        g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                    else
                    {
                        g_sWordE[i]=0;
                        break;
                    }
                }
                g_iWordC=i;
                dSetTextString("game4chinese",&g_sWordList[0][0]+g_iWordC);
                return;
            }

            int i;
            for(i=0;;i++)
            {
                if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                    g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                else
                {
                    g_sWordE[i]=0;
                    break;
                }
            }
            g_iWordC=i;
            g_iCharCnt=0;

            g_iCutChar=g_iWordC/3;
            memset(g_iCutRan,0,sizeof g_iCutRan);
            g_iCutRan[0]=1;
            g_cCutList[0]=g_sWordE[1];
            g_sWordE[1]=' ';
            for(int i=1;i<g_iCutChar;i++)
            {
                if(g_iCutRan[i-1]+1>=g_iWordC)
                {
                    g_iCutChar=i;
                    break;
                }
                int r=dRandomRange(g_iCutRan[i-1],g_iWordC-1);
                g_iCutRan[i]=r;
                g_cCutList[i]=g_sWordE[r];
                g_sWordE[r]=' ';
            }
            dSetTextString("game2english",&g_sWordE[0]);
            dSetTextString("game2chinese",&g_sWordList[g_iWordCnt][0]+g_iWordC);
        }
    }
    if(g_iGameWait==0&&g_cFindchar==g_cCutList[g_iCharCnt])
    {
        g_sWordE[g_iCutRan[g_iCharCnt]]=g_cFindchar;
        dSetTextString("game2english",&g_sWordE[0]);
        dPlaySound("luo.wav",0,g_fvolum);
        if(g_iCharCnt+1==g_iCutChar)
        {
            g_sWordE[0]=0;
            g_iWordCnt++;
            g_iGameWait=1;
        }
        else
        {
            g_iCharCnt++;
        }
        dSetTextString("game2chinese",&g_sWordList[g_iWordCnt][0]+g_iWordC);
    }
}

void Game4Run( float fDeltaTime )
{
    dShowCursor(1);
    if(g_iGameWait==1)
    {
        if(g_fWaitTime>0.2f)
        {
            g_fWaitTime-=fDeltaTime;
        }
        else
        {
            g_iGameWait=0;
            g_fWaitTime=1.0;
            if(g_iWordCnt==g_iWordNum)
            {
                dPlaySound("success.wav",0,g_fvolum);
                dSetSpriteVisible("passgame4",1);
                dSetTextChar("game4chinese",' ');
            }
            g_sCorE="";
            dSetTextChar("game4english",' ');

            int i;
            for(i=0;; i++)
            {
                if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                    g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                else
                {
                    g_sWordE[i]=0;
                    break;
                }
            }

            g_iWordC=i;
            g_iCharCnt=0;
            dSetTextString("game4chinese",&g_sWordList[g_iWordCnt][0]+g_iWordC);
         }
    }

    if(g_iGameWait==0&&g_cFindchar==g_sWordE[g_iCharCnt])
    {
        if(g_cFindchar=='a')
        {
            dPlaySound("A.wav",0,g_fvolum);
        }
        if(g_cFindchar=='b')
        {
            dPlaySound("B.wav",0,g_fvolum);
        }
        if(g_cFindchar=='c')
        {
            dPlaySound("C.wav",0,g_fvolum);
        }
        if(g_cFindchar=='d')
        {
            dPlaySound("D.wav",0,g_fvolum);
        }
        if(g_cFindchar=='e')
        {
            dPlaySound("E.wav",0,g_fvolum);
        }
        if(g_cFindchar=='f')
        {
            dPlaySound("F.wav",0,g_fvolum);
        }
        if(g_cFindchar=='h')
        {
            dPlaySound("H.wav",0,g_fvolum);
        }
        if(g_cFindchar=='i')
        {
            dPlaySound("I.wav",0,g_fvolum);
        }
        if(g_cFindchar=='g')
        {
            dPlaySound("G.wav",0,g_fvolum);
        }
        if(g_cFindchar=='k')
        {
            dPlaySound("K.wav",0,g_fvolum);
        }
        if(g_cFindchar=='l')
        {
            dPlaySound("L.wav",0,g_fvolum);
        }
        if(g_cFindchar=='m')
        {
            dPlaySound("M.wav",0,g_fvolum);
        }
        if(g_cFindchar=='n')
        {
            dPlaySound("N.wav",0,g_fvolum);
        }
        if(g_cFindchar=='o')
        {
            dPlaySound("O.wav",0,g_fvolum);
        }
        if(g_cFindchar=='p')
        {
            dPlaySound("P.wav",0,g_fvolum);
        }
        if(g_cFindchar=='q')
        {
            dPlaySound("Q.wav",0,g_fvolum);
        }
        if(g_cFindchar=='r')
        {
            dPlaySound("R.wav",0,g_fvolum);
        }
        if(g_cFindchar=='s')
        {
            dPlaySound("S.wav",0,g_fvolum);
        }
        if(g_cFindchar=='t')
        {
            dPlaySound("T.wav",0,g_fvolum);
        }
        if(g_cFindchar=='u')
        {
            dPlaySound("U.wav",0,g_fvolum);
        }
        if(g_cFindchar=='v')
        {
            dPlaySound("V.wav",0,g_fvolum);
        }
        if(g_cFindchar=='w')
        {
            dPlaySound("W.wav",0,g_fvolum);
        }
        if(g_cFindchar=='x')
        {
            dPlaySound("X.wav",0,g_fvolum);
        }
        if(g_cFindchar=='y')
        {
            dPlaySound("Y.wav",0,g_fvolum);
        }
        if(g_cFindchar=='z')
        {
            dPlaySound("Z.wav",0,g_fvolum);
        }
        if(g_iCharCnt+1==g_iWordC)
        {
            g_iWordCnt++;
            g_iGameWait=1;
            g_sCorE=g_sCorE+g_cFindchar;
        }
        else
        {
            g_sCorE=g_sCorE+g_cFindchar;
            g_cFindchar=0;
            g_iCharCnt++;
        }
        dSetTextString("game4english",&g_sCorE[0]);
    }
}
void Game5Run( float fDeltaTime )
{
}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{
}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove( const float fMouseX, const float fMouseY )
{
    if(g_iGameState==-1)
    {
        if(dIsPointInSprite("start",fMouseX,fMouseY))
        {
            dSetSpriteWidth("start",12);
            dSetSpriteHeight("start",10);
        }
        else
        {
            dSetSpriteWidth("start",8);
            dSetSpriteHeight("start",7);
        }
        if(dIsPointInSprite("review",fMouseX,fMouseY))
        {
            dSetSpriteWidth("review",18);
            dSetSpriteHeight("review",16);
        }
        else
        {
            dSetSpriteWidth("review",12);
            dSetSpriteHeight("review",11);
        }
        if(dIsPointInSprite("setting",fMouseX,fMouseY))
        {
            dSetSpriteWidth("setting",12);
            dSetSpriteHeight("setting",14);
        }
        else
        {
            dSetSpriteWidth("setting",9);
            dSetSpriteHeight("setting",10);
        }
    }
    if(g_iGameState==2||g_iGameState==3)
    {
        dSetSpritePosition("hand",fMouseX,fMouseY);
    }

    if(g_iGameState==5)
    {
        if(dIsPointInSprite("minus",fMouseX,fMouseY))
        {
            dSetSpriteWidth("minus",7);
            dSetSpriteHeight("minus",7);
        }
        else
        {
            dSetSpriteWidth("minus",4);
            dSetSpriteHeight("minus",4);
        }
        if(dIsPointInSprite("plus",fMouseX,fMouseY))
        {
            dSetSpriteWidth("plus",7);
            dSetSpriteHeight("plus",7);
        }
        else
        {
            dSetSpriteWidth("plus",4);
            dSetSpriteHeight("plus",4);
        }
        if(dIsPointInSprite("m1",fMouseX,fMouseY))
        {
            dSetSpriteWidth("m1",8);
            dSetSpriteHeight("m1",8);
        }
        else
        {
            dSetSpriteWidth("m1",5);
            dSetSpriteHeight("m1",5);
        }
        if(dIsPointInSprite("m2",fMouseX,fMouseY))
        {
            dSetSpriteWidth("m2",8);
            dSetSpriteHeight("m2",8);
        }
        else
        {
            dSetSpriteWidth("m2",5);
            dSetSpriteHeight("m2",5);
        }
        if(dIsPointInSprite("m3",fMouseX,fMouseY))
        {
            dSetSpriteWidth("m3",8);
            dSetSpriteHeight("m3",8);
        }
        else
        {
            dSetSpriteWidth("m3",5);
            dSetSpriteHeight("m3",5);
        }
        if(dIsPointInSprite("clear",fMouseX,fMouseY))
        {
            dSetSpriteWidth("clear",8);
            dSetSpriteHeight("clear",8);
        }
        else
        {
            dSetSpriteWidth("clear",5);
            dSetSpriteHeight("clear",5);
        }
        if(dIsPointInSprite("CET4",fMouseX,fMouseY))
        {
            dSetSpriteWidth("CET4",12);
            dSetSpriteHeight("CET4",10);
        }
        else
        {
            dSetSpriteWidth("CET4",10);
            dSetSpriteHeight("CET4",8);
        }
        if(dIsPointInSprite("CET6",fMouseX,fMouseY))
        {
            dSetSpriteWidth("CET6",12);
            dSetSpriteHeight("CET6",10);
        }
        else
        {
            dSetSpriteWidth("CET6",10);
            dSetSpriteHeight("CET6",8);
        }
        if(dIsPointInSprite("TOEFL",fMouseX,fMouseY))
        {
            dSetSpriteWidth("TOEFL",12);
            dSetSpriteHeight("TOEFL",10);
        }
        else
        {
            dSetSpriteWidth("TOEFL",10);
            dSetSpriteHeight("TOEFL",8);
        }
    }

}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
    if(g_iGameState==-1)
    {
        if(dIsPointInSprite("start",fMouseX,fMouseY))
        {
            dPlaySound("butter.wav",0,g_fvolum);
            g_iGameState=1;
            dLoadMap("game1.t2d");
            dSetSpriteAngularVelocity("map",-6);
            if(g_iWordRemain<g_iWordNum)g_iWordNum=g_iWordRemain;
            ifstream finstate(g_sFWordState);
            if(!finstate)cout<<"error!\n";
            else
            {
                for(int i=0;i<g_iWordNum;i++)
                {
                    int r=dRandomRange(1,g_iWordState[0]);
                    if(g_iWordState[r]==1)i--;
                    else
                    {
                        //g_iWordState[r]=1;
                        ifstream finword(g_sFWordList);
                        if(!finword)cout<<"error!\n";
                        else
                        {
                            string s;
                            for(int j=0;j<r;j++)
                            {
                                getline(finword,s);
                            }
                            g_sWordList[i]=s;
                            finword.close();
                            cout<<s<<endl;
                            g_iToLearn[i]=r;
                        }
                    }
                }
            }
            finstate.close();

            g_iWordCnt=0;
            int i;
            for(i=0;;i++)
            {
                if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                    g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                else
                {
                    g_sWordE[i]=0;
                    break;
                }
            }
            g_iWordC=i;
            g_iCharCnt=0;
        }

        if(dIsPointInSprite("review",fMouseX,fMouseY))
        {
            dPlaySound("butter.wav",0,g_fvolum);
            g_iGameState=3;
            dLoadMap("game2.t2d");
            if(g_iWordAlready<g_iWordNum)g_iWordNum=g_iWordAlready;
            for(int i=0;i<g_iWordNum;i++)
            {
                int r=dRandomRange(1,g_iWordState[0]);
                r=abs((int(r*g_fRandomSeed))%(g_iWordAlready+g_iWordRemain));
                if(r==0)r=1;

                if(g_iWordState[r]==0)i--;
                else
                {
                    g_iWordState[r]=1;
                    ifstream finword(g_sFWordList);
                    if(!finword)cout<<"error!\n";
                    else
                    {
                        string s;
                        for(int j=0;j<r;j++)
                        {
                            getline(finword,s);
                        }
                        g_sWordList[i]=s;
                        finword.close();
                        cout<<s<<endl;
                    }
                }
            }
            g_iWordCnt=0;
            int i;
            for(i=0;;i++)
            {
                if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                    g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                else
                {
                    g_sWordE[i]=0;
                    break;
                }
            }
            g_iWordC=i;

            g_iCharCnt=0;


            g_iCutChar=g_iWordC/3;
            memset(g_iCutRan,0,sizeof g_iCutRan);
            g_iCutRan[0]=1;
            g_cCutList[0]=g_sWordE[1];
            g_sWordE[1]=' ';
            for(int i=1;i<g_iCutChar;i++)
            {
                if(i>0&&g_iCutRan[i-1]+1>=g_iWordC)
                {
                    g_iCutChar=i;
                    break;
                }
                int r=dRandomRange(g_iCutRan[i-1],g_iWordC-1);
                g_iCutRan[i]=r;
                g_cCutList[i]=g_sWordE[r];
                g_sWordE[r]=' ';
            }

            dSetTextString("game2chinese",&g_sWordList[0][0]+g_iWordC);
            dSetTextString("game2english",&g_sWordE[0]);
        }

        if(dIsPointInSprite("setting",fMouseX,fMouseY))
        {
            dPlaySound("butter.wav",0,g_fvolum);
            g_iGameState=5;
            dLoadMap("setting.t2d");
            dSetTextValue("wordnum",g_iWordNum);
        }
    }
    if(g_iGameState==1)
    {
        if(dIsPointInSprite("game1back",fMouseX,fMouseY))
            g_iGameState=0;
        if(dIsPointInSprite("game1next",fMouseX,fMouseY))
        {
            g_iGameState=2;
            dLoadMap("game2.t2d");
            g_iWordCnt=0;

            int i;
            for(i=0;;i++)
            {
                if(g_sWordList[g_iWordCnt][i]>='a'&&g_sWordList[g_iWordCnt][i]<='z')
                    g_sWordE[i]=g_sWordList[g_iWordCnt][i];
                else
                {
                    g_sWordE[i]=0;
                    break;
                }
            }
            g_iWordC=i;
            g_iCharCnt=0;
        }
    }
    if(g_iGameState==2||g_iGameState==3)
    {
        if(dIsPointInSprite("game2back",fMouseX,fMouseY))
            g_iGameState=0;
        if(dIsPointInSprite(g_sTouch,fMouseX,fMouseY))
        {
            dCloneSprite(g_sTouch,"clone");
            dSetSpriteLinearVelocity("clone",0,5);
            g_cFindchar=g_sTouch[0];
        }
    }
    if(g_iGameState==4)
    {
        if(dIsPointInSprite("game4back",fMouseX,fMouseY))
            g_iGameState=0;
    }
    if(g_iGameState==5)
    {
        if(dIsPointInSprite("settingback",fMouseX,fMouseY))
        {
            g_iGameState=0;

            ofstream foutinfo(g_sFWordInfo);
            if(!foutinfo)cout<<"error!";
            else
            {
                foutinfo<<g_iWordNum<<endl;
                foutinfo<<g_fvolum<<endl;
                foutinfo<<g_iWordListNum<<endl;
            }
            foutinfo.close();
        }

        if(dIsPointInSprite("plus",fMouseX,fMouseY))
            g_iWordNum++;
        if(dIsPointInSprite("minus",fMouseX,fMouseY))
            g_iWordNum--;
        if(dIsPointInSprite("m1",fMouseX,fMouseY))
        {
            g_fvolum=1;
            dStopAllSound();
            dPlaySound("butter.wav",0,g_fvolum);
        }

        if(dIsPointInSprite("m2",fMouseX,fMouseY))
        {
            g_fvolum=0.6;
            dStopAllSound();
            dPlaySound("butter.wav",0,g_fvolum);
        }
        if(dIsPointInSprite("m3",fMouseX,fMouseY))
        {
            g_fvolum=0;
            dStopAllSound();
            dPlaySound("butter.wav",0,0.3);
        }
        dSetTextValue("wordnum",g_iWordNum);

        if(dIsPointInSprite("clear",fMouseX,fMouseY))
        {
            ofstream fout(g_sFWordState);
            if(!fout)cout<<"error!";
            else
            {
                for(int i=0;i<=g_iWordState[0];i++)
                    fout<<'0';
            }
            fout.close();
        }
        if(dIsPointInSprite("TOEFL",fMouseX,fMouseY))
        {
            strcpy(g_sFWordList,"TOEFL570.txt");
            strcpy(g_sFWordState,"wordstate570.txt");
            g_iWordListNum=1;
        }
        if(dIsPointInSprite("CET4",fMouseX,fMouseY))
        {
            strcpy(g_sFWordList,"CET4.txt");
            strcpy(g_sFWordState,"wordstate3371.txt");
            g_iWordListNum=2;
        }
        if(dIsPointInSprite("CET6",fMouseX,fMouseY))
        {
            strcpy(g_sFWordList,"CET6.txt");
            strcpy(g_sFWordState,"wordstate2076.txt");
            g_iWordListNum=3;
        }
    }
}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{
    if(g_iGameState==1)
    {
        if(KEY_SPACE==iKey)
        {
            //确定
            g_cFindchar=g_cTouchChar;
            g_iGameWait=0;
            g_fWaitTime=2.f;
        }
        if(KEY_DOWN==iKey)
        {
            dSetSpritePosition("boy1",-7,15);
        }
        if(KEY_UP==iKey)
        {
            dSetSpritePosition("boy1",-7,-16);
        }
        if(KEY_LEFT==iKey)
        {
            float f=dGetSpriteRotation("map");
            dSetSpriteRotation("map",f+25);
        }
        if(KEY_RIGHT==iKey)
        {
            float f=dGetSpriteRotation("map");
            dSetSpriteRotation("map",f-15);
        }
    }
    if(g_iGameState==4)
    {
        if(KEY_A==iKey)
        {
            g_cFindchar='a';
        }
        if(KEY_B==iKey)
        {
            g_cFindchar='b';
        }
        if(KEY_C==iKey)
        {
            g_cFindchar='c';
        }
        if(KEY_D==iKey)
        {
            g_cFindchar='d';
        }
        if(KEY_E==iKey)
        {
            g_cFindchar='e';
        }
        if(KEY_F==iKey)
        {
            g_cFindchar='f';
        }
        if(KEY_G==iKey)
        {
            g_cFindchar='g';
        }
        if(KEY_H==iKey)
        {
            g_cFindchar='h';
        }
        if(KEY_I==iKey)
        {
            g_cFindchar='i';
        }
        if(KEY_J==iKey)
        {
            g_cFindchar='j';
        }
        if(KEY_K==iKey)
        {
            g_cFindchar='k';
        }
        if(KEY_L==iKey)
        {
            g_cFindchar='l';
        }
        if(KEY_M==iKey)
        {
            g_cFindchar='m';
        }
        if(KEY_N==iKey)
        {
            g_cFindchar='n';
        }
        if(KEY_O==iKey)
        {
            g_cFindchar='o';
        }
        if(KEY_P==iKey)
        {
            g_cFindchar='p';
        }
        if(KEY_Q==iKey)
        {
            g_cFindchar='q';
        }
        if(KEY_R==iKey)
        {
            g_cFindchar='r';
        }
        if(KEY_S==iKey)
        {
            g_cFindchar='s';
        }
        if(KEY_T==iKey)
        {
            g_cFindchar='t';
        }
        if(KEY_U==iKey)
        {
            g_cFindchar='u';
        }
        if(KEY_V==iKey)
        {
            g_cFindchar='v';
        }
        if(KEY_W==iKey)
        {
            g_cFindchar='w';
        }
        if(KEY_X==iKey)
        {
            g_cFindchar='x';
        }
        if(KEY_Y==iKey)
        {
            g_cFindchar='y';
        }
        if(KEY_Z==iKey)
        {
            g_cFindchar='z';
        }
    }
}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{
}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    if(g_iGameState==2||g_iGameState==3)
    {
        if(strcmp(szSrcName,"hand")==0)strcpy(g_sTouch,szTarName);
        else strcpy(g_sTouch,szSrcName);
    }
    if(g_iGameState==1)
        g_cTouchChar=szTarName[0];
}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
}
