/*
Copyright (c) 2004-2007, John Hurst
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
  /*! \file    KM_fileio.cpp
    \version $Id: KM_fileio.cpp,v 1.13 2007/10/25 18:44:45 msheby Exp $
    \brief   portable file i/o
  */

#include <KM_fileio.h>
#include <KM_log.h>
#include <fcntl.h>
#include <assert.h>
#ifdef KM_WIN32
#include <direct.h>
#endif

using namespace Kumu;

#ifdef KM_WIN32
typedef struct _stati64 fstat_t;
#define S_IFLNK 0


// win32 has WriteFileGather() and ReadFileScatter() but they
// demand page alignment and page sizing, making them unsuitable
// for use with arbitrary buffer sizes.
struct iovec {
  char* iov_base; // stupid iovec uses char*
  int   iov_len;
};
#else
#include <sys/uio.h>
typedef struct stat     fstat_t;
#endif

//
static void
split(const std::string& str, char separator, std::list<std::string>& components)
{
  const char* pstr = str.c_str();
  const char* r = strchr(pstr, separator);

  while ( r != 0 )
    {
      assert(r >= pstr);
      if ( r > pstr )
	{
	  std::string tmp_str;
	  assert(r - pstr < 100);
	  tmp_str.assign(pstr, (r - pstr));
	  components.push_back(tmp_str);
	}

      pstr = r + 1;
      r = strchr(pstr, separator);
    }

  if( strlen(pstr) > 0 )
    components.push_back(std::string(pstr));
}


//
static Kumu::Result_t
do_stat(const char* path, fstat_t* stat_info)
{
  KM_TEST_NULL_STR_L(path);
  KM_TEST_NULL_L(stat_info);

  Kumu::Result_t result = Kumu::RESULT_OK;

#ifdef KM_WIN32
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);

  if ( _stati64(path, stat_info) == (__int64)-1 )
    result = Kumu::RESULT_FILEOPEN;

  ::SetErrorMode( prev );
#else
  if ( stat(path, stat_info) == -1L )
    result = Kumu::RESULT_FILEOPEN;

  if ( stat_info->st_mode & (S_IFREG|S_IFLNK|S_IFDIR) == 0 )
    result = Kumu::RESULT_FILEOPEN;
#endif

  return result;
}

#ifndef KM_WIN32

//
static Kumu::Result_t
do_fstat(HANDLE handle, fstat_t* stat_info)
{
  KM_TEST_NULL_L(stat_info);

  Kumu::Result_t result = Kumu::RESULT_OK;

  if ( fstat(handle, stat_info) == -1L )
    result = Kumu::RESULT_FILEOPEN;

  if ( stat_info->st_mode & (S_IFREG|S_IFLNK|S_IFDIR) == 0 )
    result = Kumu::RESULT_FILEOPEN;

  return result;
}

#endif


//
bool
Kumu::PathExists(const std::string& pathname)
{
  if ( pathname.empty() )
    return false;

  fstat_t info;

  if ( KM_SUCCESS(do_stat(pathname.c_str(), &info)) )
    return true;

  return false;
}

//
bool
Kumu::PathIsFile(const std::string& pathname)
{
  if ( pathname.empty() )
    return false;

  fstat_t info;

  if ( KM_SUCCESS(do_stat(pathname.c_str(), &info)) )
    {
      if ( info.st_mode & ( S_IFREG|S_IFLNK ) )
        return true;
    }

  return false;
}


//
bool
Kumu::PathIsDirectory(const std::string& pathname)
{
  if ( pathname.empty() )
    return false;

  fstat_t info;

  if ( KM_SUCCESS(do_stat(pathname.c_str(), &info)) )
    {
      if ( info.st_mode & S_IFDIR )
        return true;
    }

  return false;
}

//
Kumu::fsize_t
Kumu::FileSize(const std::string& pathname)
{
  if ( pathname.empty() )
    return 0;

  fstat_t info;

  if ( KM_SUCCESS(do_stat(pathname.c_str(), &info)) )
    {
      if ( info.st_mode & ( S_IFREG|S_IFLNK ) )
        return(info.st_size);
    }

  return 0;
}

