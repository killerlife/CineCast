# Copyright 2002-2004 The Apache Software Foundation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# All these setting come from the configure script
# We use configure to do most of the hard work as it is more
# designed to handle different system complexity than make
#

# Process this file with autoconf to produce a configure script

AC_INIT([[XML-Security-C]], [1.2.0], [security-dev@xml.apache.org], [xml-security-c])

# framework is a unique directory in src/ - allows configure to check
# it is running in the correct directory

AC_CONFIG_SRCDIR(framework)

# Define the files we wish to generate

AC_CONFIG_FILES([Makefile \
	canon/Makefile \
	transformers/Makefile \
	dsig/Makefile \
	enc/Makefile \
	enc/OpenSSL/Makefile \
	enc/XSCrypt/Makefile \
	framework/Makefile \
	utils/Makefile \
	utils/unixutils/Makefile \
	tools/Makefile \
	xenc/Makefile \
	xenc/impl/Makefile \
	xkms/Makefile \
	xkms/impl/Makefile \
	samples/Makefile])
# AC_CONFIG_FILES([Makefile canon/Makefile transformers/Makefile dsig/Makefile enc/Makefile enc/OpenSSL/Makefile framework/Makefile utils/Makefile utils/unixutils/Makefile tools/Makefile samples/Makefile])
AC_CONFIG_HEADERS([framework/XSECConfig.hpp])

# Get user options

AC_ARG_WITH(xalan, 
	AC_HELP_STRING(--with-xalan, 
		[Enable Xalan integration (Default = yes)]), 
	use_xalan=$withval, use_xalan=yes)

AC_ARG_ENABLE(debug,
	AC_HELP_STRING(--enable-debug,
		[Compile a version of the library with symbols (Default = no)]),
	enable_debug=$enableval, enable_debug=no)

if test "$enable_debug" = "yes" ; then
	CFLAGS="${CFLAGS} -g"
	CXXFLAGS="${CXXFLAGS} -g"
else
	CFLAGS="${CFLAGS} -O2 -DNDEBUG"
	CXXFLAGS="${CXXFLAGS} -O2 -DNDEBUG"
fi

# Create the Makefile.in files in each directory
library_l1_src_dirs="canon transformers dsig enc framework utils xenc xkms"
library_l2_src_dirs="enc/OpenSSL enc/XSCrypt utils/unixutils xenc/impl xkms/impl"

AC_CONFIG_COMMANDS_PRE(
[# Define the library directories
for lib_dir in ${library_l1_src_dirs}
do
	AC_MSG_NOTICE([Creating Makefile.in in ${lib_dir}])
	lib_dir_tran=`echo ${lib_dir} | sed "s/\//_/g"`
    sed "s/__DIRECTORY__/${lib_dir_tran}/g" Makefile.in.tmpl | \
    sed "s/__DIRECTORY_PATH__/${lib_dir}/g" | \
	sed "s/__SRCBASEDISTANCE__/\.\./g" > ${lib_dir}/Makefile.in
done
# Now layer 2
for lib_dir in ${library_l2_src_dirs}
do
	AC_MSG_NOTICE([Creating Makefile.in in ${lib_dir}])
	lib_dir_tran=`echo ${lib_dir} | sed "s/\//_/g"`
	lib_dir_sed=`echo ${lib_dir} | sed "s/\//\\\\\\\\\\//g"`
    sed "s/__DIRECTORY__/${lib_dir_tran}/g" Makefile.in.tmpl | \
    sed "s/__DIRECTORY_PATH__/${lib_dir_sed}/g" | \
	sed "s/__SRCBASEDISTANCE__/\.\.\/\.\./g" > ${lib_dir}/Makefile.in
done])

# Check for basic programs

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_AWK
AC_CHECK_PROG(AUTOCONF, autoconf, autoconf, true)

# AC_PROG_CC Defines the OBJEXT
objsuffix=".${OBJEXT}"


# Set up versions

package_version_major=`echo ${PACKAGE_VERSION} | ${AWK} -F '.' '{print $1}'`
package_version_medium=`echo ${PACKAGE_VERSION} | ${AWK} -F '.' '{print $2}'`
package_version_minor=`echo ${PACKAGE_VERSION} | ${AWK} -F '.' '{print $3}'`

package_lib_major=${package_version_major}${package_version_medium}
package_lib_minor=${package_version_minor}
package_lib_version=${package_lib_major}.${package_lib_minor}

