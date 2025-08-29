# V1.1: LIMIT CALC MODES - SPEED UP PROCESS, ANY QFLG=1 DATA PROCESSED
# V1.2: INITIALIZE PARAMS CORRECTLY, DECREASE NUMBER OF ITTERATIONS ACCORDING STATS to max=100 instead of 1000. 
# Speed up search -10/-20% to execution time using more close initial approx
# Use 100 first samples to estimate median initial approximation closer, then apply the to the whole dataset
# Works well on both radars (i.e. any pulse shapes and geophysical conditions)
# V1.4: ADDED MPINC dependence (in previous versions it was 2.4msec hardcoded)
# V1.5: changed curve_fit to least_squares - program 30% faster, very small number of errors

import numpy as np
from spectrum import arma_estimate, arma2psd
# import pylab
import matplotlib.pyplot as pp
import numpy as np
import sys
import scipy.stats
import scipy.optimize
from spectrum import arma_estimate, arma2psd
import multiprocessing
import time

# ZERO_RIGHT=True
ZERO_RIGHT=False
ACF_LEN=119 
# MPINC=500
lim=0.1


def model(x, *prms, GCount=-1,MeansArray=np.array([-50,3,20])):
    ret = 0
    Means=MeansArray
    for i in range(GCount):    
     ret += prms[GCount*0+i]*scipy.stats.norm.pdf(x, loc=Means[i] ,scale=prms[GCount*1+i]) #amplitude is energy of the spectral component (~P0)
    return ret





## search spectrum, calculate maximums exceeding lim level

from scipy.signal import find_peaks
import numpy as np

def getMaxPos(x, spec, lim=1e-100): # gives -0.05 from getMaxPos() time - a bit faster

    peak_indices, _ = find_peaks(spec)
    if len(peak_indices) == 0: return np.empty((0, 2))

    peak_x = x[peak_indices]
    peak_y = spec[peak_indices]

    # Объединяем в массив и сортируем по убыванию амплитуды
    peaks = np.column_stack([peak_x, peak_y])
    peaks = peaks[peaks[:, 1].argsort()[::-1]]  # сортировка по убыванию

    max_amp = peaks[:, 1].max()
    if max_amp == 0: return np.empty((0, 2))
    peaks[:, 1] /= max_amp
    return peaks[peaks[:, 1] > lim]








from scipy.optimize import least_squares

def residuals(prms, x, y, GCount, MeansArray):
    return y - model(x, *prms, GCount=GCount, MeansArray=MeansArray)



def FitA(qflg,st,calculatePW=True,Winit=20.,Pmod=20.):
#  calculatePW=False
#  calculatePW=True
  poses=getMaxPos(np.array(range(st.shape[0])),st,lim)

#LIMIT CALCULATIONS - IF TOO MUCH MODES - EXIT, IT LOOKS LIKE NOISE
#  if(qflg!=1 and poses[poses[:,1]>0.1].shape[0]>5):
  if(qflg!=1 and poses[poses[:,1]>0.1].shape[0]>5 ):
   poses=[]
   if(Winit>1 or Pmod>1):
    return ([],[],[],0)
   else:
    return ([],[],[],0,[])
#/LIMIT

  #fit curve by gaussian mixture model
  ## fill initial values and bounds
  GCount=0  
  MeansArray=[]
  params=[0]
  for pos in poses:
    MeansArray.append(pos[0])  
    GCount+=1
#  params=[1.]*GCount*2
#  params=list(st[np.array(MeansArray).astype(int)]*10.)+[10.]*GCount
#  params=list(st[np.array(MeansArray).astype(int)]*10.)+[3.]*GCount
#  Winit=20. # initial spectral width 7.-mgw  20/20 - EKB, 70/70 - MGW
#  Pmod=20. #Winit*2.5 #sqrt(2Pi)
  params=list(st[np.array(MeansArray).astype(int)]*Pmod)+[Winit]*GCount  # good initial approximation for ekb and magw: -10% to execution time
  bounds=[[0]*GCount*2,[(st.max()+1e-11)*st.shape[0]]*GCount+[st.shape[0]]*GCount]
#  params=[1.]*GCount*2

  ## Fit gaussian mixture model
  x=np.array(range(0,st.shape[0]))
  pmodel=lambda a, *b: model(a, *b, GCount=GCount, MeansArray=MeansArray)
  infodict=''
#  fitted_params, aa, infodict, _, _  = scipy.optimize.curve_fit(lambda a, *b: model(a, *b, GCount=GCount, MeansArray=MeansArray), x, st, p0=params, bounds=bounds,maxfev=1000, full_output=True, method='lm')
#  print('itt:',infodict,fitted_params)
  if calculatePW:
   try: # maxfev to increase number of itterations - sometimes there are no enough of them and error is generated