//
static PathCompList_t&
s_PathMakeCanonical(PathCompList_t& CList, char separator, bool is_absolute)
{
  PathCompList_t::iterator ci, ri; // component and removal iterators

  for ( ci = CList.begin(); ci != CList.end(); ci++ )
    {
      if ( *ci == "." && ( CList.size() > 1 || is_absolute ) )
        {
          ri = ci++;
          CList.erase(ri);
        }
      else if ( *ci == ".." && ci != CList.begin() )
	{
	  ri = ci;
	  ri--;
	      
	  if ( *ri != ".." )
	    {
	      CList.erase(ri);
	      ri = ci++;
	      CList.erase(ri);
            }
        }
    }

  return CList;
}

//
std::string
Kumu::PathMakeCanonical(const std::string& Path, char separator)
{
  PathCompList_t CList;
  bool is_absolute = PathIsAbsolute(Path, separator);
  s_PathMakeCanonical(PathToComponents(Path, CList, separator), separator, is_absolute);

  if ( is_absolute )
    return ComponentsToAbsolutePath(CList, separator);

  return ComponentsToPath(CList, separator);
}

//
bool
Kumu::PathsAreEquivalent(const std::string& lhs, const std::string& rhs)
{
  return PathMakeCanonical(lhs) == PathMakeCanonical(rhs);
}

//
Kumu::PathCompList_t&
Kumu::PathToComponents(const std::string& Path, PathCompList_t& CList, char separator)
{
  split(Path, separator, CList);
  return CList;
}

//
std::string
Kumu::ComponentsToPath(const PathCompList_t& CList, char separator)
{
  if ( CList.empty() )
    return "";

  PathCompList_t::const_iterator ci = CList.begin();
  std::string out_path = *ci;

  for ( ci++; ci != CList.end(); ci++ )
    out_path += separator + *ci;

  return out_path;
}

//
std::string
Kumu::ComponentsToAbsolutePath(const PathCompList_t& CList, char separator)
{
  std::string out_path;

  if ( CList.empty() )
    out_path = separator;
  else
    {
      PathCompList_t::const_iterator ci;

      for ( ci = CList.begin(); ci != CList.end(); ci++ )
	out_path += separator + *ci;
    }

  return out_path;
}

//
bool
Kumu::PathHasComponents(const std::string& Path, char separator)
{
  if ( strchr(Path.c_str(), separator) == 0 )
    return false;

  return true;
}

//
bool
Kumu::PathIsAbsolute(const std::string& Path, char separator)
{
  if ( Path.empty() )
    return false;

  if ( Path[0] == separator)
    return true;

  return false;
}

//
std::string
Kumu::PathMakeAbsolute(const std::string& Path, char separator)
{
  if ( Path.empty() )
    {
      std::string out_path;
      out_path = separator;
      return out_path;
    }

  if ( PathIsAbsolute(Path, separator) )
    return Path;

  char cwd_buf [MaxFilePath];
  if ( getcwd(cwd_buf, MaxFilePath) == 0 )
    {
      DefaultLogSink().Error("Error retrieving current working directory.");
      return "";
    }

  PathCompList_t CList;
  CList.push_back(cwd_buf);
  CList.push_back(Path);

  return ComponentsToAbsolutePath(s_PathMakeCanonical(CList, separator, true), separator);
}

//
std::string
Kumu::PathMakeLocal(const std::string& Path, const std::string& Parent)
{
  size_t pos = Path.find(Parent);

  if ( pos == 0 ) // Parent found at offset 0
    return Path.substr(Parent.size()+1);

  return Path;
}

//
std::string
Kumu::PathBasename(const std::string& Path, char separator)
{
  PathCompList_t CList;
  PathToComponents(Path, CList, separator);

  if ( CList.empty() )
    return "";

  return CList.back();
}

//
std::string
Kumu::PathDirname(const std::string& Path, char separator)
{
  PathCompList_t CList;
  bool is_absolute = PathIsAbsolute(Path, separator);
  PathToComponents(Path, CList, separator);

  if ( CList.empty() )
    return is_absolute ? "/" : "";

  CList.pop_back();

  if ( is_absolute )
    return ComponentsToAbsolutePath(CList, separator);

  return ComponentsToPath(CList, separator);
}

//
std::string
Kumu::PathGetExtension(const std::string& Path)
{
  std::string Basename = PathBasename(Path);
  const char* p = strrchr(Basename.c_str(), '.'); 

  if ( p++ == 0 )
    return "";

  return p;
}

