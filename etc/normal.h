#ifndef _NORMAL_H_    // _FUNC_H_가 정의되어있지 않다면
#define _NORMAL_H_    // _FUNC_H_를 정의하라 (중복을 방지하는 trick)
// 전처리기: 컴파일이 되기 전에 실행됨(일종의 옵션을 주는 기능)
// #pragma once보다 표준적인 방법

double normpdf(double x, double mu = 0, double sigma = 1);     // 함수 원형(Prototype) 선언(declare)
double normcdf(double x, double mu = 0, double sigma = 1);

// 함수는 선언과 정의가 쌍을 이뤄야 함

#endif
