.\"
.\"Copyright (c) 1991-1994  Sony Corporation
.\"
.\"Permission is hereby granted, free of charge, to any person obtaining
.\"a copy of this software and associated documentation files (the
.\""Software"), to deal in the Software without restriction, including
.\"without limitation the rights to use, copy, modify, merge, publish,
.\"distribute, sublicense, and/or sell copies of the Software, and to
.\"permit persons to whom the Software is furnished to do so, subject to
.\"the following conditions:
.\"
.\"The above copyright notice and this permission notice shall be
.\"included in all copies or substantial portions of the Software.
.\"
.\"THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
.\"EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
.\"MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
.\"IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
.\"DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
.\"OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
.\"THE USE OR OTHER DEALINGS IN THE SOFTWARE.
.\"
.\"Except as contained in this notice, the name of Sony Corporation
.\"shall not be used in advertising or otherwise to promote the sale, use
.\"or other dealings in this Software without prior written authorization
.\"from Sony Corporation.
.\"
.\"Author: Noriyuki Hosoe, Sony Corporation
.\"
.\"$SonyRCSfile: makedict.ms,v $
.\"$SonyRevision: 1.1 $
.\"$SonyDate: 1994/06/03 08:00:47 $
.\"
.TL
���������ġ��� makedict
.RS
.LP
\fBmakedict\fP �ϡ���̾�����Ѵ������� \fBsj3serv\fP �ǻ��Ѥ��뼭���
�������뤿��Υġ���Ǥ���
.sp
���Υġ������Ѥ��뤳�Ȥˤ�ꡢ���ǥ������Խ����줿����� \fBsj3serv\fP ��
���Ѳ�ǽ�ʷ��֤��Ѵ����뤳�Ȥ�����ޤ���
.sp
�Խ���ǽ�ե�����ϡ��ʲ��Τ褦�ʷ����򤷤Ƥ��ޤ���
.RS
.DS
��������������������������������������������������Ƭ��:
���������������������������������������������������ޣ�:
���������������������������������������������������ޣ�:
���������������������������������������������������ޣ�:
������������������������˰�������������������������ޣ�:
��������������������������
.DE
.RE
���줾��Υե�����ɤϰʲ��ΰ�̣������ޤ���
.RS
.DS
�裱�ե�����ɡ����ɤߡ����ѤҤ餬�ʡ�
�裲�ե�����ɡ�����Ū�ν���
�裳�ե�����ɡ����ʻ���¤�
.DE
.RE
�����ǳƥե�����ɴ֤ϡ����İʾ�Υ��ڡ������뤤�ϥ��֤Ƕ��ڤ��Ƥ���
ɬ�פ�����ޤ����ޤ��ƹԤϡ��ɤߤǥ����Ȥ���Ƥ��ʤ���Фʤ�ޤ���
.sp
�裳�ե�����ɡ��ʻ�Υե�����ɡˤˤϡ��̻���������Ƥ����ʻ�򵭽Ҥ��ޤ���
.sp
���Ĥνϸ�ˤ�������ʣ�����ʻ����ꤹ��Ȥ��ˤϡ��ʲ��Τ褦�ˤ��ޤ���
.RS
.DS
�������������������������������������������������壴:���ޣ�:
���������������������������������������������������ޣ�:���ʣ�:
�������������������������ġ����������������������壱:������:
��������������������������
.DE
.RE
�ʤ����μ���Ǥϡ��θ���̾�졦���졦Ϣ�λ�ʤ�����ѤΤ�ΡˤˤĤ��Ƥ�
�ϸ줽�Τޤޤ��Ѹ���ư�졦���ƻ졦����ư��ʤɳ��Ѥ����Ρˤ�
�ؤ��Ƥϸ촴�Τߤ���Ͽ���ʤ���Фʤ�ޤ���
.sp
���Τ褦���Խ���ǽ�ե����뤫�� \fBsj3serv\fP �ǻ��Ѳ�ǽ�ʼ������������ˤϡ�
\fBmakedict\fP ����Ѥ��ưʲ��Τ褦�ˤ��ޤ���
.RS
.DS
makedict \fIsrc_file_name\fP \fIdst_file_name\fP
.DE
.RE
���顼���ʤ��¹Ԥ���λ�����Ȥ��ˤϡ�\fIsrc_file_name\fP �ǻ��ꤵ�줿
�Խ���ǽ�ե����뤬�Ѵ����졢\fIdst_file_name\fP �ǻ��ꤵ�줿�ե������
��Ǽ����Ƥ��ޤ���
.RE