//
std::string
Kumu::PathSetExtension(const std::string& Path, const std::string& Extension) // empty extension removes
{
  std::string Basename = PathBasename(Path);
  const char* p = strrchr(Basename.c_str(), '.'); 

  if ( p != 0 )
    Basename = Basename.substr(0, p - Basename.c_str());

  if ( Extension.empty() )
    return Basename;

  return Basename + "." + Extension;
}

//
Kumu::PathList_t&
Kumu::FindInPaths(const IPathMatch& Pattern, const Kumu::PathList_t& SearchPaths,
		  Kumu::PathList_t& FoundPaths, bool one_shot, char separator)
{
  PathList_t::const_iterator si;
  for ( si = SearchPaths.begin(); si != SearchPaths.end(); si++ )
    {
      FindInPath(Pattern, *si, FoundPaths, one_shot, separator);

      if ( one_shot && ! FoundPaths.empty() )
	break;
    }

  return FoundPaths;
}

//
Kumu::PathList_t&
Kumu::FindInPath(const IPathMatch& Pattern, const std::string& SearchDir,
		  Kumu::PathList_t& FoundPaths, bool one_shot, char separator)
{
  char name_buf[MaxFilePath];
  DirScanner Dir;

  if ( KM_SUCCESS(Dir.Open(SearchDir.c_str())) )
    {
      while ( KM_SUCCESS(Dir.GetNext(name_buf)) )
	{
	  if ( name_buf[0] == '.' ) continue; // no hidden files
	  std::string tmp_path = SearchDir + separator + name_buf;

	  if ( PathIsDirectory(tmp_path.c_str()) )
	    FindInPath(Pattern, tmp_path, FoundPaths, one_shot, separator);
	  
	  else if ( Pattern.Match(name_buf) )
	    {
	      FoundPaths.push_back(SearchDir + separator + name_buf);
	      if ( one_shot )
		break;
	    }
	}
    }

  return FoundPaths;
}


#ifndef KM_WIN32

//
Kumu::PathMatchRegex::PathMatchRegex(const std::string& s)
{
  int result = regcomp(&m_regex, s.c_str(), REG_NOSUB); // (REG_EXTENDED|REG_NOSUB|REG_NEWLINE));

  if ( result )
    {
      char buf[128];
      regerror(result, &m_regex, buf, 128);
      DefaultLogSink().Error("PathMatchRegex: %s\n", buf);
      regfree(&m_regex);
    }
}

Kumu::PathMatchRegex::PathMatchRegex(const PathMatchRegex& rhs) {
  m_regex = rhs.m_regex;
}

Kumu::PathMatchRegex::~PathMatchRegex() {
  regfree(&m_regex);
}

bool
Kumu::PathMatchRegex::Match(const std::string& s) const {
  return ( regexec(&m_regex, s.c_str(), 0, 0, 0) == 0 );
}



//
Kumu::PathMatchGlob::PathMatchGlob(const std::string& glob)
{
  std::string regex; // convert glob to regex

  for ( const char* p = glob.c_str(); *p != 0; p++ )
    {
      switch (*p)
	{
	case '.':  regex += "\\.";  break;
	case '*':  regex += ".*";   break;
	case '?':  regex += ".?";   break;
	default:   regex += *p;
	}
    }
  regex += '$';

  int result = regcomp(&m_regex, regex.c_str(), REG_NOSUB);

  if ( result )
    {
      char buf[128];
      regerror(result, &m_regex, buf, 128);
      DefaultLogSink().Error("PathMatchRegex: %s\n", buf);
      regfree(&m_regex);
    }
}

Kumu::PathMatchGlob::PathMatchGlob(const PathMatchGlob& rhs) {
  m_regex = rhs.m_regex;
}

Kumu::PathMatchGlob::~PathMatchGlob() {
  regfree(&m_regex);
}

bool
Kumu::PathMatchGlob::Match(const std::string& s) const {
  return ( regexec(&m_regex, s.c_str(), 0, 0, 0) == 0 );
}

#endif

//------------------------------------------------------------------------------------------
// portable aspects of the file classes

const int IOVecMaxEntries = 32; // we never use more that 3, but that number seems somehow small...

//
class Kumu::FileWriter::h__iovec
{
public:
  int            m_Count;
  struct iovec   m_iovec[IOVecMaxEntries];
  h__iovec() : m_Count(0) {}
};