# "cache" the check for gcc -E

AC_TRY_CPP()
AC_LANG(C++)
AC_TRY_CPP()

# Check for Intel > 8.0 (which fools configure into thinking it is GNU
if test "x${ac_cv_cxx_compiler_gnu}" = "xyes"; then
	AC_MSG_CHECKING([for Intel compiler disguised as GNU])
	intel_check=`${CXX} -v 2>&1 | ${AWK} '{print $1}'`
	if test "x${intel_check}" = "xVersion"; then
		AC_MSG_RESULT([yes])
		ac_cv_cxx_compiler_gnu="no"
	else
		AC_MSG_RESULT([no])
	fi
fi

AC_LANG(C)

# Checks for required libraries
AC_CHECK_LIB(m, floor)

# Determine the host system and Makefile fragment

# Check for required includes
AC_CHECK_HEADERS(unistd.h direct.h)

AC_CANONICAL_HOST

# Platform-specific Makefile setup
# We have put most of the platform specific configuration here rather than
# in the Makefile.  Autoconf is designed with platform portability in mind,
# so by putting the complexity here we should be able to create a more
# portable makefile.

case "${host}" in
        *-*-solaris*)   platform=SOLARIS 
						shlibsuffix=.so
						lib_name="lib${PACKAGE_TARNAME}.so.${package_lib_version}"
					    lib_major_name="lib${PACKAGE_TARNAME}.so.${package_lib_major}"
						lib_short_name="lib${PACKAGE_TARNAME}.so"
						if test "x${ac_cv_cxx_compiler_gnu}" = "xyes"; then
						# if test "x${CXX}" = "xg++"; then
							PLATFORM_OPTIONS=["-Wall"]
							PIC=["-fPIC"]
						else
							# Not sure if these will work - only tested with g++
							PIC=["-KPIC"]
							PLATFORM_OPTIONS=["-mt"]
						fi
						# Should use -D_REENTRANT - but not yet OK
						PLATFORM_OPTIONS=["${PLATFORM_OPTIONS} ${PIC} -DSOLARIS"]
						CC1=["${CXX} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						CC4=["${CC} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						MAKE_SHARED=["${CXX} ${CXXFLAGS} -DSOLARIS -G ${PIC}"]
						LINK=["${CXX} ${CXXFLAGS} -DSOLARIS ${PIC}"]
						LINK_COMMAND_1=["(cd \$(LIB_DIR) ; rm -f \$(LIBMAJORNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBMAJORNAME))"]
						LINK_COMMAND_2=["(cd \$(LIB_DIR) ; rm -f \$(LIBSHORTNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBSHORTNAME))"]
						INSTALL_COMMAND_1=["\$(INSTALL) \${THISLIB} \$(DESTDIR)\${libdir}"]
						INSTALL_COMMAND_2=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBMAJORNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBMAJORNAME}"]
						INSTALL_COMMAND_3=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBSHORTNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBSHORTNAME}"]

						;;

        *-*-netbsd*)   platform=NETBSD 
						shlibsuffix=.so
						lib_name="lib${PACKAGE_TARNAME}.so.${package_lib_version}"
					    lib_major_name="lib${PACKAGE_TARNAME}.so.${package_lib_major}"
						lib_short_name="lib${PACKAGE_TARNAME}.so"
						PLATFORM_OPTIONS=["-Wall"]
						PIC=["-fPIC"]

						# Should use -D_REENTRANT - but not yet OK
						PLATFORM_OPTIONS=["${PLATFORM_OPTIONS} ${PIC} -DNETBSD"]
						CC1=["${CXX} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						CC4=["${CC} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						MAKE_SHARED=["${CXX} ${CXXFLAGS} -DNETBSD -shared ${PIC}"]
						LINK=["${CXX} ${CXXFLAGS} -DNETBSD ${PIC}"]
						LINK_COMMAND_1=["(cd \$(LIB_DIR) ; rm -f \$(LIBMAJORNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBMAJORNAME))"]
						LINK_COMMAND_2=["(cd \$(LIB_DIR) ; rm -f \$(LIBSHORTNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBSHORTNAME))"]
						INSTALL_COMMAND_1=["\$(INSTALL) \${THISLIB} \$(DESTDIR)\${libdir}"]
						INSTALL_COMMAND_2=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBMAJORNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBMAJORNAME}"]
						INSTALL_COMMAND_3=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBSHORTNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBSHORTNAME}"]

						;;

        *-*-freebsd*)   platform=FREEBSD 
						shlibsuffix=.so
						lib_name="lib${PACKAGE_TARNAME}.so.${package_lib_version}"
					    lib_major_name="lib${PACKAGE_TARNAME}.so.${package_version_major}"
						lib_short_name="lib${PACKAGE_TARNAME}.so"
						PLATFORM_OPTIONS=["-Wall"]
						PIC=["-fPIC"]

						# Should use -D_REENTRANT - but not yet OK
						PLATFORM_OPTIONS=["${PLATFORM_OPTIONS} ${PIC} -DFREEBSD"]
						CC1=["${CXX} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						CC4=["${CC} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						MAKE_SHARED=["${CXX} ${CXXFLAGS} -DFREEBSD -shared ${PIC} -pthread -lc_r"]
						LINK=["${CXX} ${CXXFLAGS} -DFREEBSD ${PIC} -pthread -lc_r"]
						LINK_COMMAND_1=["(cd \$(LIB_DIR) ; rm -f \$(LIBMAJORNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBMAJORNAME))"]
						LINK_COMMAND_2=["(cd \$(LIB_DIR) ; rm -f \$(LIBSHORTNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBSHORTNAME))"]
						INSTALL_COMMAND_1=["\$(INSTALL) \${THISLIB} \$(DESTDIR)\${libdir}"]
						INSTALL_COMMAND_2=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBMAJORNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBMAJORNAME}"]
						INSTALL_COMMAND_3=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBSHORTNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBSHORTNAME}"]

						;;

        *-*-linux*)     platform=LINUX ;
						shlibsuffix=.so
						lib_name="lib${PACKAGE_TARNAME}.so.${package_lib_version}"
					    lib_major_name="lib${PACKAGE_TARNAME}.so.${package_lib_major}"
						lib_short_name="lib${PACKAGE_TARNAME}.so"
						#if test "x${CXX}" = "xg++"; then
						if test "x${ac_cv_cxx_compiler_gnu}" = "xyes"; then
							PLATFORM_OPTIONS=["-Wall"]
							PIC=["-fPIC"]
						else
							# Not sure if these will work - only tested with g++
							PIC=["-KPIC"]
							PLATFORM_OPTIONS=["-w1 -mp -prec_div"]
						fi
						# Should use -D_REENTRANT - but not yet OK
						PLATFORM_OPTIONS=["${PLATFORM_OPTIONS} ${PIC} -DLINUX"]
						CC1=["${CXX} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						CC4=["${CC} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						MAKE_SHARED=["${CXX} ${CXXFLAGS} -Wl,-soname,\$(LIBNAME) -DLINUX -shared ${PIC}"]
						LINK=["${CXX} ${CXXFLAGS} -DLINUX -ldl ${PIC}"]
						LINK_COMMAND_1=["(cd \$(LIB_DIR) ; rm -f \$(LIBMAJORNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBMAJORNAME))"]
						LINK_COMMAND_2=["(cd \$(LIB_DIR) ; rm -f \$(LIBSHORTNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBSHORTNAME))"]
						INSTALL_COMMAND_1=["\$(INSTALL) \${THISLIB} \$(DESTDIR)\${libdir}"]
						INSTALL_COMMAND_2=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBMAJORNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBMAJORNAME}"]
						INSTALL_COMMAND_3=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBSHORTNAME} && \$(LN_S) \${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBSHORTNAME}"]

						;;

		*-*-cygwin*)	platform=CYGWIN
						shlibsuffix=.dll
						lib_name="cyg${PACKAGE_TARNAME}-${package_lib_version}.dll"
					    lib_major_name="lib${PACKAGE_TARNAME}.dll.a"
						lib_short_name="cyg${PACKAGE_TARNAME}.dll"
						# Assume g++
						PLATFORM_OPTIONS=["-Wall"]

						# Should use -D_REENTRANT - but not yet OK
						PLATFORM_OPTIONS=["${PLATFORM_OPTIONS} -DCYGWIN"]
						CC1=["${CXX} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						CC4=["${CC} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
						MAKE_SHARED=["${CXX} ${CXXFLAGS} -Wl,-soname,\$(LIBNAME) -DCYGWIN -shared -Xlinker --warn-once -Xlinker -Map -Xlinker ../lib/lib${PACKAGE_TARNAME}.map -Xlinker --output-def -Xlinker ../lib/lib${PACKAGE_TARNAME}.def -Xlinker --out-implib -Xlinker ../lib/\$(LIBMAJORNAME)"]
						LINK=["${CXX} ${CXXFLAGS} -DCYGWIN"]

						INSTALL_COMMAND_1=["\$(INSTALL) \${THISLIB} \$(DESTDIR)\${bindir}"]
						INSTALL_COMMAND_2=["\$(INSTALL) \$(LIB_DIR)/\$(LIBMAJORNAME) \$(DESTDIR)\${libdir}"]

						;;
						
#        *-*-irix*)      platform=IRIX ;;
#        *-*-aix*)       platform=AIX ;;
#        *-*-hp*)        platform=HPUX ;
#                        case "${host}" in
#                            *-*-hpux11*)  osver=HPUX11;;
#                            *-*-hpux10*)  osver=HPUX10;;
#                            *)            osver=HPUX11;;
#                        esac;;
#        *-*-mvs*)       platform=OS390 ;;
#        *-*-osf*)       platform=TRU64 ;;

        *-apple-darwin*)
                        platform=MACOSX 
                        shlibsuffix=.dylib
                        lib_name="lib${PACKAGE_TARNAME}.${package_lib_version}.dylib"
                        lib_major_name="lib${PACKAGE_TARNAME}.${package_lib_major}.dylib"
                        lib_short_name="lib${PACKAGE_TARNAME}.dylib"
                        if test "x${ac_cv_cxx_compiler_gnu}" = "xyes"; then
                            PLATFORM_OPTIONS=["-Wall"]
                            PIC=["-fPIC"]
                        else
                            # Not sure if these will work - only tested with g++
                            PIC=["-KPIC"]
                            PLATFORM_OPTIONS=["-w1 -mp -prec_div"]
                        fi
                        # Should use -D_REENTRANT - but not yet OK
                        PLATFORM_OPTIONS=["${PLATFORM_OPTIONS} ${PIC} -DMACOSX"]
                        CC1=["${CXX} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
                        CC4=["${CC} ${CXXFLAGS} ${PLATFORM_OPTIONS}"]
                        MAKE_SHARED=["${CXX} ${CXXFLAGS} -Xlinker -install_name -Xlinker \$(LIBNAME) -DMACOSX -dynamiclib ${PIC}"]
                        LINK=["${CXX} ${CXXFLAGS} -DMACOSX ${PIC}"]
                        LINK_COMMAND_1=["(cd \$(LIB_DIR) ; rm -f \$(LIBMAJORNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBMAJORNAME))"]
                        LINK_COMMAND_2=["(cd \$(LIB_DIR) ; rm -f \$(LIBSHORTNAME) ; \$(LN_S) \$(LIBNAME) \$(LIBSHORTNAME))"]
                        INSTALL_COMMAND_1=["\$(INSTALL) \${THISLIB} \${DESTDIR}\${libdir}"]
                        INSTALL_COMMAND_2=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBMAJORNAME} && \$(LN_S) \$(DESTDIR)\${libdir}/\${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBMAJORNAME}"]
                        INSTALL_COMMAND_3=["\$(RM) \$(DESTDIR)\${libdir}/\${LIBSHORTNAME} && \$(LN_S) \$(DESTDIR)\${libdir}/\${LIBNAME} \$(DESTDIR)\${libdir}/\${LIBSHORTNAME}"]
                        ;;

        *)              platform=UNKNOWN ;
						AC_MSG_ERROR([Unknown Platform.  Aborting])
						;;
