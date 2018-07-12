#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import numpy as np

class VolSurface:
    def __init__(self, data):
        self.data = data
    
    def vol_interpolate(self, minN, maxN, snum, tnum):
        '''
        self.data: 블룸버그에서 손크롤링한 엑셀 파일
        minN: 가격 변화의 미니멈
        maxN: 가격 변화의 맥시멈
        snum: 가격을 쪼개는 개수
        tnum: 시간을 쪼개는 개수
        
        결과는 2d, 넘파이 어레이로 나옴, 컬럼이 가격, 인덱스가 시간
        '''
        #self.data['date'] = np.array(self.data['date'],dtype = 'datetime64')
        #self.data = self.data.set_index('date')
        
#        self.data = self.data + 10
        
        xp = list(self.data.columns.values*100)
        snum_list = list(np.linspace(maxN, minN, snum))

        desired = np.array([[np.nan]*(snum)] * (tnum))
        v_desired = snum_list
        v_mid = np.array([[np.nan]*(snum)]*(len(self.data)))
        basedate = np.datetime64('2017-11-24')
        matdate = np.datetime64('2020-11-23')
#        dt_list = list(np.linspace(0, (np.datetime64(matdate,'D') - np.datetime64(basedate,'D')).astype(int) , tnum))
        dt_list = list(np.linspace((np.datetime64(matdate,'D') - np.datetime64(basedate,'D')).astype(int), 0 , tnum))        
        datesto_int_list = []
        
        for d in self.data.index:
            datesto_int_list.append((np.datetime64(d,'D') - np.datetime64(basedate,'D')).astype(int))
        
        for i in range(0,len(self.data)):
            fp = list(self.data.iloc[i].values)
            v_mid[i] = np.interp(v_desired, xp, fp)
        
        xp = datesto_int_list
        for i in range(0,len(v_mid.T)):
            fp = list(v_mid[:,i])
            desired[:,i] = np.interp(dt_list, xp, fp)
        
        return desired/100

class YieldTermStructure:
    def __init__(self, data):
        self.rates = data
        
    def interest_mesh(self,mintime,maxtime,tnum):
        desired_dt = np.linspace(maxtime,mintime,tnum)
        x = self.rates[:,0]
        result = np.array([[np.nan]*(len(self.rates[0])-1)]*len(desired_dt))
        for c in range(1,len(self.rates[0])):
            fx = self.rates[:,c]
            result[:,c-1] = np.interp(desired_dt, x, fx)
        return result/100