//
Kumu::fsize_t
Kumu::FileReader::Size() const
{
#ifdef KM_WIN32
  return FileSize(m_Filename.c_str());
#else
  fstat_t info;

  if ( KM_SUCCESS(do_fstat(m_Handle, &info)) )
    {
      if ( info.st_mode & ( S_IFREG|S_IFLNK ) )
        return(info.st_size);
    }
#endif

  return 0;
}

// these are declared here instead of in the header file
// because we have a mem_ptr that is managing a hidden class
Kumu::FileWriter::FileWriter() 
{
}

Kumu::FileWriter::~FileWriter() {}

//
Kumu::Result_t
Kumu::FileWriter::Writev(const byte_t* buf, ui32_t buf_len)
{
  assert( ! m_IOVec.empty() );
  register h__iovec* iov = m_IOVec;
  KM_TEST_NULL_L(buf);

  if ( iov->m_Count >= IOVecMaxEntries )
    {
      DefaultLogSink().Error("The iovec is full! Only %u entries allowed before a flush.\n",
			     IOVecMaxEntries);
      return RESULT_WRITEFAIL;
    }

  iov->m_iovec[iov->m_Count].iov_base = (char*)buf; // stupid iovec uses char*
  iov->m_iovec[iov->m_Count].iov_len = buf_len;
  iov->m_Count++;

  return RESULT_OK;
}


#ifdef KM_WIN32
//------------------------------------------------------------------------------------------
//

