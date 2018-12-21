
//! Possible Error codes for bios API.
/*!
	This is the list of errors that can be returned from bios API.
	Errors after UNKNOWN are extended errors that are not present in the native API.
	
*/
enum EBiosError
{
	OK,					//!< Mo error, the value of OK is 0. Can be returned from any call.
	CANCEL				//!< The operation was cancelled. See individual calls for details.
	BAD_PARAMETER,		//!< Atleast one of the input parameters is incorrect
	NOT_FOUND,			//!< The call could not find some data. See individual calls for details.
	NOT_IMPLEMENTED,	//!< The call is not supported for the current platform. Can be returned from any call.
	PURCHASE_REQUIRED	//!< The action cannot be performed because the user is banned.
	NOT_ALLOWED			//!< The action is not allowed.
	BANNED				//!< The user is banned from some action.
	LOGICAL				//!< API usage error. For example, usage of objects whos native lifetime ended. Can be returned from any call.
	UNKNOWN,			//!< Any other error. Can be returned from any call.
	
	BAD_SCRIPT,			//!< Extended error. Script not properly linked with engine. May be removed in future.
};