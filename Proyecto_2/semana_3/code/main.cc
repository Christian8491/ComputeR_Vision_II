// Copyright 2008 Isis Innovation Limited
// This is the main extry point for PTAM
#include <stdlib.h>
#include <iostream>
#include <gvars3/instances.h>
#include "System.h"
#include "ucsp.h"


using namespace std;
using namespace GVars3;

model rabbit;

bool  flagRabbit=0;
bool  flagHorse=0;
bool  flagDragon=1;
bool  flagDolphin=0;
bool  flagWoola=0;
bool  flagUCSP=0;

int main()
{
  cout << "  Welcome to PTAM " << endl;
  cout << "  --------------- " << endl;
  cout << "  Parallel tracking and mapping for Small AR workspaces" << endl;
  cout << "  Copyright (C) Isis Innovation Limited 2008 " << endl;  
  cout << endl;
  cout << "  Parsing settings.cfg ...." << endl;
  GUI.LoadFile("settings.cfg");
  readObj();
  GUI.StartParserThread(); // Start parsing of the console input
  atexit(GUI.StopParserThread); 
  
  try
    {
      System s;
      s.Run();
    }
  catch(CVD::Exceptions::All e)
    {
      cout << endl;
      cout << "!! Failed to run system; got exception. " << endl;
      cout << "   Exception was: " << endl;
      cout << e.what << endl;
    }
  //return 0;
}