#    print('src:',params)
#    fitted_params, aa, infodict, _, _  = scipy.optimize.curve_fit(lambda a, *b: model(a, *b, GCount=GCount, MeansArray=MeansArray), x, st, p0=params, bounds=bounds,maxfev=100, full_output=True)
#    fitted_params, aa  = scipy.optimize.curve_fit(lambda a, *b: model(a, *b, GCount=GCount, MeansArray=MeansArray), x, st, p0=params, bounds=bounds,maxfev=100)
    res = least_squares(residuals, params, args=(x, st, GCount, MeansArray),
###                    bounds=(bounds[0], bounds[1]), ftol=1e-4, xtol=1e-4, max_nfev=100)
                    bounds=bounds, ftol=1e-4, xtol=1e-4, max_nfev=100) # makes faster 1.5 times in comparision with curve_fit
    fitted_params = res.x if res.success else params

#    print('itt:',infodict['nfev'],fitted_params)
#    print('itt:',infodict['nfev'])
   except:
    fitted_params=list(st[np.array(MeansArray).astype(int)])+[-1.]*GCount
  else:
   fitted_params=list(st[np.array(MeansArray).astype(int)])+[-1.]*GCount
  inc=np.array(fitted_params)/np.array(params)
  
  #resort again
  fitted_new=[]
  L=len(fitted_params)//2
  inc=inc.reshape(L,2)
#  for i in range(L):
#   print(inc[i,0],inc[i,1])
  for i in range(L):
   fitted_new.append([fitted_params[i],fitted_params[L+i],MeansArray[i]])
  sorted_res = sorted(fitted_new, key=lambda x: x[0], reverse=True)
  sorted_res = np.array(sorted_res)
  MeansArray = list(sorted_res[:,2])
  fitted_params = list(np.concatenate([sorted_res[:,0:1],sorted_res[:,1:2]])[:,0])
  if(Winit>1 or Pmod>1):
   return (fitted_params,GCount,MeansArray,len(poses))
  else:
   return (fitted_params,GCount,MeansArray,len(poses),inc)















# parallel spectraum calculate
def MapProcess(datasrc,Winit=1.,Pmod=1.):
 show_all_peaks=True
# show_all_peaks=False
 calc_as_mean=True # way of calculating mean/max peak amplitude
# calc_as_mean=False # way of calculating mean/max peak amplitude
 data,f0,v0,w0,bm0,ch0,t0,r0,p0,qflg,MPINC=MapProcess2(datasrc)
# print('qfg:',qflg)
# print('MPINC:',MPINC)
 t=np.linspace(-data.shape[0]/2,data.shape[0]/2,data.shape[0])
 datamax=(np.abs(data).max()+1e-11) #/256.  # absolutely necessary to arma_estimate work correctly - problem is in arcovar check of e.imag <1.e-4 - for big values causes error
 data/=datamax

 a,b, rho = arma_estimate(data, 10, 10, data.shape[0]-10)
 psd = arma2psd(A=a, B=b, rho=rho, norm=True)

 psd*=datamax

 psd=np.fft.fftshift(psd)  # PSD spectrum
 f=np.linspace(0,ACF_LEN,psd.shape[0])
#removed double: res=getMaxPos(f,psd,lim)
 res2=FitA(qflg,psd,Winit=Winit,Pmod=Pmod)

 fitted_params=res2[0]
 GCount=res2[1]
 MeansArray=res2[2]
 if Winit>1 and Pmod>1:
  pass
 else: 
  IncArray=res2[4]
