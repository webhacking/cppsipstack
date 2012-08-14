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

#include "SipNISTList.h"
#include "SipStack.h"
#include "SipDeleteQueue.h"

CSipNISTList::CSipNISTList()
{
}

CSipNISTList::~CSipNISTList()
{
}

/**
 * @brief Non-Invite Server Transaction List �� SIP �޽����� �߰��Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipNISTList::Insert( CSipMessage * pclsMessage )
{
	std::string strKey;
	NON_INVITE_TRANSACTION_MAP::iterator	itMap;
	bool bRes = false;

	if( pclsMessage == NULL ) return false;
	if( GetKey( pclsMessage, strKey ) == false ) return false;

	if( pclsMessage->IsRequest() )
	{
		m_clsMutex.acquire();
		itMap = m_clsMap.find( strKey );

		if( itMap == m_clsMap.end() )
		{
			CSipNonInviteTransaction * psttTransaction = new CSipNonInviteTransaction();
			if( psttTransaction )
			{
				psttTransaction->m_pclsRequest = pclsMessage;
				gettimeofday( &psttTransaction->m_sttStartTime, NULL );

				m_clsMap.insert( NON_INVITE_TRANSACTION_MAP::value_type( strKey, psttTransaction ) );
				bRes = true;
			}
		}
		else
		{
			if( itMap->second->m_pclsResponse )
			{
				m_pclsSipStack->Send( itMap->second->m_pclsResponse );
			}
		}
		m_clsMutex.release();
	}
	else
	{
		m_clsMutex.acquire();
		itMap = m_clsMap.find( strKey );
		if( itMap != m_clsMap.end() )
		{
			if( itMap->second->m_pclsResponse == NULL )
			{
				itMap->second->m_pclsResponse = pclsMessage;
				bRes = true;
			}
			else if( itMap->second->m_pclsResponse->m_iStatusCode != pclsMessage->m_iStatusCode )
			{
				gclsSipDeleteQueue.Insert( itMap->second->m_pclsResponse );
				itMap->second->m_pclsResponse = pclsMessage;
				bRes = true;
			}

			if( bRes )
			{
				if( pclsMessage->m_iStatusCode >= 200 )
				{
					gettimeofday( &itMap->second->m_sttStopTime, NULL );
				}

				pclsMessage->MakePacket();
			}
		}
		m_clsMutex.release();
	}

	return bRes;
}

/**
 * @brief Non-Invite Server Transaction List ���� �������� �׸񿡼� ������ �׸��� �����Ѵ�.
 * @param psttTime ������ �ð�
 */
void CSipNISTList::Execute( struct timeval * psttTime )
{
	if( m_clsMap.size() == 0 ) return;

	NON_INVITE_TRANSACTION_MAP::iterator	itMap, itNext;
	OSIP_MESSAGE_LIST	clsResponseList;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( itMap->second->m_sttStopTime.tv_sec > 0 )
		{
			if( DiffTimeval( &itMap->second->m_sttStopTime, psttTime ) >= 32000 )
			{
				itNext = itMap;
				++itNext;

				delete itMap->second;
				m_clsMap.erase( itMap );

				if( itNext == m_clsMap.end() ) break;
				itMap = itNext;
				goto LOOP_START;
			}
		}
	}
	m_clsMutex.release();
}

/**
 * @brief Non-Invite Server Transaction List �� ��� �׸��� �����Ѵ�.
 */
void CSipNISTList::DeleteAll( )
{
	NON_INVITE_TRANSACTION_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		delete itMap->second;
	}

	m_clsMap.clear();
	m_clsMutex.release();
}

/**
 * @brief Non-Invite Server Transaction List �� ũ�⸦ �����Ѵ�.
 * @returns Non-Invite Server Transaction List �� ũ�⸦ �����Ѵ�.
 */
int CSipNISTList::GetSize( )
{
	return m_clsMap.size();
}

void CSipNISTList::GetString( std::string & strBuf )
{

}