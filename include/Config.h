#pragma once

#ifndef _CONFIG_H

#ifdef ORBITDYN_STATIC
//#ifndef ORBITDYN_DLL
	#define ORBITDYN_VAR extern
	#define ORBITDYN_API
#else
	#ifdef OrbitDyn_EXPORTS
		#define ORBITDYN_VAR extern __declspec(dllexport)
		#define ORBITDYN_API __declspec(dllexport)
	#else
		#define ORBITDYN_VAR extern __declspec(dllimport)
		#define ORBITDYN_API __declspec(dllimport)
	#endif //ORBITDYN_EXPORTS
#endif //ORBITDYN_DLL

// �Ƿ�ʹ�������ļ���δ����ʱ��ʹ��DE405������������ת���������������ļ�
// ����ʱ������Ҫ��Щ�ļ��������������ý��ƽ����⣬��ʱû���������ǵ�����

//#define NONE_DATA_FILE_MODE

#endif

