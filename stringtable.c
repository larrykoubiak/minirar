#include "minirar.h"
#include "stringtable.h"

char *GetFormatName(RARFORMAT fmt)
{
	char *type = NULL;
	switch(fmt) {
		case RARFMT14:
			type="RARFMT14";
			break;
		case RARFMT15:
			type="RARFMT15";
			break;
		case RARFMT50:
			type="RARFMT50";
			break;
		case RARFMT_FUTURE:
			type="RARFMT_FUTURE";
		default:
			type="RARFMT_NONE";
	}
	return type;
}

char *GetHeaderTypeName(HEADER_TYPE ht)
{
	char *type = NULL;
	switch(ht) {
		case HEAD_MARK:
			type="HEAD_MARK";
			break;
		case HEAD_MAIN:
			type="HEAD_MAIN";
			break;
		case HEAD_FILE:
			type="HEAD_FILE";
			break;
		case HEAD_SERVICE:
			type="HEAD_SERVICE";
			break;
		case HEAD_CRYPT:
			type="HEAD_CRYPT";
			break;
		case HEAD_ENDARC:
			type="HEAD_ENDARC";
			break;
		case HEAD_UNKNOWN:
			type="HEAD_UNKNOWN";
			break;
		case HEAD3_MARK:
			type="HEAD3_MARK";
			break;
		case HEAD3_MAIN:
			type="HEAD3_MAIN";
			break;
		case HEAD3_FILE:
			type="HEAD3_FILE";
			break;
		case HEAD3_CMT:
			type="HEAD3_CMT";
			break;
		case HEAD3_AV:
			type="HEAD3_AV";
			break;
		case HEAD3_OLDSERVICE:
			type="HEAD3_OLDSERVICE";
			break;
		case HEAD3_PROTECT:
			type="HEAD3_PROTECT";
			break;
		case HEAD3_SIGN:
			type="HEAD3_SIGN";
			break;
		case HEAD3_SERVICE:
			type="HEAD3_SERVICE";
			break;
		case HEAD3_ENDARC:
			type="HEAD3_ENDARC";
			break;
		default:
			type="INVALID_HEADER";
			break;
	}
	return type;
}