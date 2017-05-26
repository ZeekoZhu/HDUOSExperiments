#ifndef _MY_UTILS
#pragma once
#define _MY_UITLS
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#define TRYCATCH(pred,msg) if(pred) {fprintf(stderr, "ERROR: %s -- %s\n", strerror(errno), msg); exit(1);}
/**
 * \brief 找出数组中第一个符合条件的元素的下标
 * \param type 数组元素类型
 * \param array 数组
 * \param len 数组长度
 * \param predicate 筛选器，使用 _it 作为迭代器
 * \param res 结果索引，如果不存在这样的元素，将返回 -1
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
 * \brief 使用指定动作初始化数组中的每个元素
 * \param type 数组元素类型
 * \param array 数组
 * \param len 数组长度
 * \param action 对每个元素执行的操作，_it 作为迭代器
 */
#define ARRAYINIT(type,array,len,action)\
	for(int _TMP_I = 0; _TMP_I < len; _TMP_I++)\
	{\
		type* _it = &array[_TMP_I];\
		action\
	}
#endif
