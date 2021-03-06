package com.cppsipstack;

/** SipUserAgent callback 으로 호출된 이벤트를 안드로이드 Handler 로 GUI 에 전달할 때에 사용되는 항목 구분자
 * @ingroup SipUserAgentAndroid
 * @author 이영한 ( http://blog.naver.com/websearch )
 */
public interface SipUserAgentHandlerDefine
{
	final String COMMAND = "command";
	
	final String EVENT_REGISTER = "EventRegister";
	final String EVENT_INCOMING_CALL = "EventIncomingCall";
	final String EVENT_CALL_RING = "EventCallRing";
	final String EVENT_CALL_START = "EventCallStart";
	final String EVENT_CALL_END = "EventCallEnd";
	final String EVENT_REINVITE = "EventReInvite";
	final String EVENT_PRACK = "EventPrack";
	final String EVENT_TRANSFER = "EventTransfer";
	final String EVENT_BLIND_TRANSFER = "EventBlindTransfer";
	final String EVENT_MESSAGE = "EventMessage";
	
	final String ARG_CALL_ID = "strCallId";
	final String ARG_INFO = "clsInfo";
	final String ARG_STATUS = "iStatus";
	final String ARG_FROM = "strFrom";
	final String ARG_TO = "strTo";
	final String ARG_RTP = "clsRtp";
	final String ARG_REMOTE_RTP = "clsRemoteRtp";
	final String ARG_LOCAL_RTP = "clsLocalRtp";
	final String ARG_SIP_STATUS = "iSipStatus";
	final String ARG_REFER_TO_CALL_ID = "strReferToCallId";
	final String ARG_SCREENED_TRANSFER = "bScreenedTransfer";
	final String ARG_REFER_TO_ID = "strReferToId";
	final String ARG_SMS = "strSms";
}
