/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "StringUtility.h"
#include <string.h>
#include <stdlib.h>

/**
 * @ingroup SipPlatform
 * @brief 문자열에 포함된 문자열을 수정한다.
 * @param strCallId	문자열
 * @param pszBefore 수정 대상 문자열
 * @param pszAfter	수정 대상 문자열을 수정할 문자열
 */
void ReplaceString( std::string & strCallId, const char * pszBefore, const char * pszAfter )
{
	size_t iPos = strCallId.find( pszBefore );
	size_t iBeforeLen = strlen( pszBefore );
	size_t iAfterLen = strlen( pszAfter );

	while( iPos < std::string::npos )
	{
		strCallId.replace( iPos, iBeforeLen, pszAfter );
		iPos = strCallId.find( pszBefore, iPos + iAfterLen );
	}
}

/**
 * @ingroup SipPlatform
 * @brief 문자열에 포함된 키의 값을 추출한다. 
 *				"app=36;msg=36;hotline=46;presence=36; broadcast=46" 문자열에서 
 *				app 의 값을 추출하고 싶으면 pszKey 에 "app=" 를 입력하고 cSep 에 ';' 를 입력하면 된다.
 * @param strText		문자열
 * @param pszKey		키
 * @param cSep			구분자
 * @param strValue	키의 값을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool SearchValue( std::string & strText, const char * pszKey, char cSep, std::string & strValue )
{
	strValue.clear();

	size_t iPos = strText.find( pszKey );
	if( iPos < std::string::npos )
	{
		size_t iKeyLen = strlen( pszKey );
		size_t iEndPos = strText.find( cSep, iPos + iKeyLen );

		if( iEndPos < std::string::npos )
		{
			strValue = strText.substr( iPos + iKeyLen, iEndPos - ( iPos + iKeyLen ) );
		}
		else
		{
			strValue = strText.substr( iPos + iKeyLen );
		}

		return true;
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief 문자열에 포함된 키의 값을 추출한다. 
 *				"app=36;msg=36;hotline=46;presence=36; broadcast=46" 문자열에서 
 *				app 의 값을 추출하고 싶으면 pszKey 에 "app=" 를 입력하고 cSep 에 ';' 를 입력하면 된다.
 * @param strText		문자열
 * @param pszKey		키
 * @param cSep			구분자
 * @param iValue		키의 값을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool SearchValue( std::string & strText, const char * pszKey, char cSep, int & iValue )
{
	std::string	strValue;

	if( SearchValue( strText, pszKey, cSep, strValue ) )
	{
		iValue = atoi( strValue.c_str() );

		return true;
	}

	return false;
}
