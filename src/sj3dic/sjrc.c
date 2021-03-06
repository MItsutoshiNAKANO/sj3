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
 * $SonyRCSfile: sjrc.c,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1997/01/23 11:26:31 $
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <sys/types.h>
#include "sj3dic.h"
#include "sjctype.h"
#include "sjtool.h"
#include "sj_string.h"

char	RCfile[LONGLENGTH];

static char	*rcfile = ".sjrc";


static int	user_euc = 0;
static int	file_code = SYS_SJIS;

struct functbl funcs[] = {
	{ "dictionary",	set_dict },
	{ "userdic",	set_dict },
	{ "server",	set_server },
	{ NULL,		NULL }
};

#define TOLOWER(c) (isupper(c) ? tolower(c) : (c))

int
sjset_code()
{
	char *loc;

	loc = setlocale(LC_CTYPE, "");

	if (!loc) {
		fprintf(stderr, "Error: can't get current locale\n");
		exit(1);
	}
	if (strncmp(loc, "ja_JP.SJIS", 10) == 0) {
		user_euc = 0;
	} else if (strncmp(loc, "ja", 2) == 0 || strncmp(loc, "Ja", 2) == 0) {
		user_euc = 1;
		file_code = -1;
	} else {
		fprintf(stderr, "Warrning: In this locale japanese will not be used.\n");
		user_euc = 1;
		file_code = -1;
	  }

	return 1;
}


int
getsjrc()
{
	FILE *fd, *fopen();
	char *p;
	char *getenv();

	sjset_code();
	RCfile[0] = '\0';
	if ((p = getenv("SJRC")) != NULL && *p != '\0') {
		strlcpy(RCfile, home_dir, sizeof(RCfile));
		if (*p != '/')
			strlcat(RCfile, "/", sizeof(RCfile));
		strlcat(RCfile, p, sizeof(RCfile));
		if ((fd = fopen (RCfile, "r")) != NULL) {
			setrc (RCfile, fd);
			fclose (fd);
			return TRUE;
		}
	}
	if (home_dir[0] != '\0') {
		strlcpy(RCfile, home_dir, sizeof(RCfile));
		strlcat(RCfile, "/", sizeof(RCfile));
		strlcat(RCfile, rcfile, sizeof(RCfile));
		if ((fd = fopen (RCfile, "r")) != NULL) {
			setrc (RCfile, fd);
			fclose (fd);
			return TRUE;
		}
	}
	RCfile[0] = '\0';
	return FALSE;
}


void
setrc(char *file, FILE *fd)
{
	char		line[MAXLINE];
	int	w;
	struct wordent word[WORDN];
	char	*p;
	struct functbl *functp;

	while ((p = fgets (line, MAXLINE, fd)) != NULL) {
		if ((w = getword(line, word)) <= 0)
			continue;
		functp = funcs;
		p = word[0].word_str;
		while (functp->keyword != NULL) {
			if (much(p, functp->keyword)) {
				if (functp->func) (*(functp->func))(word);
				break;;
			}
			functp++;
		}
	}
}


int
much(char *s1, char *s2)
{
	char c1, c2;

	if (s1 == NULL)
		return 0;
	while (*s2 != '\0') {
		c1 = *s1++;
		c2 = *s2++;
		if (TOLOWER(c1) != TOLOWER(c2))
			return 0;
	}
	return 1;
}


int
getword(char *s, struct wordent word[])
{
	u_char	c, cc;
	char *p;
	int	i, wcount;

	if (IsDelimitor(*s)) {
		s++;
	} else {
		p = s;
		while(!IsDelimitor(*s)) {
			if (IsTerminator(*s) || isTerminator(*s))
				break;
			s++;
		}
		*s++ = '\0';
		if (much(p, term_name) == 0)
			return 0;
	}
	i = wcount = 0;
	p = word[wcount].word_str;
	while (*s != '\0') {
		c = *s++;
		if (file_code == SYS_SJIS) {
			if (issjis1(c) && issjis2(*s)) {
				if (i < MAXWORD - 2) {
					*p++ = c;
					*p++ = *s;
					i += 2;
				}
				s++;
				continue;
			}
		} else if (file_code == SYS_EUC) {
			if (iseuc(c) && iseuc(*s)) {
				if (i < MAXWORD - 2) {
					cc = (c << 8) + (*s & 0xff);
					cc = euc2sjis(cc);
					*p++ = (cc >> 8) & 0xff;
					*p++ = cc & 0xff;
					i += 2;
				}
				s++;
				continue;
			} 
			if (iseuckana(c) && iskana2(*s))
				c = *s++;
		} else {
			if (issjis1(c) && issjis2(*s)) {
				file_code = SYS_SJIS;
				s--;
				continue;
			}
			if ((iseuc(c) && iseuc(*s)) || 
				(iseuckana(c) && iskana2(*s))) {
				file_code = SYS_EUC;
				s--;
				continue;
			}
		}
		if (isTerminator(c))
			break;
		if (IsDelimitor(c)) {
			if (i > 0) {
				if (++wcount >= WORDN - 1)
					break;
				*p = '\0';
				i = 0;
				p = word[wcount].word_str;
			}
			continue;
		}
		if (IsEscape(c))
			c = *s++;
		if (IsTerminator(c))
			break;
		if (i < MAXWORD - 1) {
			*p++ = c;
			i++;
		}
	}
	*p = '\0';
	if (word[wcount].word_str[0] != '\0')
		word[++wcount].word_str[0] = '\0';
	return wcount;
}

int
IsTerminator(u_char c)
{
	return (c == '\n') ? 1 : 0;
}

int
isTerminator(u_char c)
{
	return (c == '#') ? 1 : 0;
}

int
IsEscape(u_char c)
{
	return (c == '\\') ? 1 : 0;
}

int
IsDelimitor(u_char c)
{
	return (c == ' ' || c == '\t' || c == '.') ? 1 : 0;
}

void
set_dict(struct wordent word[])
{
	setdicname(word[1].word_str);
}

void
set_server(struct wordent word[])
{
	setsjserv(word[1].word_str);
}