esac

# Find Xerces

# Check for Xerces using XERCESCROOT (override any system version)

AC_LANG(C++)

if test $XERCESCROOT; then

  OLDCPPFLAGS=$CPPFLAGS
  CPPFLAGS=["${CPPFLAGS} -I${XERCESCROOT}/src"]

  AC_MSG_CHECKING([for Xerces 2.x headers in XERCESCROOT/src])
  AC_TRY_RUN([#include <xercesc/util/XercesVersion.hpp>
    int main(int argc, char **argv) {
      if (XERCES_VERSION_MAJOR < 2)
        return 1;
      else
        return 0;
    }],
    LIBS="${LIBS} -L${XERCESCROOT}/lib -lxerces-c"
    xerces_found=yes
    [AC_MSG_RESULT([found])],
    CPPFLAGS=$OLD_CPPFLAGS
    [AC_MSG_RESULT([no])])

fi

if test -z $xerces_found ; then

  # Xerces was not found

  AC_MSG_CHECKING([for Xerces 2.x headers in system includes])

  AC_TRY_RUN([#include <xercesc/util/XercesVersion.hpp>
    int main(int argc, char **argv) {
      if (XERCES_VERSION_MAJOR < 2)
        return 1;
      else
        return 0;
    }],
    LIBS="${LIBS} -lxerces-c"
    [AC_MSG_RESULT([found])],
    [AC_MSG_ERROR([Xerces 2.0 not found.  Ensure XERCESCROOT points to base directory of Xerces and Xerces 2.0 or later is available])])

