#ifndef _MY_UTILS
#pragma once
#define _MY_UITLS
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#define TRYCATCH(pred,msg) if(pred) {fprintf(stderr, "ERROR: %s -- %s\n", strerror(errno), msg); exit(1);}
/**
 * \brief �ҳ������е�һ������������Ԫ�ص��±�
 * \param type ����Ԫ������
 * \param array ����
 * \param len ���鳤��
 * \param predicate ɸѡ����ʹ�� _it ��Ϊ������
 * \param res �����������������������ĺ����������� -1
 */
#define ARRAYFIRST(_type,array,len,predicate,res)\
	int res = -1;\
	for(int _TMP_I = 0; _TMP_I < len; _TMP_I++)\
	{\
		_type* _it = &array[_TMP_I];\
		if(predicate)\
		{\
			res = _TMP_I;\
			break;\
		}\
	}
/**
 * \brief ʹ��ָ��������ʼ�������е�ÿ��Ԫ��
 * \param type ����Ԫ������
 * \param array ����
 * \param len ���鳤��
 * \param action ��ÿ��Ԫ��ִ�еĲ�����_it ��Ϊ������
 */
#define ARRAYINIT(type,array,len,action)\
	for(int _TMP_I = 0; _TMP_I < len; _TMP_I++)\
	{\
		type* _it = &array[_TMP_I];\
		action\
	}
#endif
