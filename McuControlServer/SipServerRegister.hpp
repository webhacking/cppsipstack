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

bool CSipServer::RecvRegisterRequest( int iThreadId, CSipMessage * pclsMessage )
{
	// 모든 클라이언트의 로그인을 허용한다.
	CSipFrom clsContact;

	gclsUserMap.Insert( pclsMessage, &clsContact );

	CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
	if( pclsResponse )
	{
		pclsResponse->m_clsContactList.push_back( clsContact );

		gclsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	return false;
}
