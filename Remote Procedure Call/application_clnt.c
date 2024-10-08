/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "application.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

Response_Auth *
authenticate_1(Request_Auth *argp, CLIENT *clnt)
{
	static Response_Auth clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, AUTHENTICATE,
		(xdrproc_t) xdr_Request_Auth, (caddr_t) argp,
		(xdrproc_t) xdr_Response_Auth, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

Response_Approve *
approve_1(Request_Approve *argp, CLIENT *clnt)
{
	static Response_Approve clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, APPROVE,
		(xdrproc_t) xdr_Request_Approve, (caddr_t) argp,
		(xdrproc_t) xdr_Response_Approve, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

Response_Bearer *
bearer_1(Request_Bearer *argp, CLIENT *clnt)
{
	static Response_Bearer clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BEARER,
		(xdrproc_t) xdr_Request_Bearer, (caddr_t) argp,
		(xdrproc_t) xdr_Response_Bearer, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

Renew_Response_Bearer *
renew_bearer_1(Renew_Request_Bearer *argp, CLIENT *clnt)
{
	static Renew_Response_Bearer clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, RENEW_BEARER,
		(xdrproc_t) xdr_Renew_Request_Bearer, (caddr_t) argp,
		(xdrproc_t) xdr_Renew_Response_Bearer, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

Response_Validate_Action *
validate_action_1(Request_Validate_Action *argp, CLIENT *clnt)
{
	static Response_Validate_Action clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, VALIDATE_ACTION,
		(xdrproc_t) xdr_Request_Validate_Action, (caddr_t) argp,
		(xdrproc_t) xdr_Response_Validate_Action, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
