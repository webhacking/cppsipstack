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

#include "SipPlatformDefine.h"
#include "SdpAttribute.h"
#include <stdlib.h>
#include "MemoryDebug.h"

/**
 * @ingroup SdpParser
 * @brief 생성자
 */
CSdpAttribute::CSdpAttribute()
{
}

/**
 * @ingroup SdpParser
 * @brief 생성자
 * @param pszName		애트리뷰트 이름
 * @param pszValue	애트리뷰트 값
 */
CSdpAttribute::CSdpAttribute( const char * pszName, const char * pszValue )
{
	if( pszName )
	{
		m_strName = pszName;
	}

	if( pszValue )
	{
		m_strValue = pszValue;
	}
}

/**
 * @ingroup SdpParser
 * @brief 소멸자
 */
CSdpAttribute::~CSdpAttribute()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP attribute 의 value 문자열을 파싱한다.
 * @param pszText		SDP attribute 의 value 문자열
 * @param iTextLen	SDP attribute 의 value 문자열 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1를 리턴한다.
 */
int CSdpAttribute::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ':' )
		{
			m_strName.append( pszText, iPos );
			m_strValue.append( pszText + iPos + 1, iTextLen - ( iPos + 1 ) );
			break;
		}
	}

	if( m_strName.empty() )
	{
		m_strName.append( pszText, iTextLen );
	}

	return iTextLen;
}

/**
 * @ingroup SdpParser
 * @brief SDP attribute 의 value 문자열을 저장한다.
 * @param pszText		SDP attribute 의 value 문자열을 저장할 변수
 * @param iTextSize pszText 변수의 크기
 * @returns 성공하면 저장된 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSdpAttribute::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( Empty() ) return -1;

	if( m_strValue.empty() )
	{
		return snprintf( pszText, iTextSize, "%s", m_strName.c_str() );
	}
	else
	{
		return snprintf( pszText, iTextSize, "%s:%s", m_strName.c_str(), m_strValue.c_str() );
	}
}

/**
 * @ingroup SdpParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSdpAttribute::Clear()
{
	m_strName.clear();
	m_strValue.clear();
}

/**
 * @ingroup SdpParser
 * @brief 데이터가 존재하면 false 를 리턴하고 그렇지 않으면 true 를 리턴한다.
 */
bool CSdpAttribute::Empty()
{
	if( m_strName.empty() ) return true;

	return false;
}

/**
 * @ingroup SdpParser
 * @brief payload type 을 리턴한다.
 * @returns payload type 을 리턴한다.
 */
int CSdpAttribute::GetPayLoadType()
{
	if( strcmp( m_strName.c_str(), "rtpmap" ) && strcmp( m_strName.c_str(), "fmtp" ) ) return -1;

	int iLen = (int)m_strValue.length();
	const char * pszValue = m_strValue.c_str();

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == ' ' )
		{
			if( i > 0 )
			{
				std::string strValue;

				strValue.append( pszValue, i );

				return atoi( strValue.c_str() );
			}

			break;
		}
	}

	return -1;
}

/**
 * @ingroup SdpParser
 * @brief encoding name 을 가져온다.
 * @param strName encoding name 을 저장할 변수
 * @returns 성공적으로 encoding name 를 가져오면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSdpAttribute::GetEncodingName( std::string & strName )
{
	strName.clear();

	if( strcmp( m_strName.c_str(), "rtpmap" ) ) return false;

	int iLen = (int)m_strValue.length();
	const char * pszValue = m_strValue.c_str();
	int iStartPos = -1;

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == ' ' )
		{
			iStartPos = i + 1;
		}
		else if( pszValue[i] == '/' )
		{
			strName.append( pszValue + iStartPos, i - iStartPos );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SdpParser
 * @brief SDP attribute 의 value 를 파싱하여서 clsParamList 객체에 저장한다.
 * @param clsParamList parameter list 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSdpAttribute::GetParameterList( CSipParameterList & clsParamList )
{
	clsParamList.ClearParam();

	if( strcmp( m_strName.c_str(), "fmtp" ) ) return false;

	int iLen = (int)m_strValue.length();
	const char * pszValue = m_strValue.c_str();
	int iStartPos = -1;

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == ' ' )
		{
			iStartPos = i + 1;
			break;
		}
	}

	if( clsParamList.HeaderListParamParse( pszValue + iStartPos, iLen - iStartPos ) > 0 )
	{
		return true;
	}

	return false;
}
