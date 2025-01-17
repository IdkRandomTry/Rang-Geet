#include <Windows.h>
#include <string>
#include <iostream>

// From Raylib
extern "C" {
  void *GetWindowHandle(void);
}

std::string OpenFileDialog(const char* filter) {
  OPENFILENAME ofn = {0}; 
  TCHAR szFile[260]={0};
  // Initialize remaining fields of OPENFILENAME structure
  ofn.lStructSize = sizeof(ofn); 
  ofn.hwndOwner = (HWND) GetWindowHandle();
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile); 
  ofn.lpstrFilter = filter; 
  ofn.nFilterIndex = 1; 
  ofn.lpstrFileTitle = NULL; 
  ofn.nMaxFileTitle = 0; 
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  
  if(GetOpenFileName(&ofn) == TRUE)
  { 
    std::cout << ofn.lpstrFile << std::endl;
    return std::string(ofn.lpstrFile);
  }
  return "";
}