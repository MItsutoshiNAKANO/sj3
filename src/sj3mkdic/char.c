/*
 * Copyright (c) 1991-1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: char.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:32 $
 */


#include <stdio.h>
#include <sys/types.h>
#include "sj_euc.h"
#include "sj3_dict_const.h"

#include "sj3mkdic.h"

int
cnvyomi(int code)
{
	u_short hh;
	u_char	high;
	u_char low;

	hh = ((code >> 16) & 0xffff);
	high = ((code >> 8) & 0xff);
	low = (code & 0xff);

	if (!hh) {
		if (high == 0xa1) {
			switch (low) {
			      case 0xbc:	return 1;
			      case 0xf4:	return 2;
			      case 0xf7:	return 3;
			      case 0xa7:	return 4;
			}
		}
		else if (high == 0xa2) {
			if (low == 0xa9)
			  return 4;
		}
		else if (high == 0xa3) {
			if (low < 0xb0)
			  ;
			else if (low <= 0xb9)
			  return(low - 0xb0 + 0x10);
			else if (low < 0xc0)
			  ;
			else if (low <= 0xda)
			  return(low - 0xc1 + 0x1A);
			else if (low < 0xe1)
			  ;
			else if (low <= 0xfa)
			  return(low - 0xe1 + 0x34);
		}
		else if (high == 0xa4) {
			if (low < 0xa1)
			  ;
			else if (low <= 0xf3)
			  return(low - 0xa1 + 0x4e);
		}
		else if (high == 0xa5) {
			switch (low) {
			      case 0xf4:	return 0xa1;
			      case 0xf5:	return 0xa2;
			      case 0xf6:	return 0xa3;
			}
		}
	}

	return 0;
}


int
h2kcode(int code)
{
	u_short hh;
	u_char	high;
	u_char	low;

	hh = ((code >> 16) & 0xffff);
	high = ((code >> 8) & 0xff);
	low = (code & 0xff);

	if (!hh) {
		if (high != 0xa4)
		  return code;
		else
		  return (code + 0x0100);
	}
	return code; /* XXX */
}


int
codesize(u_char code)
{
	switch (code & KANJIMODEMASK) {
	      case ZENHIRAASSYUKU:
	      case ZENKATAASSYUKU:
	      case KANJIASSYUKU:
	      case KANJISTREND:
		return 1;
	      case LEADINGHANKAKU:
	      case OFFSETASSYUKU:
	      case AIATTRIBUTE:
		return 2;
	      default:
		return 2;
	}
}


void
output_knj(FILE *fp, u_char *p, int l)
{
	while (l > 0) {
		switch (*p & KANJIMODEMASK) {
		      case ZENHIRAASSYUKU:
			fprintf(fp, "\244\322%d", (*p++ & 0x0f) + 1);
			l--;
			break;
		      case ZENKATAASSYUKU:
			fprintf(fp, "\245\253%d", (*p++ & 0x0f) + 1);
			l--;
			break;
		      case KANJIASSYUKU:
			fprintf(fp, "\260\265%1x", (*p++ & 0x0f));
			l--;
			break;
		      case LEADINGHANKAKU:
#ifndef USEHANKAKUINDICT
			l = 0;
			fprintf(stderr, "\244\252\244\253\244\267\244\244");
#else
			fputc(*p++, fp); l--;
			fputc(*p++, fp); l--;
#endif
			break;
		      case OFFSETASSYUKU:
			fprintf(fp, "ofs%1x", (*p++ & 0x0f));
			l--;
			fprintf(fp, "%02x", *p++); l--;
			break;
		      case AIATTRIBUTE:
			fprintf(fp, "\302\260%1x", (*p++ & 0x0f));
			l--;
			fprintf(fp, "%02x", *p++); l--;
			break;
		      case KANJISTREND:
			p++; l--;
			break;
		      default:
			if (p[1] & 0x80)
			  fputc(SS3, fp);
			fputc((*p | 0x80), fp); l--; p++;
			fputc((*p | 0x80), fp); l--; p++;

                }
	}
}


void
output_str(FILE *fp, char *p)
{
	while (*p) { fputc(*p, fp); p++; }
}



void
output_int(FILE *fp, int *p)
{
	while (*p) {
		if (*p < 0x100) {
			fputc(*p, fp); p++;
		}
		else if (*p < 0x10000) {
			fputc((*p >> 8) & 0xff, fp);
			fputc(*p & 0xff, fp); p++;
		}
		else if (*p < 0x1000000) {
			fputc((*p >> 16) & 0xff, fp);
			fputc((*p >> 8) & 0xff, fp);
			fputc(*p & 0xff, fp); p++;
		}
		else {
			fputc((*p >> 24) & 0xff, fp);
			fputc((*p >> 16) & 0xff, fp);
			fputc((*p >> 8) & 0xff, fp);
			fputc(*p & 0xff, fp); p++;
		}
	}
}



int
yomi2zen(int code)
{
	static	char	num[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	switch (code) {
	case 0x01:	return 0xa1bc;
	case 0x02:	return 0xa1f4;
	case 0x03:	return 0xa1f7;
	case 0x04:	return 0xa2a9;
	case 0xa1:	return 0xa5f4;
	case 0xa2:	return 0xa5f5;
	case 0xa3:	return 0xa5f6;
	default:	break;
	}

	if (code < 0x10)
		;

	else if (code < 0x1a)
		return(0xa300 + code - 0x10 + 0xb0);

	else if (code < 0x34)
		return(0xa300 + code - 0x1a + 0xc1);

	else if (code < 0x4e)
		return(0xa300 + code - 0x34 + 0xe1);

	else if (code < 0xa1)
		return(0xa400 + code - 0x4e + 0xa1);

	return ((num[(code >> 4) & 0x0f] << 8) | num[code & 0x0f]);
}



void
output_yomi(FILE *fp, u_char *p)
{
	int	i;

	while (*p) {
		i = yomi2zen(*p++);
		fputc((i >> 8) & 0xff, fp);
		fputc(i & 0xff, fp);
	}
}
