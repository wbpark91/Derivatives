#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import numpy as np
import copy

class FDM_Engine:
    def __init__(self):
        pass
    
    def initial_mesh(self, minvalue, maxvalue, num, coupon, barrier):      
        '''
        tau = 0 시점의 mesh를 만들어주는 함수
        가로축: x축
        세로축: y축
        
        ===Input===
        minvalue: 표준화된 최소값
        maxvalue: 표준화된 최대값
        num: mesh를 쪼개는 개수
        coupon: 시간 순서대로 상환시 지급하는 쿠폰(ex: [0.025, 0.05, 0.075, 0.1, 0.125, 0.15])
        barrier: knock-in 배리어
        
        변수 coupon과 barrier는 class를 만들 때 멤버 변수화될 것임
        '''        
        price_range = np.linspace(maxvalue, minvalue, num)
        
        self.x = price_range
        self.y = price_range
        
        x, y = np.meshgrid(price_range, price_range)    #(x와 y의 (num * num) matrix를 만듬)
        mesh = np.where(x > y, y, x)                    #x와 y중 mininum을 component로 갖는 행렬
        mesh = np.where(mesh >= 100 * barrier, 100 * (1 + coupon[-1]), mesh)     #barrier보다 크면 쿠폰 지급, 아닌 경우 원금 손실
        
        return mesh, price_range, price_range
    
    def Thomas_Algorithm(self, a_, d_):
        #Deep copy
        a = copy.deepcopy(a_)
        d = copy.deepcopy(d_)
        
        #Tridiagonal Solver: Ax = d
        if len(a.T) != len(d):    
            raise IndexError("Dimension of A and d should be equal")
        
        for i in range(len(a) - 1):
            d[i+1] += (-a[i+1][i] / a[i][i]) * d[i]
            a[i+1] += (-a[i+1][i] / a[i][i]) * a[i]
            
        x = np.zeros(d.shape)
        x[-1] = d[-1] / a[-1][-1]
        for i in range(len(a) - 2, -1, -1):
            x[i] = (d[i] - a[i][i+1] * x[i+1]) / a[i][i]
        
        return x

    def early_redemption(self, mesh, x, y, condition, coupon):
        '''
        조기상환 여부를 판단, 조기상환되는 경우 mesh에 해당 payoff를 반영하는 함수
        
        ===Input===
        mesh: 해당 시점 mesh
        x, y: 자산 x, y의 가격 범위
        condition: 조기상환 조건(min(x,y) > 100 * condition이면 조기상환)
        coupon: 조기상환시 지급하는 쿠폰
        
        '''
        x, y = np.meshgrid(x, y)
        min_xy = np.where(x > y, y, x)
        mask = np.where(min_xy >= 100 * condition, True, False) #조기상환 조건을 만족하면 True, 그렇지 않으면 False
        mesh = np.where(mask == True, 100 * (1 + coupon), mesh) #True이면 조기상환 쿠폰 지급, 그렇지 않으면 지급하지 않음
        return mesh
    
    def fill_boundary(self,mesh):
        '''
        Linear Boundary condition을 이용, boundary 부분을 채우는 함수
        '''
        mesh[0] = 2 * mesh[1] - mesh[2]
        mesh[-1] = 2 * mesh[-2] - mesh[-3]
        mesh[:,0] = 2 * mesh[:, 1] - mesh[:,2]
        mesh[:,-1] = 2 * mesh[:, -2] - mesh[:, -3]
        
        return mesh
    
    def findProportion(self, x, x0):
        '''
        x0에 인접한 index와 보간하는 비율을 찾아주는 함수
        k1: x0보다 크거나 같으면서 가장 가까운 값
        k2: x0보다 작거나 같으면서 가장 가까운 값
        idx1: k1의 인덱스
        idx2: k2의 인덱스
        p: k1 * p + k2 * (1 - p) = x0 을 만족하는 p
        '''
        
        idx1 = int(np.argwhere(x >= x0)[-1])
        idx2 = int(np.argwhere(x <= x0)[0])
        k1 = x[idx1]
        k2 = x[idx2]
        
        if k1 != k2:
            p = (x0 - k2) / (k1 - k2)
        else:
            p = 1
        
        return p, idx1, idx2
    
    def interpolatePrice(self, mesh, x, y, x0 = 100, y0 = 100):
        '''
        주어진 mesh에서 보간법을 이용, 가격을 계산하는 함수
        
        ===Input===
        mesh: 해당 시점 mesh(가로: y, 세로: x)
        x, y: 자산 x, y의 가격 범위가 나타난 array
        x0, y0: 자산 x, y의 현재가격 (Default: 100)
        '''
        
        px, xidx1, xidx2 = self.findProportion(x, x0)
        py, yidx1, yidx2 = self.findProportion(y, y0)
        
        # x축 보간
        h1 = px * mesh[xidx1, yidx1] + (1 - px) * mesh[xidx2, yidx1]
        h2 = px * mesh[xidx1, yidx2] + (1 - px) * mesh[xidx2, yidx2]
        
        # y축 보간
        value = py * h1 + (1 - py) * h2
        
        return value*100

