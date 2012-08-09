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

#include "XmlElement.h"

static bool Test( const char * pszText, const char * pszResult )
{
	CXmlElement	clsXml;
	int		iPos, iTextLen = strlen(pszText);
	char	szResult[1024];

	iPos = clsXml.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "xml(%s) parse error\n", pszText );
		return false;
	}

	clsXml.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "xml(%s) tostring error(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

bool TestXml()
{
	if( Test( 
		"<userlist>\r\n"
		"  <user id=\"1000\">\r\n"
		"    <password>1234</password>\r\n"
		"  </user>\r\n"
		"</userlist>\r\n",
		"<userlist>\n"
		"<user id=\"1000\">\n"
		"<password>1234</password>\n"
		"</user>\n"
		"</userlist>\n" ) == false ) return false;

	if( Test( 
		"<userlist>\r\n"
		"	<user id=\"1000\">\r\n"
		"		<password>1234</password>\r\n"
		"	</user>\r\n"
		"</userlist>\r\n",
		"<userlist>\n"
		"<user id=\"1000\">\n"
		"<password>1234</password>\n"
		"</user>\n"
		"</userlist>\n" ) == false ) return false;

	if( Test( 
		"<userlist>\n"
		"	<user id=\"1000\" password=\"1234\" />\n"
		"	<user id=\"1001\" password=\"4321\" />\n"
		"</userlist>\n",
		"<userlist>\n"
		"<user id=\"1000\" password=\"1234\"/>\n"
		"<user id=\"1001\" password=\"4321\"/>\n"
		"</userlist>\n" ) == false ) return false;

	return true;
}