/*
armadillo����blas_win32��lapack_win32

*/

#pragma once

#ifdef _MSC_VER

#if _MSC_VER > 1500

#ifndef ARMA_USE_BLAS
#define ARMA_USE_BLAS
#endif

#ifndef ARMA_USE_LAPACK
#define ARMA_USE_LAPACK
#endif

#endif

#pragma warning(push)

// glue_times_meat.hpp(485): warning C4100: ��B��: δ���õ��β�
#pragma warning(disable:4100)

// Mat_meat.hpp(999): warning C4244: ��=��: �ӡ�const arma::uword��ת������arma::uhword�������ܶ�ʧ����
#pragma warning(disable:4244)

// mul_gemm.hpp(196): warning C4189: ��B_n_rows��: �ֲ������ѳ�ʼ����������
#pragma warning(disable:4189)

// fn_norm.hpp(530): warning C4702: �޷����ʵĴ���
#pragma warning(disable:4702)


#include "armadillo.h"

#pragma warning(pop)

#if _MSC_VER > 1500
#pragma comment(lib,"blas.lib")
#pragma comment(lib,"lapack.lib")
#pragma comment(lib,"libf2c.lib")
#endif

#endif // _MSC_VER