#%%    
class OSM_Pricing(FDM_Engine):
    '''
    Operating Splitting Method를 사용하기 위한 함수를 모아 놓은 클래스
    '''
    def __init__(self):
        #import ELS_Pricing_Pack
        #self.Param = ELS_Pricing_Pack.Parameters()
        pass
        
    def tridiagonal_A(self, vols1, vols2, Xs, Ys, h, dt, r, lmbda, tnum, snum):
        vols1 = vols1[:,1:-1]
        vols2 = vols2[:,1:-1]
        Xs, Ys = Xs[1:-1], Ys[1:-1]
        Ai=[]
        Aj=[]
        for n in range(tnum-1):
            xalpha = -0.5*((vols1[n]**2).T)*(Xs**2)/(h**2)
            xbeta = 1/dt+((vols1[n]**2).T)*(Xs**2)/(h**2)+r[0,n]*Xs/h+lmbda*r[0,n]
            xgamma = -0.5*((vols1[n]**2).T)*(Xs**2)/(h**2)-r[0,n]*Xs/h
            Ax = np.zeros(shape=(vols1.shape[1],vols1.shape[1]))
            np.fill_diagonal(Ax,xbeta,0)
            np.fill_diagonal(Ax[1:],xalpha[1:])
            np.fill_diagonal(Ax[:,1:],xgamma[:-1])
            i = 0
            Ax[i,i] = 2*xalpha[i]+xbeta[i]
            Ax[i,i+1] = xgamma[i]-xalpha[i]
            i = (snum-3)
            Ax[i,i-1] = xalpha[i]-xgamma[i]
            Ax[i,i] = xbeta[i]+2*xgamma[i]
            Ai.append(Ax)
            
            yalpha = -0.5*((vols2[n]**2).T)*(Ys**2)/(h**2)
            ybeta = 1/dt+((vols2[n]**2).T)*(Ys**2)/(h**2)+r[0,n]*Ys/h+lmbda*r[0,n]
            ygamma = -0.5*((vols2[n]**2).T)*(Ys**2)/(h**2)-r[0,n]*Ys/h
            Ay = np.zeros(shape=(vols2.shape[1],vols2.shape[1]))
            np.fill_diagonal(Ay,ybeta,0)
            np.fill_diagonal(Ay[1:],yalpha[1:])
            np.fill_diagonal(Ay[:,1:],ygamma[:-1])
            j = 0
            Ay[j,j] = 2*yalpha[j]+ybeta[0]
            Ay[j,j+1] = -yalpha[j]+ygamma[j]
            j = (snum-3)
            Ay[j,j-1] = yalpha[j]-ygamma[j]
            Ay[j,j] = ybeta[j]+2*ygamma[j]
            Aj.append(Ay)
            
        return Ai, Aj

    
    def step1(self,A, x, y, rate, vol1, vol2, mesh, dt, h, lmd, rho, layer):
        '''
        OSM의 step1 구현
        
        ===Input==
        A: Au = f 식의 tridiagonal matrix
        x, y: 자산 x, y의 price range
        vol1: volatility surface of x
        vol2: volatility surface of y
        mesh: 이전 시점 mesh
        dt: 시간 간격
        h: x, y 자산 가격의 간격
        lmd: lambda1
        rho: 두 자산 간 상관계수
        layer: 현재 시점을 나타내는 변수
        '''
        
        f = np.zeros([len(mesh) - 2, len(mesh.T) - 2])  # f matrix
        newmesh = np.zeros([len(mesh), len(mesh.T)])
        x = x[1:-1]                 # 자산 x의 Boundary 제외
        y = y[1:-1]                 # 자산 y의 Boundary 제외
        
        f += lmd * rho * vol2[layer][1:-1] * y * (mesh[2:,2:] - mesh[2:, 1:-1] \
                             - mesh[1:-1, 2:] + mesh[1:-1, 1:-1]) / (h ** 2)
        f = (vol1[layer][1:-1] * x * f.T).T
        f += mesh[1:-1, 1:-1] / dt
        newmesh[1:-1, 1:-1] = self.Thomas_Algorithm(A, f)
        #print(newmesh)
        newmesh = self.fill_boundary(newmesh)
        
        return newmesh
    
    def step2(self, A, x, y, rate, vol1, vol2, mesh, dt, h, lmd, rho, layer):
        '''
        A: A matrix, 크기는 인풋 mesh의 0시점과 N+1이 빠져 행, 열로 2개가 작은 N-1개씩의 행, 열로 이루어져있다.
        vol1: x에 대한 volatility surface
        vol2: y에 대한 volatility surface
        mesh: 전 시점의 mesh. 크기는 0~N+1까지 행과 열이 있다.
        layer: 몇 층(시간)이냐
        '''
        newmesh = np.array( [[np.nan]*len(mesh)]*len(mesh) )
        N = len(mesh)-1
        
        i_j = mesh[1:N,1:N]
        ip1_jp1 = mesh[1+1:,1+1:]
        ip1 = mesh[1+1:,1:N] #쉬프트 안 한만큼 1 더 빼줘야 함. 쑤도 알고리즘에서 for loop이 N까지 돌기 때문
        jp1 = mesh[1:N,1+1:]
        
        v1 = vol1[layer][1:N][np.newaxis].T #방향 바꿔줘야 함
        v2 = vol2[layer][1:N]

        x = x[1:-1][np.newaxis].T # 자산 x의 Boundary 제외
        y = y[1:-1] # 자산 y의 Boundary 제외        
        
        g = (ip1_jp1 - ip1 - jp1 + i_j)*(1-lmd)*rho*v1*v2*x*y / (h**2) + i_j/dt

        newmesh[1:N, 1:N] = self.Thomas_Algorithm(A,g)
        return self.fill_boundary(newmesh).T



