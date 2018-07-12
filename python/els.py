#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd
import os
import marketvariable
import fdm_engine as fdm

class ELS:
    def __init__(self, basedate, matdate, earlydate, condition, coupon, barrier):
        
        '''
        basedate: 평가일(datetime)
        matdate: 만기일(datetime)
        earlydate: 조기상환일(datetime의 리스트 혹은 array)
        condition: 조기상환 조건(array 혹은 list)
        coupon: 지급되는 쿠폰, 만기시 지급되는 쿠폰도 포함(array 혹은 list)
        barrier: 배리어
        '''
        
        self.basedate = basedate
        self.matdate = matdate
        self.earlydate = earlydate
        self.condition = condition
        self.coupon = coupon
        self.barrier = barrier
    
    def setMarketVariable(self, rate, vol1, vol2, rho):
        
        '''
        시장변수 지정 함수
        '''
        
        self.rate = rate
        self.vol1 = vol1
        self.vol2 = vol2
        self.rho = rho
        
    def dt_calculator(self, tnum):
        
        '''
        조기상환 시점까지 몇 dt를 가야하는지를 계산
        '''
        
        total_period = (np.datetime64(self.matdate,'D') - np.datetime64(self.basedate,'D')).astype(int)
        days_interval = total_period/tnum
        
        results = []
        for d in self.earlydate:
            dt_at = abs(round((np.datetime64(d,'D') - np.datetime64(self.matdate,'D')).astype(int)/days_interval))
            results.append(dt_at)    
            
        return results
    
    def setEngine(self, code):
        
        '''
        FDM Pricing 방법을 결정하는 함수
        code = 0 : OSM
        code = 1 : ADI
        
        '''
        if code == 0:
            self.engine = fdm.OSM_Pricing()
        elif code == 1:
            self.engine = fdm.ADI_Pricing()
        else:
            raise KeyError("Choose 0 for OSM, 1 for ADI")
        
    def price(self, minvalue, maxvalue, snum, tnum, lmd = 0.5):
        
        '''
        FDM Pricing 함수
        '''
        
        t = (self.matdate - self.basedate) / np.timedelta64(365, 'D')
        dt = t / tnum
        h = (maxvalue - minvalue) / snum
        coupon = self.coupon[:-1]
        
        vol1 = self.vol1.vol_interpolate(minvalue, maxvalue, snum, tnum)
        vol2 = self.vol2.vol_interpolate(minvalue, maxvalue, snum, tnum)
        r = self.rate.interest_mesh(0, t, tnum).T
        init_mesh, x, y = self.engine.initial_mesh(minvalue, maxvalue, snum, self.coupon, self.barrier)
        
        A = self.engine.tridiagonal_A(vol1, vol2, x, y, h, dt, r, lmd, tnum, snum)
        
        whereis_rd = self.dt_calculator(tnum)
        
        trange = np.linspace(0, t, tnum)
        meshes_list = []
        
        meshes_list.append(init_mesh)
        prev_mesh = init_mesh
        
        for i in range(1, len(trange)):
            
            step1_mesh = self.engine.step1(A[0][i-1], x, y, r[1], vol1, vol2, prev_mesh, dt, h, lmd, self.rho, i)
            step2_mesh = self.engine.step2(A[1][i-1], x, y, r[1], vol1, vol2, step1_mesh, dt, h, lmd, self.rho, i)
            
            if i in whereis_rd:
                idx = whereis_rd.index(i)
                redempt_mesh = self.engine.early_redemption(step2_mesh, x, y,
                                                       self.condition[idx], coupon[idx])
                meshes_list.append(redempt_mesh)
                prev_mesh = redempt_mesh
            
            else:
                meshes_list.append(step2_mesh)
                prev_mesh = step2_mesh
        
        price = self.engine.interpolatePrice(meshes_list[-1], x, y)
        self.meshlist = meshes_list
        self.dt = dt
        self.h = h
        
        return price, meshes_list
    
    def greek(self):
        delta = []
        gamma = []
        theta = []
        length = len(self.meshlist[0])
        for k in range(len(self.meshlist)):
            #Delta
            temp_x = np.zeros([length, length])
            temp_y = np.zeros([length, length])
            temp_x[1:] = -np.diff(self.meshlist[k], axis = 0) / self.h
            temp_y[:,1:] = -np.diff(self.meshlist[k], axis = 1) / self.h
            delta.append([temp_x, temp_y])
            
            #Gamma
            temp_xx = np.zeros([length, length])
            temp_yy = np.zeros([length, length])
            temp_xy = np.zeros([length, length])
            
            temp_xx[1:-1] = np.diff(self.meshlist[k], 2, axis = 0) / (self.h ** 2)
            temp_yy[:, 1:-1] = np.diff(self.meshlist[k], 2, axis = 1) / (self.h ** 2)
            
            temp_xy[1:, 1:] = (self.meshlist[k][:-1, :-1] - \
                self.meshlist[k][:-1, 1:] - self.meshlist[k][1:, :-1] +\
                self.meshlist[k][1:, 1:]) / self.h
            
            gamma.append([temp_xx, temp_yy, temp_xy])
            
            #Theta
            if k != len(self.meshlist) - 1:
                theta.append((self.meshlist[k] - self.meshlist[k+1]) / (self.dt * 365))
        
        #현재 Greek 계산
        cur_theta = self.engine.interpolatePrice(theta[-1], self.engine.x,
                                                 self.engine.y)
        cur_delta_x = self.engine.interpolatePrice(delta[-1][0], self.engine.x,
                                                   self.engine.y)
        cur_delta_y = self.engine.interpolatePrice(delta[-1][1], self.engine.x,
                                                   self.engine.y)
        cur_gamma_x = self.engine.interpolatePrice(gamma[-1][0], self.engine.x,
                                                   self.engine.y)
        cur_gamma_y = self.engine.interpolatePrice(gamma[-1][1], self.engine.x,
                                                   self.engine.y)
        cur_gamma_xy = self.engine.interpolatePrice(gamma[-1][2], self.engine.x,
                                                    self.engine.y)
            
        return ([cur_delta_x, cur_delta_y, cur_gamma_x, cur_gamma_y, cur_gamma_xy, cur_theta],
                {'delta': delta, 'gamma': gamma, 'theta': theta})
    
    def vega(self, minvalue, maxvalue, snum, tnum, lmd = 0.5, bp=0.0001):
        self.vol1.data += bp
        vol1_up = self.price(minvalue, maxvalue, snum, tnum, lmd)
        
        self.vol1.data -= 2*bp #1bp 만큼 올렸었으니 한 비피 작은 거를 구하기 위해서는 2bp만큼 빼줌
        vol1_down = self.price(minvalue, maxvalue, snum, tnum, lmd)
        self.vol1.data += bp #다시 되돌려줌
        
        results = {}
        vega_x_list = []
        for i in range(0,len(vol1_up[1])):
            vega_x = (vol1_up[1][i] - vol1_down[1][i])/(2*bp)
            vega_x_list.append(vega_x)
        results['x'] = vega_x_list

        self.vol2.data += bp
        vol2_up = self.price(minvalue, maxvalue, snum, tnum, lmd)
        
        self.vol2.data -= 2*bp #1bp 만큼 올렸었으니 한 비피 작은 거를 구하기 위해서는 2bp만큼 빼줌
        vol2_down = self.price(minvalue, maxvalue, snum, tnum, lmd)
        self.vol2.data += bp #다시 되돌려줌
        
        vega_y_list = []
        for i in range(0,len(vol1_up[1])):
            vega_y = (vol2_up[1][i] - vol2_down[1][i])/(2*bp)
            vega_y_list.append(vega_y)
        results['y'] = vega_y_list
        
        cur_vega_x = self.engine.interpolatePrice(results['x'][-1], self.engine.x, self.engine.y)
        cur_vega_y = self.engine.interpolatePrice(results['y'][-1], self.engine.x, self.engine.y)
        return cur_vega_x, cur_vega_y, results
    
    def rho_greek(self, minvalue, maxvalue, snum, tnum, lmd = 0.5, bp=0.001):
        self.rate.rates[:, 1:] += bp
        rate_up = self.price(minvalue, maxvalue, snum, tnum, lmd)
        self.rate.rates[:, 1:] -= 2*bp #1bp 만큼 올렸었으니 한 비피 작은 거를 구하기 위해서는 2bp만큼 빼줌
        rate_down = self.price(minvalue, maxvalue, snum, tnum, lmd)
        self.rate.rates[:, 1:] += bp #다시 되돌려 줌
        
        rho_list = []
        for i in range(0,len(rate_up[1])):
            rho = (rate_up[1][i] - rate_down[1][i])/(2*bp)
            rho_list.append(rho)
        
        cur_rho = self.engine.interpolatePrice(rho_list[-1], self.engine.x,
                                               self.engine.y)
        return cur_rho, rho_list
            
            
