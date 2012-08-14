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

#include "SipParserDefine.h"
#include "SipFrom.h"
#include "SipUtility.h"

CSipFrom::CSipFrom()
{
}

CSipFrom::~CSipFrom()
{
}

/**
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipFrom Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipFrom::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iDisplayNamePos = -1, iUriStartPos = -1, iUriEndPos = -1, iLen;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( iDisplayNamePos != -1 )
		{
			if( pszText[iPos] == '"' )
			{
				m_strDisplayName.append( pszText + iDisplayNamePos, iPos - iDisplayNamePos );
				iDisplayNamePos = -1;
			}
		}
		else if( iUriStartPos != -1 )
		{
			if( pszText[iPos] == '>' )
			{
				iUriEndPos = iPos;
				++iPos;
				break;
			}
		}
		else if( pszText[iPos] == '"' )
		{
			iDisplayNamePos = iPos + 1;
		}
		else if( pszText[iPos] == '<' )
		{
			iUriStartPos = iPos + 1;

			if( m_strDisplayName.empty() && iPos > 0 )
			{
				for( int i = iPos - 1; i >= 0; --i )
				{
					if( pszText[i] != ' ' && pszText[i] != '\t' )
					{
						m_strDisplayName.append( pszText, i + 1 );
						break;
					}
				}
			}
		}
		else if( pszText[iPos] == ';' || pszText[iPos] == ',' )
		{
			break;
		}
	}

	if( iUriStartPos != -1 && iUriEndPos != -1 )
	{
		m_clsUri.Parse( pszText + iUriStartPos, iUriEndPos - iUriStartPos );
	}
	else
	{
		m_clsUri.Parse( pszText, iPos );
	}

	while( iPos < iTextLen )
	{
		if( pszText[iPos] == ' ' || pszText[iPos] == '\t' || pszText[iPos] == ';' )
		{
			++iPos;
			continue;
		}
		else if( pszText[iPos] == ',' )
		{
			break;
		}

		iLen = ParseSipParameter( m_clsParamList, pszText + iPos, iTextLen - iPos );
		if( iLen == -1 ) return -1;
		iPos += iLen;
	}

	return iPos;
}

/**
 * @brief SIP �޽����� ���Ե� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipFrom::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen = 0, iPos;

	if( m_strDisplayName.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\"%s\" <", m_strDisplayName.c_str() );
	}
	else
	{
		pszText[iLen++] = '<';
	}

	iPos = m_clsUri.ToString( pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	if( iLen == iTextSize ) return -1;
	pszText[iLen++] = '>';

	iPos = MakeSipParameterString( m_clsParamList, pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

void CSipFrom::AddParam( const char * pszName, const char * pszValue )
{
	AddSipParameter( m_clsParamList, pszName, pszValue );
}

void CSipFrom::AddTag()
{
	char szTag[SIP_TAG_MAX_SIZE];

	SipMakeTag( szTag, sizeof(szTag) );

	AddParam( "tag", szTag );
}

bool CSipFrom::GetParam( const char * pszName, std::string & strValue )
{
	return SearchSipParameter( m_clsParamList, pszName, strValue );
}

/**
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipFrom::Clear()
{
	m_strDisplayName.clear();
	m_clsUri.Clear();
	m_clsParamList.clear();
}

bool CSipFrom::Empty()
{
	return m_clsUri.Empty();
}

int ParseSipFrom( SIP_FROM_LIST & clsList, const char * pszText, int iTextLen )
{
	int iPos, iCurPos = 0;
	CSipFrom	clsFrom;

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == ',' )
		{
			++iCurPos;
			continue;
		}

		iPos = clsFrom.Parse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;

		clsList.push_back( clsFrom );
	}

	return iCurPos;
}