#%%
class ADI_Pricing(FDM_Engine):
    '''
    ADI를 사용하기 위한 함수를 모아 놓은 클래스
    '''
    def __init__(self):
        pass

    def tridiagonal_A(self, vols1, vols2, Xs, Ys, h, dt, r, lmbda, tnum, snum):
        vols1, vols2 = vols1[:,1:-1], vols2[:,1:-1]
        Xs, Ys = Xs[1:-1], Ys[1:-1]
        Ai, Aj = [], []
        for n in range(tnum-1):
            xalpha = -0.25*((vols1[n]**2).T)*(Xs**2)/(h**2)
            xbeta = 1/dt+ 0.5*((vols1[n]**2).T)*(Xs**2)/(h**2)+0.5*r[0,n]*Xs/h+0.5*r[0,n]
            xgamma = -0.25*((vols1[n]**2).T)*(Xs**2)/(h**2)-0.5*r[0,n]*Xs/h
            Ax = np.zeros(shape=(vols1.shape[1],vols1.shape[1]))
            np.fill_diagonal(Ax,xbeta,0)
            np.fill_diagonal(Ax[1:],xalpha[1:])
            np.fill_diagonal(Ax[:,1:],xgamma[:-1])
            i = 0
            Ax[i,i] = 2*xalpha[i]+xbeta[i]
            Ax[i,i+1] = xgamma[i]-xalpha[i]
            i = (snum-3)
            Ax[i,i-1] = xalpha[i]-xgamma[i]
            Ax[i,i] = xbeta[i]+2*xgamma[i]
            Ai.append(Ax)
            
            yalpha = -0.25*((vols2[n]**2).T)*(Ys**2)/(h**2)
            ybeta = 1/dt+0.5*((vols2[n]**2).T)*(Ys**2)/(h**2)+0.5*r[0,n]*Ys/h+0.5*r[0,n]
            ygamma = -0.25*((vols2[n]**2).T)*(Ys**2)/(h**2)-0.5*r[0,n]*Ys/h
            Ay = np.zeros(shape=(vols2.shape[1],vols2.shape[1]))
            np.fill_diagonal(Ay,ybeta,0)
            np.fill_diagonal(Ay[1:],yalpha[1:])
            np.fill_diagonal(Ay[:,1:],ygamma[:-1])
            j = 0
            Ay[j,j] = 2*yalpha[j]+ybeta[j]
            Ay[j,j+1] = -yalpha[j]+ygamma[j]
            j = (snum-3)
            Ay[j,j-1] = yalpha[j]-ygamma[j]
            Ay[j,j] = ybeta[j]+2*ygamma[j]
            Aj.append(Ay)
            
        return Ai, Aj

    
    def step1(self, A, x, y, rate, vol1, vol2, mesh, dt, h, lmd, rho, layer):
        '''
        ADI의 step1 구현
        
        ===Input==
        A: Au = f 식의 tridiagonal matrix
        x, y: 자산 x, y의 price range
        rate: 이자율 array
        vol1: volatility surface of x
        vol2: volatility surface of y
        mesh: 이전 시점 mesh
        dt: 시간 간격
        h: x, y 자산 가격의 간격
        rho: 두 자산 간 상관계수
        layer: 현재 시점을 나타내는 변수
        '''
        
        f = np.zeros([len(mesh) - 2, len(mesh.T) - 2])  # f matrix
        newmesh = np.zeros([len(mesh), len(mesh.T)])
        x = x[1:-1]                 # 자산 x의 Boundary 제외
        y = y[1:-1]                 # 자산 y의 Boundary 제외
        
        # 4th Term
        f += 0.5 * rho * vol2[layer][1:-1] * y * (mesh[2:, 2:] + mesh[:-2, :-2] \
                             - mesh[:-2, 2:] - mesh[2:, :-2]) / (4 * h ** 2)
        f += (vol1[layer][1:-1] * x * f.T).T
        
        # 3rd Term
        f += 0.5 * y * rate[layer] * (mesh[1:-1, 2:] - mesh[1:-1, 1:-1]) / h
        
        # 2nd Term
        f += (mesh[1:-1, 2:] - 2 * mesh[1:-1, 1:-1] + mesh[1:-1, :-2]) * \
              (vol2[layer][1:-1] * y) ** 2 / (4 * h ** 2)
        
        # 1st Term
        f += mesh[1:-1, 1:-1] / dt
        
        newmesh[1:-1, 1:-1] = self.Thomas_Algorithm(A, f)
        newmesh = self.fill_boundary(newmesh)
        
        return newmesh

    def step2(self, A, x, y, rate, vol1, vol2, mesh, dt, h, lmd, rho, layer):
        '''
        A: A matrix, 크기는 인풋 mesh의 0시점과 N+1이 빠져 행, 열로 2개가 작은 N-1개씩의 행, 열로 이루어져있다.
        vol1: x에 대한 volatility surface
        vol2: y에 대한 volatility surface
        mesh: 전 시점의 mesh. 크기는 0~N+1까지 행과 열이 있다.
        layer: 몇 층(시간)이냐
        '''
        newmesh = np.array( [[np.nan]*len(mesh)]*len(mesh) )
        N = len(mesh)-1
        
        i_j = mesh[1:N,1:N]
        ip1_jp1 = mesh[1+1:,1+1:]
        ip1 = mesh[1+1:,1:N] #쉬프트 안 한만큼 1 더 빼줘야 함. 쑤도 알고리즘에서 for loop이 N까지 돌기 때문
        
        im1 = mesh[1-1:N-1,1:N]
        im1_jm1 = mesh[1-1:N-1,1-1:N-1]
        im1_jp1 = mesh[1-1:N-1,1+1:N+1]
        ip1_jm1 = mesh[1+1:N+1,1-1:N-1]
        
        v1 = vol1[layer][1:N][np.newaxis].T #방향 바꿔줘야 함
        v2 = vol2[layer][1:N]
        r = rate[layer]
        
        x = x[1:-1][np.newaxis].T # 자산 x의 Boundary 제외
        y = y[1:-1] # 자산 y의 Boundary 제외   
        
        first = i_j/dt
        second = (ip1 - 2*i_j + im1)/(h**2) * ((v1*x)**2)/4
        third = (ip1 - i_j)*r*x/h/2
        fourth = (ip1_jp1 + im1_jm1 - im1_jp1 - ip1_jm1)*rho*v1*v2*x*y / (4*(h**2))
        
        g = first + second + third + fourth

        newmesh[1:N, 1:N] = self.Thomas_Algorithm(A,g)
        return self.fill_boundary(newmesh).T