author: Oleg I.Berngardt, 2025
license: GPL

version: 1.4
Fast version - made adaptive initial approximation for fitting P,W . Correct velocity calculation using prm.mpinc interpulse interval instead of 2.4msec. Calculate every sample with fit.qflg==1(for each nodes #) and p0>3dB(for modes #<=5) 

version: 0.3
Added elevation calculations using FITACF xarr.phi0, hdw and calibration coefficients

version: 0.2
Process rawacf by AAMPACF algorithm and store FITACF & AAMPACF results in CBOR format


Requires :

    gcc/make development:
     (Ubuntu: sudo apt install gcc-11 binutils make)

    python with libraries (see python.requirements.txt), v.3.12, see Makefiles, might be running under venv
     Install under Ubuntu:
       sudu apt install python3-dev python3-pip
       pip3 install -r python.requirements.txt

    python development libraries (sudo apt install python3-dev)

    cbor  (Ubuntu: sudo apt install libcbor-dev)

    zlib  (Ubuntu: sudo apt install zlib1g-dev)

    RST-lite (Follow instructions at https://github.com/vtsuperdarn/RSTLite)

Compile:
    make clean all


Processing multiple files:

    put files into data.src/RADARNAME/YEAR directory (bzip2-ed rawacfs)

    run 1.TEST.pl RADARNAME YEAR MONTH
    
    get results in data.out directory

    During processing it also calibrates radar elevations and calculate elevation for each signal


Currently supports the following radars (listed in cal (calibraton) directory):

    SECIRA:

 	ekb, mgw
