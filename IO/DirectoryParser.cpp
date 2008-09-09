/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.

   
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

/**
  \file    DirectoryParser.cpp
  \author    Jens Krueger
        SCI Institute
        University of Utah
  \version  1.2
  \date    July 2008
*/

#include "DirectoryParser.h"

#include <sys/stat.h>
#include <Basics/SysTools.h>
#include <algorithm>
#include <memory.h>


using namespace std;

DirectoryParser::DirectoryParser(void)
{
}

DirectoryParser::~DirectoryParser(void)
{
  for (unsigned int i = 0; i<m_FileStacks.size(); i++) delete m_FileStacks[i];
  m_FileStacks.clear();
}

/*************************************************************************************/

SimpleFileInfo::SimpleFileInfo(const std::string& strFileName) :   
  m_strFileName(strFileName),
  m_iImageIndex(0),
  m_iOffsetToData(0),
  m_iDataSize(0)
{
  m_wstrFileName = wstring(strFileName.begin(),strFileName.end());  
}

SimpleFileInfo::SimpleFileInfo(const std::wstring& wstrFileName) :   
  m_wstrFileName(wstrFileName),
  m_iImageIndex(0),
  m_iOffsetToData(0),
  m_iDataSize(0)
{
  m_strFileName = string(wstrFileName.begin(),wstrFileName.end());  
}

SimpleFileInfo::SimpleFileInfo() :   
  m_strFileName(""),
  m_wstrFileName(L""),
  m_iImageIndex(0),
  m_iOffsetToData(0),
  m_iDataSize(0)
{}

SimpleFileInfo::SimpleFileInfo(const SimpleFileInfo* other) :
  m_strFileName(other->m_strFileName),
  m_wstrFileName(other->m_wstrFileName),
  m_iImageIndex(other->m_iImageIndex),
  m_iOffsetToData(other->m_iOffsetToData),
  m_iDataSize(other->m_iDataSize)
{
}

bool SimpleFileInfo::GetData(void* pData, unsigned int iLength, unsigned int iOffset) {
  ifstream fs;
  fs.open(m_strFileName.c_str(),fstream::binary);
  if (fs.fail()) return false;

  fs.seekg(m_iOffsetToData+iOffset, ios_base::cur);
  fs.read((char*)pData, iLength);

  fs.close();
  return true;
}

bool SimpleFileInfo::GetData(void** pData) {
  if (*pData == NULL) *pData = (void*)new char[GetDataSize()];

  memset(*pData, 0, GetDataSize());
  return GetData(*pData, GetDataSize(), 0);
}


/*************************************************************************************/

FileStackInfo::FileStackInfo() :
  m_ivSize(0,0,0),
  m_fvfAspect(1,1,1),
  m_iAllocated(0),
  m_iStored(0),
  m_iComponentCount(1),
  m_bIsBigEndian(false)
{}

FileStackInfo::FileStackInfo(const FileStackInfo* other) :
  m_ivSize(other->m_ivSize),
  m_fvfAspect(other->m_fvfAspect),
  m_iAllocated(other->m_iAllocated),
  m_iStored(other->m_iStored),
  m_iComponentCount(other->m_iComponentCount),
  m_bIsBigEndian(other->m_bIsBigEndian)
{
  for (unsigned int i=0;i<other->m_Elements.size();i++) {
    SimpleFileInfo* e = new SimpleFileInfo(other->m_Elements[i]);
    m_Elements.push_back(e);
  }
}

FileStackInfo::~FileStackInfo() {
  for (unsigned int i=0;i<m_Elements.size();i++) delete m_Elements[i];
}

FileStackInfo::FileStackInfo( UINTVECTOR3  ivSize, FLOATVECTOR3 fvfAspect, unsigned int iAllocated, unsigned int iStored,
                              unsigned int iComponentCount, bool bIsBigEndian, const std::string& strDesc) :
	m_ivSize(ivSize),
	m_fvfAspect(fvfAspect),
	m_iAllocated(iAllocated),
	m_iStored(iStored),
	m_iComponentCount(iComponentCount),
	m_bIsBigEndian(bIsBigEndian),
	m_strDesc(strDesc)
{
}