# print(res2)

 df=1/(MPINC*1e-6)/2. #descreet size
 c=300000000        #ligght speed
 f0=f0*1000; 	    #sounding frequency
 VF=c/2/f0	    # doppler to velocity proportion
 v=df*VF	    # upper limit for ARMA spectrum

 P=np.array(fitted_params)
 P=P[:P.shape[0]//2]

 W=np.array(fitted_params)
 W=W[W.shape[0]//2:]*v/(psd.shape[0]/2)

 V=np.array(MeansArray)
 V2=V-psd.shape[0]/2
 V2=V2*v/(psd.shape[0]/2)
 res=[]
 if show_all_peaks:
  for st_idx in range(P.shape[0]):
#   print(int(bm0),int(ch0),t0,int(r0),np.round(P[st_idx]/P[0],3),np.round(P[st_idx],3),np.round(V[st_idx]),np.round(W[st_idx]),W.shape[0],float(v0),float(w0),np.round(p0,1),P.shape[0],st_idx,flush=True)
#   res.append(sprint(int(bm0),int(ch0),t0,int(r0),np.round(P[st_idx]/P[0],3),np.round(P[st_idx],3),np.round(V[st_idx]),np.round(W[st_idx]),W.shape[0],float(v0),float(w0),np.round(p0,1),P.shape[0],st_idx,flush=True))
#   if st_idx==0:
   if Winit>1 and Pmod>1:
    res.append(' '.join([str(x) for x in [int(bm0),int(ch0),float(t0),int(r0),float(np.round(P[st_idx]/P[0],3)),float(np.round(P[st_idx],3)),\
            float(np.round(V2[st_idx])),float(np.round(W[st_idx])),W.shape[0],float(v0),float(w0),float(np.round(p0,1)),\
            P.shape[0],st_idx,np.round(V[st_idx])]]))
   else:
    res.append(' '.join([str(x) for x in [int(bm0),int(ch0),float(t0),int(r0),float(np.round(P[st_idx]/P[0],3)),float(np.round(P[st_idx],3)),\
            float(np.round(V2[st_idx])),float(np.round(W[st_idx])),W.shape[0],float(v0),float(w0),float(np.round(p0,1)),\
            P.shape[0],st_idx,np.round(V[st_idx]),IncArray[st_idx,0],IncArray[st_idx,1]]]))
 if(len(res)>0):
#  print('res:',res)
  return '\n'.join(res)+'\n'
 else:
  return ''














# model
def MapProcess2(datasrc):
 return datasrc[0],datasrc[1][2],datasrc[1][6],datasrc[1][7], datasrc[1][0], datasrc[1][1], datasrc[1][3], datasrc[1][4], datasrc[1][5],datasrc[1][8],datasrc[1][9]
















class fastMapProcess(object):
    def __init__(self, Winit, Pmod):
        self.Winit = Winit
        self.Pmod = Pmod
    def __call__(self, src):
        return MapProcess(src,Winit=self.Winit,Pmod=self.Pmod)


def AAMPACF(content=""):
 eps=1e-7
 dataset=[]
 dataset_Y=[]
 full_dataset=[]
# start=time.time()
 for s in content.split("\n"):
  s=s.replace('inf','-1');
#  if(len(s.split())==189):
  if(len(s.split())==190):
   d=[float(x) for x in s.split()]
   d=np.array(d)
   full_dataset.append(d[:10])
#   MPINC=d[9]
   R=d[10::3]+1j*d[11::3]
   R=np.concatenate([R[:0:-1],R])
   idx=R.shape[0]//2
   R[:idx]=R[:idx].conj()
   R[idx]=np.abs(R[idx+1])
   if ZERO_RIGHT:
    R[idx:]=0  #zero right part
   VW=d[6:8]
   dataset.append(R)
   dataset_Y.append(VW)
# end=time.time()
# print('prep:',end-start)
# start=time.time()
 dataset=np.array(dataset)
 dataset_Y=np.array(dataset_Y)
 full_dataset=np.array(full_dataset)
# print('np conv:',time.time()-start)


# dataset=dataset[:1000]
# dataset_Y=dataset_Y[:1000]

#print(dataset.shape,dataset_Y.shape)
 if len(dataset.shape)<=1:
  return ''
 ACF_LEN=dataset.shape[1]
# print('ACFL:',ACF_LEN)
 data=dataset
 dataY=dataset_Y

 KK=20

 data=dataset

 KK=data.shape[0]
 stats=[]

# Parallel process spectrums
# start=time.time()
 data2=[[data[i],full_dataset[i]] for i in range(data.shape[0])]
# print('prep data2:',time.time()-start)
# print(len(data2))

# pool = multiprocessing.Pool(processes=32) #no effect
 pool = multiprocessing.Pool()
#BUFF=10
#for j in range(0,len(data2)-BUFF,BUFF):
# start=time.time()
# data_out=pool.map(MapProcess,data2) #parallel
# data_out=pool.map(MapProcess,data2[:10]) #parallel

# init params for fastMap
# data_out=map(MapProcess,data2[:100]) #seq
 data_out=pool.map(MapProcess,data2[:100]) #parallel
 stats=list(data_out)
 d=[]
 for s2 in  stats:
  for s in s2.split('\n'):
   if len(s.split())>0:
    d.append([float(x) for x in s.split()])
 d=np.array(d)
 Winit=np.median(d[:,-1])
 Pinit=np.median(d[:,-2])
# print('Winit',Winit,'Pinit',Pinit)
# fastMapProcess=lambda x: MapProcess(x,Winit=Winit,Pmod=Pinit)
# fastMapProcess=lambda x: MapProcess(x,Winit=1.1,Pmod=1.1)
# data_out=pool.map(fastMapProcess,data2) #seq
 start=time.time()
 data_out=pool.map(fastMapProcess(Winit=Winit,Pmod=Pinit),data2) #parallel
# data_out=map(fastMapProcess(Winit=Winit,Pmod=Pinit),data2) #sequential
# data_out=pool.map(fastMapProcess(Winit=1.1,Pmod=1.1),data2) #parallel
 stats=list(data_out)
# print('executed:',time.time()-start)
 res='';
 for s in stats:
  res+=''.join(s)
# print('joint :',time.time()-start)

 return res












# Read dataset
if __name__=='__main__':
 fname=sys.argv[1]

 f=open(fname,'rt')
 filecontent=f.read()
 start=time.time()
 res=AAMPACF(filecontent)
 end=time.time()
 print('AAMPACF time:',end-start)
 print(res)
 quit()