fi

# Find out some properties of the version of Xerces we have

AC_MSG_CHECKING([whether Xerces XMLFormatter requires a version])

AC_TRY_COMPILE([#include <xercesc/framework/XMLFormatter.hpp>
	#include <xercesc/util/XercesDefs.hpp>
	#include <xercesc/util/XMLUniDefs.hpp>],
	[using namespace XERCES_CPP_NAMESPACE;
	  XMLCh s_encoding[] = {
		chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_8, chNull
	  };
	  XMLFormatter f(s_encoding,
		NULL,
		NULL, 
		XMLFormatter::NoEscapes, 
		XMLFormatter::UnRep_CharRef);
	],
	[AC_MSG_RESULT([yes])]
	[AC_DEFINE(XSEC_XERCES_REQUIRES_MEMMGR)]
	[AC_DEFINE(XSEC_XERCES_FORMATTER_REQUIRES_VERSION)],
	[AC_MSG_RESULT([no])])
	
AC_MSG_CHECKING([whether Xerces XMLString has ::release])

AC_TRY_COMPILE([#include <xercesc/util/XMLString.hpp>],
	[using namespace XERCES_CPP_NAMESPACE;
		XMLCh * tst;
		XMLString::release(&tst);
	],
	[AC_MSG_RESULT([yes])]
	[AC_DEFINE(XSEC_XERCES_XMLSTRING_HAS_RELEASE)],
	[AC_MSG_RESULT([no])])

AC_MSG_CHECKING([whether Xerces XMLElement has ::setIdAttribute])

AC_TRY_COMPILE([#include <xercesc/dom/DOM.hpp>],
	[using namespace XERCES_CPP_NAMESPACE;
		DOMElement * elt;
		elt->setIdAttribute(NULL);
	],
	[AC_MSG_RESULT([yes])]
	[AC_DEFINE(XSEC_XERCES_XMLSTRING_HAS_RELEASE)],
	[AC_MSG_RESULT([no])])

# Now check for Xalan

if test "$use_xalan" = "yes" ; then

  if test $XALANCROOT; then

    AC_MSG_CHECKING([for Xalan headers in XALANCROOT])

    OLD_CPPFLAGS=$CPPFLAGS
    # Updated to include nls/include as this is generally needed for
    # compilation against non-installed xalan
    CPPFLAGS=["${CPPFLAGS} -I${XALANCROOT}/src -I${XALANCROOT}/nls/include"]

    AC_TRY_CPP([#include <xalanc/Include/XalanVersion.hpp>], 
      xalan_found=yes 
      LIBS="${LIBS} -L${XALANCROOT}/lib -lxalan-c"
      [AC_MSG_RESULT([found])], 
      CPPFLAGS=$OLD_CPPFLAGS
      [AC_MSG_RESULT([no])]);

  fi

  if test -z $xalan_found ; then

    AC_MSG_CHECKING([for Xalan in system includes])
    AC_TRY_CPP([#include <xalanc/Include/XalanVersion.hpp>], 
      xalan_found=yes 
      LIBS="${LIBS} -lxalan-c"
      [AC_MSG_RESULT([found])],
      [AC_DEFINE(XSEC_NO_XALAN)] 
      [AC_MSG_RESULT([no - WARNING - configuring for no Xalan])]);

  fi

else

  # NO_XALAN is defined - so we skip

  AC_MSG_NOTICE([Warning - Xalan being configured out - XPath and XSLT will not be available])
  AC_DEFINE(XSEC_NO_XALAN)

fi


# Check for handling of XSLException types
if test "$xalan_found" = "yes" ; then

  AC_MSG_CHECKING([whether XSLException::getType() returns XalanDOMString])

  AC_TRY_COMPILE([#include <xalanc/PlatformSupport/XSLException.hpp>], 
    [ try {
          int x = 1;
      } 
      catch (XALAN_CPP_NAMESPACE_QUALIFIER XSLException &e) {
        e.getType().c_str();
      }
    ],
    AC_DEFINE(XSEC_XSLEXCEPTION_RETURNS_DOMSTRING)
    [AC_MSG_RESULT([yes])], 
    [AC_MSG_RESULT([no])]);

  AC_MSG_CHECKING([whether XercesParserLiaison ctor takes XercesDOMSupport])

  AC_TRY_COMPILE([#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
    #include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>],
	[ 	XALAN_CPP_NAMESPACE_QUALIFIER XercesDOMSupport 
           theDOMSupport;
	    XALAN_CPP_NAMESPACE_QUALIFIER XercesParserLiaison 
           theParserLiaison(theDOMSupport);
	],
	AC_DEFINE(XSEC_XERCESPARSERLIAISON_REQS_DOMSUPPORT)
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])]);

  AC_MSG_CHECKING([whether XPathEvaluator::selectNodeList reqs NodeRefList])

  AC_TRY_COMPILE([#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
	#include <xalanc/XPath/XPathEvaluator.hpp>
	#include <xalanc/XPath/NodeRefList.hpp>
    #include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>],
	[ 	XALAN_CPP_NAMESPACE_QUALIFIER XercesDOMSupport 
           theDOMSupport;
		XALAN_CPP_NAMESPACE_QUALIFIER XPathEvaluator	theEvaluator;
		XALAN_CPP_NAMESPACE_QUALIFIER NodeRefList	theResult(
			theEvaluator.selectNodeList(
			theDOMSupport,
			NULL,
			NULL,
			NULL));
	],
	[AC_MSG_RESULT([no])],
	AC_DEFINE(XSEC_SELECTNODELIST_REQS_NODEREFLIST)
	[AC_MSG_RESULT([yes])]);

  AC_MSG_CHECKING([whether Xalan classes req MemoryManager])

  AC_TRY_COMPILE([
 	#include <xalanc/XPath/XPathDefinitions.hpp>
    #include <xalanc/XPath/Function.hpp>],
    [
    }
	XALAN_CPP_NAMESPACE_BEGIN

	class FunctionTest : public Function
	{
		public:
 	       typedef Function        ParentType;

    	    FunctionTest();

        	virtual ~FunctionTest();      
	        virtual XObjectPtr
    	    execute(
        	   XPathExecutionContext& executionContext,
               XalanNode* context,
               const XObjectArgVectorType& args,
               const LocatorType* locator) const;

            XObjectPtr
            execute(
                XPathExecutionContext&          executionContext,
                XalanNode*                                      context,
                const XObjectPtr                        arg1,
                const XObjectPtr                        arg2,
                const LocatorType*                      locator) const;

            XObjectPtr
            execute(
                XPathExecutionContext&          executionContext,
                XalanNode*                                      context,
                   
                const XObjectPtr                        arg1,
                const XObjectPtr                        arg2,
                const XObjectPtr                        arg3,
                const LocatorType*                      locator) const;

	#if !defined(XALAN_NO_USING_DECLARATION)
        using ParentType::execute;
	#endif

	#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
        virtual Function*
	#else
        virtual FunctionTest*
	#endif
        clone() const;

	protected:

        virtual const XalanDOMString
        getError() const;

    private:

        // Not implemented...
        FunctionTest&
        operator=(const FunctionTest&);

        bool
        operator==(const FunctionTest&) const;
	};

  void main2(void){

	FunctionTest t;

  }

XALAN_CPP_NAMESPACE_END
  int main3(void) {

	],
	[AC_MSG_RESULT([no])],
	AC_DEFINE(XSEC_XALAN_REQS_MEMORYMANAGER)
	[AC_MSG_RESULT([yes])]);

fi

AC_LANG(C)

# Check for OpenSSL
if test $OPENSSL ; then

# First check includes

AC_MSG_CHECKING([for OpenSSL headers in OPENSSL])

OLD_CPPFLAGS=$CPPFLAGS
CPPFLAGS=["${CPPFLAGS} -I${OPENSSL}/include"]

AC_TRY_CPP([#include <openssl/opensslv.h>], 
   openssl_found=yes 
   AC_DEFINE(HAVE_OPENSSL)
   [AC_MSG_RESULT([found])], 
   CPPFLAGS=$OLD_CPPFLAGS
   [AC_MSG_RESULT([no])]);

# Now check libs

OLD_LIBS=$LIBS
LIBS=["${LIBS} -L${OPENSSL} -lcrypto"]

AC_CHECK_LIB([crypto], 
   [EVP_EncryptInit],
   openssllib_found=yes,
   [LIBS=$OLD_LIBS])

   if test -z $openssllib_found ; then

   LIBS=["${LIBS} -L${OPENSSL}/lib -lcrypto"]
   AC_CHECK_LIB([crypto],
      [EVP_DecryptInit],
      openssllib_found=yes,
      [LIBS=$OLD_LIBS]
	  [AC_MSG_ERROR(["Unable to find libcrypto"])])

  fi

fi

if test -z $openssl_found ; then

AC_MSG_CHECKING([for OpenSSL in system includes])
AC_TRY_CPP([#include <openssl/opensslv.h>], 
   openssl_found=yes 
   AC_DEFINE(HAVE_OPENSSL)
   LIBS="${LIBS} -lcrypto"
   [AC_MSG_RESULT([found])],
   [AC_MSG_RESULT([no])]
   [AC_MSG_ERROR([OpenSSL header files not found.  Please define OPENSSL])]);

fi

# Now try to find out some things about this version of OpenSSL

AC_LANG(C++)

AC_MSG_CHECKING([for const input buffers in OpenSSL])
AC_TRY_COMPILE([#include <openssl/rsa.h>],
	[ const unsigned char * buf;
	  unsigned char * outbuf;
	  RSA rsa;

		RSA_private_decrypt(1,
							buf,
							outbuf,
							&rsa,
							RSA_PKCS1_PADDING);

	],
	[AC_MSG_RESULT([yes])]
	[AC_DEFINE(XSEC_OPENSSL_CONST_BUFFERS)],
	[AC_MSG_RESULT([no])])

AC_CHECK_DECL(EVP_aes_128_cbc,
	[AC_DEFINE(XSEC_OPENSSL_HAVE_AES)],
	,[#include <openssl/evp.h>])

AC_CHECK_DECL(EVP_CIPHER_CTX_set_padding(NULL,0),
	[AC_DEFINE(XSEC_OPENSSL_CANSET_PADDING)],
	,[#include <openssl/evp.h>])

AC_CHECK_DECL(CRYPTO_cleanup_all_ex_data,
	[AC_DEFINE(XSEC_OPENSSL_HAVE_CRYPTO_CLEANUP_ALL_EX_DATA)],
	,[#include <openssl/crypto.h>])

AC_MSG_CHECKING([for const input buffer in loadX509Base64Bin])
AC_TRY_COMPILE([#include <openssl/x509.h>],
	[ const unsigned char * buf;
	  d2i_X509(NULL, &buf, 1);
	],
	[AC_MSG_RESULT([yes])]
	[AC_DEFINE(XSEC_OPENSSL_D2IX509_CONST_BUFFER)],
	[AC_MSG_RESULT([no])])

# Build the object file lists necessary to link the library
# At the same time, build the include list

lib_obj_files=""

for lib_dir in ${library_l1_src_dirs}
do
	AC_CREATE_OBJ_FILELIST([${lib_dir}], 
		[*.cpp], 
		[\.cpp], 
		[[${objsuffix}]], 
		[../../lib/obj])
	eval ${lib_dir}_obj_files="\${ac_find_files}"

	# Create the libraries list
	AC_REPLACE_STRING([${ac_find_files}], [[\.\.\/\.\.\/]], [[\.\.\/]])
	lib_obj_files="${lib_obj_files} $ac_replace_string"

	# Create the Include list
	AC_CREATE_FILELIST([${lib_dir}],[*.hpp])
	eval ${lib_dir}_include_files="\${ac_find_files}"

done

for lib_dir in ${library_l2_src_dirs}
do
	lib_dir_tran=`echo ${lib_dir} | sed "s/\//_/g"`
	AC_CREATE_OBJ_FILELIST([${lib_dir}], 
		[*.cpp], 
		[\.cpp], 
		[[${objsuffix}]], 
		[../../../lib/obj])
	eval ${lib_dir_tran}_obj_files="\${ac_find_files}"

	# Create the libraries list
	AC_REPLACE_STRING([${ac_find_files}], [[\.\.\/\.\.\/\.\.\/]], [[\.\.\/]])
	lib_obj_files="${lib_obj_files} $ac_replace_string"

	# Create the Include list
	AC_CREATE_FILELIST([${lib_dir}],[*.hpp])
	eval ${lib_dir_tran}_include_files="\${ac_find_files}"


done

# Check to ensure the config.hpp file has been added :

check_cnf=`echo ${framework_include_files} | grep XSECConfig`

if test "${framework_include_files}" != "${check_cnf}" ; then
	framework_include_files="${framework_include_files} XSECConfig.hpp"
fi

# Use the found lists to populate the relevant makefiles

AC_SUBST(canon_obj_files)
AC_SUBST(transformers_obj_files)
AC_SUBST(dsig_obj_files)
AC_SUBST(enc_obj_files)
AC_SUBST(enc_OpenSSL_obj_files)
AC_SUBST(enc_XSCrypt_obj_files)
AC_SUBST(framework_obj_files)
AC_SUBST(utils_obj_files)
AC_SUBST(utils_unixutils_obj_files)
AC_SUBST(lib_obj_files)
AC_SUBST(xenc_obj_files)
AC_SUBST(xenc_impl_obj_files)
AC_SUBST(xkms_obj_files)
AC_SUBST(xkms_impl_obj_files)

AC_SUBST(canon_include_files)
AC_SUBST(transformers_include_files)
AC_SUBST(dsig_include_files)
AC_SUBST(enc_include_files)
AC_SUBST(enc_OpenSSL_include_files)
AC_SUBST(enc_XSCrypt_include_files)
AC_SUBST(framework_include_files)
AC_SUBST(utils_include_files)
AC_SUBST(utils_unixutils_include_files)
AC_SUBST(lib_include_files)
AC_SUBST(xenc_include_files)
AC_SUBST(xkms_include_files)

# Populate Install directories

AC_SUBST(prefix)
AC_SUBST(exec_prefix)
AC_SUBST(includedir)
AC_SUBST(libdir)
AC_SUBST(bindir)

# Populate special platform specific make commands

AC_SUBST(LINK_COMMAND_1)
AC_SUBST(LINK_COMMAND_2)
AC_SUBST(INSTALL_COMMAND_1)
AC_SUBST(INSTALL_COMMAND_2)
AC_SUBST(INSTALL_COMMAND_3)

# Now populate the platform and Compiler related variables.

AC_SUBST(platform)

cxx=${CXX}
AC_SUBST(cxx)

cc=${CC}
AC_SUBST(cc)

AC_SUBST(prefix)
AC_SUBST(osver)

cxxflags=${CXXFLAGS}
AC_SUBST(cxxflags)

cppflags=${CPPFLAGS}
AC_SUBST(cppflags)

cflags=${CFLAGS}
AC_SUBST(cflags)

ldflags=${LDFLAGS}
AC_SUBST(ldflags)

libs=${LIBS}
AC_SUBST(libs)

transcoder=${TRANSCODER}
AC_SUBST(transcoder)

threads=${THREADS}
AC_SUBST(threads)

bitstobuild=${BITSTOBUILD}
AC_SUBST(bitstobuild)

cc1=${CC1}
AC_SUBST(cc1)

cc4=${CC4}
AC_SUBST(cc4)

ln_s=${LN_S}
AC_SUBST(ln_s)

install=${INSTALL}
install_data=${INSTALL_DATA}

AC_SUBST(install)
AC_SUBST(install_data)

make_shared=${MAKE_SHARED}
AC_SUBST(make_shared)

link=${LINK}
AC_SUBST(link)

AC_SUBST(objsuffix)
AC_SUBST(shlibsuffix)

# Versioning
package_version=${PACKAGE_VERSION}
AC_SUBST(package_version)
AC_SUBST(package_version_major)
AC_DEFINE_UNQUOTED(XSEC_VERSION, "$package_version")
AC_DEFINE_UNQUOTED(XSEC_VERSION_MAJOR, $package_version_major)
AC_DEFINE_UNQUOTED(XSEC_VERSION_MEDIUM, $package_version_medium)
AC_DEFINE_UNQUOTED(XSEC_VERSION_MINOR, $package_version_minor)

package_name=${PACKAGE_NAME}
AC_SUBST(package_name)
package_tarname=${PACKAGE_TARNAME}
AC_SUBST(package_tarname)

# Library names
AC_SUBST(lib_name)
AC_SUBST(lib_major_name)
AC_SUBST(lib_short_name)

# output the Makefiles
AC_OUTPUT()
