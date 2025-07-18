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
# ZERO_RIGHT=True
ZERO_RIGHT=False
ACF_LEN=119 
lim=0.1

def model(x, *prms, GCount=-1,MeansArray=np.array([-50,3,20])):
    ret = 0
    Means=MeansArray
    for i in range(GCount):    
     ret += prms[GCount*0+i]*scipy.stats.norm.pdf(x, loc=Means[i] ,scale=prms[GCount*1+i]) #amplitude is energy of the spectral component (~P0)
    return ret

## search spectrum, calculate maximums exceeding lim level
def getMaxPos(x,spec,lim=1e-100):
    res=[]
    for i in range(1,spec.shape[0]-1):
        if spec[i-1]<spec[i]>spec[i+1]:
            res.append([x[i],spec[i]])
    sorted_res = sorted(res, key=lambda x: x[1], reverse=True)
    sorted_res=np.array(sorted_res)
    sorted_res[:,1]=sorted_res[:,1]/sorted_res[:,1].max()
    return sorted_res[sorted_res[:,1]>lim]

def FitA(st):
#  calculatePW=False
  calculatePW=True
  poses=getMaxPos(np.array(range(st.shape[0])),st,lim)

  #fit curve by gaussian mixture model
  ## fill initial values and bounds
  GCount=0  
  MeansArray=[]
  params=[0]
  for pos in poses:
    MeansArray.append(pos[0])  
    GCount+=1
  params=[1.]*GCount*2
  bounds=[[0]*GCount*2,[(st.max()+1e-11)*st.shape[0]]*GCount+[st.shape[0]]*GCount]

  ## Fit gaussian mixture model
  x=np.array(range(0,st.shape[0]))
  pmodel=lambda a, *b: model(a, *b, GCount=GCount, MeansArray=MeansArray)
  if calculatePW:
   try: # maxfev to increase number of itterations - sometimes there are no enough of them and error is generated
    fitted_params,_ = scipy.optimize.curve_fit(lambda a, *b: model(a, *b, GCount=GCount, MeansArray=MeansArray), x, st, p0=params, bounds=bounds,maxfev=1000)  
   except:
    fitted_params=list(st[np.array(MeansArray).astype(int)])+[-1.]*GCount
  else:
   fitted_params=list(st[np.array(MeansArray).astype(int)])+[-1.]*GCount

  #resort again
  fitted_new=[]
  L=len(fitted_params)//2
  for i in range(L):
   fitted_new.append([fitted_params[i],fitted_params[L+i],MeansArray[i]])
  sorted_res = sorted(fitted_new, key=lambda x: x[0], reverse=True)
  sorted_res = np.array(sorted_res)
  MeansArray = list(sorted_res[:,2])
  fitted_params = list(np.concatenate([sorted_res[:,0:1],sorted_res[:,1:2]])[:,0])
  return (fitted_params,GCount,MeansArray,len(poses))

# parallel spectraum calculate
def MapProcess(datasrc):
 show_all_peaks=True
# show_all_peaks=False
 calc_as_mean=True # way of calculating mean/max peak amplitude
# calc_as_mean=False # way of calculating mean/max peak amplitude
 data,f0,v0,w0,bm0,ch0,t0,r0,p0=MapProcess2(datasrc)

 t=np.linspace(-data.shape[0]/2,data.shape[0]/2,data.shape[0])
 datamax=(np.abs(data).max()+1e-11) #/256.  # absolutely necessary to arma_estimate work correctly - problem is in arcovar check of e.imag <1.e-4 - for big values causes error
 data/=datamax

 a,b, rho = arma_estimate(data, 10, 10, data.shape[0]-10)
 psd = arma2psd(A=a, B=b, rho=rho, norm=True)

 psd*=datamax

 psd=np.fft.fftshift(psd)  # PSD spectrum
 f=np.linspace(0,ACF_LEN,psd.shape[0])
 res=getMaxPos(f,psd,lim)
 res2=FitA(psd)

 fitted_params=res2[0]
 GCount=res2[1]
 MeansArray=res2[2]


 df=1/(2.4*1e-3)/2. #descreet size
 c=300000000        #ligght speed
 f0=f0*1000; 	    #sounding frequency
 VF=c/2/f0	    # doppler to velocity proportion
 v=df*VF	    # upper limit for ARMA spectrum

 P=np.array(fitted_params)
 P=P[:P.shape[0]//2]

 W=np.array(fitted_params)
 W=W[W.shape[0]//2:]*v/(psd.shape[0]/2)

 V=np.array(MeansArray)
 V=V-psd.shape[0]/2
 V=V*v/(psd.shape[0]/2)
 res=[]
 if show_all_peaks:
  for st_idx in range(P.shape[0]):
#   print(int(bm0),int(ch0),t0,int(r0),np.round(P[st_idx]/P[0],3),np.round(P[st_idx],3),np.round(V[st_idx]),np.round(W[st_idx]),W.shape[0],float(v0),float(w0),np.round(p0,1),P.shape[0],st_idx,flush=True)
#   res.append(sprint(int(bm0),int(ch0),t0,int(r0),np.round(P[st_idx]/P[0],3),np.round(P[st_idx],3),np.round(V[st_idx]),np.round(W[st_idx]),W.shape[0],float(v0),float(w0),np.round(p0,1),P.shape[0],st_idx,flush=True))
   res.append(' '.join([str(x) for x in [int(bm0),int(ch0),float(t0),int(r0),float(np.round(P[st_idx]/P[0],3)),float(np.round(P[st_idx],3)),\
            float(np.round(V[st_idx])),float(np.round(W[st_idx])),W.shape[0],float(v0),float(w0),float(np.round(p0,1)),\
            P.shape[0],st_idx]]))
 if(len(res)>0):
#  print('res:',res)
  return '\n'.join(res)+'\n'
 else:
  return ''


# model
def MapProcess2(datasrc):
 return datasrc[0],datasrc[1][2],datasrc[1][6],datasrc[1][7], datasrc[1][0], datasrc[1][1], datasrc[1][3], datasrc[1][4], datasrc[1][5]







def AAMPACF(content=""):
 eps=1e-7
 dataset=[]
 dataset_Y=[]
 full_dataset=[]
 for s in content.split("\n"):
  s=s.replace('inf','-1');
  if(len(s.split())==189):
   d=[float(x) for x in s.split()]
   d=np.array(d)
   full_dataset.append(d[:9])
   R=d[9::3]+1j*d[10::3]
   R=np.concatenate([R[:0:-1],R])
   idx=R.shape[0]//2
   R[:idx]=R[:idx].conj()
   R[idx]=np.abs(R[idx+1])
   if ZERO_RIGHT:
    R[idx:]=0  #zero right part
   VW=d[6:8]
   dataset.append(R)
   dataset_Y.append(VW)
 dataset=np.array(dataset)
 dataset_Y=np.array(dataset_Y)
 full_dataset=np.array(full_dataset)


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
 data2=[[data[i],full_dataset[i]] for i in range(data.shape[0])]
# print(len(data2))

 pool = multiprocessing.Pool()
#BUFF=10
#for j in range(0,len(data2)-BUFF,BUFF):
 data_out=pool.map(MapProcess,data2) #parallel
 stats=list(data_out)
# print(stats)
 res='';
 for s in stats:
  res+=''.join(s)
 return res





# Read dataset
if __name__=='__main__':
 fname=sys.argv[1]

 f=open(fname,'rt')
 filecontent=f.read()
 res=AAMPACF(filecontent)
 print(res)
 quit()