if __name__ == '__main__':
    
    #Volatility, Interest Rate
    hscei_vol = marketvariable.VolSurface(pd.read_excel('loc_vol_HSCEI.xlsx', index_col = 0))
    eurostoxx_vol = marketvariable.VolSurface(pd.read_excel('loc_vol_EuroStoxx50.xlsx', index_col = 0))
    rate = marketvariable.YieldTermStructure(np.array(pd.read_excel('interest_rate.xlsx')))
    
    #조기상환일
    early_dates = [np.datetime64('2018-05-21'), np.datetime64('2018-11-22'),
                   np.datetime64('2019-05-22'), np.datetime64('2019-11-22'),
                   np.datetime64('2020-05-22') ]
    
    #평가일과 만기
    basedate = np.datetime64('2017-11-24')
    matdate = np.datetime64('2020-11-23')
    
    #Time과 가격을 쪼개는 횟수
    tnum = 100
    snum = 200
    
    #표준화된 자산의 최솟값과 최대값
    minvalue = 50
    maxvalue = 200
    
    #지급되는 쿠폰, 조기상환 조건, 배리어
    coupon = np.array([0.025, 0.05, 0.075, 0.1, 0.125, 0.15])
    condition = np.array([.90, .90, .90, .85, .85])
    barrier = 0.65
    
    #객체 생성 및 시장변수 지정
    els = ELS(basedate, matdate, early_dates, condition, coupon, barrier)
    els.setMarketVariable(rate, hscei_vol, eurostoxx_vol, 0.38)
    
    #Pricing Engine 설정: OSM
    els.setEngine(0)
    
    #가격 평가: 가격과 mesh list가 결과로 나옴
    result = els.price(minvalue, maxvalue, snum, tnum)
    print("OSM Price: %.4f" %result[0])
    greek = els.greek()
    vega = els.vega(minvalue, maxvalue, snum, tnum)
    #Pricing Engine 재설정: ADI
    els.setEngine(1)
    result1 = els.price(minvalue, maxvalue, snum, tnum)
    print("ADI Price: %.4f" %result1[0])
    
    