Kumu::Result_t
Kumu::FileReader::OpenRead(const char* filename) const
{
  KM_TEST_NULL_STR_L(filename);
  const_cast<FileReader*>(this)->m_Filename = filename;
  
  // suppress popup window on error
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);

  const_cast<FileReader*>(this)->m_Handle = ::CreateFile(filename,
			  (GENERIC_READ),                // open for reading
			  FILE_SHARE_READ,               // share for reading
			  NULL,                          // no security
			  OPEN_EXISTING,                 // read
			  FILE_ATTRIBUTE_NORMAL,         // normal file
			  NULL                           // no template file
			  );

  ::SetErrorMode(prev);

  return ( m_Handle == INVALID_HANDLE_VALUE ) ?
    Kumu::RESULT_FILEOPEN : Kumu::RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Close() const
{
  if ( m_Handle == INVALID_HANDLE_VALUE )
    return Kumu::RESULT_FILEOPEN;

  // suppress popup window on error
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
  BOOL result = ::CloseHandle(m_Handle);
  ::SetErrorMode(prev);
  const_cast<FileReader*>(this)->m_Handle = INVALID_HANDLE_VALUE;

  return ( result == 0 ) ? Kumu::RESULT_FAIL : Kumu::RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Seek(Kumu::fpos_t position, SeekPos_t whence) const
{
  if ( m_Handle == INVALID_HANDLE_VALUE )
    return Kumu::RESULT_STATE;

  LARGE_INTEGER in;
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
  in.QuadPart = position;
  in.LowPart = ::SetFilePointer(m_Handle, in.LowPart, &in.HighPart, whence);
  HRESULT LastError = GetLastError();
  ::SetErrorMode(prev);

  if ( (LastError != NO_ERROR
	&& (in.LowPart == INVALID_SET_FILE_POINTER
	    || in.LowPart == ERROR_NEGATIVE_SEEK )) )
    return Kumu::RESULT_READFAIL;
  
  return Kumu::RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Tell(Kumu::fpos_t* pos) const
{
  KM_TEST_NULL_L(pos);

  if ( m_Handle == (HANDLE)-1L )
    return Kumu::RESULT_FILEOPEN;

  LARGE_INTEGER in;
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
  in.QuadPart = (__int64)0;
  in.LowPart = ::SetFilePointer(m_Handle, in.LowPart, &in.HighPart, FILE_CURRENT);
  HRESULT LastError = GetLastError();
  ::SetErrorMode(prev);

  if ( (LastError != NO_ERROR
	&& (in.LowPart == INVALID_SET_FILE_POINTER
	    || in.LowPart == ERROR_NEGATIVE_SEEK )) )
    return Kumu::RESULT_READFAIL;

  *pos = (Kumu::fpos_t)in.QuadPart;
  return Kumu::RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Read(byte_t* buf, ui32_t buf_len, ui32_t* read_count) const
{
  KM_TEST_NULL_L(buf);
  Result_t result = Kumu::RESULT_OK;
  DWORD    tmp_count;
  ui32_t tmp_int;

  if ( read_count == 0 )
    read_count = &tmp_int;

  *read_count = 0;

  if ( m_Handle == INVALID_HANDLE_VALUE )
    return Kumu::RESULT_FILEOPEN;
  
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
  if ( ::ReadFile(m_Handle, buf, buf_len, &tmp_count, NULL) == 0 )
    result = Kumu::RESULT_READFAIL;

  ::SetErrorMode(prev);

  if ( tmp_count == 0 ) /* EOF */
    result = Kumu::RESULT_ENDOFFILE;

  if ( KM_SUCCESS(result) )
    *read_count = tmp_count;

  return result;
}



//------------------------------------------------------------------------------------------
//

//
Kumu::Result_t
Kumu::FileWriter::OpenWrite(const char* filename)
{
  KM_TEST_NULL_STR_L(filename);
  m_Filename = filename;
  
  // suppress popup window on error
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);

  m_Handle = ::CreateFile(filename,
			  (GENERIC_WRITE|GENERIC_READ),  // open for reading
			  FILE_SHARE_READ,               // share for reading
			  NULL,                          // no security
			  CREATE_ALWAYS,                 // overwrite (beware!)
			  FILE_ATTRIBUTE_NORMAL,         // normal file
			  NULL                           // no template file
			  );

  ::SetErrorMode(prev);

  if ( m_Handle == INVALID_HANDLE_VALUE )
    return Kumu::RESULT_FILEOPEN;
  
  m_IOVec = new h__iovec;
  return Kumu::RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileWriter::Writev(ui32_t* bytes_written)
{
  assert( ! m_IOVec.empty() );
  register h__iovec* iov = m_IOVec;
  ui32_t tmp_int;

  if ( bytes_written == 0 )
    bytes_written = &tmp_int;

  if ( m_Handle == INVALID_HANDLE_VALUE )
    return Kumu::RESULT_STATE;

  *bytes_written = 0;
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
  Result_t result = Kumu::RESULT_OK;

  // AFAIK, there is no writev() equivalent in the win32 API
  for ( register int i = 0; i < iov->m_Count; i++ )
    {
      ui32_t tmp_count = 0;
      BOOL wr_result = ::WriteFile(m_Handle,
				   iov->m_iovec[i].iov_base,
				   iov->m_iovec[i].iov_len,
				   (DWORD*)&tmp_count,
				   NULL);

      if ( wr_result == 0 || tmp_count != iov->m_iovec[i].iov_len)
	{
	  result = Kumu::RESULT_WRITEFAIL;
	  break;
	}

      *bytes_written += tmp_count;
    }

  ::SetErrorMode(prev);
  iov->m_Count = 0; // error nor not, all is lost

  return result;
}

//
Kumu::Result_t
Kumu::FileWriter::Write(const byte_t* buf, ui32_t buf_len, ui32_t* bytes_written)
{
  KM_TEST_NULL_L(buf);
  ui32_t tmp_int;

  if ( bytes_written == 0 )
    bytes_written = &tmp_int;

  if ( m_Handle == INVALID_HANDLE_VALUE )
    return Kumu::RESULT_STATE;

  // suppress popup window on error
  UINT prev = ::SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
  BOOL result = ::WriteFile(m_Handle, buf, buf_len, (DWORD*)bytes_written, NULL);
  ::SetErrorMode(prev);

  if ( result == 0 || *bytes_written != buf_len )
    return Kumu::RESULT_WRITEFAIL;

  return Kumu::RESULT_OK;
}

#else // KM_WIN32
//------------------------------------------------------------------------------------------
// POSIX

//
Kumu::Result_t
Kumu::FileReader::OpenRead(const char* filename) const
{
  KM_TEST_NULL_STR_L(filename);
  const_cast<FileReader*>(this)->m_Filename = filename;
  const_cast<FileReader*>(this)->m_Handle = open(filename, O_RDONLY, 0);
  return ( m_Handle == -1L ) ? RESULT_FILEOPEN : RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Close() const
{
  if ( m_Handle == -1L )
    return RESULT_FILEOPEN;

  close(m_Handle);
  const_cast<FileReader*>(this)->m_Handle = -1L;
  return RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Seek(Kumu::fpos_t position, SeekPos_t whence) const
{
  if ( m_Handle == -1L )
    return RESULT_FILEOPEN;

  if ( lseek(m_Handle, position, whence) == -1L )
    return RESULT_BADSEEK;

  return RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Tell(Kumu::fpos_t* pos) const
{
  KM_TEST_NULL_L(pos);

  if ( m_Handle == -1L )
    return RESULT_FILEOPEN;

  Kumu::fpos_t tmp_pos;

  if (  (tmp_pos = lseek(m_Handle, 0, SEEK_CUR)) == -1 )
    return RESULT_READFAIL;

  *pos = tmp_pos;
  return RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileReader::Read(byte_t* buf, ui32_t buf_len, ui32_t* read_count) const
{
  KM_TEST_NULL_L(buf);
  i32_t  tmp_count = 0;
  ui32_t tmp_int = 0;

  if ( read_count == 0 )
    read_count = &tmp_int;

  *read_count = 0;

  if ( m_Handle == -1L )
    return RESULT_FILEOPEN;

  if ( (tmp_count = read(m_Handle, buf, buf_len)) == -1L )
    return RESULT_READFAIL;

  *read_count = tmp_count;
  return (tmp_count == 0 ? RESULT_ENDOFFILE : RESULT_OK);
}


//------------------------------------------------------------------------------------------
//

//
Kumu::Result_t
Kumu::FileWriter::OpenWrite(const char* filename)
{
  KM_TEST_NULL_STR_L(filename);
  m_Filename = filename;
  m_Handle = open(filename, O_RDWR|O_CREAT|O_TRUNC, 0664);

  if ( m_Handle == -1L )
    {
      DefaultLogSink().Error("Error opening file %s: %s\n", filename, strerror(errno));
      return RESULT_FILEOPEN;
    }

  m_IOVec = new h__iovec;
  return RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileWriter::OpenModify(const char* filename)
{
  KM_TEST_NULL_STR_L(filename);
  m_Filename = filename;
  m_Handle = open(filename, O_RDWR|O_CREAT, 0664);

  if ( m_Handle == -1L )
    {
      DefaultLogSink().Error("Error opening file %s: %s\n", filename, strerror(errno));
      return RESULT_FILEOPEN;
    }

  m_IOVec = new h__iovec;
  return RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileWriter::Writev(ui32_t* bytes_written)
{
  assert( ! m_IOVec.empty() );
  register h__iovec* iov = m_IOVec;
  ui32_t tmp_int;

  if ( bytes_written == 0 )
    bytes_written = &tmp_int;

  if ( m_Handle == -1L )
    return RESULT_STATE;

  int total_size = 0;
  for ( int i = 0; i < iov->m_Count; i++ )
    total_size += iov->m_iovec[i].iov_len;

  int write_size = writev(m_Handle, iov->m_iovec, iov->m_Count);
  
  if ( write_size == -1L || write_size != total_size )
    return RESULT_WRITEFAIL;

  iov->m_Count = 0;
  *bytes_written = write_size;  
  return RESULT_OK;
}

//
Kumu::Result_t
Kumu::FileWriter::Write(const byte_t* buf, ui32_t buf_len, ui32_t* bytes_written)
{
  KM_TEST_NULL_L(buf);
  ui32_t tmp_int;

  if ( bytes_written == 0 )
    bytes_written = &tmp_int;

  if ( m_Handle == -1L )
    return RESULT_STATE;

  int write_size = write(m_Handle, buf, buf_len);

  if ( write_size == -1L || (ui32_t)write_size != buf_len )
    return RESULT_WRITEFAIL;

  *bytes_written = write_size;
  return RESULT_OK;
}


#endif // KM_WIN32

//------------------------------------------------------------------------------------------


//
Kumu::Result_t
Kumu::ReadFileIntoString(const char* filename, std::string& outString, ui32_t max_size)
{
  fsize_t    fsize = 0;
  ui32_t     read_size = 0;
  FileReader File;
  ByteString ReadBuf;

  KM_TEST_NULL_STR_L(filename);

  Result_t result = File.OpenRead(filename);

  if ( KM_SUCCESS(result) )
    {
      fsize = File.Size();

      if ( fsize > (Kumu::fpos_t)max_size )
	{
	  DefaultLogSink().Error("%s: exceeds available buffer size (%u)\n", filename, max_size);
	  return RESULT_ALLOC;
	}

      if ( fsize == 0 )
	{
	  DefaultLogSink().Error("%s: zero file size\n", filename);
	  return RESULT_READFAIL;
	}

      result = ReadBuf.Capacity((ui32_t)fsize);
    }

  if ( KM_SUCCESS(result) )
    result = File.Read(ReadBuf.Data(), ReadBuf.Capacity(), &read_size);

  if ( KM_SUCCESS(result) )
    outString.assign((const char*)ReadBuf.RoData(), read_size);

  return result;
}


//
Kumu::Result_t
Kumu::WriteStringIntoFile(const char* filename, const std::string& inString)
{
  FileWriter File;
  ui32_t write_count = 0;
  KM_TEST_NULL_STR_L(filename);

  Result_t result = File.OpenWrite(filename);

  if ( KM_SUCCESS(result) )
    result = File.Write((byte_t*)inString.c_str(), inString.length(), &write_count);

  return result;
}


//------------------------------------------------------------------------------------------
//

// Win32 directory scanner
//
#ifdef KM_WIN32

//
//
Result_t
Kumu::DirScanner::Open(const char* filename)
{
  KM_TEST_NULL_STR_L(filename);

  // we need to append a '*' to read the entire directory
  ui32_t fn_len = strlen(filename); 
  char* tmp_file = (char*)malloc(fn_len + 8);

  if ( tmp_file == 0 )
    return RESULT_ALLOC;

  strcpy(tmp_file, filename);
  char* p = &tmp_file[fn_len] - 1;

  if ( *p != '/' && *p != '\\' )
    {
      p++;
      *p++ = '/';
    }

  *p++ = '*';
  *p = 0;
  // whew...

  m_Handle = _findfirsti64(tmp_file, &m_FileInfo);
  Result_t result = RESULT_OK;

  if ( m_Handle == -1 )
    result = RESULT_NOT_FOUND;

  return result;
}


//
//
Result_t
Kumu::DirScanner::Close()
{
  if ( m_Handle == -1 )
    return RESULT_FILEOPEN;

  if ( _findclose((long)m_Handle) == -1 )
    return RESULT_FAIL;

  m_Handle = -1;
  return RESULT_OK;
}


// This sets filename param to the same per-instance buffer every time, so
// the value will change on the next call
Result_t
Kumu::DirScanner::GetNext(char* filename)
{
  KM_TEST_NULL_L(filename);

  if ( m_Handle == -1 )
    return RESULT_FILEOPEN;

  if ( m_FileInfo.name[0] == '\0' )
    return RESULT_ENDOFFILE;

  strncpy(filename, m_FileInfo.name, MaxFilePath);
  Result_t result = RESULT_OK;

  if ( _findnexti64((long)m_Handle, &m_FileInfo) == -1 )
    {
      m_FileInfo.name[0] = '\0';
	  
      if ( errno != ENOENT )
	result = RESULT_FAIL;
    }

  return result;
}


#else // KM_WIN32

// POSIX directory scanner

//
Result_t
Kumu::DirScanner::Open(const char* filename)
{
  KM_TEST_NULL_STR_L(filename);

  Result_t result = RESULT_OK;

  if ( ( m_Handle = opendir(filename) ) == NULL )
    {
      if ( errno == ENOENT )
	result = RESULT_ENDOFFILE;

      else
	result = RESULT_FAIL;
    }

  return result;
}


//
Result_t
Kumu::DirScanner::Close()
{
  if ( m_Handle == NULL )
    return RESULT_FILEOPEN;

  if ( closedir(m_Handle) == -1 )
    return RESULT_FAIL;

  m_Handle = NULL;
  return RESULT_OK;
}


//
Result_t
Kumu::DirScanner::GetNext(char* filename)
{
  KM_TEST_NULL_L(filename);

  if ( m_Handle == NULL )
    return RESULT_FILEOPEN;

  struct dirent* entry;

  for (;;)
    {
      if ( ( entry = readdir(m_Handle)) == NULL )
	return RESULT_ENDOFFILE;

      break;
    }

  strncpy(filename, entry->d_name, MaxFilePath);
  return RESULT_OK;
}


#endif // KM_WIN32



//
// end KM_fileio.cpp
//
